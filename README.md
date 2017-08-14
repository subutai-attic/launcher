Subutai Launcher is an installer-like application for Windows, Linux and MacOS operating systems. The aim of 
this project is to help users to setup applications required to start Subutai usage. It will also help to
setup a Subutai Peer.

Launcher is written in C++ programming language with use of Poco C++ Libraries and JUCE Framework. 

It is actively uses Python as a scripting language. 

Building on Windows
===================

* Download and compile Poco C++ Libraries
* Set `POCO_ROOT` environment variable pointing to top Poco directory

Building on Linux or MacOS
==========================

Simply running ordinary build commands should work:
```
./configure
make
```

If you with to customize build (e.g. choose different version of Python) then you should provide
additional arguments to configure script. Run `./confugure --help` to learn more. Most of the
options available is for developers only, so you may want to look at CONTRIBUTING.md as well.

File System
===========

Launcher uses different installation locations depending on platform:

* Linux uses `/opt/subutai`
* MacOS uses `/usr/local/share/subutai`
* Windows uses `TBD`

Under these directory several extra directories are created:

* `/bin` - collection of all binaries like p2p, tray and launcher itself
* `/etc` - configuration for different subutai applications and resources
* `/launcher` - files related to launcher (Python standard library)

Reporting issues
================

Use GitHub issues to report issues. Do not forget to add a label to determine a platform you are experiencing problems on. 
To contribute into Launcher read CONTRIBUTING.md that will help you to build master/dev version of a launcher and setup
developer's environment

JUCE Patches
============

JUCE requires some changes in code in order to work.

1. `JuceLibraryCode/modules/juce_events/messages/juce_ApplicationBase.cpp`
In methods `getCommandLineParameterArray()` following lines:
```
(juce_argv + 1, juce_argc - 1)
```
should be changed to
```
(juce_argv/* + 1/*, juce_argc/* - 1*/)
```
