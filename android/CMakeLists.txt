cmake_minimum_required(VERSION 3.9.0)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 17)

set(LIBSODIUM_DIR ${CMAKE_CURRENT_LIST_DIR}/../libsodium/build)

if (${ANDROID_ABI} STREQUAL "armeabi-v7a")
  set(LIBSODIUM_DIR ${LIBSODIUM_DIR}/libsodium-android-armv7-a)
elseif (${ANDROID_ABI} STREQUAL "arm64-v8a")
  set(LIBSODIUM_DIR ${LIBSODIUM_DIR}/libsodium-android-armv8-a+crypto)
elseif (${ANDROID_ABI} STREQUAL "x86")
  set (LIBSODIUM_DIR ${LIBSODIUM_DIR}/libsodium-android-i686)
elseif (${ANDROID_ABI} STREQUAL "x86_64")
  set (LIBSODIUM_DIR ${LIBSODIUM_DIR}/libsodium-android-westmere)
endif()

add_library(sodium SHARED IMPORTED)
include_directories(${LIBSODIUM_DIR}/include/)
set_target_properties(sodium PROPERTIES IMPORTED_LOCATION ${LIBSODIUM_DIR}/lib/libsodium.so)

include_directories(
  ../cpp
  "${NODE_MODULES_DIR}/react-native/React"
  "${NODE_MODULES_DIR}/react-native/React/Base"
  "${NODE_MODULES_DIR}/react-native/ReactCommon/jsi"
)

add_library(nacljsi
  SHARED
  "${NODE_MODULES_DIR}/react-native/ReactCommon/jsi/jsi/jsi.cpp"
  ../cpp/react-native-nacl-jsi.cpp
  ../cpp/aes.cpp
  ../cpp/argon2id.cpp
  ../cpp/box.cpp
  ../cpp/constants.cpp
  ../cpp/random.cpp
  ../cpp/secretbox.cpp
  ../cpp/sign.cpp
  ../cpp/utils.cpp
  cpp-adapter.cpp
)

target_link_libraries(nacljsi sodium)
