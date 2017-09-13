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
    return;
    h.sendLog(Poco::Message::Priority::PRIO_WARNING, "Test Warning");
    h.sendLog(Poco::Message::Priority::PRIO_NOTICE, "Test Notice");
    h.sendLog(Poco::Message::Priority::PRIO_ERROR, "Test Error");
    h.sendLog(Poco::Message::Priority::PRIO_CRITICAL, "Test Critical");
    h.sendLog(Poco::Message::Priority::PRIO_FATAL, "Test Fatal");
    //h.sendLog(SubutaiLauncher::HL_INFO, "Launcher Testsuite: Log INFO");
    //h.sendLog(SubutaiLauncher::HL_WARNING, "Launcher Testsuite: Log WARNING");
    //h.sendLog(SubutaiLauncher::HL_ERROR, "Launcher Testsuite: Log ERROR");
    //h.sendLog(SubutaiLauncher::HL_FATAL, "Launcher Testsuite: Log FATAL");
}

void HubTest::testEncode()
{
    {
        SubutaiLauncher::Hub h;
        std::string input = "New installation starting";
        std::string expect = "TmV3IGluc3RhbGxhdGlvbiBzdGFydGluZw==";
        std::string result = h.encode(input);
        assert(result == expect);
    }
    {
        SubutaiLauncher::Hub h;
        std::string input = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Mauris rhoncus vitae dui ac posuere. Nulla fringilla non nunc ut hendrerit. Duis faucibus lectus at ex vestibulum, volutpat tempor purus consequat. Donec mattis fringilla lobortis. Duis vitae leo efficitur, auctor libero ac, imperdiet ex. Donec non condimentum tortor. Morbi accumsan ligula sit amet mi gravida, at fermentum ligula convallis. Nulla sed lorem at libero convallis condimentum. Integer feugiat enim risus, ut hendrerit tortor rutrum eu. In consectetur felis mollis odio interdum blandit. Integer et pulvinar mauris. Aenean sagittis cursus lorem. Nulla mi velit, aliquet ut faucibus et, tempus feugiat tellus. Proin sit amet nibh ut dui bibendum ultricies.";
        std::string expect = "TG9yZW0gaXBzdW0gZG9sb3Igc2l0IGFtZXQsIGNvbnNlY3RldHVyIGFkaXBpc2NpbmcgZWxpdC4gTWF1cmlzIHJob25jdXMgdml0YWUgZHVpIGFjIHBvc3VlcmUuIE51bGxhIGZyaW5naWxsYSBub24gbnVuYyB1dCBoZW5kcmVyaXQuIER1aXMgZmF1Y2lidXMgbGVjdHVzIGF0IGV4IHZlc3RpYnVsdW0sIHZvbHV0cGF0IHRlbXBvciBwdXJ1cyBjb25zZXF1YXQuIERvbmVjIG1hdHRpcyBmcmluZ2lsbGEgbG9ib3J0aXMuIER1aXMgdml0YWUgbGVvIGVmZmljaXR1ciwgYXVjdG9yIGxpYmVybyBhYywgaW1wZXJkaWV0IGV4LiBEb25lYyBub24gY29uZGltZW50dW0gdG9ydG9yLiBNb3JiaSBhY2N1bXNhbiBsaWd1bGEgc2l0IGFtZXQgbWkgZ3JhdmlkYSwgYXQgZmVybWVudHVtIGxpZ3VsYSBjb252YWxsaXMuIE51bGxhIHNlZCBsb3JlbSBhdCBsaWJlcm8gY29udmFsbGlzIGNvbmRpbWVudHVtLiBJbnRlZ2VyIGZldWdpYXQgZW5pbSByaXN1cywgdXQgaGVuZHJlcml0IHRvcnRvciBydXRydW0gZXUuIEluIGNvbnNlY3RldHVyIGZlbGlzIG1vbGxpcyBvZGlvIGludGVyZHVtIGJsYW5kaXQuIEludGVnZXIgZXQgcHVsdmluYXIgbWF1cmlzLiBBZW5lYW4gc2FnaXR0aXMgY3Vyc3VzIGxvcmVtLiBOdWxsYSBtaSB2ZWxpdCwgYWxpcXVldCB1dCBmYXVjaWJ1cyBldCwgdGVtcHVzIGZldWdpYXQgdGVsbHVzLiBQcm9pbiBzaXQgYW1ldCBuaWJoIHV0IGR1aSBiaWJlbmR1bSB1bHRyaWNpZXMu";
        std::string result = h.encode(input);
        std::printf("%s\n", result.c_str());
        assert(result == expect);
    }
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
    CppUnit_addTest(pSuite, HubTest, testEncode);

    return pSuite;
}
