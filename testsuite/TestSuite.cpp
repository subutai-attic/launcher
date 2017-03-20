#include "Poco/CppUnit/TestRunner.h"
#include "Poco/CppUnit/TestSuite.h"

#include "LogTest.h"
#include "DownloaderTest.h"
#include "PeerSetupTest.h"
#include "SSHTest.h"

class SubutaiLauncherTestSuite
{
public:
	static CppUnit::Test* suite()
	{
		CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SubutaiLauncher");

		pSuite->addTest(LogTest::suite());
		pSuite->addTest(DownloaderTest::suite());
        pSuite->addTest(PeerSetupTest::suite());
        pSuite->addTest(SSHTest::suite());

		return pSuite;
	}
};

CppUnitMain(SubutaiLauncherTestSuite)
