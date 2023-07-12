TARGET_GCC=aarch64-gcc-7
INCLUDE_DIR=$(shell $(TARGET_GCC) -print-file-name=plugin)
SRC_DIR=src

SRC_FILES=plugin.cc

CXX=g++
CXX_FLAGS= -I$(INCLUDE_DIR)/include -fPIC -fno-rtti -O2

plugin.so: $(addprefix $(SRC_DIR)/, $(SRC_FILES))
	$(TARGET_GCC) -shared $(CXX_FLAGS) $^ -o $@

CXXFLAGS = -std=c++2a

HEADER_DIR = include

CXXFLAGS += -I$(HEADER_DIR)

SRC_DIR = src
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