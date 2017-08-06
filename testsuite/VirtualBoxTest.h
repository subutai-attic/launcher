#ifndef __VIRTUAL_BOX_TEST_H__
#define __VIRTUAL_BOX_TEST_H__

#include "Poco/CppUnit/TestCase.h"
#include "VirtualBox.h"

class VirtualBoxTest : public CppUnit::TestCase
{
public:
	VirtualBoxTest(const std::string& name);
	~VirtualBoxTest();

	void setUp();
	void tearDown();

	void testGetHostOnlyAdapter();
	void testGetPeers();

	static CppUnit::Test* suite();
};

#endif
