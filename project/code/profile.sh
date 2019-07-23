#!/bin/bash
# Debian: run chmod u+x build.sh if complaining about permissions
# Make sure EOL sequence is LF! Editing scripts on Windows can change that

# Stop at script errors
set -e

# Set profile output directory
Version="$4"
ProfileDir="../profile/${Version}"

# Make build directory if it doesn't exist
if [ ! -d "${ProfileDir}" ]; then
	mkdir -p ${ProfileDir}
fi

# Bash Timer
./benchmark.sh $1 time

# System call tracer
./benchmark.sh $1 "strace -c"

# Performance counters
./benchmark.sh $1 "perf stat -c"

# Profiler
./benchmark.sh $1 "perf record -o perf_${Version}"

# GUI output
if [ $2 -eq 1 ]
then
    pushd ../build/linux
    perf report
    popd
fi

# Chache misses
./benchmark.sh $1 "valgrind --tool=cachegrind --branch-sim=yes --cachegrind-out-file=cache_${Version}.out"
pushd ../build/linux
cg_annotate cache_${Version}.out /mnt/FC663B45663AFFC6/Projects/Current/Programming/Morph/project/code/morph.cpp /mnt/FC663B45663AFFC6/Projects/Current/Programming/Morph/project/code/wav.cpp /mnt/FC663B45663AFFC6/Projects/Current/Programming/Morph/project/code/file.cpp /mnt/FC663B45663AFFC6/Projects/Current/Programming/Morph/project/code/source.cpp --auto=yes

# GUI output
if [ $3 -eq 1 ]
then
    kcachegrind cache_${Version}.out 
fi
popd