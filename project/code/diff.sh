#!/bin/bash
# Debian: run chmod u+x build.sh if complaining about permissions
# Make sure EOL sequence is LF! Editing scripts on Windows can change that

# Stop at script errors
set -e

# Move to profile path
SourceA="$1"
SourceB="$2"
pushd ../profile

# Launch meld
meld -a -n --label="Time"   ./${SourceA}/time_${SourceA}.txt            ./${SourceB}/time_${SourceB}.txt            &
meld -a -n --label="Trace"  ./${SourceA}/trace_${SourceA}.txt           ./${SourceB}/trace_${SourceB}.txt           &
meld -a -n --label="Stat"   ./${SourceA}/stat_${SourceA}.txt            ./${SourceB}/stat_${SourceB}.txt            &
meld -a -n --label="Memory" ./${SourceA}/memory_${SourceA}.txt          ./${SourceB}/memory_${SourceB}.txt          &
meld -a -n --label="Cache"  ./${SourceA}/cache_report_${SourceA}.txt    ./${SourceB}/cache_report_${SourceB}.txt    &

# exit
popd