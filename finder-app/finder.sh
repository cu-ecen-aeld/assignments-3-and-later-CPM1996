#!/bin/bash

numFiles=0
numMatches=0

function searchDir() {
    currentDirPath=$1

    for file in $currentDirPath/*; do
        if [[ -f $file ]]; then
            ((numFiles++))
            numCurrentMatches=$(cat $file | grep -c $searchstr)
            ((numMatches += numCurrentMatches))
        elif [[ -d $file ]]; then
            searchDir $file
        fi
    done
}

if [[ $# -ne 2 ]]; then
    echo "Usage: $0 /path/to/dir 'string to search'"
    exit 1
fi

filesdir=$1
searchstr=$2

if [[ ! -d $filesdir ]]; then
    echo "Argument $filesdir is not a valid path to a directory"
    exit 1
fi

searchDir $filesdir

echo "The number of files are $numFiles and the number of matching lines are $numMatches"

exit 0
