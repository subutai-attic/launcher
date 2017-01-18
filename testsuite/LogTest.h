#ifndef __LOG_TEST_H__
#define __LOG_TEST_H__

#include "Poco/CppUnit/TestCase.h"

class LogTest : public CppUnit::TestCase
{
public:
	LogTest(const std::string& name);
	~LogTest();

	void setUp();
	void tearDown();

	void debug();

	static CppUnit::Test* suite();
};

#endif
