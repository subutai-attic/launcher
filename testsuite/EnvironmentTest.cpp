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

void EnvironmentTest::testGetDefaultGateway()
{
    SubutaiLauncher::Environment env;
    std::string result = env.getDefaultGateway();
    std::printf("Gateway: %s", result.c_str());
    assert(result != "unknown");
}

CppUnit::Test * EnvironmentTest::suite()
{
    CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("EnvironmentTest");

    CppUnit_addTest(pSuite, EnvironmentTest, testGetDefaultGateway);

    return pSuite;
}
