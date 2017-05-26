#ifndef __SL_TEST_H__
#define __SL_TEST_H__

#include <Poco/CppUnit/TestCase.h>

class SLTest : public CppUnit::TestCase
{
public:
	SLTest(const std::string& name);
	~SLTest();

	void setUp();
	void tearDown();

    void testGetScheme();
    void testFailedScript();
    void testFailedScriptThread();

	static CppUnit::Test* suite();
};

#endif
