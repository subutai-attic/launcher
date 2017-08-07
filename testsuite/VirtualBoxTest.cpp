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

void VirtualBoxTest::testGetPeers()
{
	SubutaiLauncher::VirtualBox vb;
	auto list = vb.getPeers();
	for (auto it = list.begin(); it != list.end(); it++)
	{
		std::printf("VBox Virtual Machine %s\n", (*it).name.c_str());
	}
}

void VirtualBoxTest::testIsMachineRunning()
{
	SubutaiLauncher::Core c;
	std::printf("!\n");
	SubutaiLauncher::VirtualBox vb;
	auto list = vb.getPeers();
	for (auto it = list.begin(); it != list.end(); it++)
	{
		auto rc = vb.isMachineRunning((*it).name);
		assert(rc);
		assert(!rc);
	}
}

CppUnit::Test * VirtualBoxTest::suite()
{
    CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("VirtualBoxTest");

	CppUnit_addTest(pSuite, VirtualBoxTest, testGetHostOnlyAdapter);
	CppUnit_addTest(pSuite, VirtualBoxTest, testGetPeers);
	CppUnit_addTest(pSuite, VirtualBoxTest, testIsMachineRunning);

    return pSuite;
}
