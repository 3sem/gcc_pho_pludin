CXX = g++
CXXFLAGS = -std=c++2a

HEADER_DIR = include

CXXFLAGS += -I$(HEADER_DIR)

SRC_DIR = src
SOURCES = main.cc utilities.cc
SRC := $(addprefix $(SRC_DIR)/, $(SOURCES))

OBJ = $(SOURCES:.cc=.o)

EXEC = shuffle

all: $(EXEC)

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
