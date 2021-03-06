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

	// Test functions
	void testSLF_SetProgress();
    void testSLF_HelloWorld();
    void testSLF_Import();

	static CppUnit::Test* suite();
};

#endif
