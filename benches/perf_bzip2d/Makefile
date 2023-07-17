ZCC = $(AARCH_GCC) 

QEMU = qemu-aarch64
QEMU_ARG = -L /usr/aarch64-linux-gnu 

DATASET = small.bz2

CCC_OPTS = -O2
LOC_OPTS = -fplugin=../../../plugin.so -fplugin-arg-plugin-pass_replace -fplugin-arg-plugin-pass_file=../loc_lists/list1.txt -fplugin-arg-plugin-pass_file=../loc_lists/list2.txt -fplugin-arg-plugin-pass_file=../loc_lists/list3.txt

OUT_DIR = datasets

build: src/*.c include/*.h ../../../plugin.so $(addprefix ../loc_lists/, list1.txt list2.txt list3.txt)	
	$(ZCC) $(CCC_OPTS) $(LOC_OPTS) -lm -I./include src/*.c -o ../bench.elf

run_cmd: run.sh
	cp run.sh ../
	
test_cmd: test.sh $(OUT_DIR)/base_unpacked $(OUT_DIR)/base_packed 
	cp test.sh ../

build_base: src/*.c include/*.h
	$(ZCC) $(CCC_OPTS) -lm -I./include src/*.c -o ../bench.elf

base.out: src/*.c include/*.h
	$(ZCC) $(CCC_OPTS) -lm -I./include src/*.c -o base.out

$(OUT_DIR)/base_unpacked: base.out $(DATASET)
	$(QEMU) $(QEMU_ARG) ./base.out -d -k -c $(DATASET) > $(OUT_DIR)/base_unpacked

$(OUT_DIR)/base_packed: $(OUT_DIR)/base_unpacked 
	$(QEMU) $(QEMU_ARG) ./base.out -z -k -c $(OUT_DIR)/base_unpacked > $(OUT_DIR)/base_packed

clean:
	rm -f *.out *.o *.a *.s *.i *.I $(OUT_DIR)/*
