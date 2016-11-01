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

#include "SubutaiException.h"
#include "Core.h"
#include "SL.h"

using namespace CppUnit;

class SLTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(SLTest);
    CPPUNIT_TEST(testScript);
    CPPUNIT_TEST(testDownloadScript);
    CPPUNIT_TEST_SUITE_END();
    public:
    void setUp(void);
    void tearDown(void);
    protected:
    void testScript(void);
    void testDownloadScript(void);
};

void SLTest::setUp(void)
{
    std::printf("\n=== Test Script Execution Output =============================\n");
}

void SLTest::tearDown(void)
{
    std::printf("=== End of Test Script Execution Output ======================\n");
    std::printf("Result");
}

void SLTest::testScript()
{
    auto core = new SubutaiLauncher::Core(std::vector<std::string>());
    core->initializePython();
    SubutaiLauncher::SL sl("../testsuite");
    try 
    {
        sl.open("unit-test");
        sl.execute();
    } 
    catch (SubutaiLauncher::SubutaiException e)
    {
        std::printf("\n!Exception: %s\n", e.displayText().c_str());
    }
    catch (std::exception e)
    {
        std::printf("\n!Exception: %s\n", e.what());
    }
    CPPUNIT_ASSERT(sl.exitCode() == 0);
    delete core;
}

void SLTest::testDownloadScript(void)
{
    const std::string& md5 = "97f62c6d7959d3dec022d0d8c4a2f085";
    auto core = new SubutaiLauncher::Core(std::vector<std::string>());
    core->initializePython();
    SubutaiLauncher::SL sl("../testsuite");
    try 
    {
        sl.open("unit-test-download");
        sl.execute();
    } 
    catch (SubutaiLauncher::SubutaiException e)
    {
        std::printf("\n!Exception: %s\n", e.displayText().c_str());
    }
    catch (std::exception e)
    {
        std::printf("\n!Exception: %s\n", e.what());
    }
    delete core;
}

CPPUNIT_TEST_SUITE_REGISTRATION(SLTest);
