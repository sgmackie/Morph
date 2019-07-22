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
CompilerFlags="-g -gcodeview -pedantic"

# Set warning labels:
CommonWarnings="-Wall -Werror"

# Set Compiler optimsation level
CompilerOpt="-O0"

# Set logging flags
LogFlags=""

# Set profile flags
ProfileFlags=""

# Set Linux libraries
Libs="-lm"

# Run Clang compiler
clang++ ${CompilerFlags} ${CommonWarnings} ${CompilerOpt} ${LogFlags} ${ProfileFlags} ${Libs} ${PlatformFiles} -o ${Platform}

# Exit
popd > /dev/null