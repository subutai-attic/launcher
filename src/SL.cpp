#include "SL.h"

namespace SubutaiLauncher
{

    SL::SL(const std::string& dir) :
        _running(true),
        _exitCode(-1000),
        _dir(dir)
    {
        _logger = &Poco::Logger::get("subutai");
        _logger->debug("Starting scripting language interface");
        if (dir != "/")
        {
            char dst[1024];
            wchar_t *path, *newpath;

            path = Py_GetPath();
#if LAUNCHER_WINDOWS
            size_t ret;
            wcstombs_s(&ret, dst, path, sizeof(dst));
#else
            std::wcstombs(dst, path, sizeof(dst));
#endif
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
            //std::wprintf(L"NEWPATH: %sl\n", newpath);
            PySys_SetPath(newpath);
            delete newpath;
        }

    }

    SL::~SL()
    {

    }

    void SL::open(const std::string& path)
    {
        auto settings = Session::instance()->getSettings();
        std::string p = settings->getTmpPath().c_str();
        _logger->trace("SL::Opening tmp path: %s", p);
        p.append(PATH_DELIM);
        p.append(path);
        p.append(".py");
        _logger->trace("SL::Open path %s", p);

        FileSystem fs(_dir);
        _logger->trace("SL::open dir: %s", _dir);

        std::string f = path;
        f.append(".py");
        _logger->trace("SL::open file: %s", f);
        if (!fs.isFileExists(f) && !fs.isFileExists(p)) 
        {
            throw SubutaiException("Script file doesn't exists", 1);
        }
        _name = PyUnicode_FromString(path.c_str());
    }

    void SL::execute(std::string module) 
    {
        _name = PyUnicode_DecodeFSDefault(module.c_str());
        _logger->debug("SL::Executing %s [Module: %s]", _name, module);
        execute();
    }

    void SL::execute()
    {
        _logger->debug("Preparing script for execution");
        std::string pErrorText("");
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
        _running = true;
        std::string pName(PyBytes_AsString(PyUnicode_AsEncodedString(_name, "utf-8", "")));
        _logger->information("Starting script execution: %s", pName);

        if (_name == NULL)
        {
            ncenter->stop();
            _logger->error("SL::execute: Empty module name: %s", _module);
            throw SLException("Empty module name", 12);
        }
        PyObject* sysPath = PySys_GetObject((char*)"path");
        if (sysPath == NULL)
        {
            _logger->error("Failed to extract path from python system");
        }
        else
        {
            PyObject* tmpPath = PyUnicode_FromString(_dir.c_str());
            PyList_Append(sysPath, tmpPath);
        }

        _module = PyImport_Import(_name);
        if (!_module) {
            _logger->error("Can't find specified module [a]");
            ncenter->add(SCRIPT_FINISHED);
            ncenter->stop();
            if (PyErr_Occurred())
            {
                PyErr_Print();
                /*
                // TODO: Add ptype and ptraceback
                PyObject *ptype, *pvalue, *ptraceback;
                PyErr_Fetch(&ptype, &pvalue, &ptraceback);
                char *pErrorMessage = PyBytes_AsString(PyUnicode_AsEncodedString(pvalue, "utf-8", ""));
                std::string pErr = Poco::format("%s", std::string(pErrorMessage));
                _logger->critical("Script Error [f]: %s", pErr);
                ncenter->add(SCRIPT_FINISHED);
                ncenter->stop();
                _running = false;
                throw SLException(pErr);
                */
            }
            else
            {
                PyErr_Print();
                throw SLException("Cannot find specified module", 7);
            }
        }
        Py_XDECREF(_name);

        if (_module == NULL || _module == 0) {
            ncenter->add(SCRIPT_FINISHED);
            ncenter->stop();
            _logger->debug("SL::execute Can't find module %s", _module);
            throw SLException("Cannot find specified module", 7);
        }

        //Py_DECREF(_name);
        //PyEval_InitThreads();
        //Py_DECREF(PyImport_ImportModule("threading"));

        PyObject *pFunc, *pArgs, *pValue;
        if (!(_module == NULL || _module == 0))
        {
            pFunc = PyObject_GetAttrString(_module, "subutaistart");

            if (pFunc && PyCallable_Check(pFunc))
            {
                if (PyErr_Occurred())
                {
                    std::printf("2\n");
                    // TODO: Add ptype and ptraceback
                    PyObject *ptype, *pvalue, *ptraceback;
                    PyErr_Fetch(&ptype, &pvalue, &ptraceback);
                    char *pErrorMessage = PyBytes_AsString(PyUnicode_AsEncodedString(pvalue, "utf-8", ""));
                    std::string pErr = Poco::format("%s", std::string(pErrorMessage));
                    _logger->critical("Script Error [a]: %s", pErr);
                    ncenter->add(SCRIPT_FINISHED);
                    ncenter->stop();
                    _running = false;
                    throw SLException(pErr);
                }
                pArgs = PyTuple_New(0);
                pValue = PyObject_CallObject(pFunc, pArgs);
                Py_DECREF(pArgs);
                if (pValue != NULL)
                {
                    _exitCode = PyLong_AsLong(pValue);
                    Py_DECREF(pValue);
                }
                else
                {
                    Py_DECREF(pFunc);
                    Py_DECREF(_module);
                    if (PyErr_Occurred())
                    {
                        std::printf("3\n");
                        PyErr_Print();
                        PyObject *ptype, *pvalue, *ptraceback;
                        PyErr_Fetch(&ptype, &pvalue, &ptraceback);
                        char *pErrorMessage = PyBytes_AsString(PyUnicode_AsEncodedString(pvalue, "utf-8", ""));
                        std::string pErr = Poco::format("%s", std::string(pErrorMessage));
                        _logger->critical("Script Error [b]: %s", pErr);
                        handleTraceback((PyTracebackObject*)ptraceback, 5);
                        ncenter->add(SCRIPT_FINISHED);
                        ncenter->stop();
                        _running = false;
                        throw SLException(pErr);
                    }
                }
            }
            else
            {
                if (PyErr_Occurred())
                {
                    std::printf("4\n");
                    // TODO: Add ptype and ptraceback
                    PyObject *ptype, *pvalue, *ptraceback;
                    PyErr_Fetch(&ptype, &pvalue, &ptraceback);
                    char *pErrorMessage = PyBytes_AsString(PyUnicode_AsEncodedString(pvalue, "utf-8", ""));
                    std::string pErr = Poco::format("%s", std::string(pErrorMessage));
                    _logger->critical("Script Error [d]: Cannot find specified module: %s", pErr);
                    ncenter->add(SCRIPT_FINISHED);
                    ncenter->stop();
                    _running = false;
                    throw SLException(pErr);
                }
            }
            Py_XDECREF(pFunc);
            Py_DECREF(_module);
        }
        else
        {
            ncenter->stop();
            PyErr_Print();
            if (PyErr_Occurred())
            {
                std::printf("5\n");
                PyObject *ptype, *pvalue, *ptraceback;
                PyErr_Fetch(&ptype, &pvalue, &ptraceback);
                char *pErrorMessage = PyBytes_AsString(PyUnicode_AsEncodedString(pvalue, "utf-8", ""));
                std::string pErr = Poco::format("%s", std::string(pErrorMessage));
                _logger->critical("Script Error [e]: Cannot find specified module: %s", pErr);
                ncenter->add(SCRIPT_FINISHED);
                ncenter->stop();
                _running = false;
                throw SLException(pErr);
            }
            throw SLException("Cannot find specified module", 7);
        }
        if (_exitCode != 0)
        {
            _logger->critical("Script execution failed: %ld", _exitCode);
            if (PyErr_Occurred())
            {
                std::printf("6\n");
                PyObject *ptype, *pvalue, *ptraceback;
                PyErr_Fetch(&ptype, &pvalue, &ptraceback);
                char *pErrorMessage = PyBytes_AsString(PyUnicode_AsEncodedString(pvalue, "utf-8", ""));
                std::string pErr = Poco::format("%s", std::string(pErrorMessage));
                _logger->critical("Script Error [c]: %s", pErr);
                ncenter->add(SCRIPT_FINISHED);
                ncenter->stop();
                _running = false;
                throw SLException(pErr);
            }
        }
        else
        {
            _logger->information("Script execution completed without any errors: %ld", _exitCode);
        }
        ncenter->stop();
        _running = false;
    }

