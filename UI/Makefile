CC = g++
TARGET = subutai-launcher
INCLUDES = -Iinclude -I$(PYLIB_HEADER_DIR) -I$(PYCONFIG_HEADER_DIR) -I$(OPENSSL_DIR)/include -I../include
#ST_LIBS = -g -ggdb $(LIBDIRS) -Wl,-Bdynamic $(EXTRA_LIBS) -lm -l$(PYTHON_VER) -ldl -lpthread -Wl,-Bstatic -lsubutai-launcher -lPocoNetSSL -lPocoCrypto -lPocoNet -lPocoUtil -lPocoXML -lPocoJSON -Wl,-Bstatic -lPocoFoundation -Wl,-Bdynamic $(SYSLIBS) -lssl -lcrypto -lcurl -lssh
#MACOS ONLY
ST_LIBS = -g -ggdb $(LIBDIRS) $(EXTRA_LIBS) -lm -l$(PYTHON_VER) -ldl -lpthread -lsubutai-launcher -lPocoNetSSL -lPocoCrypto -lPocoNet -lPocoUtil -lPocoXML -lPocoJSON -lPocoFoundation $(SYSLIBS) -lssl -lcrypto -lcurl -lssh
SH_LIBS = -g -ggdb $(LIBDIRS) $(EXTRA_LIBS) -lm -l$(PYTHON_VER) -ldl -lpthread -lsubutai-launcher -lPocoNetSSL -lPocoCrypto -lPocoNet -lPocoUtil -lPocoXML -lPocoJSON -lPocoFoundation $(SYSLIBS) -lssl -lcrypto -lcurl -lssh
#CFLAGS = -Wall $(EXTRA_DEFINES) -DDEBUG=1 -D_DEBUG=1 -DJUCE_APP_VERSION=1.0.0 -DBUILD_SCHEME="\"$(BUILD_SCHEME)\"" -DJUCE_APP_VERSION_HEX=0x10000 $(INCLUDES) -I/usr/include -I/usr/include/freetype2 -IJuceLibraryCode -IJuceLibraryCode/modules -std=gnu++11
CFLAGS = -Wall $(EXTRA_DEFINES) -DJUCE_APP_VERSION=1.0.0 -DJUCE_APP_VERSION_HEX=0x10000 $(INCLUDES) -I/usr/include -I/usr/include/freetype2 -IJuceLibraryCode -IJuceLibraryCode/modules -std=gnu++11
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

OBJS = $(BUILD_DIR)/BannerComponent.o \
	   $(BUILD_DIR)/CommunityComponent.o \
	   $(BUILD_DIR)/ComponentChooser.o \
	   $(BUILD_DIR)/HubComponent.o \
	   $(BUILD_DIR)/InstallationComponent.o \
	   $(BUILD_DIR)/InstallationDialog.o \
	   $(BUILD_DIR)/LibraryComponent.o \
	   $(BUILD_DIR)/LibraryItem.o \
	   $(BUILD_DIR)/LibraryItemTab.o \
	   $(BUILD_DIR)/LibrarySystemCheck.o \
	   $(BUILD_DIR)/LoginScreen.o \
	   $(BUILD_DIR)/Logo.o \
	   $(BUILD_DIR)/Main.o \
	   $(BUILD_DIR)/MainComponent.o \
	   $(BUILD_DIR)/MainWindow.o \
	   $(BUILD_DIR)/MarketplaceComponent.o \
	   $(BUILD_DIR)/Sidebar.o \
	   $(BUILD_DIR)/SocialMediaIcons.o \
	   $(BUILD_DIR)/SystemCheck.o \
	   $(BUILD_DIR)/Wizard.o \
	   $(BUILD_DIR)/WizardFinish.o \
	   $(BUILD_DIR)/WizardInstall.o \
	   $(BUILD_DIR)/WizardIntro.o \
	   $(BUILD_DIR)/WizardWindow.o

