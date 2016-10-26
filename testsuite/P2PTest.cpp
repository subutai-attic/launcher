#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/ui/text/TextTestRunner.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/XmlOutputter.h>

#include "P2P.h"

using namespace CppUnit;

class P2PTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(P2PTest);
    CPPUNIT_TEST(testFindInstallation);
    CPPUNIT_TEST(testExtractVersion);
    CPPUNIT_TEST_SUITE_END();
    public:
    void setUp(void);
    void tearDown(void);
    protected:
    void testFindInstallation(void);
    void testExtractVersion(void);
};

void P2PTest::setUp(void)
{

}

void P2PTest::tearDown(void)
{

}

void P2PTest::testFindInstallation()
{
    SubutaiLauncher::P2P p;
    CPPUNIT_ASSERT(p.findInstallation());
}

void P2PTest::testExtractVersion()
{
    SubutaiLauncher::P2P p;
    p.findInstallation();
    auto version = p.extractVersion();
    CPPUNIT_ASSERT(!version.empty());
    std::printf("p2p version: %s", version.c_str());
}

CPPUNIT_TEST_SUITE_REGISTRATION(P2PTest);