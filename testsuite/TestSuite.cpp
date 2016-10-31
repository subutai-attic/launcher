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

int main(int argc, char* argv[])
{
    Arguments::instance()->set(argc, argv);
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
