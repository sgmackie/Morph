#!/bin/bash
# Debian: run chmod u+x build.sh if complaining about permissions
# Make sure EOL sequence is LF! Editing scripts on Windows can change that

# Stop at script errors
set -e

# Move to build path
pushd ../build/linux

# Create benchmark paths
Output1="/mnt/FC663B45663AFFC6/Projects/Current/Programming/Morph/project/data/Output_1"
Output5="/mnt/FC663B45663AFFC6/Projects/Current/Programming/Morph/project/data/Output_5"
Output10="/mnt/FC663B45663AFFC6/Projects/Current/Programming/Morph/project/data/Output_10"

# Delete last outputs
rm -d -f ${Output_1}
rm -d -f ${Output_5}
rm -d -f ${Output_10}

# Make build directory if it doesn't exist
if [ ! -d "${Output1}" ]; then
	mkdir -p ${Output1}
fi
if [ ! -d "${Output5}" ]; then
	mkdir -p ${Output5}
fi
if [ ! -d "${Output10}" ]; then
	mkdir -p ${Output10}
fi

# 1 Morph
if [ $1 -eq 1 ] 
then
    $2 ./morph "/mnt/FC663B45663AFFC6/Projects/Current/Programming/Morph/project/data/BOOM_Bird" "/mnt/FC663B45663AFFC6/Projects/Current/Programming/Morph/project/data/BOOM_Tiger" 1 ${Output1} 1 0.5 0.9
    echo
# 5 Morph
elif [ $1 -eq 5 ] 
then
    $2 ./morph "/mnt/FC663B45663AFFC6/Projects/Current/Programming/Morph/project/data/BOOM_Bird" "/mnt/FC663B45663AFFC6/Projects/Current/Programming/Morph/project/data/BOOM_Tiger" 5 ${Output5} 1 0.5 0.9
    echo
# 10 Morph    
else
    $2 ./morph "/mnt/FC663B45663AFFC6/Projects/Current/Programming/Morph/project/data/BOOM_Bird" "/mnt/FC663B45663AFFC6/Projects/Current/Programming/Morph/project/data/BOOM_Tiger" 10 ${Output10} 1 0.5 0.9
    echo
fi

popd