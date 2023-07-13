#! /bin/bash

fail_count=0
color="0;32m"

for ((i=1;i<=$1;i++))
do
	echo -e "\nIteration "$i"\n"

	make shuffle_test
	if [ $? -gt 0 ]
	then
		TTIME=$(date +%d_%H_%M)_$i
		mkdir "broken_lists/$TTIME"
		cp list1.txt list2.txt list3.txt "broken_lists/$TTIME"
		let "fail_count+=1"
		color="0;31m"
	fi
done

echo -e "\n$1 iterations on list$2:\033[$color $fail_count\033[0m failed"
