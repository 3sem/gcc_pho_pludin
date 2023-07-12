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

test_list3: plugin.so shuffle3
	make -C ./benches/bzip2d test_list3

CXXFLAGS = -std=c++2a

HEADER_DIR = include

CXXFLAGS += -I$(HEADER_DIR)

SOURCES = main.cc utilities.cc
SRC := $(addprefix $(SRC_DIR)/, $(SOURCES))

OBJECTS = $(SOURCES:.cc=.o)
OBJ_DIR = obj
OBJ := $(addprefix $(OBJ_DIR)/, $(OBJECTS))

EXEC = shuffle

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(EXEC): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(EXEC)

shuffle1 : $(EXEC)
	./$(EXEC) lists/to_shuffle1.txt

shuffle2 : $(EXEC)
	./$(EXEC) lists/to_shuffle2.txt

shuffle3 : $(EXEC)
	./$(EXEC) lists/to_shuffle3.txt

shuffle4 : $(EXEC)
	./$(EXEC) lists/to_shuffle4.txt

$(OBJ_DIR)/main.o: src/main.cc include/utilities.hh include/driver.hh include/state_machine.hh

$(OBJ_DIR)/utilities.o: src/utilities.cc include/utilities.hh

clean_obj:
	rm $(OBJ_DIR)/*.o

clean_bad_lists:
	rm -r broken_lists/*
