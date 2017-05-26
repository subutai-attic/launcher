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

CppUnit::Test* HubTest::suite()
{
    CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("HubTest");

    CppUnit_addTest(pSuite, HubTest, testAuth);
    CppUnit_addTest(pSuite, HubTest, testBalance);

    return pSuite;
}
