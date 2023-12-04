#!/bin/sh

filesdir=$1
keySearch=$2
status=$?

# Return variable
WITHERROR=1
RUNCOMPLETLY=0
# Error messages
ERRORARGU="ERROR: two arguement are required."
ERRORNOTDIR="ERROR: ${filesdir} is not a directory"

# Check the number of argument that scripts need
if [ $# -ne 2 ];then
    echo $ERRORARGU
    exit $WITHERROR
fi

# Check if file directory is specified
if [ ! -d $filesdir ]; then
    echo $ERRORNOTDIR
    exit $WITHERROR
fi

# Calculate the number of files and matching lines with KeySearch
fileCount=$(find "$filesdir" -type f | wc -l)
matchCount=$(grep -r "$keySearch" "$filesdir" | wc -l)

echo "The number of files are ${fileCount} and the number of matching lines are ${matchCount}"

exit $RUNCOMPLETLY