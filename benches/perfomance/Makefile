.SECONDARY:

.PHONY: all_lists test_list2 clean

all_lists: ../../shuffle $(addprefix ../../lists/, to_shuffle1.txt to_shuffle2.txt to_shuffle3.txt)  
	cd ../../;\
	./shuffle lists/to_shuffle1.txt benches/perfomance/loc_lists/list1.txt;\
	./shuffle lists/to_shuffle2.txt benches/perfomance/loc_lists/list2.txt nobreak;\
	./shuffle lists/to_shuffle3.txt benches/perfomance/loc_lists/list3.txt

test_list2: ../../shuffle $(addprefix ../../lists/, to_shuffle1.txt to_shuffle2.txt to_shuffle3.txt)  
	cd ../../;\
	./shuffle lists/to_shuffle2.txt benches/perfomance/loc_lists/list2.txt nobreak;\
	cp lists/to_shuffle1.txt benches/perfomance/loc_lists/list1.txt;\
	cp lists/to_shuffle3.txt benches/perfomance/loc_lists/list3.txt

clean: 
	rm bench.elf run.sh test.sh
