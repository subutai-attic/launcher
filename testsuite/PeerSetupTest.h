#ifndef __LOG_TEST_H__
#define __LOG_TEST_H__

#include <Poco/CppUnit/TestCase.h>

class PeerSetupTest : public CppUnit::TestCase
{
public:
	PeerSetupTest(const std::string& name);
	~PeerSetupTest();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();
};

#endif
