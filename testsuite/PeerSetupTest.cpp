#include "PeerSetupTest.h"
#include "SubutaiLauncher.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"

PeerSetupTest::PeerSetupTest(const std::string& name) : CppUnit::TestCase(name)
{

}

PeerSetupTest::~PeerSetupTest()
{

}

void PeerSetupTest::setUp()
{
    std::printf("Complete peer installation test\n");
}

void PeerSetupTest::tearDown()
{
}

void PeerSetupTest::runPeerSetup() 
{
    SubutaiLauncher::Core *c = new SubutaiLauncher::Core(std::vector<std::string>());
    c->initializePython();
    //c->run();
    SubutaiLauncher::SL sl("./");
    sl.open("unit-test-peer-setup");
    sl.execute();
}

CppUnit::Test * PeerSetupTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("PeerSetupTest");

    CppUnit_addTest(pSuite, PeerSetupTest, runPeerSetup);

	return pSuite;
}
