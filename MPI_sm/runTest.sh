#!/bin/bash
if [ "$#" -lt 1 ]
then
  echo "Usage: $0  compilerResults [numberOfIterations]"
  exit 1
fi

nloops=3

npt=`grep -c ^processor /proc/cpuinfo`
slots=`numactl -H | grep available | awk '{}{print $2}{}'`
np="$(($npt / 1))"
npps="$(($np / $slots))"
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

echo $sequence
if [ -n "$LM_LICENSE_FILE" ]; then
    echo "Pgi Compiler"
elif [ -n "$INTEL_LICENSE_FILE" ]; then
    echo "Intel Compiler"
else
    echo "Gnu Compiler"
fi


rm -f Mpi_sm_Result.txt
for i in 1 `seq 2 2 $np`; do

    for j in  `seq 1 $nloops`; do
        echo number of processors: $i
        mpiexec -n $i ./secondDerivative $2 | grep finish >>  Mpi_sm_Result.txt
    done
done

mkdir -p ../../plots/$(hostname)/$1
cat Mpi_sm_Result.txt | awk '{}{print $2, $6+$11+$16, $6, $11, $16}{}'   | sort  -k1,1n -k2,2n   | awk 'BEGIN{ prev=-1} { if ($1 != prev) { print $0; prev=$1}  } END{}'  > ../../plots/$(hostname)/$1/MPI_sm.txt

cat Mpi_sm_Result.txt | awk '{}{print $2, $6, $11, $16}{}'  | sort  -k1,1n -k2,2n   | awk 'BEGIN{ prev=-1} { if ($1 != prev) { print $0; prev=$1}  } END{}' | awk '{}{print $1, $2}{}' > minX

cat Mpi_sm_Result.txt | awk '{}{print $2, $6, $11, $16}{}'  | sort  -k1,1n -k3,3n   | awk 'BEGIN{ prev=-1} { if ($1 != prev) { print $0; prev=$1}  } END{}' | awk '{}{ print $3} {}' > minY

cat Mpi_sm_Result.txt | awk '{}{print $2, $6, $11, $16}{}'  | sort  -k1,1n -k4,4n   | awk 'BEGIN{ prev=-1} { if ($1 != prev) { print $0; prev=$1}  } END{}' | awk '{}{ print $4} {}' > minZ

paste minX minY minZ > ../../plots/$(hostname)/$1/MPI_sm.txt

rm Mpi_sm_Result.txt minX minY minZ

