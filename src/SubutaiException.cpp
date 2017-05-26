#include "SubutaiException.h"
#include <typeinfo>


namespace SubutaiLauncher {

POCO_IMPLEMENT_EXCEPTION(SubutaiException, Poco::Exception, "Launcher Exception")

}
