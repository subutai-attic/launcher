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

class PrivilegesTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(PrivilegesTest);
    CPPUNIT_TEST(testSudo);
    CPPUNIT_TEST_SUITE_END();
    public:
    void setUp(void);
    void tearDown(void);
    protected:
    void testSudo(void);
};

void PrivilegesTest::setUp(void)
{

}

void PrivilegesTest::tearDown(void)
{

}

void PrivilegesTest::testSudo()
{
    SubutaiLauncher::SubutaiProcess p;
    std::vector<std::string> args;
    args.push_back("touch");
    args.push_back("/test-file");
    p.launch("gksudo", args, "/usr/bin");

    SubutaiLauncher::SubutaiProcess p2;
    args.clear();
    args.push_back("/test-file2");
    p.launch("touch", args, "/usr/bin/touch");
}

CPPUNIT_TEST_SUITE_REGISTRATION(PrivilegesTest);
*/
