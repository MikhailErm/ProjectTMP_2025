# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\EchoServer_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\EchoServer_autogen.dir\\ParseCache.txt"
  "EchoServer_autogen"
  )
endif()
