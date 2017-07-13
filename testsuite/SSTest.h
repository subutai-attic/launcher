#ifndef __SS_TEST_H__
#define __SS_TEST_H__

#include <Poco/CppUnit/TestCase.h>

class SSTest : public CppUnit::TestCase
{
public:
	SSTest(const std::string& name);
	~SSTest();

	void setUp();
	void tearDown();

    void testCheckPeerInstall();

	static CppUnit::Test* suite();
};

#endif
