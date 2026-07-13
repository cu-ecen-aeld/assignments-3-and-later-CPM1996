#!/bin/bash

if [[ $# -ne 2 ]]; then
    echo "Usage: $0 /path/to/dir 'string to write'"
    exit 1
fi

filePath=$1
fileContent=$2

dirname $filePath

dirPath=$(dirname $filePath)
mkdir -p $dirPath || exit 1

echo $fileContent > $filePath
exit $?