    int SL::handleTraceback(PyTracebackObject *tb, long limit)
    {
        int err = 0;
        long depth = 0;
        PyTracebackObject *tb1 = tb;
        while (tb1 != NULL) {
            depth++;
            tb1 = tb1->tb_next;
        }
        while (tb != NULL && err == 0) {
            if (depth <= limit) {
                err = tracebackLine(
                        tb->tb_frame->f_code->co_filename,
                        tb->tb_lineno,
                        tb->tb_frame->f_code->co_name);
            }
            depth--;
            tb = tb->tb_next;
            if (err == 0)
                err = PyErr_CheckSignals();
        }
        return err;
    }

    int SL::tracebackLine(PyObject *filename, int lineno, PyObject *name)
    {
        int err = 0;
        //PyObject *line;

        if (filename == NULL || name == NULL)
            return -1;

        char *pFilename = PyBytes_AsString(PyUnicode_AsEncodedString(filename, "utf-8", ""));
        char *pName = PyBytes_AsString(PyUnicode_AsEncodedString(name, "utf-8", ""));
        _logger->critical("File %s [%d] in %s", std::string(pFilename), lineno, std::string(pName));

        return err;
        /*
           line = PyUnicode_FromFormat("  File \"%U\", line %d, in %U\n",
           filename, lineno, name);
           if (line == NULL)
           return -1;

        //err = PyFile_WriteObject(line, f, Py_PRINT_RAW);
        Py_DECREF(line);
        if (err != 0)
        return err;
        if (_Py_DisplaySourceLine(f, filename, lineno, 4))
        PyErr_Clear();
        return err;*/
    }

    std::thread SL::executeInThread()
    {
        _logger->debug("Starting execution in thread");
        return std::thread([=] { execute(); });
    }

    std::thread SL::executeInThread(const std::string& module)
    {
        _logger->debug("Starting execution in thread with specific module");
        return std::thread([=] { execute(module); });
    }

    long SL::exitCode() {
        return _exitCode;
    }

    bool SL::running()
    {
        return _running;
    }

}
