.SECONDARY:

.PHONY: all_lists test_list2 clean

all_lists: ../../shuffle $(addprefix ../../lists/, to_shuffle1.txt to_shuffle2.txt to_shuffle3.txt)  
	../../shuffle --input_file=../../lists/to_shuffle1.txt --output_file=loc_lists/list1.txt --log_file=../../unique_passes.txt
	../../shuffle --input_file=../../lists/to_shuffle2.txt --output_file=loc_lists/list2.txt --log_file=../../unique_passes.txt
	../../shuffle --input_file=../../lists/to_shuffle3.txt --output_file=loc_lists/list3.txt --log_file=../../unique_passes.txt

test_list2: ../../shuffle $(addprefix ../../lists/, to_shuffle1.txt to_shuffle2.txt to_shuffle3.txt)  
	../../shuffle --input_file=../../lists/to_shuffle2.txt --output_file=loc_lists/list2.txt --log_file=../../unique_passes.txt
	cp ../../lists/to_shuffle1.txt loc_lists/list1.txt
	cp ../../lists/to_shuffle3.txt loc_lists/list3.txt

clean: 
	rm bench.elf run.sh test.sh
