/*
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

#include "SubutaiProcess.h"

using namespace CppUnit;

class SubutaiProcessTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(SubutaiProcessTest);
    CPPUNIT_TEST(testNormalCommand);
    CPPUNIT_TEST(testBadCommand);
    CPPUNIT_TEST_SUITE_END();
    public:
    void setUp(void);
    void tearDown(void);
    protected:
    void testNormalCommand(void);
    void testBadCommand(void);
};

void SubutaiProcessTest::setUp(void)
{

}

void SubutaiProcessTest::tearDown(void)
{

}

void SubutaiProcessTest::testNormalCommand()
{
    std::printf("Running ls -l /\n");
    std::vector<std::string> args;
    args.push_back("-l");
    args.push_back("/");
    SubutaiLauncher::SubutaiProcess p;
    p.launch("ls", args, "/bin");
    int exit_code = p.wait();
    auto buffer = p.getOutputBuffer();
    CPPUNIT_ASSERT(exit_code == 0);
    CPPUNIT_ASSERT(!buffer.empty());
}

void SubutaiProcessTest::testBadCommand()
{

}

CPPUNIT_TEST_SUITE_REGISTRATION(SubutaiProcessTest);
*/
