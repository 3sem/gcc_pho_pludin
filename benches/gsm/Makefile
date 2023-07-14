.SECONDARY:

ZCC = $(AARCH_GCC) 

QEMU = qemu-aarch64
QEMU_ARG = -L /usr/aarch64-linux-gnu 

DATASET = 1.au

CCC_OPTS = -O2 -DSASR -DSTUPID_COMPILER -DNeedFunctionPrototypes=1 -w
SHUFFLE_OPTS = -fplugin=../../plugin.so -fplugin-arg-plugin-pass_replace -fplugin-arg-plugin-pass_file=../../list1.txt -fplugin-arg-plugin-pass_file=../../list2.txt -fplugin-arg-plugin-pass_file=../../list3.txt

OUT_DIR = datasets

.PHONY: test_lists 

test_lists: shuffled.out $(addprefix ../../, list1.txt list2.txt list3.txt) $(OUT_DIR)/base_unpacked
	$(QEMU) $(QEMU_ARG) ./shuffled.out -d -k -c $(DATASET) > $(OUT_DIR)/shuffled_unpacked
	cmp $(OUT_DIR)/shuffled_unpacked $(OUT_DIR)/base_unpacked

test_list%: shuffled%.out $(addprefix ../../, list%.txt) $(OUT_DIR)/base_unpacked
	$(QEMU) $(QEMU_ARG) ./$< -ps -c $(DATASET) > $(OUT_DIR)/shuffled_unpacked
	cmp $(OUT_DIR)/shuffled_unpacked $(OUT_DIR)/base_unpacked

shuffled1.out : ../../list1.txt ../../plugin.so *.c *.h 
	$(ZCC) $(CCC_OPTS) -fplugin=../../plugin.so -fplugin-arg-plugin-pass_replace -fplugin-arg-plugin-pass_file=$< -fplugin-arg-plugin-pass_file=../../lists/list3.txt -lm *.c -o $@

shuffled2.out : ../../list2.txt ../../plugin.so *.c *.h 
	$(ZCC) $(CCC_OPTS) -fplugin=../../plugin.so -fplugin-arg-plugin-pass_replace -fplugin-arg-plugin-pass_file=$< -fplugin-arg-plugin-pass_file=../../lists/list3.txt -lm *.c -o $@

shuffled3.out : ../../list3.txt ../../plugin.so *.c *.h 
	$(ZCC) $(CCC_OPTS) -fplugin=../../plugin.so -fplugin-arg-plugin-pass_replace -fplugin-arg-plugin-pass_file=$< -lm *.c -o $@

shuffled.out: *.c *.h ../../plugin.so $(addprefix ../../, list1.txt list2.txt list3.txt)	
	$(ZCC) $(CCC_OPTS) $(SHUFFLE_OPTS) -lm *.c -o shuffled.out

base.out: *.c *.h
	$(ZCC) $(CCC_OPTS) -lm *.c -o base.out

$(OUT_DIR)/base_unpacked: base.out $(DATASET)
	$(QEMU) $(QEMU_ARG) ./base.out -ps -c $(DATASET) > $(OUT_DIR)/base_unpacked

clean:
	rm -f *.out *.o *.a *.s *.i *.I $(OUT_DIR)/*
