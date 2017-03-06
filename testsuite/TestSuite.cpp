#include "Poco/CppUnit/TestRunner.h"
#include "Poco/CppUnit/TestSuite.h"

#include "LogTest.h"
#include "DownloaderTest.h"

class SubutaiLauncherTestSuite
{
public:
	static CppUnit::Test* suite()
	{
		CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SubutaiLauncher");

		pSuite->addTest(LogTest::suite());
		pSuite->addTest(DownloaderTest::suite());

		return pSuite;
	}
};

CppUnitMain(SubutaiLauncherTestSuite)
