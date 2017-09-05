#include "SLTest.h"
#include "SubutaiLauncher.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"

SLTest::SLTest(const std::string& name) : CppUnit::TestCase(name)
{

}

SLTest::~SLTest()
{

}

void SLTest::setUp()
{
    Poco::Logger::get("subutai").setLevel("fatal");
}

void SLTest::tearDown()
{

}

void SLTest::testGetScheme() 
{
    SubutaiLauncher::Core *c = new SubutaiLauncher::Core("bin/testsuite");
    c->initializePython();
    SubutaiLauncher::SL sl("/tmp/subutai");
    sl.open("unit-test-get-scheme");
    sl.execute();
    delete c;
}

void SLTest::testFailedScript()
{
    SubutaiLauncher::Core *c = new SubutaiLauncher::Core("bin/testsuite");
    c->initializePython();
    SubutaiLauncher::SL sl("/tmp/subutai");
    sl.open("unit-test-failed-script");
    try 
    {
        sl.execute();
    }
    catch (SubutaiLauncher::SLException& e)
    {
        std::printf("%s", e.displayText().c_str());
    }
    /*
       std::thread t;
       try 
       {
       t = sl.executeInThread();
       }
       catch (SubutaiLauncher::SLException& e)
       {
       std::printf("Exception %s\n", e.displayText().c_str());
       }
       if (t.joinable()) t.join();*/
    delete c;
}

void SLTest::testFailedScriptThread()
{

}

void SLTest::testSLF_SetProgress()
{
    SubutaiLauncher::Core *c = new SubutaiLauncher::Core("bin/testsuite");
    c->initializePython();
    SubutaiLauncher::Session::instance();
    SubutaiLauncher::SL sl("/tmp/subutai");
    sl.open("slf-set-progress");
    sl.execute();

    SubutaiLauncher::NotificationCenter* nc = SubutaiLauncher::Session::instance()->getNotificationCenter();
    SubutaiLauncher::NotificationMessage n;
    n.type = SubutaiLauncher::N_INFO;
    bool t10 = true;
    bool t50 = true;
    bool t70 = true;
    bool t100 = true;
    while (n.type != SubutaiLauncher::N_EMPTY)
    {
        n = nc->dispatchNotification();
        if (n.type == SubutaiLauncher::N_DOUBLE_DATA)
        {
            double d;
            n.message.convert(d);
            if (t10)
            {
                assert(d == 10.0);
                t10 = false;
            }
            else if (t50)
            {
                assert(d == 50.0);
                t50 = false;
            }
            else if (t70)
            {
                assert(d == 70.0);
                t70 = false;
            }
            else if (t100)
            {
                assert(d == 100.0);
                t100 = false;
            }
        }
    }
    delete c;
}

void SLTest::testSLF_HelloWorld()
{
    try 
    {
        SubutaiLauncher::Core *c = new SubutaiLauncher::Core("bin/testsuite");
        c->initializePython();
        SubutaiLauncher::Session::instance();
        SubutaiLauncher::SL sl("/tmp/subutai");
        sl.open("slf-hello-world");
        sl.execute();
        assert(sl.exitCode() == 0);
    } 
    catch (SubutaiLauncher::SubutaiException& exc)
    {
        assert(false);
    }
    catch (SubutaiLauncher::SLException& exc)
    {
        assert(false);
    }
}

// Test importing another python file
void SLTest::testSLF_Import()
{
    try 
    {
        SubutaiLauncher::Core *c = new SubutaiLauncher::Core("bin/testsuite");
        c->initializePython();
        SubutaiLauncher::Session::instance();
        SubutaiLauncher::SL sl("/tmp/subutai");
        sl.open("slf-import");
        sl.execute();
        assert(sl.exitCode() == 0);
    } 
    catch (SubutaiLauncher::SubutaiException& exc)
    {
        assert(false);
    }
    catch (SubutaiLauncher::SLException& exc)
    {
        assert(false);
    }

}

CppUnit::Test * SLTest::suite()
{
    CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SLTest");

    CppUnit_addTest(pSuite, SLTest, testSLF_SetProgress);
    CppUnit_addTest(pSuite, SLTest, testSLF_HelloWorld);
    CppUnit_addTest(pSuite, SLTest, testSLF_Import);
    CppUnit_addTest(pSuite, SLTest, testGetScheme);
    CppUnit_addTest(pSuite, SLTest, testFailedScript);
    //CppUnit_addTest(pSuite, SLTest, testFailedScriptThread);

    return pSuite;
}
