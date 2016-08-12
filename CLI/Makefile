CC=g++

TARGET = sl-cli
EXTRA_LIBS_DIR = ../third-party
VB_DIR = ../third-party/xpcom
VB = -I$(VB_DIR) -I$(VB_DIR)/xpcom -I$(VB_DIR)/nsprpub -I$(VB_DIR)/string -I$(VB_DIR)/ipcd
INCLUDES = -Iinclude -I/usr/include/python2.7 -I../include $(VB) -I../third-party/md5 -I../third-party/json
LIBS = -g -ggdb -lm -lrt -lpython2.7 -lcurl -lsubutai-launcher -L../bin
CFLAGS = -L/lib/x86_64-linux-gnu -Wno-write-strings $(INCLUDES) $(LIBS) -std=c++11 -DRT_OS_LINUX

SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
OUTPUT_DIR = ../bin

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
HEADERS = $(wildcard $(INCLUDE_DIR)/*.h)
OBJECTS = $(patsubst %,$(BUILD_DIR)/%.o, $(subst src/,,$(subst .cpp,,$(SOURCES))))

.PHONE: lib all clean

binary: directories
binary: $(OUTPUT_DIR)/$(TARGET)

all: binary

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
	$(CC) -Wall $(CFLAGS) -c $< -o $@

$(OUTPUT_DIR)/$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

directories:
	@mkdir -p build

clean:
	@rm -rf build/*

mrproper:
	@rm -rf build
