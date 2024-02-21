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


cmake --build build/${TARGET}/${ARCH}/${CONFIG}

