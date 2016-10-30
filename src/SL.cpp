#include "SL.h"

SubutaiLauncher::SL::SL(const std::string& dir) : 
    _exitCode(-1000),
    _dir(dir)
{
    if (dir != "/")
    {
        /*  
        char buf[1024];
        std::sprintf(buf, "sys.path.append(\"%s\")", dir.c_str());
        PyRun_SimpleString("import sys");
        PyRun_SimpleString(buf);
        */

#if PY_MAJOR_VERSION >= 3
        wchar_t *path, *newpath;
        path = Py_GetPath(); 
        newpath = new wchar_t[1024];
        wcscpy(newpath, path); 
        wcscat(newpath, L":");
        auto d = std::wstring(dir.begin(), dir.end());
        wcscat(newpath, d.c_str()); 
        wcscat(newpath, L":.");
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
    _name = PyUnicode_DecodeFSDefault(path.c_str());
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
	std::printf("Starting script execution\n");
	PyObject* sysPath = PySys_GetObject((char*)"path");
#if PY_MAJOR_VERSION >= 3
    PyObject* tmpPath = PyUnicode_DecodeFSDefault("/tmp");
#else
	PyObject* tmpPath = PyString_FromString("/tmp");
#endif
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
#if PY_MAJOR_VERSION >= 3
                _exitCode = PyLong_AsLong(pValue);
#else
				_exitCode = PyInt_AsLong(pValue);
#endif
				printf("Result of call: %ld\n", _exitCode);
				Py_DECREF(pValue);
			}
			else {
				Py_DECREF(pFunc);
				Py_DECREF(_module);
				PyErr_Print();
				throw SLException("Script execution failed", 5);
			}
		}
		else {
			if (PyErr_Occurred()) PyErr_Print();
			throw SLException("Cannot find subutaistart() function", 6);
		}
		Py_XDECREF(pFunc);
		Py_DECREF(_module);
	}
	else {
        PyErr_Print();
		throw SLException("Cannot find specified module", 7);
	}
	std::printf("Script exection completed without any errors\n");
}

long SubutaiLauncher::SL::exitCode() {
	return _exitCode;
}

