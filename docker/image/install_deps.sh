#!/bin/sh

set -e -x

echo "Installing dependencies"

apt-get update

apt-get install -y software-properties-common

add-apt-repository -y ppa:ubuntu-toolchain-r/test

apt-get install -y wget rsync

# LLVM
wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add -
add-apt-repository -y "deb http://apt.llvm.org/jammy/ llvm-toolchain-jammy-17 main"

# CMake
wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc | apt-key add -
apt-add-repository 'deb https://apt.kitware.com/ubuntu/ jammy main'

apt-get update

apt-get install -y \
	ssh \
	vim \
	make \
	cmake \
	build-essential \
	git \
	g++-14 \
	clang-17 \
	clang-format-17 \
	clang-tidy-17 \
	libasio-dev \
	libc++-17-dev \
	libc++abi-17-dev \
	libclang-rt-17-dev \
	clangd-17 \
	lldb-17 \
	gdb \
