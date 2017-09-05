CC = g++
TARGET = subutai-launcher
INCLUDES = -Iinclude -I$(PYLIB_HEADER_DIR) -I$(PYCONFIG_HEADER_DIR) -I$(OPENSSL_HEADERS_DIR) -I../include
#MACOS ONLY
ST_LIBS = -g -ggdb $(LIBDIRS) $(WLSH) $(EXTRA_LIBS) -lm $(WLST) $(WLWA) -l$(PYTHON_VER) $(WLNWA) $(WLSH) -ldl -lpthread $(WLST) -lsubutai-launcher -lPocoNetSSL -lPocoCrypto -lPocoNet -lPocoUtil -lPocoXML -lPocoJSON -lPocoFoundation $(WLSH) $(SYSLIBS) -lssl -lcrypto -lssh -lutil $(LINKER_OPTIONS)
#ST_LIBS = -g -ggdb $(LIBDIRS) $(WLSH) $(EXTRA_LIBS) -lm $(WLST) -l$(PYTHON_VER) $(WLSH) -ldl -lpthread $(WLST) -lsubutai-launcher -lPocoNetSSL -lPocoCrypto -lPocoNet -lPocoUtil -lPocoXML -lPocoJSON -lPocoFoundation $(WLSH) $(SYSLIBS) -lssl -lcrypto -lssh -lutil
SH_LIBS = -g -ggdb $(LIBDIRS) $(EXTRA_LIBS) -lm -l$(PYTHON_VER) -ldl -lpthread -lsubutai-launcher -lPocoNetSSL -lPocoCrypto -lPocoNet -lPocoUtil -lPocoXML -lPocoJSON -lPocoFoundation $(SYSLIBS) -lssl -lcrypto -lssh
#CXXFLAGS = -g -Wall $(EXTRA_DEFINES) $(BUILD_SCHEME_DEF) -DLIGHT_MODE=1 -DJUCE_APP_VERSION=1.0.0 -DJUCE_APP_VERSION_HEX=0x10000 $(INCLUDES) -I/usr/include -I/usr/include/freetype2 -IJuceLibraryCode -IJuceLibraryCode/modules -std=gnu++11 -Wno-unused-result -Wsign-compare -DNDEBUG -fwrapv -O0 $(DEFINES)
CXXFLAGS = -g -Wall -DLIGHT_MODE=1 -DJUCE_APP_VERSION=1.0.0 -DJUCE_APP_VERSION_HEX=0x10000 $(INCLUDES) -I/usr/include -I/usr/include/freetype2 -IJuceLibraryCode -IJuceLibraryCode/modules -std=gnu++11 -Wno-unused-result -Wsign-compare -DNDEBUG -fwrapv -O0 $(DEFINES)
LDFLAGS = -g $(BLDFLAGS)

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
	   $(BUILD_DIR)/LauncherButton.o \
	   $(BUILD_DIR)/Splash.o \
	   $(BUILD_DIR)/WizardWindow.o

