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
tps=$((npps*tpc))

seqArray=()
##########################################
k=0
for i in  `seq 0 $((npps-1))`; do
    kk=$((k))
    for j in `seq 0 $((numaNodes-1))`; do
        seqArray[i*$numaNodes+j]=$((kk))
	kk=$((kk+tps))
    done
    k=$((k+tpc))
done
##########################################

#echo ${seqArray[*]}
sequence=''
for p in `seq 0 $((  npm1  ))`; do
    sequence+=${seqArray[p]}','
done
sequence=${sequence%?}
#echo $sequence

if [ -n "$PGI" ]; then
    echo "Pgi Compiler"
elif [ -n "$INTEL_LICENSE_FILE" ]; then
    echo "Intel Compiler"
else
    echo "Gnu Compiler"
fi

rm -f Mpi_sm_Result.txt

for i in 1 `seq 2 2 $np`; do
    for j in  `seq 1 $nloops`; do
        echo number of processors: $i, run number: $j
        aprun -cc $sequence -n $i -N $i  ./secondDerivative $2 | grep finish >>  Mpi_sm_Result.txt
    done
done

mkdir -p ../../plots/$(hostname)/$1
cat Mpi_sm_Result.txt | awk '{}{print $2, $6+$11+$16, $6, $11, $16}{}'   | sort  -k1,1n -k2,2n   | awk 'BEGIN{ prev=-1} { if ($1 != prev) { print $0; prev=$1}  } END{}'  > ../../plots/$(hostname)/$1/MPI_sm.txt

cat Mpi_sm_Result.txt | awk '{}{print $2, $6, $11, $16}{}'  | sort  -k1,1n -k2,2n   | awk 'BEGIN{ prev=-1} { if ($1 != prev) { print $0; prev=$1}  } END{}' | awk '{}{print $1, $2}{}' > minX

cat Mpi_sm_Result.txt | awk '{}{print $2, $6, $11, $16}{}'  | sort  -k1,1n -k3,3n   | awk 'BEGIN{ prev=-1} { if ($1 != prev) { print $0; prev=$1}  } END{}' | awk '{}{ print $3} {}' > minY

cat Mpi_sm_Result.txt | awk '{}{print $2, $6, $11, $16}{}'  | sort  -k1,1n -k4,4n   | awk 'BEGIN{ prev=-1} { if ($1 != prev) { print $0; prev=$1}  } END{}' | awk '{}{ print $4} {}' > minZ

paste minX minY minZ > ../../plots/$(hostname)/$1/MPI_sm.txt

rm Mpi_sm_Result.txt minX minY minZ

