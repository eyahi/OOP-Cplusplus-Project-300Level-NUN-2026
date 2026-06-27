# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\ToDoApp_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\ToDoApp_autogen.dir\\ParseCache.txt"
  "ToDoApp_autogen"
  )
endif()
