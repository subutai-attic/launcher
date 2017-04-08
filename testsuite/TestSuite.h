#include "Poco/CppUnit/TestRunner.h"
#include "Poco/CppUnit/TestSuite.h"

#include "DownloaderTest.h"
#include "PeerSetupTest.h"
#include "SSHTest.h"
#include "SLTest.h"
#include "HubTest.h"

class SubutaiLauncherTestSuite
{
    public:
        static CppUnit::Test* suite();
};
