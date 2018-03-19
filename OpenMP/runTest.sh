#!/bin/bash
if [ "$#" -lt 1 ]
then
  echo "Usage: $0  compilerResults [numberOfIterations]"
  exit 1
fi

nloops=3


npt=`grep -c ^processor /proc/cpuinfo`
slots=`numactl -H | grep available | awk '{}{print $2}{}'`
np="$(($npt / $slots))"
npps="$(($np / 2))"
npm1="$(($np - 1))"

sequence=''
##########################################
for i in  `seq 0 $((npps-1))`; do
    sequence+=$i','
    sequence+=$(($i +  $((np/2))  ))','
done
##########################################
#for i in `seq 0 $((npm1))`; do
#    sequence+=$i','
#done
##########################################
#for i in `seq 0 2 $((npm1))`; do
#    sequence+=$i','
#done
#for i in `seq 1 2 $((npm1))`; do
#    sequence+=$i','
#done
##########################################

sequence=${sequence%?}
if [ -n "$LM_LICENSE_FILE" ]; then
    echo "Pgi Compiler"
    export MP_BIND="yes"
    export MP_BLIST=$sequence
    #export MP_BLIST="0-$npm1"
    echo $MP_BLIST
elif [ -n "$INTEL_LICENSE_FILE" ]; then
    echo "Intel Compiler"
    export OMP_PLACES=sockets
    export OMP_PROC_BIND=true
    #export KMP_AFFINITY=scatter
    # needed to use dissabled in Blue waters
    #export KMP_AFFINITY=disabled
else
    echo "Gnu Compiler"
    #export OMP_PLACES=sockets
    #export OMP_PROC_BIND=true
    export GOMP_CPU_AFFINITY=$sequence
    #export GOMP_CPU_AFFINITY="0-$npm1"
fi

echo $sequence

rm -f OpenMp_Result.txt
for i in  `seq 1 $np`; do
    export OMP_NUM_THREADS=$i
    for j in  `seq 1 $nloops`; do
        echo number of threads: $i
        ./secondDerivative $2 | grep finish >>  OpenMp_Result.txt
    done
done

mkdir -p ../../plots/$(hostname)/$1
cat OpenMp_Result.txt | awk '{}{print $2, $6+$11+$16, $6, $11, $16}{}'   | sort  -k1,1n -k2,2n   | awk 'BEGIN{ prev=-1} { if ($1 != prev) { print $0; prev=$1}  } END{}'  > ../../plots/$(hostname)/$1/OpenMP.txt

rm OpenMp_Result.txt

