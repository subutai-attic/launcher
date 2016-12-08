#include "SL.h"

SubutaiLauncher::SL::SL(const std::string& dir) : 
    _exitCode(-1000),
    _dir(dir)
{
    auto logger = Log::instance()->logger();
    logger->debug() << "Starting Scripting Language interface" << std::endl;
    if (dir != "/")
    {
#if PY_MAJOR_VERSION >= 3
        char dst[1024];
        wchar_t *path, *newpath;

        path = Py_GetPath(); 
        std::wcstombs(dst, path, sizeof(dst));
        logger->debug() << "Default path in Python: " << dst << std::endl;
        newpath = new wchar_t[1024];
        wcscpy(newpath, path); 
        wcscat(newpath, L":");
        auto d = std::wstring(dir.begin(), dir.end());
        wcscat(newpath, d.c_str()); 
        wcscat(newpath, L":.");
        std::wcstombs(dst, newpath, sizeof(dst));
        logger->debug() << "New path in Python: " << dst << std::endl;
        std::wprintf(L"NEWPATH: %sl\n", newpath);
        PySys_SetPath(newpath); 
#else
        char *path, *newpath; 
        path = Py_GetPath(); 
        newpath = new char[strlen(path)+dir.length()+6]; 
        strcpy(newpath, path); 
        strcat(newpath, ":");
        strcat(newpath, dir.c_str()); 
        strcat(newpath, ":.");
        PySys_SetPath(newpath); 
        delete[] newpath;
#endif
        // ":." for unix, or ";." for windows 
    }

}

SubutaiLauncher::SL::~SL()
{

}

void SubutaiLauncher::SL::open(const std::string& path)
{
    auto p = path;
    p.append(".py");
    FileSystem fs(_dir);
    if (!fs.isFileExists(p)) {
        throw SubutaiException("Script file doesn't exists", 1);
    }
#if PY_MAJOR_VERSION >= 3
    _name = PyUnicode_FromString(path.c_str());
#else
    _name = PyString_FromString(path.c_str());
#endif
}

void SubutaiLauncher::SL::execute(std::string module) {
#if PY_MAJOR_VERSION >= 3
    _name = PyUnicode_DecodeFSDefault(module.c_str());
#else
    _name = PyString_FromString(module.c_str());
#endif
    execute();
}

void SubutaiLauncher::SL::execute()
{
    auto ncenter = Session::instance()->getNotificationCenter();
    while (ncenter->isRunning())
    {
        usleep(100);
    }
    ncenter->clear();
    ncenter->start();
    auto l = Log::instance()->logger();
    l->info() << "Starting script execution" << std::endl;
    if (_name == NULL)
    {
        ncenter->stop();
        throw SLException("Empty module name", 12);
    }
    PyObject* sysPath = PySys_GetObject((char*)"path");
#if PY_MAJOR_VERSION >= 3
    PyObject* tmpPath = PyUnicode_FromString(_dir.c_str());
    //PyObject* tmpPath = PyUnicode_DecodeFSDefault(_dir.c_str());
#else
    PyObject* tmpPath = PyString_FromString(_dir.c_str());
#endif
    PyList_Append(sysPath, tmpPath);
    _module = PyImport_Import(_name);
    Py_DECREF(_name);
    Py_DECREF(PyImport_ImportModule("threading"));
    PyEval_InitThreads();

    PyObject *pFunc, *pArgs, *pValue;
    if (_module != NULL) {
        pFunc = PyObject_GetAttrString(_module, "subutaistart");

        if (pFunc && PyCallable_Check(pFunc)) {
            l->error() << "subutaistart() entry point was not found" << std::endl;
            pArgs = PyTuple_New(0);
            pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);
            if (pValue != NULL) {
#if PY_MAJOR_VERSION >= 3
                _exitCode = PyLong_AsLong(pValue);
#else
                _exitCode = PyInt_AsLong(pValue);
#endif
                l->debug() << "Script execution exit status: " << _exitCode << std::endl;
                Py_DECREF(pValue);
            }
            else {
                Py_DECREF(pFunc);
                Py_DECREF(_module);
                PyErr_Print();
                ncenter->stop();
                throw SLException("Script execution failed", 5);
            }
        }
        else {
            if (PyErr_Occurred()) PyErr_Print();
            ncenter->stop();
            throw SLException("Cannot find subutaistart() function", 6);
        }
        Py_XDECREF(pFunc);
        Py_DECREF(_module);
    }
    else {
        PyErr_Print();
        ncenter->stop();
        throw SLException("Cannot find specified module", 7);
    }
    l->info() << "Script execution completed without any errors" << std::endl;
    ncenter->stop();
}

std::thread SubutaiLauncher::SL::executeInThread()
{
    Log::instance()->logger()->debug() << "Starting script in thread" << std::endl;
    return std::thread([=] { execute(); });
}

std::thread SubutaiLauncher::SL::executeInThread(const std::string& module)
{
    Log::instance()->logger()->debug() << "Starting script in thread" << std::endl;
    return std::thread([=] { execute(module); });
}

long SubutaiLauncher::SL::exitCode() {
    return _exitCode;
}

