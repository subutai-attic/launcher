#ifndef __SUBUTAI_EXCEPTION_H__
#define __SUBUTAI_EXCEPTION_H__

#include "Vars.h"
#include "Poco/Exception.h"

namespace SubutaiLauncher
{

POCO_DECLARE_EXCEPTION(Subutai_API, SubutaiException, Poco::Exception)
    
}

#endif
