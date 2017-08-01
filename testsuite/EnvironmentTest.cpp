#include "EnvironmentTest.h"
#include "SubutaiLauncher.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"

EnvironmentTest::EnvironmentTest(const std::string& name) : CppUnit::TestCase(name)
{

}

EnvironmentTest::~EnvironmentTest()
{

}

void EnvironmentTest::setUp()
{
}

void EnvironmentTest::tearDown()
{
}

void EnvironmentTest::testCreateShortcut()
{
	std::vector<std::string> args;
	SubutaiLauncher::Core c(args);
	SubutaiLauncher::Environment env;
	env.CreateShortcut("TestSuite.exe", "Test Suite");
}

void EnvironmentTest::testGetDefaultGateway()
{
	std::vector<std::string> args;
	SubutaiLauncher::Core c(args);
    SubutaiLauncher::Environment env;
    std::string result = env.getDefaultGateway();
    std::printf("Gateway: %s\n", result.c_str());
    assert(result != "unknown");
}

void EnvironmentTest::testGetDesktopDirectory()
{
	std::vector<std::string> args;
	SubutaiLauncher::Core c(args);
	SubutaiLauncher::Environment env;
	std::string result = env.getDesktopDirectory();
	std::printf("Desktop: %s\n", result.c_str());
}

void EnvironmentTest::testUpdatePath()
{
	SubutaiLauncher::Environment env;
	env.updatePath(SubutaiLauncher::Session::instance()->getSettings()->getInstallationPath() + "bin");
}

#if LAUNCHER_WINDOWS
void EnvironmentTest::testWriteE2ERegistry()
{
	std::vector<std::string> args;
	SubutaiLauncher::Core c(args);
	SubutaiLauncher::Environment env;
	env.writeE2ERegistry("");
}
#endif

CppUnit::Test * EnvironmentTest::suite()
{
    CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("EnvironmentTest");

	CppUnit_addTest(pSuite, EnvironmentTest, testCreateShortcut);
    CppUnit_addTest(pSuite, EnvironmentTest, testGetDefaultGateway);
	CppUnit_addTest(pSuite, EnvironmentTest, testGetDesktopDirectory);
	CppUnit_addTest(pSuite, EnvironmentTest, testUpdatePath);
#if LAUNCHER_WINDOWS
	CppUnit_addTest(pSuite, EnvironmentTest, testWriteE2ERegistry);
#endif

    return pSuite;
}
