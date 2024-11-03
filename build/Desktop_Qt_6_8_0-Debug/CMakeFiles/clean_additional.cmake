# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/XIXITextEditor_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/XIXITextEditor_autogen.dir/ParseCache.txt"
  "XIXITextEditor_autogen"
  )
endif()
