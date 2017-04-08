CC = g++
TARGET = subutai-launcher
EXTRA_LIB_DIR = ../third-party
INCLUDES = -Iinclude -I$(PYLIB_HEADER_DIR) -I$(PYCONFIG_HEADER_DIR) -I$(OPENSSL_DIR)/include -I../include -I../third-party/md5 -I../third-party/json
#ST_LIBS = -g -ggdb $(LIBDIRS) -Wl,-Bdynamic $(EXTRA_LIBS) -lm -l$(PYTHON_VER) -ldl -lpthread -Wl,-Bstatic -lsubutai-launcher -lPocoNetSSL -lPocoCrypto -lPocoNet -lPocoUtil -lPocoXML -lPocoJSON -Wl,-Bstatic -lPocoFoundation -Wl,-Bdynamic $(SYSLIBS) -lssl -lcrypto -lcurl -lssh
#MACOS ONLY
ST_LIBS = -g -ggdb $(LIBDIRS) $(EXTRA_LIBS) -lm -l$(PYTHON_VER) -ldl -lpthread -lsubutai-launcher -lPocoNetSSL -lPocoCrypto -lPocoNet -lPocoUtil -lPocoXML -lPocoJSON -lPocoFoundation $(SYSLIBS) -lssl -lcrypto -lcurl -lssh
SH_LIBS = -g -ggdb $(LIBDIRS) $(EXTRA_LIBS) -lm -l$(PYTHON_VER) -ldl -lpthread -lsubutai-launcher -lPocoNetSSL -lPocoCrypto -lPocoNet -lPocoUtil -lPocoXML -lPocoJSON -lPocoFoundation $(SYSLIBS) -lssl -lcrypto -lcurl -lssh
#CFLAGS = -Wall $(EXTRA_DEFINES) -DDEBUG=1 -D_DEBUG=1 -DJUCE_APP_VERSION=1.0.0 -DBUILD_SCHEME="\"$(BUILD_SCHEME)\"" -DJUCE_APP_VERSION_HEX=0x10000 $(INCLUDES) -I/usr/include -I/usr/include/freetype2 -IJuceLibraryCode -IJuceLibraryCode/modules -std=gnu++11
CFLAGS = -Wall $(EXTRA_DEFINES) -DJUCE_APP_VERSION=1.0.0 -DBUILD_SCHEME="\"$(BUILD_SCHEME)\"" -DJUCE_APP_VERSION_HEX=0x10000 $(INCLUDES) -I/usr/include -I/usr/include/freetype2 -IJuceLibraryCode -IJuceLibraryCode/modules -std=gnu++11
#-std=c++11

JUCE = JuceLibraryCode

SRC_DIR = Source
BUILD_DIR = ../build/ui
OUTPUT_DIR = ../bin

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
HEADERS = $(wildcard $(SRC_DIR)/*.h)
OBJECTS = $(patsubst %,$(BUILD_DIR)/%.o, $(subst $(SRC_DIR)/,,$(subst .cpp,,$(SOURCES))))
JUCE_OBJECTS = $(BUILD_DIR)/juce_core.o \
			   $(BUILD_DIR)/juce_data_structures.o \
			   $(BUILD_DIR)/juce_events.o \
			   $(BUILD_DIR)/juce_graphics.o \
			   $(BUILD_DIR)/juce_gui_basics.o \
			   $(BUILD_DIR)/juce_gui_extra.o

shared-bin: $(TARGET)

static-bin: $(TARGET)-static

$(TARGET): directories
$(TARGET): $(OUTPUT_DIR)/$(TARGET)

$(TARGET)-static: directories
$(TARGET)-static: $(OUTPUT_DIR)/$(TARGET)-static

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
	$(CC)  -Wall $(CFLAGS) -c $< -o $@

ifdef STATIC_BUILD
$(OUTPUT_DIR)/$(TARGET)-static: $(OBJECTS) $(JUCE_OBJECTS)
	$(CC) -Xlinker -search_paths_first -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.12.sdk -stdlib=libc++ -mmacosx-version-min=10.12 -framework Foundation -framework Carbon -framework Cocoa -framework IOKit -framework QuartzCore -framework WebKit $(OBJECTS) $(JUCE_OBJECTS) -Wall $(ST_LIBS) -o $@
endif

ifdef SHARED_BUILD
$(OUTPUT_DIR)/$(TARGET): $(OBJECTS) $(JUCE_OBJECTS)
	$(CC) -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.12.sdk -stdlib=libc++ -mmacosx-version-min=10.12 -framework Foundation -framework Carbon -framework Cocoa -framework IOKit -framework QuartzCore -framework WebKit $(OBJECTS) $(JUCE_OBJECTS) -Wall $(SH_LIBS) -o $@
endif
	
$(BUILD_DIR)/juce_core.o: $(JUCE)/juce_core.$(CODE_EXT)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/juce_data_structures.o: $(JUCE)/juce_data_structures.$(CODE_EXT)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/juce_events.o: $(JUCE)/juce_events.$(CODE_EXT)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/juce_graphics.o: $(JUCE)/juce_graphics.$(CODE_EXT)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/juce_gui_basics.o: $(JUCE)/juce_gui_basics.$(CODE_EXT)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/juce_gui_extra.o: $(JUCE)/juce_gui_extra.$(CODE_EXT)
	$(CC) $(CFLAGS) -c $< -o $@

directories:
	@mkdir -p $(BUILD_DIR)

clean:
	-rm $(OUTPUT_DIR)/$(TARGET)
	-rm $(OUTPUT_DIR)/$(TARGET)-static

mrproper: clean
