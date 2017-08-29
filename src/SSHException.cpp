#include "SSHException.h"
#include <typeinfo>

namespace SubutaiLauncher {

POCO_IMPLEMENT_EXCEPTION(SSHException, Poco::Exception, "SSH Exception")

}