$(BUILD_DIR)/BannerComponent.o: $(SRC_DIR)/BannerComponent.cpp $(SRC_DIR)/BannerComponent.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/CommunityComponent.o: $(SRC_DIR)/CommunityComponent.cpp $(SRC_DIR)/CommunityComponent.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/ComponentChooser.o: $(SRC_DIR)/ComponentChooser.cpp $(SRC_DIR)/ComponentChooser.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/HubComponent.o: $(SRC_DIR)/HubComponent.cpp $(SRC_DIR)/HubComponent.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/InstallationComponent.o: $(SRC_DIR)/InstallationComponent.cpp $(SRC_DIR)/InstallationComponent.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/InstallationDialog.o: $(SRC_DIR)/InstallationDialog.cpp $(SRC_DIR)/InstallationDialog.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/LibraryComponent.o: $(SRC_DIR)/LibraryComponent.cpp $(SRC_DIR)/LibraryComponent.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/LibraryItem.o: $(SRC_DIR)/LibraryItem.cpp $(SRC_DIR)/LibraryItem.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/LibraryItemTab.o: $(SRC_DIR)/LibraryItemTab.cpp $(SRC_DIR)/LibraryItemTab.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/LibrarySystemCheck.o: $(SRC_DIR)/LibrarySystemCheck.cpp $(SRC_DIR)/LibrarySystemCheck.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/LoginScreen.o: $(SRC_DIR)/LoginScreen.cpp $(SRC_DIR)/LoginScreen.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/Logo.o: $(SRC_DIR)/Logo.cpp $(SRC_DIR)/Logo.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/Main.o: $(SRC_DIR)/Main.cpp $(SRC_DIR)/Main.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/MainComponent.o: $(SRC_DIR)/MainComponent.cpp $(SRC_DIR)/MainComponent.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/MainWindow.o: $(SRC_DIR)/MainWindow.cpp $(SRC_DIR)/MainWindow.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/MarketplaceComponent.o: $(SRC_DIR)/MarketplaceComponent.cpp $(SRC_DIR)/MarketplaceComponent.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/Sidebar.o: $(SRC_DIR)/Sidebar.cpp $(SRC_DIR)/Sidebar.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/SocialMediaIcons.o: $(SRC_DIR)/SocialMediaIcons.cpp $(SRC_DIR)/SocialMediaIcons.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/SystemCheck.o: $(SRC_DIR)/SystemCheck.cpp $(SRC_DIR)/SystemCheck.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/Wizard.o: $(SRC_DIR)/Wizard.cpp $(SRC_DIR)/Wizard.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/WizardFinish.o: $(SRC_DIR)/WizardFinish.cpp $(SRC_DIR)/WizardFinish.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/WizardInstall.o: $(SRC_DIR)/WizardInstall.cpp $(SRC_DIR)/WizardInstall.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/WizardIntro.o: $(SRC_DIR)/WizardIntro.cpp $(SRC_DIR)/WizardIntro.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/WizardWindow.o: $(SRC_DIR)/WizardWindow.cpp $(SRC_DIR)/WizardWindow.h
	$(CC) $(CFLAGS) -c $< -o $@

shared-bin: $(TARGET)

static-bin: $(TARGET)-static

$(TARGET): directories
$(TARGET): $(OUTPUT_DIR)/$(TARGET)

$(TARGET)-static: directories
$(TARGET)-static: $(OUTPUT_DIR)/$(TARGET)-static

ifdef STATIC_BUILD
$(OUTPUT_DIR)/$(TARGET)-static: $(OBJS) $(JUCE_OBJECTS)
	$(CC) -Xlinker -search_paths_first -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.12.sdk -stdlib=libc++ -mmacosx-version-min=10.12 -framework Foundation -framework Carbon -framework Cocoa -framework IOKit -framework QuartzCore -framework WebKit $(OBJECTS) $(JUCE_OBJECTS) -Wall $(ST_LIBS) -o $@
endif

ifdef SHARED_BUILD
$(OUTPUT_DIR)/$(TARGET): $(OBJS) $(JUCE_OBJECTS)
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
	@rm -fv $(OUTPUT_DIR)/$(TARGET)
	@rm -fv $(OUTPUT_DIR)/$(TARGET)-static
	@rm -fv $(OBJS)
	@rm -fv $(JUCE_OBJECTS)

mrproper: clean
