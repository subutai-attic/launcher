CC=g++

TARGET = subutai-launcher
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

binary: directories
binary: $(OUTPUT_DIR)/$(TARGET)

all: binary

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
	$(CC) -Wall $(CFLAGS) -c $< -o $@

$(OUTPUT_DIR)/$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

directories:
	@mkdir -p bin
	@mkdir -p build

clean:
	@rm -rf bin/*
	@rm -rf build/*

mrproper:
	@rm -rf bin
	@rm -rf build
