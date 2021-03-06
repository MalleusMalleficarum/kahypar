cmake_minimum_required(VERSION 3.0)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

project(KaHyPar CXX C)
set(PROJECT_VENDOR "Sebastian Schlag")
set(PROJECT_CONTACT "kahypar@sebastianschlag.de")
set(PROJECT_URL "https://bitbucket.org/schlag/hypergraph")
set(PROJECT_DESCRIPTION "KaHyPar: Karlsruhe Hypergraph Partitioning")
#set(BUILD_SHARED_LIBS_OFF)
set(CMAKE_EXE_LINKER_FLAGS "-static-libstdc++")



include_directories(${PROJECT_SOURCE_DIR})

find_package(Threads REQUIRED)
message(STATUS "Found Threads: ${CMAKE_THREAD_LIBS_INIT}")

#ensure that gmock is built before tests are executed
add_subdirectory(external_tools/googletest)
include_directories(SYSTEM ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(SYSTEM ${gtest_SOURCE_DIR}/../googlemock/include ${gtest_SOURCE_DIR}/../googlemock/)

#xxhash
include_directories(SYSTEM external_tools/xxHash/)

option(KAHYPAR_USE_MINIMAL_BOOST
  "Download boost automatically and compile required libraries." OFF)

if(KAHYPAR_USE_MINIMAL_BOOST)
  include(ExternalProject)
  ExternalProject_Add(boost
    URL
    http://downloads.sourceforge.net/project/boost/boost/1.56.0/boost_1_56_0.tar.bz2
    #URL_MD5 6095876341956f65f9d35939ccea1a9f
    DOWNLOAD_DIR /tmp/
    PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/external_tools/boost
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND "")

  include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/external_tools/boost/src/boost/)

  # glob boost sources
  file(GLOB MINI_BOOST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/external_tools/boost/src/boost/libs/program_options/src/*.cpp)

  add_library(mini_boost STATIC ${MINI_BOOST_SOURCES})
  set_target_properties(mini_boost PROPERTIES LINKER_LANGUAGE CXX)
  set(Boost_LIBRARIES mini_boost)
else()
  set(BOOST_MIN_VERSION "1.48.0")
  find_package(Boost ${BOOST_MIN_VERSION} COMPONENTS program_options REQUIRED)
  if(Boost_FOUND)
    include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
    message(STATUS "Boost Include: ${Boost_INCLUDE_DIR}")
    message(STATUS "Boost Library Dirs: ${Boost_LIBRARY_DIRS}")
    message(STATUS "Boost Libraries: ${Boost_LIBRARIES}")
  endif()
endif()

if(NOT CMAKE_BUILD_TYPE)
  set( CMAKE_BUILD_TYPE Debug CACHE STRING
       "Choose the type of build, options are: Debug Release, RelWithDebInfo"
       FORCE )
endif()

# add a target to generate API documentation with Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile.in @ONLY)
add_custom_target(doc
${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile.in
WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
COMMENT "Generating API documentation with Doxygen" VERBATIM)
endif(DOXYGEN_FOUND)

find_package(Git)

include(GetGitRevisionDescription)
get_git_head_revision(KAHYPAR_VERSION_GIT_REFSPEC KAHYPAR_VERSION_GIT_SHA1)
if(KAHYPAR_VERSION_GIT_REFSPEC)
  message(STATUS "Detected git refspec ${KAHYPAR_VERSION_GIT_REFSPEC} sha ${KAHYPAR_VERSION_GIT_SHA1}")
  configure_file(${PROJECT_SOURCE_DIR}/kahypar/git_revision.h.in ${PROJECT_BINARY_DIR}/kahypar/git_revision.h)
  # add the binary tree to the search path for include files so that we will find GitRevision.h
  include_directories(${PROJECT_BINARY_DIR})
else(KAHYPAR_VERSION_GIT_REFSPEC)
  # we do need the sha hash to log with build version was used during experiments
  message(STATUS "No git refspec detected")
  configure_file(${PROJECT_SOURCE_DIR}/kahypar/git_revision.h.in ${PROJECT_BINARY_DIR}/kahypar/git_revision.h)
  include_directories(${PROJECT_BINARY_DIR})
endif(KAHYPAR_VERSION_GIT_REFSPEC)

add_custom_target(AnalyzeModifiedSources  perl "${PROJECT_SOURCE_DIR}/codestyle/analyze-source.pl" "-w")
add_custom_target(AnalyzeAllSources  perl "${PROJECT_SOURCE_DIR}/codestyle/analyze-source.pl" "-aw")

if(NOT MSVC)
  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -Wextra ")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wunused ")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wmaybe-uninitialized")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wfatal-errors")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wcast-qual")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Woverloaded-virtual")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wredundant-decls")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Winit-self")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wnoexcept")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPARANOID ")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weffc++")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-function")
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
  endif()

  include(CheckSSE4_2)
  if( BUILTIN_POPCNT )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.2")
  endif()

  include(CheckCXXCompilerFlag)
  check_cxx_compiler_flag(-mcrc32 KAHYPAR_HAS_CRC32)
  if(KAHYPAR_HAS_CRC32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mcrc32")
  endif()

  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -std=c++1y")
  endif()

  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -pthread -g3")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -mtune=native -march=native")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -mtune=native -march=native -g3 ") 

  if(ENABLE_PROFILE MATCHES ON)
    message(STATUS "Profiling activated")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DENABLE_PROFILE")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g3 -DENABLE_PROFILE -fno-omit-frame-pointer")
    set(PROFILE_FLAGS "-lprofiler")
  endif()

elseif(MSVC)
  string(REGEX REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  string(REGEX REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
  string(REGEX REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
  # Force to always compile with W4
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
  # raise warnings as errors
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")



  ### disable verbose warnings:
  # warning C4589: Constructor of abstract class '...' ignores initializer for
  # virtual base class '...' (false positive warnings)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4589")
  # warning C4505: 'function' : unreferenced local function has been removed
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4505")
  # warning C4127: conditional expression is constant
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4127")
  # warning C4458: declaration of '...' hides class member
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4458")
  # warning C4459: declaration of '...' hides global declaration
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4459")
  # warning C4702: unreachable code
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4702")
  # warning C4250: ABC inherits XYZ via dominance
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4250")
  # warning C4503: decorated name length exceeded, name was truncated
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4503")
  # disable lots of warnings about "unsecure" C runtime function
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
  # disable "The POSIX name for this item is deprecated. Instead, use the ISO C
  # and C++ conformant name.", Nope. We will not.
  add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
  # disable lots of warnings about "unsecure" STL functions
  add_definitions(-D_SCL_SECURE_NO_WARNINGS)
  # windef.h bizzarly defines min and max as macros, unless this is defined.
  add_definitions(-DNOMINMAX)
endif()

message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}")
message(STATUS "CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}")

include(gmock)
enable_testing()
add_subdirectory(kahypar/application)
add_subdirectory(tools)
add_subdirectory(tests)
