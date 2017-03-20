#ifndef __PEER_SETUP_TEST_H__
#define __PEER_SETUP_TEST_H__

#include <Poco/CppUnit/TestCase.h>

class PeerSetupTest : public CppUnit::TestCase
{
public:
	PeerSetupTest(const std::string& name);
	~PeerSetupTest();

	void setUp();
	void tearDown();

    void runPeerSetup();

	static CppUnit::Test* suite();
};

#endif
