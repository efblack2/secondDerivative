#!/bin/bash

cd OpenMP/buildGnu
make clean; make
../runTest.sh gnu 3
cd ../../MPI_sm/buildGnu
make clean; make
../runTest.sh gnu 3
cd ../../

source setIcc intel64
source setImpi

cd OpenMP/buildIntel
make clean; make
../runTest.sh intel 3
cd ../../MPI_sm/buildIntel
make clean; make
../runTest.sh intel 3
cd ../../


source setPgi 18.1
source setPgiMpi 18.10

cd OpenMP/buildPgi
make clean; make
../runTest.sh pgi 3
cd ../../MPI_sm/buildPgi
make clean; make
../runTest.sh pgi 3
cd ../../

