#include "SSHTest.h"
#include "SubutaiLauncher.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"

SSHTest::SSHTest(const std::string& name) : CppUnit::TestCase(name)
{

}

SSHTest::~SSHTest()
{

}

void SSHTest::setUp()
{
}

void SSHTest::tearDown()
{
}

void SSHTest::testFindInstallation()
{
    SubutaiLauncher::SSH p;
    assert(p.findInstallation());
}

void SSHTest::testConnect()
{
    SubutaiLauncher::SSH *p = new SubutaiLauncher::SSH();
    p->setHost("127.0.0.1", 22);
    p->connect();
    auto connected = p->isConnected();
    delete p;
    assert(connected);
}

void SSHTest::testAuthenticate()
{
    return;
    SubutaiLauncher::SSH p;
    p.setHost("127.0.0.1", 22);
    p.setUsername("ubuntu", "ubuntu");
    p.connect();
    p.authenticate();
    assert(p.isAuthenticated());
}

void SSHTest::testCommand()
{
    return;
    SubutaiLauncher::SSH p;
    p.setHost("127.0.0.1", 22);
    p.setUsername("ubuntu", "ubuntu");
    p.connect();
    p.authenticate();
    assert(p.isAuthenticated());
    p.execute("touch /tmp/ssh-test");
}

CppUnit::Test * SSHTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SSHTest");

    CppUnit_addTest(pSuite, SSHTest, testFindInstallation);
    CppUnit_addTest(pSuite, SSHTest, testConnect);
    CppUnit_addTest(pSuite, SSHTest, testAuthenticate);
    CppUnit_addTest(pSuite, SSHTest, testCommand);

	return pSuite;
}

