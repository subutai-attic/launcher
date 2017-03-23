#include "SLTest.h"
#include "SubutaiLauncher.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"

SLTest::SLTest(const std::string& name) : CppUnit::TestCase(name)
{

}

SLTest::~SLTest()
{

}

void SLTest::setUp()
{
}

void SLTest::tearDown()
{
}

void SLTest::testGetScheme() 
{
    SubutaiLauncher::Core *c = new SubutaiLauncher::Core(std::vector<std::string>());
    c->initializePython();
    SubutaiLauncher::SL sl("./");
    sl.open("unit-test-get-scheme");
    sl.execute();
}

CppUnit::Test * SLTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SLTest");

    CppUnit_addTest(pSuite, SLTest, testGetScheme);

	return pSuite;
}
