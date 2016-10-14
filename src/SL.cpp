#include "SL.h"

namespace SubutaiLauncher {

    SL::SL() : _exitCode(-1000) 
    {

    }

    SL::~SL() 
    {

    }

    void SL::open(const std::string& path) 
    {
        auto p = path;
        p.append(".py");
        FileSystem fs;
        if (!fs.isFileExists(p)) {
            throw SubutaiException("Script file doesn't exists", 1);
        }
        _name = PyString_FromString(path.c_str());
    }

    void SL::execute(std::string module) {
        _name = PyString_FromString(module.c_str());
        execute();
    }

    void SL::execute() 
    {
        std::printf("Starting script execution\n");
        PyObject* sysPath = PySys_GetObject((char*)"path");
        PyObject* tmpPath = PyString_FromString("/tmp");
        PyList_Append(sysPath, tmpPath);
        _module = PyImport_Import(_name);
        Py_DECREF(_name);
        PyObject *pFunc, *pArgs, *pValue;
        if (_module != NULL) {
            pFunc = PyObject_GetAttrString(_module, "subutaistart");

            if (pFunc && PyCallable_Check(pFunc)) {
                std::printf("subutaistart entry point found\n");
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
