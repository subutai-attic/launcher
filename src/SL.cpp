#include "SL.h"

SubutaiLauncher::SL::SL(const std::string& dir) : 
    _exitCode(-1000),
    _dir(dir)
{
    auto logger = Log::instance()->logger();
    logger->debug() << "SL::SL Starting Scripting Language interface" << std::endl;
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
        logger->debug() << "SL::SL Default path in Python: " << dst << std::endl;
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
        logger->debug() << "SL::SL New path in Python: " << dst << std::endl;
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
    l->info() << "SL::open tmp path: " << p << std::endl;
    p.append(PATH_DELIM);
    p.append(path);
    p.append(".py");
    l->info() << "SL::open path: " << p << std::endl;

    FileSystem fs(_dir);
    //    l->info() << "FileSytem fs(_dir): " << _dir << std::endl;

    //    FileSystem fs();
    l->debug() << "SL::open FileSystem fs(_dir): " << _dir << std::endl;

    std::string f = path;
    f.append(".py");
    l->debug() << "SL::open f: " << f << std::endl;
    if (!fs.isFileExists(f) && !fs.isFileExists(p)) {
        throw SubutaiException("Script file doesn't exists", 1);
    }
#if PY_MAJOR_VERSION >= 3
    _name = PyUnicode_FromString(path.c_str());
#else
    _name = PyString_FromString(path.c_str());
#endif
    l->debug() << "SL::open _name=" << _name << " path=" << path  << std::endl;
}

void SubutaiLauncher::SL::execute(std::string module) {
#if PY_MAJOR_VERSION >= 3
    _name = PyUnicode_DecodeFSDefault(module.c_str());
#else
    _name = PyString_FromString(module.c_str());
#endif

    Log::instance()->logger()->debug() << "SL::execute_name: " << _name << "   module " << module << std::endl;
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
    l->debug() << "SL::execute Starting script execution" << std::endl;

    l->debug() << "SL::execute _name " << _name << std::endl;

    if (_name == NULL  )
    {
        ncenter->stop();
        l->error() << "SL::execute module name zero: exception Empty module name, 12 " << _module << std::endl;
        PyErr_Print();
        throw SLException("Empty module name", 12);
    }
    PyObject* sysPath = PySys_GetObject((char*)"path");
    l->debug() << "SL::execute   sysPath " << sysPath << " _dir: " << _dir  << std::endl;
    //l->debug() << "SL::execute   sysPath " << std::endl;

#if PY_MAJOR_VERSION >= 3
    PyObject* tmpPath = PyUnicode_FromString(_dir.c_str());
    //PyObject* tmpPath = PyUnicode_DecodeFSDefault(_dir.c_str());
#else
    PyObject* tmpPath = PyString_FromString(_dir.c_str());
#endif
    PyList_Append(sysPath, tmpPath);

    //l->debug() << "SL::execute sysPath " << sysPath  << "     tmpPath " << tmpPath << " _dir " << _dir  << std::endl;
    //l->debug() << "SL::execute sysPath     tmpPath     _dir before import "  << std::endl;
    try 
    {
        l->debug() << "SL::execute  PyImport_Import(_name) before "  << std::endl;
        _module = PyImport_Import(_name);
        if (!_module){
            l->error() << "SL::execute module name zero: exception Empty module name, 12 " << _module << std::endl;
            PyErr_Print();
            throw SLException("Cannot find specified module", 7);
        }
        l->debug() << "SL::execute Py_DECREF(_name) before "  << std::endl;
        Py_XDECREF(_name);
        l->debug() << "SL::execute Py_DECREF(_name) after "  << std::endl;
    }
    catch (std::exception const &exc)
    {
        l->error() << "SL::execute() Exception caught: " <<  exc.what()  << std::endl;
        PyErr_Print();
        //std::cerr << "Exception caught " << exc.what() << "\n";
    }
    catch (...)
    {
        std::exception_ptr p = std::current_exception();
#if LAUNCHER_LINUX
        l->error() << "SL::execute() Unknown exception caught: " <<  (p ? p.__cxa_exception_type()->name() : "null")  << std::endl;
#endif
        PyErr_Print();
        //std::clog <<(p ? p.__cxa_exception_type()->name() : "null") << std::endl;
    }
    l->debug() << "SL::execute module tryed , 7 " << std::endl;

    if (_module == NULL || _module == 0){
        //    if (_module == nullptr){
        l->error() << "SL::execute module import Cannot find specified module, 7 " << _module << std::endl;
        //PyErr_Print();
        throw SLException("Cannot find specified module", 7);
    }

    l->debug() << "SL::execute import module " << _module << " _name "  << std::endl;

    //Py_DECREF(_name);
    Py_DECREF(PyImport_ImportModule("threading"));
    PyEval_InitThreads();

    PyObject *pFunc, *pArgs, *pValue;
    if (!(_module == NULL || _module == 0)) {
        pFunc = PyObject_GetAttrString(_module, "subutaistart");

        if (pFunc && PyCallable_Check(pFunc)) {
            l->error() << "SL::execute subutaistart() entry point was not found" << std::endl;
            PyErr_Print();
            pArgs = PyTuple_New(0);
            pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);
            if (pValue != NULL) {
#if PY_MAJOR_VERSION >= 3
                _exitCode = PyLong_AsLong(pValue);
#else
                _exitCode = PyInt_AsLong(pValue);
#endif
                l->debug() << "SL::execute func: " << pFunc << " args:"  << pArgs << std::endl;
                Py_DECREF(pValue);
            }
            else {
                Py_DECREF(pFunc);
                Py_DECREF(_module);
                PyErr_Print();
                ncenter->stop();
                l->error() << "SL::execute exit code: " << _exitCode << std::endl;
                PyErr_Print();
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
        l->error() << "SL::execute _module zero: " << _module << std::endl;
        //try {
        PyErr_Print();
        //}
        //catch  
        ncenter->stop();
        l->error() << "SL::execute _module zero: Cannot find specified module " << _module << std::endl;
        PyErr_Print();
        throw SLException("Cannot find specified module", 7);
    }
    l->info() << "SL::execute Script execution completed without any errors" << std::endl;
    ncenter->stop();
    }

    std::thread SubutaiLauncher::SL::executeInThread()
    {
        Log::instance()->logger()->debug() << "SL::executeInThread   Starting script in thread" << std::endl;
        return std::thread([=] { execute(); });
    }

    std::thread SubutaiLauncher::SL::executeInThread(const std::string& module)
    {
        Log::instance()->logger()->debug() << "SL::executeInThread    Starting script in thread" << std::endl;
        return std::thread([=] { execute(module); });
    }

    long SubutaiLauncher::SL::exitCode() {
        return _exitCode;
    }

