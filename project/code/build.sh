#!/bin/bash
# Debian: run chmod u+x build.sh if complaining about permissions
# Make sure EOL sequence is LF! Editing scripts on Windows can change that

# Stop at script errors
set -e

# Set name of the main .cpp file for building
Platform=morph
CurDir=$(pwd)

# Set build directory
BuildDir="${CurDir}/../build/linux"

# Make build directory if it doesn't exist
if [ ! -d "${BuildDir}" ]; then
	mkdir -p ${BuildDir}
fi

# Move to build directory
pushd ${BuildDir} > /dev/null

# Set compiler arguments
PlatformFiles="${CurDir}/${Platform}.cpp"

# Set compiler flags:
if [ $1 -eq 1 ]
then
    echo "gcc"
    CompilerFlags="-g -pedantic"
else
    echo "Clang"
    CompilerFlags="-g -gcodeview -pedantic"    
fi

# Set warning labels:
if [ $1 -eq 1 ]
then
    CommonWarnings="-Wall -Werror -Wno-error=multichar -Wno-error=unused-result"
else
    CommonWarnings="-Wall -Werror -Wno-four-char-constants"
fi

# Set Compiler optimsation level
if [ $2 -eq 1 ]
then
    echo "Optimised Build"
    CompilerOpt="-O3 -march=native"    
else
    echo "Debug Build"
    CompilerOpt="-O0"
fi

# Set logging flags
LogFlags=""

# Set profile flags
ProfileFlags=""

# Set Linux libraries
Libs="-lm"

# Run compiler
# Set warning labels:
if [ $1 -eq 1 ]
then
    g++ ${CompilerFlags} ${CommonWarnings} ${CompilerOpt} ${LogFlags} ${ProfileFlags} ${Libs} ${PlatformFiles} -o ${Platform}
else
    clang++-8 ${CompilerFlags} ${CommonWarnings} ${CompilerOpt} ${LogFlags} ${ProfileFlags} ${Libs} ${PlatformFiles} -o ${Platform}
fi

# Exit
popd > /dev/null
