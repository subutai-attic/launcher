#include "VirtualBoxTest.h"
#include "SubutaiLauncher.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"

VirtualBoxTest::VirtualBoxTest(const std::string& name) : CppUnit::TestCase(name)
{

}

VirtualBoxTest::~VirtualBoxTest()
{

}

void VirtualBoxTest::setUp()
{
}

void VirtualBoxTest::tearDown()
{
}

CppUnit::Test * VirtualBoxTest::suite()
{
    CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("VirtualBoxTest");


    return pSuite;
}