$(BUILD_DIR)/BannerComponent.o: $(SRC_DIR)/BannerComponent.cpp $(SRC_DIR)/BannerComponent.h
	$(CC) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/CommunityComponent.o: $(SRC_DIR)/CommunityComponent.cpp $(SRC_DIR)/CommunityComponent.h
	$(CC) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/ComponentChooser.o: $(SRC_DIR)/ComponentChooser.cpp $(SRC_DIR)/ComponentChooser.h
	$(CC) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/HubComponent.o: $(SRC_DIR)/HubComponent.cpp $(SRC_DIR)/HubComponent.h
	$(CC) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/InstallationComponent.o: $(SRC_DIR)/InstallationComponent.cpp $(SRC_DIR)/InstallationComponent.h
	$(CC) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/InstallationDialog.o: $(SRC_DIR)/InstallationDialog.cpp $(SRC_DIR)/InstallationDialog.h
	$(CC) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/LibraryComponent.o: $(SRC_DIR)/LibraryComponent.cpp $(SRC_DIR)/LibraryComponent.h
	$(CC) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/LibraryItem.o: $(SRC_DIR)/LibraryItem.cpp $(SRC_DIR)/LibraryItem.h
	$(CC) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/LibraryItemTab.o: $(SRC_DIR)/LibraryItemTab.cpp $(SRC_DIR)/LibraryItemTab.h
	$(CC) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/LibrarySystemCheck.o: $(SRC_DIR)/LibrarySystemCheck.cpp $(SRC_DIR)/LibrarySystemCheck.h
	$(CC) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/LoginScreen.o: $(SRC_DIR)/LoginScreen.cpp $(SRC_DIR)/LoginScreen.h
	$(CC) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/Logo.o: $(SRC_DIR)/Logo.cpp $(SRC_DIR)/Logo.h
	$(CC) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/Main.o: $(SRC_DIR)/Main.cpp $(SRC_DIR)/Main.h
	$(CC) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/MainComponent.o: $(SRC_DIR)/MainComponent.cpp $(SRC_DIR)/MainComponent.h
	$(CC) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/MainWindow.o: $(SRC_DIR)/MainWindow.cpp $(SRC_DIR)/MainWindow.h
	$(CC) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/MarketplaceComponent.o: $(SRC_DIR)/MarketplaceComponent.cpp $(SRC_DIR)/MarketplaceComponent.h
	$(CC) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/Sidebar.o: $(SRC_DIR)/Sidebar.cpp $(SRC_DIR)/Sidebar.h
	$(CC) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/SocialMediaIcons.o: $(SRC_DIR)/SocialMediaIcons.cpp $(SRC_DIR)/SocialMediaIcons.h
	$(CC) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/SystemCheck.o: $(SRC_DIR)/SystemCheck.cpp $(SRC_DIR)/SystemCheck.h
	$(CC) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/Wizard.o: $(SRC_DIR)/Wizard.cpp $(SRC_DIR)/Wizard.h
	$(CC) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/WizardFinish.o: $(SRC_DIR)/WizardFinish.cpp $(SRC_DIR)/WizardFinish.h
	$(CC) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/WizardInstall.o: $(SRC_DIR)/WizardInstall.cpp $(SRC_DIR)/WizardInstall.h
	$(CC) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/WizardIntro.o: $(SRC_DIR)/WizardIntro.cpp $(SRC_DIR)/WizardIntro.h
	$(CC) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/LauncherButton.o: $(SRC_DIR)/LauncherButton.cpp $(SRC_DIR)/LauncherButton.h
	$(CC) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/Splash.o: $(SRC_DIR)/Splash.cpp $(SRC_DIR)/Splash.h
	$(CC) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/WizardWindow.o: $(SRC_DIR)/WizardWindow.cpp $(SRC_DIR)/WizardWindow.h
	$(CC) $(CXXFLAGS) -c $< -o $@

shared-bin: $(TARGET)

static-bin: $(TARGET)-static

$(TARGET): directories
$(TARGET): $(OUTPUT_DIR)/$(TARGET)

$(TARGET)-static: directories
$(TARGET)-static: $(OUTPUT_DIR)/$(TARGET)-static

ifdef STATIC_BUILD
$(OUTPUT_DIR)/$(TARGET)-static: $(OBJS) $(JUCE_OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(JUCE_OBJECTS) $(ST_LIBS) -o $@
endif

ifdef SHARED_BUILD
$(OUTPUT_DIR)/$(TARGET): $(OBJS) $(JUCE_OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(JUCE_OBJECTS) $(SH_LIBS) -o $@
endif
	
$(BUILD_DIR)/juce_core.o: $(JUCE)/juce_core.$(CODE_EXT)
	$(CC) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/juce_data_structures.o: $(JUCE)/juce_data_structures.$(CODE_EXT)
	$(CC) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/juce_events.o: $(JUCE)/juce_events.$(CODE_EXT)
	$(CC) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/juce_graphics.o: $(JUCE)/juce_graphics.$(CODE_EXT)
	$(CC) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/juce_gui_basics.o: $(JUCE)/juce_gui_basics.$(CODE_EXT)
	$(CC) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/juce_gui_extra.o: $(JUCE)/juce_gui_extra.$(CODE_EXT)
	$(CC) $(CXXFLAGS) -c $< -o $@

directories:
	@mkdir -p $(BUILD_DIR)

clean:
	@rm -fv $(OUTPUT_DIR)/$(TARGET)
	@rm -fv $(OUTPUT_DIR)/$(TARGET)-static
	@rm -fv $(OBJS)
	@rm -fv $(JUCE_OBJECTS)

mrproper: clean
