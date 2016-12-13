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

#include "SubutaiException.h"
#include "Core.h"
#include "SL.h"

using namespace CppUnit;

class SLTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(SLTest);
    CPPUNIT_TEST(testScript);
    CPPUNIT_TEST(testDownloadScript);
    CPPUNIT_TEST(testInstall_p2p);
    CPPUNIT_TEST(testInstall_tray);
    CPPUNIT_TEST(testInstall_peer);
    CPPUNIT_TEST_SUITE_END();
    public:
    void setUp(void);
    void tearDown(void);
    protected:
    void testScript(void);
    void testDownloadScript(void);
    void testInstall_p2p(void);
    void testInstall_tray(void);
    void testInstall_peer(void);
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
    return;
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
    return;
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

void SLTest::testInstall_p2p(void)
{
    return;
    auto core = new SubutaiLauncher::Core(std::vector<std::string>());
    core->initializePython();
    SubutaiLauncher::SL sl("../scripts");
    try 
    {
        sl.open("p2p_install");
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

void SLTest::testInstall_tray(void)
{
    return;
    auto core = new SubutaiLauncher::Core(std::vector<std::string>());
    core->initializePython();
    SubutaiLauncher::SL sl("../scripts");
    try 
    {
        sl.open("tray_install");
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

void SLTest::testInstall_peer(void)
{
    return;
    auto core = new SubutaiLauncher::Core(std::vector<std::string>());
    core->initializePython();
    SubutaiLauncher::SL sl("../scripts");
    try 
    {
        sl.open("peer_install");
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
*/
