message(STATUS "downloading...
     src='http://downloads.sourceforge.net/project/boost/boost/1.56.0/boost_1_56_0.tar.bz2'
     dst='/tmp//boost_1_56_0.tar.bz2'
     timeout='none'")




file(DOWNLOAD
  "http://downloads.sourceforge.net/project/boost/boost/1.56.0/boost_1_56_0.tar.bz2"
  "/tmp//boost_1_56_0.tar.bz2"
  SHOW_PROGRESS
  # no TIMEOUT
  STATUS status
  LOG log)

list(GET status 0 status_code)
list(GET status 1 status_string)

if(NOT status_code EQUAL 0)
  message(FATAL_ERROR "error: downloading 'http://downloads.sourceforge.net/project/boost/boost/1.56.0/boost_1_56_0.tar.bz2' failed
  status_code: ${status_code}
  status_string: ${status_string}
  log: ${log}
")
endif()

message(STATUS "downloading... done")
