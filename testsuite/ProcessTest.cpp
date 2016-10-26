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

#include "Process.h"

using namespace CppUnit;

class ProcessTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(ProcessTest);
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

void ProcessTest::setUp(void)
{

}

void ProcessTest::tearDown(void)
{

}

void ProcessTest::testNormalCommand()
{
    std::printf("Running ls -l /\n");
    std::vector<std::string> args;
    args.push_back("-l");
    args.push_back("/");
    SubutaiLauncher::Process p;
    p.launch("ls", args, "/bin");
    int exit_code = p.wait();
    auto buffer = p.getOutputBuffer();
    auto err = p.getErrorBuffer();
    CPPUNIT_ASSERT(exit_code == 0);
    CPPUNIT_ASSERT(!buffer.empty());
    CPPUNIT_ASSERT(err.empty());
}

void ProcessTest::testBadCommand()
{

}

CPPUNIT_TEST_SUITE_REGISTRATION(ProcessTest);
int main(int argc, char* argv[])
{
    CPPUNIT_NS::TestResult testResult;
    CPPUNIT_NS::TestResultCollector collectedResults;
    testResult.addListener(&collectedResults);
    CPPUNIT_NS::BriefTestProgressListener progress;
    testResult.addListener(&progress);
    CPPUNIT_NS::TestRunner testRunner;
    testRunner.addTest(CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest());
    testRunner.run(testResult);
    CPPUNIT_NS::CompilerOutputter compilerOutputter(&collectedResults, std::cerr);
    compilerOutputter.write();
    return collectedResults.wasSuccessful() ? 0 : 1;
}
