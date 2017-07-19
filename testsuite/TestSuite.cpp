#include "TestSuite.h"

CppUnit::Test* SubutaiLauncherTestSuite::suite()
{
    CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SubutaiLauncher");

    pSuite->addTest(DownloaderTest::suite());
    pSuite->addTest(PeerSetupTest::suite());
    pSuite->addTest(SSHTest::suite());
    pSuite->addTest(SLTest::suite());
    pSuite->addTest(HubTest::suite());
    pSuite->addTest(VirtualBoxTest::suite());
    pSuite->addTest(EnvironmentTest::suite());
	pSuite->addTest(SSTest::suite());

    return pSuite;
}

CppUnitMain(SubutaiLauncherTestSuite)
