#include "HubTest.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"

HubTest::HubTest(const std::string& name) : CppUnit::TestCase(name)
{

}

HubTest::~HubTest()
{

}

void HubTest::setUp()
{

}

void HubTest::tearDown()
{

}

void HubTest::testAuth()
{
    std::vector<std::string> args;
    SubutaiLauncher::Core c(args);
    c.initializeSSL();
    SubutaiLauncher::Hub h;
    h.setLogin("m.savochkin@gmail.com");
    h.setPassword("testhubpassword");
    assert(h.auth());
}

void HubTest::testBalance()
{
    std::vector<std::string> args;
    SubutaiLauncher::Core c(args);
    c.initializeSSL();
    SubutaiLauncher::Hub h;
    h.setLogin("m.savochkin@gmail.com");
    h.setPassword("testhubpassword");
    h.auth();
    assert(h.balance());
}

void HubTest::testSendLog()
{
    std::vector<std::string> args;
    SubutaiLauncher::Core c(args);
    c.initializeSSL();
    SubutaiLauncher::Hub h;
    h.setLogin("m.savochkin@gmail.com");
    h.setPassword("testhubpassword");
    h.auth();
    //h.sendLog(SubutaiLauncher::HL_INFO, "Launcher Testsuite: Log INFO");
    //h.sendLog(SubutaiLauncher::HL_WARNING, "Launcher Testsuite: Log WARNING");
    //h.sendLog(SubutaiLauncher::HL_ERROR, "Launcher Testsuite: Log ERROR");
    //h.sendLog(SubutaiLauncher::HL_FATAL, "Launcher Testsuite: Log FATAL");
}

void HubTest::testAddInfo()
{
    SubutaiLauncher::Hub h;
    h.addInfo("one key", "one val");
    h.addInfo("two key", "two val\nmultiline");
    auto m = h.getInfo();
    assert(m[0].key == "one key");
    assert(m[0].value == "one val");
    assert(m[1].key == "two key");
    assert(m[1].value == "two val\\nmultiline");
}

CppUnit::Test* HubTest::suite()
{
    CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("HubTest");

    CppUnit_addTest(pSuite, HubTest, testAuth);
    CppUnit_addTest(pSuite, HubTest, testBalance);
    CppUnit_addTest(pSuite, HubTest, testSendLog);
    CppUnit_addTest(pSuite, HubTest, testAddInfo);

    return pSuite;
}
