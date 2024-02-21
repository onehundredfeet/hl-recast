#!/bin/sh
ARCH=x86_64
#ARCH=arm64
PROJECT=recast
BUILDER="ninja"
TARGET=hl
#TARGET=jvm
CONFIG=Debug

while getopts b:c:a:t: flag
do
    case "${flag}" in
        b) BUILDER=${OPTARG};;
        c) CONFIG=${OPTARG};;
        a) ARCH=${OPTARG};;
        t) TARGET=${OPTARG};;
    esac
done



cmake --install build/${TARGET}/${ARCH}/${CONFIG}
mkdir -p bin
rm -f ${PROJECT}.dylib
cp -f build/${TARGET}/${ARCH}/${CONFIG}/${PROJECT}.* .

