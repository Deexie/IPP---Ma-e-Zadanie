#!/bin/bash

if [[ $# != 2 ]]
then
    echo "Usage: $0 <program> <directory>." >&2
    exit 1
fi

PROGRAM=$1
DIR=$2
VALGRIND="valgrind --error-exitcode=15 --leak-check=full --show-leak-kinds=all --errors-for-leak-kinds=all -q"
ERR_FILE=$(mktemp test.XXX)
OUT_FILE=$(mktemp test.XXX)

for f in $DIR/*.in; do
    echo -n "Now testing ${f##*/}:   "
    
    $VALGRIND ./$PROGRAM <$f 2>$ERR_FILE >$OUT_FILE
    EXIT=$?

    # memory test
    if (( EXIT == 15 ))
    then
	echo -n "MEMORY LEAK; "
    elif (( EXIT == 0 ))
    then
	echo -n "MEMORY TEST PASSED; "
    else
	echo -n "VALGRIND ERROR. EXIT CODE: $EXIT; "
    fi

    # correctness test
    cmp -s ${f%in}out $OUT_FILE && cmp -s ${f%in}err $ERR_FILE &&
    echo "OUTPUT CORRECT" || echo "ERROR. INCORRECT OUTPUT"
done

rm -f $ERR_FILE
rm -f $OUT_FILE
