#! /bin/bash

fail_count=0
color="0;32m"
TTIME=$(date +%d_%H_%M)
list=$2
if [ $2 = "2_break" ]
then 
	list=2
fi

for ((i=1;i<=$1;i++))
do
	echo -e "\nIteration "$i"\n"

	make shuffle$2
	if [ $? -gt 0 ]
	then 
		echo -e "Shuffle failure"
		exit 1
	fi
	make -C ./benches/$3 test_list$2
	if [ $? -gt 0 ]
	then
		mkdir "broken_lists/${TTIME}_${i}"
		cp "list$list.txt" "broken_lists/${TTIME}_${i}"
		let "fail_count+=1"
		color="0;31m"
	fi
done

echo -e "\n$1 iterations on list$2:\033[$color $fail_count\033[0m failed"
if [ $fail_count -gt 0 ]
then 
	echo "Run timestamp: $TTIME"
fi
