file(REMOVE_RECURSE
  "libmini_boost.pdb"
  "libmini_boost.a"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/mini_boost.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
