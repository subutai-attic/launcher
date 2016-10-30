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
    CPPUNIT_TEST_SUITE_END();
    public:
    void setUp(void);
    void tearDown(void);
    protected:
    void testScript(void);
};

void SLTest::setUp(void)
{

}

void SLTest::tearDown(void)
{

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

CPPUNIT_TEST_SUITE_REGISTRATION(SLTest);
