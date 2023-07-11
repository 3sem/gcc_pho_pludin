TARGET_GCC=aarch-lp64-gcc-7
INCLUDE_DIR=$(shell $(TARGET_GCC) -print-file-name=plugin)
SRC_DIR=src

SRC_FILES=plugin.cc

CXX=g++
CXX_FLAGS= -I$(INCLUDE_DIR)/include -I./ -fno-rtti -fPIC -O2

plugin.so: $(addprefix $(SRC_DIR)/, $(SRC_FILES))
	$(CXX) -shared $(CXX_FLAGS) $^ -o $@
