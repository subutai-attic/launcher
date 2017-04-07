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
CXXFLAGS = $(INCLUDES) -std=c++11 -DRT_OS_LINUX -DBUILD_SCHEME="\"$(BUILD_SCHEME)\"" $(BUILD_SCHEME_DEF)
LDFLAGS = $(LIBS)

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
#test: $(OUTPUT_DIR)/$(TEST_TARGET)
test: directories lib
	$(MAKE) -C ./testsuite
#test:
#	@cp testsuite/*.py bin/
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

$(BUILD_DIR)/Core.o: $(SRC_DIR)/Core.cpp $(INCLUDE_DIR)/Core.h
	$(CC) -fPIC $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/Downloader.o: $(SRC_DIR)/Downloader.cpp $(INCLUDE_DIR)/Downloader.h
	$(CC) -fPIC $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/Environment.o: $(SRC_DIR)/Environment.cpp $(INCLUDE_DIR)/Environment.h
	$(CC) -fPIC $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/FileSystem.o: $(SRC_DIR)/FileSystem.cpp $(INCLUDE_DIR)/FileSystem.h
	$(CC) -fPIC $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/Hub.o: $(SRC_DIR)/Hub.cpp $(INCLUDE_DIR)/Hub.h
	$(CC) -fPIC $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/NotificationCenter.o: $(SRC_DIR)/NotificationCenter.cpp $(INCLUDE_DIR)/NotificationCenter.h
	$(CC) -fPIC $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/P2P.o: $(SRC_DIR)/P2P.cpp $(INCLUDE_DIR)/P2P.h
	$(CC) -fPIC $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/RootProcess.o: $(SRC_DIR)/RootProcess.cpp $(INCLUDE_DIR)/RootProcess.h
	$(CC) -fPIC $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/SL.o: $(SRC_DIR)/SL.cpp $(INCLUDE_DIR)/SL.h
	$(CC) -fPIC $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/SLException.o: $(SRC_DIR)/SLException.cpp $(INCLUDE_DIR)/SLException.h
	$(CC) -fPIC $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/SLFunctions.o: $(SRC_DIR)/SLFunctions.cpp $(INCLUDE_DIR)/SLFunctions.h
	$(CC) -fPIC $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/SSH.o: $(SRC_DIR)/SSH.cpp $(INCLUDE_DIR)/SSH.h
	$(CC) -fPIC $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/Session.o: $(SRC_DIR)/Session.cpp $(INCLUDE_DIR)/Session.h
	$(CC) -fPIC $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/Settings.o: $(SRC_DIR)/Settings.cpp $(INCLUDE_DIR)/Settings.h
	$(CC) -fPIC $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/SubutaiException.o: $(SRC_DIR)/SubutaiException.cpp $(INCLUDE_DIR)/SubutaiException.h
	$(CC) -fPIC $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/SubutaiString.o: $(SRC_DIR)/SubutaiString.cpp $(INCLUDE_DIR)/SubutaiString.h
	$(CC) -fPIC $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/Tray.o: $(SRC_DIR)/Tray.cpp $(INCLUDE_DIR)/Tray.h
	$(CC) -fPIC $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/VirtualBox.o: $(SRC_DIR)/VirtualBox.cpp $(INCLUDE_DIR)/VirtualBox.h
	$(CC) -fPIC $(CXXFLAGS) -c $< -o $@

#$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
#	$(CC) -fPIC $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/third-party/json/%.o: third-party/json/%.cpp $(HEADERS)
	$(CC) -fPIC $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/third-party/md5/%.o: third-party/md5/%.cpp $(HEADERS)
	$(CC) -fPIC $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/$(TEST_DIR)/%.o: $(TEST_DIR)/%.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@

OBJS = $(BUILD_DIR)/Core.o \
								 	 $(BUILD_DIR)/Downloader.o \
									 $(BUILD_DIR)/Environment.o \
									 $(BUILD_DIR)/FileSystem.o \
									 $(BUILD_DIR)/Hub.o \
									 $(BUILD_DIR)/NotificationCenter.o \
									 $(BUILD_DIR)/P2P.o \
									 $(BUILD_DIR)/RootProcess.o \
									 $(BUILD_DIR)/SLException.o \
									 $(BUILD_DIR)/SL.o \
									 $(BUILD_DIR)/SLFunctions.o \
									 $(BUILD_DIR)/SSH.o \
									 $(BUILD_DIR)/Session.o \
									 $(BUILD_DIR)/Settings.o \
									 $(BUILD_DIR)/SubutaiException.o \
									 $(BUILD_DIR)/SubutaiString.o \
									 $(BUILD_DIR)/Tray.o \
									 $(BUILD_DIR)/VirtualBox.o	 

$(OUTPUT_DIR)/$(DYNAMIC_LIB_TARGET): $(OBJS)
	$(CC) -shared $^ $(LDFLAGS) -o $@

$(OUTPUT_DIR)/$(STATIC_LIB_TARGET): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

#$(OUTPUT_DIR)/$(TEST_TARGET): $(T_OBJECTS)
#	$(CC) $(T_OBJECTS) -Wall $(LDFLAGS) -lPocoCppUnit -L$(OUTPUT_DIR) -lsubutai-launcher -o $@

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
	@rm -rf build/*.o
	@rm -rf build/testsuite
	@rm -rf build/third-party
	@rm -rf build/ui
	$(MAKE) -C ./CLI clean
	$(MAKE) -C ./UI clean

mrproper:
	@rm -rf bin
	@rm -rf build/.o
	@rm -rf build/testsuite
	@rm -rf build/third-party
	@rm -rf build/ui
	$(MAKE) -C ./CLI mrproper
	$(MAKE) -C ./UI mrproper
	@rm -f config.make
