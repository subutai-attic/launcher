#include "TestSuite.h"

CppUnit::Test* SubutaiLauncherTestSuite::suite()
{
    CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SubutaiLauncher");

    pSuite->addTest(DownloaderTest::suite());
    pSuite->addTest(PeerSetupTest::suite());
    pSuite->addTest(SSHTest::suite());
    pSuite->addTest(SLTest::suite());

    return pSuite;
}

CppUnitMain(SubutaiLauncherTestSuite)
