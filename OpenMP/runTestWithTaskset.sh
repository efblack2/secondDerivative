#!/bin/bash
if [ "$#" -lt 1 ] 
then
  echo "Usage: $0  compilerResults [numberOfIterations]"
  exit 1
fi

nloops=5
# needed by intel compiler in Blue Waters
export KMP_AFFINITY=disabled
# needed by intel compiler in Blue Waters

npt=`grep -c ^processor /proc/cpuinfo`
np="$(($npt / 1))"

rm -f OpenMp_Result.txt
for i in  `seq 1 $np`; do
    npm1=$(($i-1))
    export OMP_NUM_THREADS=$i
    for j in  `seq 1 $nloops`; do
        echo number of threads: $i, run number: $j , taskset 0-$npm1 
        taskset -c 0-$npm1  ./secondDerivative $2 | grep finish >>  OpenMp_Result.txt
    done
done


mkdir -p ../../plots/$(hostname)/$1
cat OpenMp_Result.txt | awk '{}{print $2, $6+$11+$16, $6, $11, $16}{}'   | sort  -k1,1n -k2,2n   | awk 'BEGIN{ prev=-1} { if ($1 != prev) { print $0; prev=$1}  } END{}'  > ../../plots/$(hostname)/$1/OpenMP_Pin.txt

rm OpenMp_Result.txt

