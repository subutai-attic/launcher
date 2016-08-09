CC=g++

TARGET = libsubutai-launcher.so
VB_DIR=/usr/lib/virtualbox/sdk/bindings/xpcom/include
VB = -I$(VB_DIR) -I$(VB_DIR)/xpcom -I$(VB_DIR)/nsprpub -I$(VB_DIR)/string -I$(VB_DIR)/ipcd
INCLUDES = -Iinclude -I/usr/include/python2.7 $(VB)
LIBS = -g -ggdb -lm -lrt -lpython2.7 -lcurl
CFLAGS = -L/lib/x86_64-linux-gnu -Wno-write-strings $(INCLUDES) $(LIBS) -std=c++11 -DRT_OS_LINUX

SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
OUTPUT_DIR = bin

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
HEADERS = $(wildcard $(INCLUDE_DIR)/*.h)
OBJECTS = $(patsubst %,$(BUILD_DIR)/%.o, $(subst src/,,$(subst .cpp,,$(SOURCES))))

.PHONE: lib all clean

all: lib cli ui files

lib: directories
lib: $(OUTPUT_DIR)/$(TARGET)

cli: lib
	$(MAKE) -C ./CLI

ui:
	$(MAKE) -C ./UI/Builds/LinuxMakefile

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
	$(CC) -fPIC $(CFLAGS) -c $< -o $@

$(OUTPUT_DIR)/$(TARGET): $(OBJECTS)
	$(CC) -shared $(OBJECTS) $(LIBS) -o $@

directories:
	@mkdir -p bin
	@mkdir -p build

files:
	@cp assets/* bin/
	@cp scripts/* bin/

clean:
	@rm -rf bin/*
	@rm -rf build/*
	$(MAKE) -C ./CLI clean

mrproper:
	@rm -rf bin
	@rm -rf build
	$(MAKE) -C ./CLI mrproper
