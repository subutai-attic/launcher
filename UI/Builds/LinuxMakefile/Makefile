# Automatically generated makefile, created by the Projucer
# Don't edit this file! Your changes will be overwritten when you re-save the Projucer project!

# (this disables dependency generation if multiple architectures are set)
DEPFLAGS := $(if $(word 2, $(TARGET_ARCH)), , -MMD)

ifndef CONFIG
  CONFIG=Debug
endif

ifeq ($(CONFIG),Debug)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Debug
  OUTDIR := ../../../bin

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=native
  endif

  CPPFLAGS := $(DEPFLAGS) -DLINUX=1 -DDEBUG=1 -D_DEBUG=1 -DJUCER_LINUX_MAKE_6D53C8B4=1 -DJUCE_APP_VERSION=1.0.0 -DJUCE_APP_VERSION_HEX=0x10000 -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode -I ../../JuceLibraryCode/modules -I ../../../include -I ../../../third-party/json -I ../../../third-party/md5 -I ../../../third-party/xpcom -I ../../../third-party/xpcom/xpcom -I ../../../third-party/xpcom/nsprpub -I ../../../third-party/xpcom/string -I ../../../third-party/xpcom/ipcd -I /usr/include/python3.5 -DCURL_STATICLIB
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -g -ggdb -O0 -DRT_OS_LINUX
  CXXFLAGS += $(CFLAGS) -std=c++11
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -L/usr/X11R6/lib/ -L../../../bin -L$(PYLIB_DIR) -lX11 -lXext -lXinerama -ldl -lfreetype -lpthread -lrt -lpython3.5 -Wl,-Bstatic -lsubutai-launcher -Wl,-Bdynamic

  TARGET := sl-ui
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(OUTDIR)/$(TARGET) $(OBJDIR)
endif

ifeq ($(CONFIG),Release)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Release
  OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=native
  endif

  CPPFLAGS := $(DEPFLAGS) -DLINUX=1 -DNDEBUG=1 -DJUCER_LINUX_MAKE_6D53C8B4=1 -DJUCE_APP_VERSION=1.0.0 -DJUCE_APP_VERSION_HEX=0x10000 -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode -I ../../JuceLibraryCode/modules
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -O3 -DRT_OS_LINUX
  CXXFLAGS += $(CFLAGS) -std=c++11
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -fvisibility=hidden -L/usr/X11R6/lib/ -lX11 -lXext -lXinerama -ldl -lfreetype -lpthread -lrt -lcurl -lpython3.5 -lsubutai-launcher 

  TARGET := UI
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(OUTDIR)/$(TARGET) $(OBJDIR)
endif

OBJECTS := \
  $(OBJDIR)/SocialMediaIcons_5a7a8dec.o \
  $(OBJDIR)/Sidebar_ebdb4dbd.o \
  $(OBJDIR)/Logo_a463cb4.o \
  $(OBJDIR)/InstallationComponent_9add89c4.o \
  $(OBJDIR)/MarketplaceComponent_dbb66a7b.o \
  $(OBJDIR)/HubComponent_4ff59811.o \
  $(OBJDIR)/CommunityComponent_da01ba9d.o \
  $(OBJDIR)/InstallationDialog_4854d4ab.o \
  $(OBJDIR)/LibraryComponent_e4ba500b.o \
  $(OBJDIR)/BannerComponent_ec70f312.o \
  $(OBJDIR)/LoginScreen_6bd9a116.o \
  $(OBJDIR)/MainComponent_a6ffb4a5.o \
  $(OBJDIR)/Main_90ebc5c2.o \
  $(OBJDIR)/juce_core_75b14332.o \
  $(OBJDIR)/juce_data_structures_72d3da2c.o \
  $(OBJDIR)/juce_events_d2be882c.o \
  $(OBJDIR)/juce_graphics_9c18891e.o \
  $(OBJDIR)/juce_gui_basics_8a6da59c.o \
  $(OBJDIR)/juce_gui_extra_4a026f23.o \

.PHONY: clean

$(OUTDIR)/$(TARGET): $(OBJECTS) $(RESOURCES)
	@echo Linking UI
	-@mkdir -p $(BINDIR)
	-@mkdir -p $(LIBDIR)
	-@mkdir -p $(OUTDIR)
	@$(BLDCMD)

clean:
	@echo Cleaning UI
	@$(CLEANCMD)

strip:
	@echo Stripping UI
	-@strip --strip-unneeded $(OUTDIR)/$(TARGET)

$(OBJDIR)/SocialMediaIcons_5a7a8dec.o: ../../Source/SocialMediaIcons.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling SocialMediaIcons.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Sidebar_ebdb4dbd.o: ../../Source/Sidebar.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Sidebar.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Logo_a463cb4.o: ../../Source/Logo.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Logo.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/InstallationComponent_9add89c4.o: ../../Source/InstallationComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling InstallationComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MarketplaceComponent_dbb66a7b.o: ../../Source/MarketplaceComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling MarketplaceComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/HubComponent_4ff59811.o: ../../Source/HubComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling HubComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CommunityComponent_da01ba9d.o: ../../Source/CommunityComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CommunityComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/InstallationDialog_4854d4ab.o: ../../Source/InstallationDialog.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling InstallationDialog.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/LibraryComponent_e4ba500b.o: ../../Source/LibraryComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling LibraryComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/BannerComponent_ec70f312.o: ../../Source/BannerComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling BannerComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/LoginScreen_6bd9a116.o: ../../Source/LoginScreen.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling LoginScreen.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MainComponent_a6ffb4a5.o: ../../Source/MainComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling MainComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Main_90ebc5c2.o: ../../Source/Main.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Main.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_core_75b14332.o: ../../JuceLibraryCode/juce_core.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_core.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_data_structures_72d3da2c.o: ../../JuceLibraryCode/juce_data_structures.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_data_structures.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_events_d2be882c.o: ../../JuceLibraryCode/juce_events.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_events.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_graphics_9c18891e.o: ../../JuceLibraryCode/juce_graphics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_graphics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_basics_8a6da59c.o: ../../JuceLibraryCode/juce_gui_basics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_basics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_extra_4a026f23.o: ../../JuceLibraryCode/juce_gui_extra.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_extra.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)
