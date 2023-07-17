#! /bin/bash

# Check for bench Makefile
if ! test -f "bench/Makefile"
then
	echo "Missing bench files"
	exit 1
fi


make -sC ./bench run_cmd
if [ $? -gt 0 ]
then
	exit $?
fi

make -sC ./bench test_cmd
if [ $? -gt 0 ]
then
	exit $?
fi

# Determining baseline
make -sC ./bench build_base
size=( $(eval "size --format=Berkeley bench.elf | tail -n1") )
base_size=${size[0]}
base_time=$(eval "/usr/bin/time -qf '%U' ./run.sh 2>&1")
base_time_short=${base_time%%.*}

echo -e "Baseline: ${base_size}B , ${base_time}s for 10 executions"

for ((i=1;i<=$2;i++))
do
	echo -e "\nIteration $i"
	# Make local shuffled lists
	make -s $1
	make -sC ./bench build

	# Measure size
	size=( $(eval "size --format=Berkeley bench.elf | tail -n1") )
	shuffled_size=${size[0]}
	if [ $shuffled_size -gt $base_size ]
	then
		echo "Size ${shuffled_size}B exceeded baseline"
		continue 
	else 
		echo "Size ok"
	fi

	
	# Measure exec time
	shuffled_time=$(eval "/usr/bin/time -qf '%U' ./run.sh 2>&1")
	shuffled_time_short=${shuffled_time%%.*}
	if [ $? -gt 0 ]
	then 
		continue 
	fi
	
	# Check results for correctness
	./test.sh
	if [ $? -gt 0 ]
	then 
		echo "Tests failed"
		continue 
	fi
	
	# Save results if got improvement
	(( diff=$shuffled_time_short-$base_time_short )) 
	if [ $diff -lt 3 ] || [ $shuffled_time_short -lt $base_time_short ]
	then 
		TTIME=$(date +%d_%H_%M)
		echo -e "Found better order, saving to lists/${TTIME}_${i}"
		echo -e "${shuffled_size}B, ${shuffled_time}s"
		echo -e "${shuffled_size}B, ${shuffled_time}s" > lists/${TTIME}_${i}/stats.txt
		cp loc_lists/* lists/${TTIME}_${i}/
	else
		echo -e "Worse: ${shuffled_size}B, ${shuffled_time}s"
	fi
done
