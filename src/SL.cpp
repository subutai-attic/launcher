#include "SL.h"

namespace SubutaiLauncher {

    SL::SL() : _exitCode(-1000) {

    }

    SL::~SL() {

    }

    void SL::open(const std::string& path) {
        auto p = path;
        p.append(".py");
        // Check if file exists
#if LAUNCHER_LINUX
        struct stat st;
        auto result = stat(p.c_str(), &st);
        if (result != 0) {
            throw SubutaiException("Script file doesn't exists", 1);
        }
#elif LAUNCHER_WINDOWS
#error Not Implemented for this platform
#elif LAUNCHER_MACOS
#error Not Implemented for this platform
#else
#error Unknown Platform
#endif
        _name = PyString_FromString(path.c_str());
    }

    void SL::execute() {
        std::printf("Starting script execution\n");
        _module = PyImport_Import(_name);
        Py_DECREF(_name);
        PyObject *pFunc, *pArgs, *pValue;
        if (_module != NULL) {

            pFunc = PyObject_GetAttrString(_module, "subutaistart");

            if (pFunc && PyCallable_Check(pFunc)) {
                pArgs = PyTuple_New(0);
                pValue = PyObject_CallObject(pFunc, pArgs);
                Py_DECREF(pArgs);
                if (pValue != NULL) {
                    _exitCode = PyInt_AsLong(pValue);
                    printf("Result of call: %ld\n", _exitCode);
                    Py_DECREF(pValue);
                }
                else {
                    Py_DECREF(pFunc);
                    Py_DECREF(_module);
                    PyErr_Print();
                    throw SLException("Script execution failed", 5);
                }
            } else {
                if (PyErr_Occurred()) PyErr_Print();
                throw SLException("Cannot find subutaistart() function", 6);
            }
            Py_XDECREF(pFunc);
            Py_DECREF(_module);
        } else {
            throw SLException("Cannot find specified module", 7);
        }
        std::printf("Script exection completed without any errors\n");
    }

    long SL::exitCode() {
        return _exitCode;
    }

};
