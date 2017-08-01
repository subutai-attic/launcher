#ifndef __ENVIRONMENT_TEST_H__
#define __ENVIRONMENT_TEST_H__

#include "Poco/CppUnit/TestCase.h"
#include "Environment.h"

class EnvironmentTest : public CppUnit::TestCase
{
public:
	EnvironmentTest(const std::string& name);
	~EnvironmentTest();

	void setUp();
	void tearDown();

	void testCreateShortcut();
    void testGetDefaultGateway();
	void testGetDesktopDirectory();
	void testUpdatePath();
#if LAUNCHER_WINDOWS
	void testWriteE2ERegistry();
#endif

	static CppUnit::Test* suite();
};

#endif
