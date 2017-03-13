include config.make

CC=g++
DYNAMIC_LIB_TARGET = libsubutai-launcher.so
STATIC_LIB_TARGET = libsubutai-launcher.a
EXTRA_LIBS_DIR = third-party
VB_DIR = third-party/xpcom
VB = -I$(VB_DIR) -I$(VB_DIR)/xpcom -I$(VB_DIR)/nsprpub -I$(VB_DIR)/string -I$(VB_DIR)/ipcd
INCLUDES = -Iinclude -I/usr/include/$(PYTHON_VER) $(VB) -Ithird-party/md5 -Ithird-party/json -I/usr/local/include
LIBS = -g -ggdb -lm -lrt -l$(PYTHON_VER) -lcurl -lssh -L$(PYLIB_DIR)  -lPocoNet -lPocoNetSSL -lPocoFoundation
CFLAGS = -L/lib/x86_64-linux-gnu $(INCLUDES) $(LIBS) -std=c++11 -DRT_OS_LINUX

SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
OUTPUT_DIR = bin
TEST_DIR = testsuite

SOURCES = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard third-party/json/*.cpp) $(wildcard third-party/md5/*.cpp)
HEADERS = $(wildcard $(INCLUDE_DIR)/*.h)
OBJECTS = $(patsubst %,$(BUILD_DIR)/%.o, $(subst src/,,$(subst .cpp,,$(SOURCES))))
TESTS = $(wildcard $(TEST_DIR)/*.cpp)
T_OBJECTS = $(patsubst %,$(BUILD_DIR)/$(TEST_DIR)/%.o, $(subst $(TEST_DIR)/,,$(subst .cpp,,$(TESTS))))

.PHONE: lib all clean

all: lib cli ui files test

lib: directories dynamic static

dynamic: directories
dynamic: $(OUTPUT_DIR)/$(DYNAMIC_LIB_TARGET)

static: directories
static: $(OUTPUT_DIR)/$(STATIC_LIB_TARGET)

test: directories
test: lib
test: $(OUTPUT_DIR)/$(DYNAMIC_LIB_TARGET)-test

cli: lib
	$(MAKE) -C ./CLI

#ui: lib
#	$(MAKE) -C ./UI/Builds/LinuxMakefile

ui: static
	$(MAKE) -C ./UI

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
	$(CC) -fPIC $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/third-party/json/%.o: third-party/json/%.cpp $(HEADERS)
	$(CC) -fPIC $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/third-party/md5/%.o: third-party/md5/%.cpp $(HEADERS)
	$(CC) -fPIC $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/$(TEST_DIR)/%.o: $(TEST_DIR)/%.cpp
	$(CC)   $(CFLAGS) -c $< -o $@

$(OUTPUT_DIR)/$(DYNAMIC_LIB_TARGET): $(OBJECTS)
	$(CC) -shared  $(OBJECTS) $(LIBS) -o $@
#	$(CC) -shared  $(OBJECTS) $(LIBS) -o $@

$(OUTPUT_DIR)/$(STATIC_LIB_TARGET): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

$(OUTPUT_DIR)/$(DYNAMIC_LIB_TARGET)-test: $(T_OBJECTS)
	$(CC) $(T_OBJECTS) -Wall $(LIBS) -lCppUnit -L$(OUTPUT_DIR) -lsubutai-launcher -o $@


directories:
	@mkdir -p $(OUTPUT_DIR)
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)/third-party/json
	@mkdir -p $(BUILD_DIR)/third-party/md5
	@mkdir -p $(BUILD_DIR)/$(TEST_DIR)
	@mkdir -p $(BUILD_DIR)/ui

files:
	@cp assets/* bin/

clean:
	@rm -rf bin/*
	@rm -rf build/*
	$(MAKE) -C ./CLI clean
	$(MAKE) -C ./UI/Makefile clean

mrproper:
	@rm -rf bin
	@rm -rf build
	$(MAKE) -C ./CLI mrproper
	$(MAKE) -C ./UI mrproper
	@rm -f config.make

