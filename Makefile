include config.make

CC=g++
DYNAMIC_LIB_TARGET = libsubutai-launcher.$(LIB_EXT)
STATIC_LIB_TARGET = libsubutai-launcher.a
EXTRA_LIBS_DIR = third-party
TEST_TARGET=testsuite
VB_DIR = third-party/xpcom
VB = -I$(VB_DIR) -I$(VB_DIR)/xpcom -I$(VB_DIR)/nsprpub -I$(VB_DIR)/string -I$(VB_DIR)/ipcd
INCLUDES = -Iinclude -I$(PYLIB_HEADER_DIR) -I$(PYCONFIG_HEADER_DIR) -I$(OPENSSL_DIR)/include $(VB) -Ithird-party/md5 -Ithird-party/json -I/usr/local/include
LIBS = -g -ggdb -lm $(SYSLIBS) -l$(PYTHON_VER) -lcurl -lssh -L$(PYLIB_DIR) -lPocoNet -lPocoNetSSL -lPocoFoundation -lPocoJSON
CFLAGS = -L/lib/x86_64-linux-gnu $(INCLUDES) $(LIBS) -std=c++11 -DRT_OS_LINUX -DBUILD_SCHEME="\"$(BUILD_SCHEME)\"" $(BUILD_SCHEME_DEF)

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

all: lib cli 
ifdef SHARED_BUILD
all: ui-shared
endif
ifdef STATIC_BUILD
all: ui-static
endif
all: files
ifdef BUILD_TESTS
all: test
endif

lib: directories 
ifdef SHARED_BUILD
lib: shared 

shared: directories
shared: $(OUTPUT_DIR)/$(DYNAMIC_LIB_TARGET)
endif
ifdef STATIC_BUILD
lib: static

static: directories
static: $(OUTPUT_DIR)/$(STATIC_LIB_TARGET)
endif

ifdef BUILD_TESTS
test: directories
test: lib
test: $(OUTPUT_DIR)/$(TEST_TARGET)
test:
	@cp testsuite/*.py bin/
endif

cli: lib
	$(MAKE) -C ./CLI

ifdef STATIC_BUILD
ui: ui-static
ui-static: static
	$(MAKE) -C ./UI
endif

ifdef SHARED_BUILD 
ui: ui-shared
ui-shared: shared
	$(MAKE) -C ./UI
endif

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

$(OUTPUT_DIR)/$(STATIC_LIB_TARGET): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

$(OUTPUT_DIR)/$(TEST_TARGET): $(T_OBJECTS)
	$(CC) $(T_OBJECTS) -Wall $(LIBS) -lPocoCppUnit -L$(OUTPUT_DIR) -lsubutai-launcher -o $@

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
	$(MAKE) -C ./UI clean

mrproper:
	@rm -rf bin
	@rm -rf build
	$(MAKE) -C ./CLI mrproper
	$(MAKE) -C ./UI mrproper
	@rm -f config.make

