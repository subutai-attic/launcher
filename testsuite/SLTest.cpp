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
    delete c;
}

void SLTest::testFailedScript()
{
    SubutaiLauncher::Core *c = new SubutaiLauncher::Core(std::vector<std::string>());
    c->initializePython();
    SubutaiLauncher::SL sl("../testsuite");
    sl.open("unit-test-failed-script");
	sl.execute();
	/*
	std::thread t;
	try 
	{
		t = sl.executeInThread();
	}
	catch (SubutaiLauncher::SLException& e)
	{
		std::printf("Exception %s\n", e.displayText().c_str());
	}
	if (t.joinable()) t.join();*/
    delete c;
}

void SLTest::testFailedScriptThread()
{

}

CppUnit::Test * SLTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SLTest");

    CppUnit_addTest(pSuite, SLTest, testGetScheme);
    CppUnit_addTest(pSuite, SLTest, testFailedScript);
    CppUnit_addTest(pSuite, SLTest, testFailedScriptThread);

	return pSuite;
}
