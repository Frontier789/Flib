#!/bin/sh

if [ $# -eq 0 ]; then
	echo "Usage: $0 library [compiler]"
	exit 0
fi

if [ $# -eq 1 ]; then
	CXX=g++
else
	CXX=$2
fi


FILE=$1

echo "int main(){}" | $CXX -l$FILE -o /dev/null -x c++ - > /dev/null 2>&1
RES=$?

exit $RES

