#!/bin/bash
# Debian: run chmod u+x build.sh if complaining about permissions
# Make sure EOL sequence is LF! Editing scripts on Windows can change that

# Stop at script errors
set -e

# Always build first
./build.sh 1 1

# Set profile output directory
Version="$4"
ProfileDir="../profile/${Version}"

# Make directory if it doesn't exist
if [ ! -d "${ProfileDir}" ]; then
	mkdir -p ${ProfileDir}
fi

# Bash Timer
# Have to call from the benchmark script itself
./benchmark.sh $1 "time -p -o time_${Version}.txt"

# System call tracer
./benchmark.sh $1 "strace -c -o trace_${Version}.txt"

# Performance counters
./benchmark.sh $1 "perf stat -c -d -B -o stat_${Version}.txt"

# Profiler
./benchmark.sh $1 "sudo perf record -g -d -s -T -P -o perf_${Version}"

# GUI output
if [ $2 -eq 1 ]
then
    pushd ../build/linux
    hotspot perf_${Version}
    popd
fi

# Memory checking
./benchmark.sh $1 "valgrind --track-origins=yes --time-stamp=yes --log-file=memory_${Version}.txt"

# Chache misses
./benchmark.sh $1 "valgrind -q --tool=cachegrind --trace-children=yes --branch-sim=yes --cachegrind-out-file=cache_${Version}.out"
pushd ../build/linux

# Annotate data
cg_annotate cache_${Version}.out /mnt/FC663B45663AFFC6/Projects/Current/Programming/Morph/project/code/morph.cpp /mnt/FC663B45663AFFC6/Projects/Current/Programming/Morph/project/code/wav.cpp /mnt/FC663B45663AFFC6/Projects/Current/Programming/Morph/project/code/file.cpp /mnt/FC663B45663AFFC6/Projects/Current/Programming/Morph/project/code/source.cpp /mnt/FC663B45663AFFC6/Projects/Current/Programming/Morph/project/code/dsp.cpp --auto=yes > cache_report_${Version}.txt

# GUI output
if [ $3 -eq 1 ]
then
    kcachegrind cache_${Version}.out 
fi
popd

# Copy results to profile
rsync -a --backup --backup-dir "$(date +"%Y_%m_%d_%I_%M_%p")" "../build/linux/" "${ProfileDir}"