#! /bin/bash

for ((i=1;i<=$1;i++))
do
	echo -e "\nIteration "$i"\n"

	make shuffle_test
	if [ $? -gt 0 ]
	then
		TTIME=$(date +%d_%H_%M)_$i
		mkdir "broken_lists/$TTIME"
		cp list1.txt list2.txt list3.txt "broken_lists/$TTIME"
	fi
done
