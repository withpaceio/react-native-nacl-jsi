#!/bin/bash

source_archive='libsodium-1.0.19.tar.gz'
source_directory='libsodium-stable'
build_directory='build'

# Go to build script directory
cd libsodium/

# Download and verify the source
rm -f $source_archive
curl https://download.libsodium.org/libsodium/releases/$source_archive > $source_archive
minisign -p libsodium.org.minisign.pub -Vm $source_archive || exit 1

# Extract the source
rm -rf $source_directory
tar -zxf $source_archive

cd $source_directory
platform=`uname`

if [ "$platform" == 'Darwin' ]; then
  IOS_VERSION_MIN=12.0.0 dist-build/apple-xcframework.sh
fi

NDK_PLATFORM=android-21 dist-build/android-armv7-a.sh
NDK_PLATFORM=android-21 dist-build/android-armv8-a.sh
NDK_PLATFORM=android-21 dist-build/android-x86.sh
NDK_PLATFORM=android-21 dist-build/android-x86_64.sh

cd ..

# Move the compiled libraries
mkdir -p $build_directory
rm -rf $build_directory/*

for directory in $source_directory/libsodium-android-*
do
  mv $directory $build_directory/
done

if [ "$platform" == 'Darwin' ]; then
  mv $source_directory/libsodium-apple $build_directory/
fi

# Create the precompiled archive
tar -zcvf build.tar.gz $build_directory

# Cleanup
rm $source_archive
rm -rf $source_directory
