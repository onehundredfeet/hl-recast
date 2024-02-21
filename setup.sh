#!/bin/sh
ARCH=x86_64
#ARCH=arm64
PROJECT=recast
BUILDER="ninja"
TARGET=hl
#TARGET=jvm
CONFIG=Debug
IDL_PATH="../hl-idl"

while getopts p:b:c:a:t:i: flag
do
    case "${flag}" in
        p) PROJECT=${OPTARG};;
        b) BUILDER=${OPTARG};;
        c) CONFIG=${OPTARG};;
        a) ARCH=${OPTARG};;
        t) TARGET=${OPTARG};;
	i) IDL_PATH=${OPTARG};;
    esac
done

GENERATOR=${BUILDER}

if [ ${BUILDER} = "make" ]; then
    GENERATOR="Unix Makefiles"
fi

if [ ${BUILDER} = "ninja" ]; then
    GENERATOR="Ninja"
fi



mkdir -p build/${TARGET}/${ARCH}/${CONFIG}
mkdir -p installed/${TARGET}/${ARCH}/${CONFIG}

mkdir -p build/${TARGET}/${ARCH}/${CONFIG}
cmake -G"${GENERATOR}" -DTARGET_ARCH=${ARCH} -DPATH_TO_IDL=${IDL_PATH} -DTARGET_HOST=${TARGET} -DCMAKE_BUILD_TYPE=${CONFIG} -DCMAKE_INSTALL_PREFIX=installed/${TARGET}/${ARCH}/${CONFIG} -B build/${TARGET}/${ARCH}/${CONFIG} 

