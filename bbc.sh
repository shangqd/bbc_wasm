#!/usr/bin/env bash

export BOOST_ROOT=~/eosio/1.8/src/boost_1_70_0

rm -rf build
mkdir build
cd build
#cmake -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang -DCMAKE_PREFIX_PATH=~/eosio/1.8  ..
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang -DCMAKE_PREFIX_PATH=~/eosio/1.8  ..
#cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=g++ -DCMAKE_C_COMPILER=gcc -DCMAKE_PREFIX_PATH=~/eosio/1.8  ..
