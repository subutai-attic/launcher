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
}

void PeerSetupTest::tearDown()
{
}

CppUnit::Test * PeerSetupTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("PeerSetupTest");

    CppUnit_addTest(pSuite, PeerSetupTest, debug);

	return pSuite;
}

/*
#include <iostream>
#include <cstdio>
#include <string>
#include <vector>

#include "Arguments.h"
#include "SubutaiLog.h"

using namespace CppUnit;

class SubutaiPeerSetupTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(SubutaiPeerSetupTest);
    CPPUNIT_TEST(testDebug);
    CPPUNIT_TEST_SUITE_END();
    public:
    void setUp(void);
    void tearDown(void);
    protected:
    void testDebug(void);
};

void SubutaiPeerSetupTest::setUp(void)
{

}

void SubutaiPeerSetupTest::tearDown(void)
{

}

void SubutaiPeerSetupTest::testDebug()
{
    auto l = SubutaiLauncher::Log::instance()->logger();
    l->debug() << "This " << "is " << "a " << "debug " << "message" << std::endl;
}

CPPUNIT_TEST_SUITE_REGISTRATION(SubutaiPeerSetupTest);
*/
