#include "Install.h"

namespace SubutaiLauncher 
{

    Install::Install()
    {

    }

    Install::~Install()
    {

    }

    void Install::preInstall()
    {
        _impl->preInstall();
    }

    void Install::install()
    {
        _impl->install();
    }

    void Install::postInstall()
    {
        _impl->postInstall();
    }

    void Install::unInstall()
    {
        _impl->unInstall();
    }

}
