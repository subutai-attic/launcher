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
#if LAUNCHER_WINDOWS
		size_t ret;
		wcstombs_s(&ret, dst, path, sizeof(dst));
#else
        std::wcstombs(dst, path, sizeof(dst));
#endif
        logger->debug() << "Default path in Python: " << dst << std::endl;
        newpath = new wchar_t[1024];
#if LAUNCHER_WINDOWS
		wcscpy_s(newpath, 1024, path);
		wcscat_s(newpath, 1024, L":");
#else
		wcscpy(newpath, path);
		wcscat(newpath, L":");
#endif
        auto d = std::wstring(dir.begin(), dir.end());
#if LAUNCHER_WINDOWS
		wcscat_s(newpath, 1024, d.c_str());
		wcscat_s(newpath, 1024, L":.");
		wcstombs_s(&ret, dst, newpath, sizeof(dst));
#else
        wcscat(newpath, d.c_str()); 
        wcscat(newpath, L":.");
		std::wcstombs(dst, newpath, sizeof(dst));
#endif
        logger->debug() << "New path in Python: " << dst << std::endl;
        std::wprintf(L"NEWPATH: %sl\n", newpath);
        PySys_SetPath(newpath); 
#else
        char *path, *newpath; 
        path = Py_GetPath(); 
        newpath = new char[strlen(path)+dir.length()+6]; 
#if LAUNCHER_LINUX || LAUNCHER_MACOS
        strcpy(newpath, path); 
        strcat(newpath, ":");
        strcat(newpath, dir.c_str()); 
        strcat(newpath, ":.");
#else
		strcpy_s(newpath, 1024, path);
		strcpy_s(newpath, 1024, ":");
		strcpy_s(newpath, 1024, dir.c_str());
		strcpy_s(newpath, 1024, ":.");
#endif
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
    auto l = Log::instance()->logger();
    //auto p = path;
    auto settings = Session::instance()->getSettings();
    std::string p = settings->getTmpPath().c_str();
    l->info() << "SL tmp path: " << p << std::endl;
    p.append(PATH_DELIM);
    p.append(path);
    p.append(".py");
    l->info() << "SL open path: " << p << std::endl;

    FileSystem fs(_dir);
//    l->info() << "FileSytem fs(_dir): " << _dir << std::endl;

//    FileSystem fs();
    l->info() << "FileSystem fs(_dir): " << _dir << std::endl;

    std::string f = path;
    f.append(".py");
    l->info() << "SL f: " << f << std::endl;
    if (!fs.isFileExists(f) && !fs.isFileExists(p)) {
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
#if LAUNCHER_LINUX || LAUNCHER_MACOS
        usleep(100);
#else
		Sleep(100);
#endif
    }
    ncenter->clear();
    ncenter->start();
    auto l = Log::instance()->logger();
    l->info() << "Starting script execution" << std::endl;

    l->info() << "SL::execute " << _name << std::endl;

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

    l->info() << "SL::execute sysPath " << sysPath << "tmpPath " << tmpPath  << std::endl;

    _module = PyImport_Import(_name);

    l->info() << "SL::execute import module " << _name << std::endl;

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

