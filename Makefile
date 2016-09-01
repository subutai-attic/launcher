CC=g++

TARGET = libsubutai-launcher.so
STARGET = libsubutai-launcher.a
EXTRA_LIBS_DIR = third-party
VB_DIR = third-party/xpcom
VB = -I$(VB_DIR) -I$(VB_DIR)/xpcom -I$(VB_DIR)/nsprpub -I$(VB_DIR)/string -I$(VB_DIR)/ipcd
INCLUDES = -Iinclude -I/usr/include/python2.7 $(VB) -Ithird-party/md5 -Ithird-party/json
LIBS = -g -ggdb -lm -lrt -lpython2.7 -lcurl
CFLAGS = -L/lib/x86_64-linux-gnu -Wno-write-strings $(INCLUDES) $(LIBS) -std=c++11 -DRT_OS_LINUX

SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
OUTPUT_DIR = bin

SOURCES = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard third-party/json/*.cpp) $(wildcard third-party/md5/*.cpp)
HEADERS = $(wildcard $(INCLUDE_DIR)/*.h)
OBJECTS = $(patsubst %,$(BUILD_DIR)/%.o, $(subst src/,,$(subst .cpp,,$(SOURCES))))

.PHONE: lib all clean

all: lib cli ui files

lib: directories dynamic static

dynamic: directories
dynamic: $(OUTPUT_DIR)/$(TARGET)

static: directories
static: $(OUTPUT_DIR)/$(STARGET)

cli: lib
	$(MAKE) -C ./CLI

ui: lib
	$(MAKE) -C ./UI/Builds/LinuxMakefile

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
	$(CC) -fPIC $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/third-party/json/%.o: third-party/json/%.cpp $(HEADERS)
	$(CC) -fPIC $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/third-party/md5/%.o: third-party/md5/%.cpp $(HEADERS)
	$(CC) -fPIC $(CFLAGS) -c $< -o $@

$(OUTPUT_DIR)/$(TARGET): $(OBJECTS)
	$(CC) -shared $(OBJECTS) $(LIBS) -o $@

$(OUTPUT_DIR)/$(STARGET): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

directories:
	@mkdir -p bin
	@mkdir -p build
	@mkdir -p build/third-party/json
	@mkdir -p build/third-party/md5

files:
	@cp assets/* bin/
	@cp scripts/* bin/

clean:
	@rm -rf bin/*
	@rm -rf build/*
	$(MAKE) -C ./CLI clean
	$(MAKE) -C ./UI/Builds/LinuxMakefile clean

mrproper:
	@rm -rf bin
	@rm -rf build
	$(MAKE) -C ./CLI mrproper
