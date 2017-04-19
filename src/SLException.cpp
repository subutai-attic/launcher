#include "SLException.h"
#include <typeinfo>

namespace SubutaiLauncher {

POCO_IMPLEMENT_EXCEPTION(SLException, Poco::Exception, "Scripting Language Exception")

}
