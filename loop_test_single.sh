#! /bin/bash

for ((i=1;i<=$1;i++))
do
	echo -e "\nIteration "$i"\n"

	make test_list$2
	if [ $? -gt 0 ]
	then
		TTIME=$(date +%d_%H_%M)_$i
		mkdir "broken_lists/$TTIME"
		cp "list$2.txt" "broken_lists/$TTIME"
	fi
done
