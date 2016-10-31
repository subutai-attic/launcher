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

#include "Arguments.h"
#include "SubutaiLog.h"

using namespace CppUnit;

class SubutaiLogTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(SubutaiLogTest);
    CPPUNIT_TEST(testDebug);
    CPPUNIT_TEST_SUITE_END();
    public:
    void setUp(void);
    void tearDown(void);
    protected:
    void testDebug(void);
};

void SubutaiLogTest::setUp(void)
{

}

void SubutaiLogTest::tearDown(void)
{

}

void SubutaiLogTest::testDebug()
{
    SubutaiLauncher::Log log(SubutaiLauncher::LL_DEBUG);
    log.debug() << "This " << "is " << "a " << "debug " << "message" << std::endl;
}

CPPUNIT_TEST_SUITE_REGISTRATION(SubutaiLogTest);
