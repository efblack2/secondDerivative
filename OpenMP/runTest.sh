#!/bin/bash
if [ "$#" -lt 1 ]
then
  echo "Usage: $0  compilerResults [numberOfIterations]"
  exit 1
fi

nloops=3

npt=`grep -c ^processor /proc/cpuinfo`
numaNodes=`lscpu | grep "NUMA node(s):" | awk '{}{print $3}{}'`
tpc=`lscpu | grep "Thread(s) per core:" | awk '{}{print $4}{}'`
np="$(($npt / $tpc))"
npps="$(($np / $numaNodes))"
npm1="$(($np - 1))"


seqArray=()
##########################################
for i in  `seq 0 $((npps-1))`; do
    for j in `seq 0 $((numaNodes-1))`; do
        seqArray[i*$numaNodes+j]=$((i+j*npps))
    done
done
##########################################
#for i in `seq 0 $((npm1))`; do
#    seqArray[i]=$i
#done
##########################################
#for i in `seq 0 2 $((npm1))`; do
#    sequence+=$i','
#done
#for i in `seq 1 2 $((npm1))`; do
#    sequence+=$i','
#done
##########################################

#echo ${seqArray[*]}
sequence=''
for p in `seq 0 $((  npm1  ))`; do
    sequence+=${seqArray[p]}','
done
sequence=${sequence%?}
export OMP_DISPLAY_ENV=true
if [ -n "$PGI" ]; then
    echo "Pgi Compiler"
    export MP_BLIST=$sequence
    export MP_BIND="yes"
    #export MP_BLIST="0-$npm1"
    echo $MP_BLIST
elif [ -n "$INTEL_LICENSE_FILE" ]; then
    echo "Intel Compiler"
    #np=15
    #npps="$(($np / $numaNodes))"
    #npm1="$(($np - 1))"
    export OMP_PROC_BIND=spread
    export OMP_PLACES=cores
    #export KMP_AFFINITY=scatter
    # needed to use dissabled in Blue waters
    #export KMP_AFFINITY=disabled
else
    echo "Gnu Compiler"
    export OMP_PROC_BIND=spread
    export OMP_PLACES=sockets
fi

rm -f OpenMp_Result.txt
for i in 1 `seq 2 2 $np`; do
    export OMP_NUM_THREADS=$i
    for j in  `seq 1 $nloops`; do
        echo number of threads: $i, run number: $j
        ./secondDerivative $2 | grep finish >>  OpenMp_Result.txt
    done
done

mkdir -p ../../plots/$(hostname)/$1

cat OpenMp_Result.txt | awk '{}{print $2, $6, $11, $16}{}'  | sort  -k1,1n -k2,2n   | awk 'BEGIN{ prev=-1} { if ($1 != prev) { print $0; prev=$1}  } END{}' | awk '{}{print $1, $2}{}' > minX

cat OpenMp_Result.txt | awk '{}{print $2, $6, $11, $16}{}'  | sort  -k1,1n -k3,3n   | awk 'BEGIN{ prev=-1} { if ($1 != prev) { print $0; prev=$1}  } END{}' | awk '{}{ print $3} {}' > minY

cat OpenMp_Result.txt | awk '{}{print $2, $6, $11, $16}{}'  | sort  -k1,1n -k4,4n   | awk 'BEGIN{ prev=-1} { if ($1 != prev) { print $0; prev=$1}  } END{}' | awk '{}{ print $4} {}' > minZ

paste minX minY minZ >  ../../plots/$(hostname)/$1/OpenMP.txt

rm OpenMp_Result.txt minX minY minZ
