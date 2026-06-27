# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CHESSGAMEFINAL_autogen"
  "CMakeFiles\\CHESSGAMEFINAL_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\CHESSGAMEFINAL_autogen.dir\\ParseCache.txt"
  )
endif()
