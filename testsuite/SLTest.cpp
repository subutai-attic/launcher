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
	std::printf("\n==========================================================\n");
}

void SLTest::testGetScheme() 
{
    SubutaiLauncher::Core *c = new SubutaiLauncher::Core(std::vector<std::string>());
    c->initializePython();
    SubutaiLauncher::SL sl("../testsuite");
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
	try 
	{
		sl.execute();
	}
	catch (SubutaiLauncher::SLException& e)
	{
		std::printf("%s", e.displayText());
	}
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

void SLTest::testSLF_SetProgress()
{
	std::printf("SetProgress SL Function test started\n");
	SubutaiLauncher::Core *c = new SubutaiLauncher::Core(std::vector<std::string>());
	c->initializePython();
	SubutaiLauncher::Session::instance();
	SubutaiLauncher::SL sl("../testsuite");
	sl.open("slf-set-progress");
	sl.execute();

	SubutaiLauncher::NotificationCenter* nc = SubutaiLauncher::Session::instance()->getNotificationCenter();
	SubutaiLauncher::NotificationMessage n;
	n.type = SubutaiLauncher::N_INFO;
	while (n.type != SubutaiLauncher::N_EMPTY)
	{
		n = nc->dispatchNotification();
		if (n.type == SubutaiLauncher::N_DOUBLE_DATA)
		{
			double d;
			n.message.convert(d);
			std::printf("Progress: %f\n", d);
		}
		else
		{
			std::printf("Unknown type\n");
		}
	}
	delete c;
	std::printf("SetProgress SL Function test finished\n");
}

CppUnit::Test * SLTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SLTest");

	CppUnit_addTest(pSuite, SLTest, testSLF_SetProgress);
    CppUnit_addTest(pSuite, SLTest, testGetScheme);
    CppUnit_addTest(pSuite, SLTest, testFailedScript);
    //CppUnit_addTest(pSuite, SLTest, testFailedScriptThread);

	return pSuite;
}
