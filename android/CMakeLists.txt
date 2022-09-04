cmake_minimum_required(VERSION 3.9.0)

set (PACKAGE_NAME "react-native-nacl-jsi")
set (CMAKE_VERBOSE_MAKEFILE ON)
set (CMAKE_CXX_STANDARD 11)

set(ARCH_DIR ${ANDROID_ARCH_NAME})

if(ANDROID_ABI STREQUAL "armeabi-v7a")
  set(ARCH_DIR "armv7-a")
elseif ( ANDROID_ABI STREQUAL "arm64-v8a")
  set( ARCH_DIR "armv8-a" )
elseif ( ANDROID_ABI STREQUAL "x86")
  set ( ARCH_DIR "i686" )
elseif ( ANDROID_ABI STREQUAL "x86_64")
  set ( ARCH_DIR "westmere" )
endif()

add_library(sodium SHARED IMPORTED)
include_directories( ${PROJECT_SOURCE_DIR}/../libsodium/libsodium-android-${ARCH_DIR}/include/ )
set_target_properties( sodium PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/../libsodium/libsodium-android-${ARCH_DIR}/lib/libsodium.so )
file(COPY ${PROJECT_SOURCE_DIR}/../libsodium/libsodium-android-${ARCH_DIR}/lib/libsodium.so DESTINATION "${PROJECT_SOURCE_DIR}/lib/${ANDROID_ABI}")

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

file (GLOB LIBRN_DIR "${PREBUILT_DIR}/${ANDROID_ABI}")

target_link_libraries(
        cpp
        sodium
)
