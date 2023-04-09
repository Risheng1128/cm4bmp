#!/usr/bin/env bash

# GNU toolchain for ARM 32 bit processor
GCC_VER=12.2.rel1
TOOLCHAIN=arm-none-eabi
TOOLCHAIN_URL=https://developer.arm.com/-/media/Files/downloads/gnu

# print commands and arguments as they are executed.
set -x

# download ARM toolchain
wget -q \
    ${TOOLCHAIN_URL}/${GCC_VER}/binrel/arm-gnu-toolchain-${GCC_VER}-x86_64-${TOOLCHAIN}.tar.xz

tar Jxvf arm-gnu-toolchain-${GCC_VER}-x86_64-${TOOLCHAIN}.tar.xz

export PATH=`pwd`/arm-gnu-toolchain-${GCC_VER}-x86_64-${TOOLCHAIN}/bin:$PATH

# build
make
make clean

# test bit band
make OUT=`pwd`/build BIN=bit-band INPUT_C_SRC=`pwd`/test/bit-band/bit-band.c
make clean

# test clock configuration
make OUT=`pwd`/build BIN=exercise1 INPUT_C_SRC=`pwd`/test/clock/exercise1.c INPUT_C_INC=-I`pwd`/test/clock
make clean
make OUT=`pwd`/build BIN=exercise2 INPUT_C_SRC=`pwd`/test/clock/exercise2.c INPUT_C_INC=-I`pwd`/test/clock
make clean
make OUT=`pwd`/build BIN=exercise3 INPUT_C_SRC=`pwd`/test/clock/exercise3.c INPUT_C_INC=-I`pwd`/test/clock
make clean
make OUT=`pwd`/build BIN=exercise4 INPUT_C_SRC=`pwd`/test/clock/exercise4.c INPUT_C_INC=-I`pwd`/test/clock
make clean
make OUT=`pwd`/build BIN=exercise5 INPUT_C_SRC=`pwd`/test/clock/exercise5.c INPUT_C_INC=-I`pwd`/test/clock
make clean
make OUT=`pwd`/build BIN=exercise6 INPUT_C_SRC=`pwd`/test/clock/exercise6.c INPUT_C_INC=-I`pwd`/test/clock
make clean

# test inline assembly
make OUT=`pwd`/build BIN=inline-asm INPUT_C_SRC=`pwd`/test/inline/inline-asm.c
make clean

# test stack pointer configuration
make OUT=`pwd`/build BIN=stack INPUT_C_SRC=`pwd`/test/stack/stack.c
make clean