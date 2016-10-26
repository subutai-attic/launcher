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

#include "SSH.h"

using namespace CppUnit;

class SSHTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(SSHTest);
    CPPUNIT_TEST(testFindInstallation);
    CPPUNIT_TEST(testConnect);
    CPPUNIT_TEST(testAuthenticate);
    CPPUNIT_TEST(testCommand);
    CPPUNIT_TEST_SUITE_END();
    public:
    void setUp(void);
    void tearDown(void);
    protected:
    void testFindInstallation(void);
    void testConnect(void);
    void testAuthenticate(void);
    void testCommand(void);
};

void SSHTest::setUp(void)
{

}

void SSHTest::tearDown(void)
{

}

void SSHTest::testFindInstallation()
{
    SubutaiLauncher::SSH p;
    CPPUNIT_ASSERT(p.findInstallation());
}

void SSHTest::testConnect()
{
    SubutaiLauncher::SSH p;
    p.setHost("127.0.0.1", 22);
    p.connect();
    CPPUNIT_ASSERT(p.isConnected());
}

void SSHTest::testAuthenticate()
{
    SubutaiLauncher::SSH p;
    p.setHost("127.0.0.1", 22);
    p.setUsername("ubuntu", "ubuntu");
    p.connect();
    p.authenticate();
    CPPUNIT_ASSERT(p.isAuthenticated());
}

void SSHTest::testCommand()
{
    SubutaiLauncher::SSH p;
    p.setHost("127.0.0.1", 22);
    p.setUsername("ubuntu", "ubuntu");
    p.connect();
    p.authenticate();
    CPPUNIT_ASSERT(p.isAuthenticated());
    p.execute("touch /tmp/ssh-test");
}

CPPUNIT_TEST_SUITE_REGISTRATION(SSHTest);
