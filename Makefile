CC=g++

TARGET = libsubutai-launcher.so
INCLUDES = -Iinclude -I/usr/include/python2.7
LIBS = -g -ggdb -lm -lrt -lpython2.7 -lcurl
CFLAGS = -L/lib/x86_64-linux-gnu -Wno-write-strings $(INCLUDES) $(LIBS) -std=c++11

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
