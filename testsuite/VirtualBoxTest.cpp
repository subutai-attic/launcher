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

#include "VirtualBox.h"

using namespace CppUnit;

class VirtualBoxTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(VirtualBoxTest);
    CPPUNIT_TEST(testParseVms);
    CPPUNIT_TEST_SUITE_END();
    public:
    void setUp(void);
    void tearDown(void);
    protected:
    void testParseVms(void);
};

void VirtualBoxTest::setUp(void)
{

}

void VirtualBoxTest::tearDown(void)
{

}

void VirtualBoxTest::testParseVms()
{
    SubutaiLauncher::VirtualBox v;
    auto vms = v.parseVms("\"Name1\" {ID1}\n\"Name2\" {ID2}\n");
    CPPUNIT_ASSERT(vms.size() == 2);
    std::printf("!!!!!!!!%s\n", vms[0].name.c_str());
    CPPUNIT_ASSERT(vms[0].name == "Name1");
    CPPUNIT_ASSERT(vms[1].name == "Name2");
}

CPPUNIT_TEST_SUITE_REGISTRATION(VirtualBoxTest);
