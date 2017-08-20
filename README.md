Subutai Launcher is an installer-like application for Windows, Linux and MacOS operating systems. The aim of 
this project is to help users to setup applications required to start Subutai usage. It will also help to
setup a Subutai Peer.

Launcher is written in C++ programming language with use of Poco C++ Libraries and JUCE Framework. 

It is actively uses Python as a scripting language. 

Building on Windows
===================

* Download and compile Poco C++ Libraries
* Set `POCO_ROOT` environment variable pointing to top Poco directory

Building on Linux
=================

1. Download and compile Poco C++ Libraries
```./configure --static --shared --omit=Data/ODBC,Data/MySQL,Data/PostgreSQL --no-tests --no-samples```
2. Download and compile cpython
```./configure --prefix=/opt/subutai/python```
3. Install libssh, libx11-dev, libfreetype6-dev, libxrandr-dev, libxinerama-dev, libxcomposite-dev, libxcursor-dev
4. Configure with ./configure script. Use `--help` option to see available configuration options
5. Build with make
```
make ui
```

Building on MacOS
=================

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
