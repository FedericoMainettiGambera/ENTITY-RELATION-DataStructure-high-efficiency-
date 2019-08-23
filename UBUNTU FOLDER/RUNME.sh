#!/bin/bash

setopt shwordsplit

#dirInput=test_completi_10000righe/test_completi_10000righe/i
#dirOutput=test_completi_10000righe/test_completi_10000righe/o
#filenames=`cat inputFilesNames.txt`

echo compiling main.c
gcc -o main -std=c11 -Wall -ggdb3 main.c

echo dirInput is $dirInput
echo dirOutput is $dirOutput
#echo filenames are $filenames

#echo converting outputs without carriageReturn
#for var in test_completi_10000righe/test_completi_10000righe/o/*.txt
#do
#	echo started converting $var
#	perl -pi -e 's/\r\n/\n/' $var
#	echo ended converting $var
#done
#echo everything has been converted

echo testing begin
for var in test_completi_10000righe/test_completi_10000righe/i/*.txt
do
	echo -----------------------------------------------------------------------------------------------
	echo START
	echo testing "$var"
	echo with output "test_completi_10000righe/test_completi_10000righe/o/$(basename "$var" .txt).txt"
	cat $var | ./main > output
	diff -q "test_completi_10000righe/test_completi_10000righe/o/$(basename "$var" .txt).txt" "output"
	echo END
	echo -----------------------------------------------------------------------------------------------
done
echo everything has been tested
