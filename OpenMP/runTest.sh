#!/bin/bash
if [ "$#" -ne 2 ] 
then
  echo "Usage: $0 derivativeLoops  compilerResults"
  exit 1
fi

#nloops=1
np=`grep -c ^processor /proc/cpuinfo`
#np=4

rm -f OpenMp_Result.txt
for i in  `seq 1 $np`; do
    export OMP_NUM_THREADS=$i
    #for j in  `seq 1 $nloops`; do
        echo number of threads: $i
        ./secondDerivative $1 | grep finish >>  OpenMp_Result.txt
    #done
done

mkdir -p ../../plots/$(hostname)/$2
cat OpenMp_Result.txt | awk '{}{print $2, $6, $11, $16}{}'  > ../../plots/$(hostname)/$2/OpenMP.txt

rm OpenMp_Result.txt

