# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\QtMusicPlayer_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\QtMusicPlayer_autogen.dir\\ParseCache.txt"
  "QtMusicPlayer_autogen"
  )
endif()
