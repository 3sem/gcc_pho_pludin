TARGET_GCC=$(AARCH_GCC)
INCLUDE_DIR=$(shell $(TARGET_GCC) -print-file-name=plugin)
SRC_DIR=src

SRC_FILES=plugin.cc

CXX=g++
CXX_FLAGS= -I$(INCLUDE_DIR)/include -I./ -fno-rtti -fPIC -O2

plugin.so: $(addprefix $(SRC_DIR)/, $(SRC_FILES)) extern_makers.cc pass_makers.cc
	$(CXX) -shared $(CXX_FLAGS) $(SRC_DIR)/plugin.cc -o $@

pass_makers.cc: pass_makers.conf
	echo "int main() {}" > temp.c
	$(TARGET_GCC) -fplugin=./plugin.so -fplugin-arg-plugin-generate_makers=pass_makers.conf temp.c -o /dev/null	
	rm temp.c

.PHONY: makers
makers: pass_makers.cc
