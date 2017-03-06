CC = g++ 
#-print-search-dirs 
TARGET = subutai-launcher
EXTRA_LIB_DIR = ../third-party
INCLUDES = -Iinclude -I/usr/include/$(PYTHON_VER) -I../include -I../third-party/md5 -I../third-party/json
LIBDIRS = -L/usr/lib/x86_64-linux-gnu -L$(PYLIB_DIR) -L../bin -L/usr/X11R6/lib
LIBS = -g -ggdb $(LIBDIRS) -Wl,-Bdynamic -lm -l$(PYTHON_VER) -lX11 -lXext -lXinerama -ldl -lfreetype -lpthread -Wl,-Bstatic -lsubutai-launcher -lPocoNetSSL -lPocoCrypto -lPocoNet -lPocoUtil -lPocoXML -lPocoJSON -Wl,-Bstatic -lPocoFoundation -Wl,-Bdynamic -lrt -lssl -lcrypto -lcurl -lssh
CFLAGS =-DLINUX=1 -DDEBUG=1 -D_DEBUG=1 -DJUCE_APP_VERSION=1.0.0 -DJUCE_APP_VERSION_HEX=0x10000 $(INCLUDES) -I/usr/include -I/usr/include/freetype2 -IJuceLibraryCode -IJuceLibraryCode/modules -std=c++11

JUCE=JuceLibraryCode

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


$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
	$(CC)  -Wall $(CFLAGS) -c $< -o $@

$(OUTPUT_DIR)/$(TARGET): $(OBJECTS) $(JUCE_OBJECTS)
	$(CC)  $(OBJECTS) $(JUCE_OBJECTS) -Wall $(LIBS) -o $@
	
$(BUILD_DIR)/juce_core.o: $(JUCE)/juce_core.cpp
	$(CC)  -Wall $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/juce_data_structures.o: $(JUCE)/juce_data_structures.cpp
	$(CC)  -Wall $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/juce_events.o: $(JUCE)/juce_events.cpp
	$(CC)  -Wall $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/juce_graphics.o: $(JUCE)/juce_graphics.cpp
	$(CC)  -Wall $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/juce_gui_basics.o: $(JUCE)/juce_gui_basics.cpp
	$(CC)  -Wall $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/juce_gui_extra.o: $(JUCE)/juce_gui_extra.cpp
	$(CC)  -Wall $(CFLAGS) -c $< -o $@

