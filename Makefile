TARGET_GCC=aarch-lp64-gcc-7
INCLUDE_DIR=$(shell $(TARGET_GCC) -print-file-name=plugin)
SRC_DIR=src

SRC_FILES=plugin.cc

CXX=g++
CXX_FLAGS= -I$(INCLUDE_DIR)/include -fPIC -fno-rtti -O2

plugin.so: $(addprefix $(SRC_DIR)/, $(SRC_FILES))
	$(CXX) -shared $(CXX_FLAGS) $^ -o $@

CXXFLAGS = -std=c++2a

HEADER_DIR = include

CXXFLAGS += -I$(HEADER_DIR)

SRC_DIR = src
SOURCES = main.cc utilities.cc
SRC := $(addprefix $(SRC_DIR)/, $(SOURCES))

OBJ = $(SOURCES:.cc=.o)

EXEC = shuffle

%.o : $(SRC_DIR)/%.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(EXEC): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(EXEC)
	rm *.o

depend:
	$(CXX) $(CXXFLAGS) $(SRC) -MM > .depend

clean:
	rm *.o

-include .depend
