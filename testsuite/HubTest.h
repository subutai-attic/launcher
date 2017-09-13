#ifndef __HUB_TEST_H__
#define __HUB_TEST_H__

#include "Poco/CppUnit/TestCase.h"

#include "Core.h"
#include "Hub.h"

class HubTest : public CppUnit::TestCase
{
    public:
        HubTest(const std::string& name);
        ~HubTest();

        void setUp();
        void tearDown();

        void testAuth();
        void testBalance();
        void testSendLog();
        void testAddInfo();
        void testEncode();
        
	    static CppUnit::Test* suite();
};

#endif
