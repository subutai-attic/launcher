#ifndef __SSH_TEST_H__
#define __SSH_TEST_H__

#include <Poco/CppUnit/TestCase.h>

class SSHTest : public CppUnit::TestCase
{
public:
	SSHTest(const std::string& name);
	~SSHTest();

	void setUp();
	void tearDown();

    void testFindInstallation();
    void testConnect();
    void testAuthenticate();
    void testCommand();
    void testCommandChain();

	static CppUnit::Test* suite();
};

#endif
