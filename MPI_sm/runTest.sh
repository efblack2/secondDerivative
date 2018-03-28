#!/bin/bash
if [ "$#" -lt 1 ]
then
  echo "Usage: $0  compilerResults [numberOfIterations]"
  exit 1
fi

nloops=3

# Determining MPI implementation and binding options #
MPI=`mpiexec --version | head -1 | awk '{print $1}' ` 

if [ "$MPI" == "HYDRA" ]; then
    echo "MPICH"
    bindings="--bind-to socket"
    export HYDRA_TOPO_DEBUG=1
elif [ "$MPI" == "Intel(R)" ]; then
    echo "Intel MPI"
    bindings="-genv I_MPI_PIN_DOMAIN=core -genv I_MPI_PIN_ORDER=scatter -genv I_MPI_DEBUG=4"
elif [ "$MPI" == "mpiexec" ]; then
    echo "open-mpi"
    bindings="--bind-to core --report-bindings"
fi
# end of Determining MPI implementation and binding options #

npt=`grep -c ^processor /proc/cpuinfo`
numaNodes=`lscpu | grep "NUMA node(s):" | awk '{}{print $3}{}'`
tpc=`lscpu | grep "Thread(s) per core:" | awk '{}{print $4}{}'`
np="$(($npt / $tpc))"
npps="$(($np / $numaNodes))"
npm1="$(($np - 1))"

if [ -n "$PGI" ]; then
    echo "Pgi Compiler"
elif [ -n "$INTEL_LICENSE_FILE" ]; then
    echo "Intel Compiler"
    #np=15
    #npps="$(($np / $numaNodes))"
    #npm1="$(($np - 1))"
else
    echo "Gnu Compiler"
fi

rm -f Mpi_sm_Result.txt
for i in 1 `seq 2 2 $np`; do

    for j in  `seq 1 $nloops`; do
        echo number of processors: $i, run number: $j
        mpiexec $bindings -n $i ./secondDerivative $2 | grep finish >>  Mpi_sm_Result.txt
    done
done

mkdir -p ../../plots/$(hostname)/$1
cat Mpi_sm_Result.txt | awk '{}{print $2, $6+$11+$16, $6, $11, $16}{}'   | sort  -k1,1n -k2,2n   | awk 'BEGIN{ prev=-1} { if ($1 != prev) { print $0; prev=$1}  } END{}'  > ../../plots/$(hostname)/$1/MPI_sm.txt

cat Mpi_sm_Result.txt | awk '{}{print $2, $6, $11, $16}{}'  | sort  -k1,1n -k2,2n   | awk 'BEGIN{ prev=-1} { if ($1 != prev) { print $0; prev=$1}  } END{}' | awk '{}{print $1, $2}{}' > minX

cat Mpi_sm_Result.txt | awk '{}{print $2, $6, $11, $16}{}'  | sort  -k1,1n -k3,3n   | awk 'BEGIN{ prev=-1} { if ($1 != prev) { print $0; prev=$1}  } END{}' | awk '{}{ print $3} {}' > minY

cat Mpi_sm_Result.txt | awk '{}{print $2, $6, $11, $16}{}'  | sort  -k1,1n -k4,4n   | awk 'BEGIN{ prev=-1} { if ($1 != prev) { print $0; prev=$1}  } END{}' | awk '{}{ print $4} {}' > minZ

paste minX minY minZ > ../../plots/$(hostname)/$1/MPI_sm.txt

rm Mpi_sm_Result.txt minX minY minZ

