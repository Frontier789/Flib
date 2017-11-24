#!/bin/bash

if [ $# -eq 0 ]; then
	echo "Usage: $0 compiler"
	exit 2
fi

DIR=$(dirname "$0")

RED='\033[0;31m'
NC='\033[0m'
GRAY='\033[1;30m'
FINE=0
function test {
	$1 $2 $3
	if [[ $? -eq 1 ]]; then
		echo -e $4
		FINE=1
	else
		echo -e "$2 found\n"
	fi
}

echo "Testing required includes"

test $DIR/testInclude.sh GL/glx.h $1 "${RED}Error: GL/glx.h not found${NC}\nPlease install opengl development library suitable for your system\nexample:\n  ${GRAY}sudo apt-get install mesa-common-dev${NC}\n"

echo "Testing required libraries"
test $DIR/testLib.sh GL $1 "${RED}Error: libGL.so not found${NC}\nPlease install opengl runtime library suitable for your system\nexample:\n  ${GRAY}sudo apt-get install mesa-common-dev${NC}\n"

if [[ $FINE -eq 0 ]]; then
	echo "All requirements found"
else
	echo "Some requirements are not found, exiting"
fi

exit $FINE
