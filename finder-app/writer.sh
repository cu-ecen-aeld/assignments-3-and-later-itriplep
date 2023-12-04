#!/bin/sh

# return variable
WITHERROR=1
RUNCOMPLETLY=0
# Error messages
ERRORARGUMENT="ERROR: two arguement are required."
ERRORNOTDIR="ERROR: ${filesdir} is not a directory."
ERRORPATH="ERROR: file path not specified."
ERRORTEXT="Error: Text string not specified."

# Check the number of argument that script needs
if [ $# -ne 2 ]; then
    echo $ERRORARGUMENT
    exit $WITHERROR
fi

# Assign arguments to variable
writefile=$1
writestr=$2

# Check if path file is specified
if [ -z "$writefile" ];then
    echo $ERRORPATH
    exit $WITHERROR
fi

# Check if Text file is specified
if [ -z "$writestr" ];then
    echo $ERRORTEXT
    exit $WITHERROR
fi

# Extract the directory path
dirpath=$(dirname "$writefile")

# Create the directory if it doesn't exist
mkdir -p "$dirpath"

# Create the file with the text variable on it
if [ -e $writefile ]; then
   echo $writestr > $writefile
else
    echo $writestr >> $writefile
fi
exit $RUNCOMPLETLY