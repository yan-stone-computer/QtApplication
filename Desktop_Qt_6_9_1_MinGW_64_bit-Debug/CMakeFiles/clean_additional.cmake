# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\presonal_to_do_list_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\presonal_to_do_list_autogen.dir\\ParseCache.txt"
  "presonal_to_do_list_autogen"
  )
endif()
