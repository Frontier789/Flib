#!/bin/sh

if [ $# -eq 0 ]; then
	echo "Usage: $0 filename [compiler]"
	exit 0
fi

if [ $# -eq 1 ]; then
	CXX=g++
else
	CXX=$2
fi

FILE=$1
echo "#include \"$FILE\"" | $CXX -M -x c++ - > /dev/null 2>&1
RES=$?

#g++ -DTOTEST="<$FILE>" -E -P showfile.cpp 
#g++ -DTOTEST="\"$FILE\"" -E -P showfile.cpp 

if [ $RES -eq 1 ]; then
	echo "#include \"$FILE\"" | $CXX -M -x c++ - > /dev/null 2>&1
	RES=$?
fi

exit $RES

