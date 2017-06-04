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

    void testGetDefaultGateway();
	void testUpdatePath();

	static CppUnit::Test* suite();
};

#endif
