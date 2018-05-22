#!/bin/bash

cd OpenMP/buildGnu
make clean; make -j
../runTest.sh gnu 5
cd ../../MPI_sm/buildGnu
make clean; make -j
../runTest.sh gnu 5
cd ../../

source setIcc intel64
source setImpi

cd OpenMP/buildIntel
make clean; make -j
../runTest.sh intel 5
cd ../../MPI_sm/buildIntel
make clean; make -j
../runTest.sh intel 5
cd ../../


source setPgi 18.4
source setPgiMpi 18.4

cd OpenMP/buildPgi
make clean; make -j
../runTest.sh pgi 5
cd ../../MPI_sm/buildPgi
make clean; make -j
../runTest.sh pgi 5
cd ../../

