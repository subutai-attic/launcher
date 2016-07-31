#ifndef __SL_H__
#define __SL_H__

#include <string>
#include <Python.h>

#include "Vars.h"
#include "SubutaiException.h"
#include "SLException.h"

#if LAUNCHER_LINUX
#include <sys/stat.h>
#endif

namespace SubutaiLauncher {

    class SL {
        public:
            SL();
            ~SL();
            void open(const std::string& path);
            void execute();
            long exitCode();
        private:
            PyObject* _name;
            PyObject* _module;
            long _exitCode;
    };

};

#endif
