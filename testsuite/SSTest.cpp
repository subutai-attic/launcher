#include "SSTest.h"
#include "SubutaiLauncher.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"

SSTest::SSTest(const std::string& name) : CppUnit::TestCase(name)
{

}

SSTest::~SSTest()
{

}

void SSTest::setUp()
{
}

void SSTest::tearDown()
{
}

void SSTest::testCheckPeerInstall()
{

}

CppUnit::Test * SSTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SSTest");

    CppUnit_addTest(pSuite, SSTest, testCheckPeerInstall);

	return pSuite;
}

