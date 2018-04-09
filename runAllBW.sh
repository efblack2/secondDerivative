#!/bin/bash

cd OpenMP/buildGnu
make clean; make -j
../runTestBW.sh gnu 3
cd ../../MPI_sm/buildGnu
make clean; make -j
../runTestBW.sh gnu 3
cd ../../

source setIcc intel64
source setImpi

cd OpenMP/buildIntel
make clean; make -j
../runTestBW.sh intel 3
cd ../../MPI_sm/buildIntel
make clean; make -j
../runTestBW.sh intel 3
cd ../../


source setPgi 18.3
source setPgiMpi 18.3

cd OpenMP/buildPgi
make clean; make -j
../runTestBW.sh pgi 3
cd ../../MPI_sm/buildPgi
make clean; make -j
../runTestBW.sh pgi 3
cd ../../

