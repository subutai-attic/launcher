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

void VirtualBoxTest::testGetHostOnlyAdapter()
{
	SubutaiLauncher::VirtualBox vb;
	vb.findInstallation();
	std::string adapter = vb.getHostOnlyAdapter();
	std::printf("Adapter: %s\n", adapter.c_str());
}

CppUnit::Test * VirtualBoxTest::suite()
{
    CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("VirtualBoxTest");

	CppUnit_addTest(pSuite, VirtualBoxTest, testGetHostOnlyAdapter);

    return pSuite;
}
