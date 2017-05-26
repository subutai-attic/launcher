CC=g++
TARGET = sl-cli
INCLUDES = -Iinclude -I$(PYLIB_HEADER_DIR) -I$(OPENSSL_DIR)/include -I../include
LIBS = -g -ggdb -lm $(SYSLIBS) -l$(PYTHON_VER) -lcurl -lssh -lsubutai-launcher -L../bin -L$(PYLIB_DIR) -lPocoNet
CFLAGS = -L/lib/x86_64-linux-gnu -Wno-write-strings $(INCLUDES) $(LIBS) -std=c++11 -DRT_OS_LINUX

SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = ../build/CLI
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
	@mkdir -p $(BUILD_DIR)

clean:
	@rm -rf $(BUILD_DIR)/*

mrproper:
	@rm -rf $(BUILD_DIR)
