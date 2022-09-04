cmake_minimum_required(VERSION 3.9.0)

set (PACKAGE_NAME "react-native-nacl-jsi")
set (CMAKE_VERBOSE_MAKEFILE ON)
set (CMAKE_CXX_STANDARD 11)

file(TO_CMAKE_PATH "${NODE_MODULES_DIR}/react-native/ReactCommon/jsi/jsi/jsi.cpp" libPath)

include_directories(
  ../cpp
  "${NODE_MODULES_DIR}/react-native/React"
  "${NODE_MODULES_DIR}/react-native/React/Base"
  "${NODE_MODULES_DIR}/react-native/ReactCommon/jsi"
)

add_library(cpp
  SHARED
  ${libPath}
  ../cpp/react-native-nacl-jsi.cpp
  cpp-adapter.cpp
)

target_link_libraries(cpp)
