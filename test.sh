#!/bin/bash

START=$(($(date +%s%N)/1000000))

rm tests/outputs/*.txt

for i in {1..100}
do
    ncat localhost 8080 < tests/inputs/1.txt &> tests/outputs/$i.txt & 
done

#NUM_FILES = $(ls -l tests/outputs/*.txt | wc -l)
#NUM_FILES = 'ls -l tests/outputs/*.txt | wc -l'
#NUMFILES = 'ls -l tests/outputs/*.txt'
NUMFILES=$(ls -l tests/outputs/*.txt | wc -l)
OPENED=$(lsof -t tests/outputs/*.txt | wc -l)

#echo $OPENED

while [ $OPENED -ne 0 ]
do
    NUMFILES=$(ls -l tests/outputs/*.txt | wc -l)
    OPENED=$(lsof -t tests/outputs/*.txt | wc -l)
    #sleep 0.010
done 

declare -i ERR_COUNT=0

for i in {1..100}
do
    DIFF=$(cmp tests/inputs/1.txt tests/outputs/$i.txt | wc -l)
    if [ $DIFF -ne 0 ]
    then
        echo "Error: " $i".txt"
        ERR_COUNT=$ERR_COUNT+1
    fi
done


END=$(($(date +%s%N)/1000000))
DIFF=$(( $END - $START ))
echo "Time: $DIFF ms"
echo "Err Rate: $ERR_COUNT / 100"
