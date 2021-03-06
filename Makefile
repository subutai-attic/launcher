include config.make

CC=g++
SHARED_TARGET = libsubutai-launcher.$(LIB_EXT)
STATIC_TARGET = libsubutai-launcher.a
TEST_TARGET=testsuite
INCLUDES = -Iinclude -I$(PYLIB_HEADER_DIR) -I$(PYCONFIG_HEADER_DIR) -I$(OPENSSL_HEADERS_DIR) -I/usr/local/include
LIBS = -g -ggdb -lm $(SYSLIBS) -l$(PYTHON_VER) $(LINKER_OPTIONS) -lssh -L$(PYLIB_DIR) -lPocoNet -lPocoNetSSL -lPocoFoundation -lPocoJSON
CXXFLAGS = -g $(INCLUDES) -std=c++11 -DLIGHT_MODE -DRT_OS_LINUX -DNDEBUG $(DEFINES) $(BUILD_SCHEME_DEF) $(EXTRA_DEFINES)
LDFLAGS = -s $(LIBS)

SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build/lib
OUTPUT_DIR = bin
TEST_DIR = testsuite

HEADERS = $(wildcard $(INCLUDE_DIR)/*.h)

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
shared: $(OUTPUT_DIR)/$(SHARED_TARGET)
endif

ifdef STATIC_BUILD
lib: static

static: directories
static: $(OUTPUT_DIR)/$(STATIC_TARGET)
endif

ifdef BUILD_TESTS
test: directories
test: lib
	$(MAKE) -C ./testsuite
#test: $(OUTPUT_DIR)/$(TEST_TARGET)
#test: directories lib
#test:
#	@cp testsuite/*.py bin/

test-ssh: test
	$(MAKE) -C ./testsuite test-ssh
endif

cli: lib
	$(MAKE) -C ./CLI

ifdef STATIC_BUILD
ui: ui-static
ui-static: static
	$(MAKE) -C ./UI static-bin
endif

ifdef SHARED_BUILD 
ui: ui-shared
ui-shared: shared
	$(MAKE) -C ./UI shared-bin
endif

$(BUILD_DIR)/Core.o: $(SRC_DIR)/Core.cpp $(INCLUDE_DIR)/Core.h
	$(CC) -fPIC $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/Downloader.o: $(SRC_DIR)/Downloader.cpp $(INCLUDE_DIR)/Downloader.h
	$(CC) -fPIC $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/EnvironmentImpl.o: $(SRC_DIR)/EnvironmentImpl.cpp $(INCLUDE_DIR)/EnvironmentImpl.h
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

$(BUILD_DIR)/SSHException.o: $(SRC_DIR)/SSHException.cpp $(INCLUDE_DIR)/SSHException.h
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

$(BUILD_DIR)/SS.o: $(SRC_DIR)/SS.cpp $(INCLUDE_DIR)/SS.h
	$(CC) -fPIC $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/AssetsManager.o: $(SRC_DIR)/AssetsManager.cpp $(INCLUDE_DIR)/AssetsManager.h
	$(CC) -fPIC $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/HubChannel.o: $(SRC_DIR)/HubChannel.cpp $(INCLUDE_DIR)/HubChannel.h
	$(CC) -fPIC $(CXXFLAGS) -c $< -o $@

OBJS = $(BUILD_DIR)/Core.o \
								 	 $(BUILD_DIR)/Downloader.o \
									 $(BUILD_DIR)/EnvironmentImpl.o \
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
									 $(BUILD_DIR)/SSHException.o \
									 $(BUILD_DIR)/Session.o \
									 $(BUILD_DIR)/Settings.o \
									 $(BUILD_DIR)/SubutaiException.o \
									 $(BUILD_DIR)/SubutaiString.o \
									 $(BUILD_DIR)/Tray.o \
									 $(BUILD_DIR)/VirtualBox.o \
									 $(BUILD_DIR)/SS.o \
									 $(BUILD_DIR)/AssetsManager.o \
									 $(BUILD_DIR)/HubChannel.o

$(OUTPUT_DIR)/$(SHARED_TARGET): $(OBJS)
	$(CC) -shared $^ $(LDFLAGS) -o $@

$(OUTPUT_DIR)/$(STATIC_TARGET): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

directories:
	@mkdir -p $(OUTPUT_DIR)
	@mkdir -p $(BUILD_DIR)
	$(MAKE) -C ./testsuite directories

files:
	@cp assets/* bin/
	$(MAKE) -C ./testsuite files

clean:
	@rm -fv $(OUTPUT_DIR)/$(SHARED_TARGET)
	@rm -fv $(OUTPUT_DIR)/$(STATIC_TARGET)
	@rm -fv $(OBJS)
	$(MAKE) -C ./CLI clean
	$(MAKE) -C ./UI clean
	$(MAKE) -C ./testsuite clean

mrproper:
	@rm -rfv $(OUTPUT_DIR)
	@rm -rfv $(BUILD_DIR)
	$(MAKE) -C ./CLI mrproper
	$(MAKE) -C ./UI mrproper
	$(MAKE) -C ./testsuite mrproper
	@rm -rfv config.make
