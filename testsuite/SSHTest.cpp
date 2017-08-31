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
    SubutaiLauncher::SSH *pSSH = new SubutaiLauncher::SSH();
    assert(pSSH->findInstallation());
    delete pSSH;
}

void SSHTest::testConnect()
{
    SubutaiLauncher::SSH *pSSH = new SubutaiLauncher::SSH();
    pSSH->setHost("127.0.0.1", 4567);
    pSSH->connect();
    auto connected = pSSH->isConnected();
    delete pSSH;
    assert(connected);
}

void SSHTest::testAuthenticate()
{
    SubutaiLauncher::SSH* pSSH = new SubutaiLauncher::SSH();
    pSSH->setHost("127.0.0.1", 4567);
    pSSH->setUsername("subutai", "ubuntai");
    pSSH->connect();
    pSSH->authenticate();
    assert(pSSH->isAuthenticated());
}

void SSHTest::testCommand()
{
    SubutaiLauncher::SSH* pSSH = new SubutaiLauncher::SSH();
    pSSH->setHost("127.0.0.1", 4567);
    pSSH->setUsername("subutai", "ubuntai");
    pSSH->connect();
    pSSH->authenticate();
    pSSH->execute("touch /tmp/unit-test-command");
    delete pSSH;
}

void SSHTest::testExecuteInShell()
{
    auto sess = SubutaiLauncher::Session::instance();
    SubutaiLauncher::SSH::initialize();
    sess->setSSHCredentials("subutai", "ubuntai", "127.0.0.1", 4567);
    sess->makeSSHSession("ut");
    std::string out;
    try 
    {
        auto s = sess->getSSHSession("ut");
        assert(s != nullptr);
        s->openShell();
        out = s->executeInShell("touch /tmp/unit-test-in-shell ;\r\r");
        s->closeShell();
        sess->finalizeSSHSession("ut");
    }
    catch (SubutaiLauncher::SSHException& exc)
    {
        std::printf("Exception: %s\n", exc.displayText().c_str());
    }
}

void SSHTest::testExecuteInThread()
{
    try 
    {
        SubutaiLauncher::SSH::initialize();
        std::printf("-1\n");
        SubutaiLauncher::SSH* pSSH = new SubutaiLauncher::SSH(true);
        std::printf("0\n");
        pSSH->openChannel();
        std::printf("1\n");
        pSSH->setHost("127.0.0.1", 4567);
        std::printf("2\n");
        pSSH->setUsername("subutai", "ubuntai");
        std::printf("3\n");
        std::thread pThread = pSSH->executeInThread("touch /tmp/unit-test-in-thread");
        std::printf("4\n");
        if (pThread.joinable()) pThread.join();
        std::printf("5\n");
        delete pSSH;
    }
    catch (SubutaiLauncher::SSHException& exc)
    {
        std::printf("Exception: %s\n", exc.displayText().c_str());
    }
}

CppUnit::Test * SSHTest::suite()
{
    CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SSHTest");

    //CppUnit_addTest(pSuite, SSHTest, testFindInstallation);
    //CppUnit_addTest(pSuite, SSHTest, testConnect);
    //CppUnit_addTest(pSuite, SSHTest, testAuthenticate);
    //CppUnit_addTest(pSuite, SSHTest, testCommand);
    //CppUnit_addTest(pSuite, SSHTest, testExecuteInThread);
    CppUnit_addTest(pSuite, SSHTest, testExecuteInShell);

    return pSuite;
}

