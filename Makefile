#Env variable AARCH_GCC with name of target compiler is required
TARGET_GCC=$(AARCH_GCC)
#Get path to header files from target gcc

INCLUDE_DIR=$(shell $(TARGET_GCC) -print-file-name=plugin)
SRC_DIR=src

SRC_FILES=plugin.cc

CXX=g++
CXX_FLAGS= -I$(INCLUDE_DIR)/include -I./ -fno-rtti -fPIC -O2

plugin.so: $(addprefix $(SRC_DIR)/, $(SRC_FILES)) extern_makers.cc pass_makers.cc
	$(CXX) -shared $(CXX_FLAGS) $(SRC_DIR)/plugin.cc -o $@

makers_gen.elf: $(SRC_DIR)/makers_gen.cc 
	$(CXX) $^ -o $@

pass_makers.cc: pass_makers.conf makers_gen.elf 
	./makers_gen.elf pass_makers.conf

.PHONY: makers all_lists test_lists shuffle1 shuffle2 shuffle3 shuffle4 shuffle_test
makers: pass_makers.cc
all_lists: shuffle1 shuffle2 shuffle3

test_lists: plugin.so
	make -C ./benches/bzip2d

shuffle_test: plugin.so all_lists
	make -C ./benches/bzip2d

test_list1: plugin.so shuffle1 
	make -C ./benches/bzip2d test_list1

test_list2: plugin.so shuffle2 
	make -C ./benches/bzip2d test_list2

test_list2_break: plugin.so shuffle2_break
	make -C ./benches/bzip2d test_list2

test_list3: plugin.so shuffle3
	make -C ./benches/bzip2d test_list3

CXXFLAGS = -std=c++2a -O2

HEADER_DIR = include

CXXFLAGS += -I$(HEADER_DIR)
GCC_MIN_VERSION=-10

SOURCES = main.cc file_parsing.cc state_machine.cc options_parsing.cc
SRC := $(addprefix $(SRC_DIR)/, $(SOURCES))

OBJECTS = $(SOURCES:.cc=.o)
OBJ_DIR = obj
OBJ := $(addprefix $(OBJ_DIR)/, $(OBJECTS))

EXEC = shuffle

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cc
	$(CXX)$(GCC_MIN_VERSION) $(CXXFLAGS) -c -o $@ $<

$(EXEC): $(OBJ)
	$(CXX)$(GCC_MIN_VERSION) $(CXXFLAGS) $(OBJ) -o $(EXEC)

shuffle1 : $(EXEC)
	./$(EXEC) --input_file=lists/to_shuffle1.txt --output_file=list1.txt --log_file=unique_passes.txt

shuffle2_break : $(EXEC)
	./$(EXEC) --input_file=lists/to_shuffle2.txt --output_file=list2.txt --log_file=unique_passes.txt --break

shuffle2 : $(EXEC)
	./$(EXEC) --input_file=lists/to_shuffle2.txt --output_file=list2.txt --log_file=unique_passes.txt

shuffle3 : $(EXEC)
	./$(EXEC) --input_file=lists/to_shuffle3.txt --output_file=list3.txt --log_file=unique_passes.txt

shuffle4 : $(EXEC)
	./$(EXEC) --input_file=lists/to_shuffle4.txt --output_file=list4.txt --log_file=unique_passes.txt

$(OBJ_DIR)/main.o: src/main.cc include/utilities.hh include/driver.hh include/state_machine.hh include/file_parsing.hh include/pass_printer.hh

$(OBJ_DIR)/file_parsing.o: src/file_parsing.cc include/file_parsing.hh include/utilities.hh

$(OBJ_DIR)/state_machine.o: src/state_machine.cc include/state_machine.hh include/utilities.hh

$(OBJ_DIR)/options_parsing.o: src/options_parsing.cc include/options_parsing.hh

clean_obj:
	rm $(OBJ_DIR)/*.o

clean_bad_lists:
	rm -r broken_lists/*
