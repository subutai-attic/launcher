include ../config.make

CC = g++
TARGET = testsuite
INCLUDES = -I../include -I$(PYLIB_HEADER_DIR) -I$(PYCONFIG_HEADER_DIR) -I$(OPENSSL_DIR)/include -I/usr/local/include
LIBS = -g -ggdb -lm $(SYSLIBS) -l$(PYTHON_VER) -lssh -L$(PYLIB_DIR) -lPocoNet -lPocoNetSSL -lPocoFoundation -lPocoJSON
CXXFLAGS = $(INCLUDES) -std=c++11 -Wall
LDFLAGS = $(LIBS) -lPocoCppUnit -L$(OUTPUT_DIR) -lsubutai-launcher

OUTPUT_DIR = ../bin
BUILD_DIR = ../build/testsuite

$(TARGET): $(OUTPUT_DIR)/libsubutai-launcher.$(LIB_EXT)
$(TARGET): $(OUTPUT_DIR)/$(TARGET)

OBJECTS = $(BUILD_DIR)/DownloaderTest.o \
		  $(BUILD_DIR)/PeerSetupTest.o \
		  $(BUILD_DIR)/SLTest.o \
		  $(BUILD_DIR)/SSHTest.o \
		  $(BUILD_DIR)/TestSuite.o \
		  $(BUILD_DIR)/HubTest.o \
		  $(BUILD_DIR)/EnvironmentTest.o \
		  $(BUILD_DIR)/VirtualBoxTest.o

$(OUTPUT_DIR)/$(TARGET): $(OBJECTS)
	$(CC) $^ $(LDFLAGS) -o $@

$(BUILD_DIR)/DownloaderTest.o: DownloaderTest.cpp DownloaderTest.h
	$(CC) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/PeerSetupTest.o: PeerSetupTest.cpp PeerSetupTest.h
	$(CC) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/SLTest.o: SLTest.cpp SLTest.h
	$(CC) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/SSHTest.o: SSHTest.cpp SSHTest.h
	$(CC) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/TestSuite.o: TestSuite.cpp TestSuite.h
	$(CC) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/HubTest.o: HubTest.cpp HubTest.h
	$(CC) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/EnvironmentTest.o: EnvironmentTest.cpp EnvironmentTest.h
	$(CC) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/VirtualBoxTest.o: VirtualBoxTest.cpp VirtualBoxTest.h
	$(CC) $(CXXFLAGS) -c $< -o $@

directories:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p /tmp/subutai

files:
	@cp *.py /tmp/subutai/
	@cp *.py $(OUTPUT_DIR)

clean:
	@rm -f $(OBJECTS)
	@rm -f $(OUTPUT_DIR)/$(TARGET)

mrproper: clean
	@rm -f $(BUILD_DIR)
