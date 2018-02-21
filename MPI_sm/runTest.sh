#!/bin/bash
if [ "$#" -ne 1 ]
then
  echo "Usage: $0  compilerResults"
  exit 1
fi

nloops=5
np=`grep -c ^processor /proc/cpuinfo`
#np=8

rm -f Mpi_sm_Result.txt
for i in  `seq 1 $np`; do

    for j in  `seq 1 $nloops`; do
        echo number of processors: $i
        mpiexec -n $i ./secondDerivative  | grep finish >>  Mpi_sm_Result.txt
    done
done

mkdir -p ../../plots/$(hostname)/$1
cat Mpi_sm_Result.txt | awk '{}{print $2, $6+$11+$16, $6, $11, $16}{}'   | sort  -k1,1n -k2,2n   | awk 'BEGIN{ prev=-1} { if ($1 != prev) { print $0; prev=$1}  } END{}'  > ../../plots/$(hostname)/$1/MPI_sm.txt

rm Mpi_sm_Result.txt

