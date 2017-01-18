#include "LogTest.h"
#include "SubutaiLauncher.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"

LogTest::LogTest(const std::string& name) : CppUnit::TestCase(name)
{

}

LogTest::~LogTest()
{

}

void LogTest::setUp()
{
}

void LogTest::tearDown()
{
}

void LogTest::debug()
{
	auto l = SubutaiLauncher::Log::instance()->logger();
	l->debug() << "This " << "is " << "a " << "debug " << "message" << std::endl;
}

CppUnit::Test * LogTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("LogTest");

    CppUnit_addTest(pSuite, LogTest, debug);

	return pSuite;
}

/*
#include <iostream>
#include <cstdio>
#include <string>
#include <vector>

#include "Arguments.h"
#include "SubutaiLog.h"

using namespace CppUnit;

class SubutaiLogTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(SubutaiLogTest);
    CPPUNIT_TEST(testDebug);
    CPPUNIT_TEST_SUITE_END();
    public:
    void setUp(void);
    void tearDown(void);
    protected:
    void testDebug(void);
};

void SubutaiLogTest::setUp(void)
{

}

void SubutaiLogTest::tearDown(void)
{

}

void SubutaiLogTest::testDebug()
{
    auto l = SubutaiLauncher::Log::instance()->logger();
    l->debug() << "This " << "is " << "a " << "debug " << "message" << std::endl;
}

CPPUNIT_TEST_SUITE_REGISTRATION(SubutaiLogTest);
*/
