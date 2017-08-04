#include "SL.h"

SubutaiLauncher::SL::SL(const std::string& dir) :
	_running(true),
	_exitCode(-1000),
	_dir(dir)
{
	_logger = &Poco::Logger::get("subutai");
	_logger->debug("Starting scripting language interface");
#if LAUNCHER_LINUX
	std::string some_path = "/opt/subutai/bin/Lib";
#elif LAUNCHER_MACOS
	std::string some_path = "/Users/mike/projects/launcher/bin/Lib";
#endif

#if LAUNCHER_MACOS || LAUNCHER_LINUX
  _logger->debug("0");
	PyObject *pSearchPathList = PyList_New(0);
  _logger->debug("1");
	PyObject *pPath = PyUnicode_FromString(some_path.c_str());
  _logger->debug("2");
	PyList_Append(pSearchPathList, pPath);
  _logger->debug("3");
	Py_DECREF(pPath);
  _logger->debug("4");
  PySys_SetObject("prefix", pSearchPathList);
  _logger->debug("5");
  Py_DECREF(pSearchPathList);
  _logger->debug("6");
#endif

	if (dir != "/")
	{
    _logger->debug("7");
#if PY_MAJOR_VERSION >= 3
		char dst[1024];
		wchar_t *path, *newpath;

		path = Py_GetPath();
    _logger->debug("8");
#if LAUNCHER_WINDOWS
		size_t ret;
		wcstombs_s(&ret, dst, path, sizeof(dst));
#else
		std::wcstombs(dst, path, sizeof(dst));
    _logger->debug("9");
#endif
		newpath = new wchar_t[1024];
    _logger->debug("10");
#if LAUNCHER_WINDOWS
		wcscpy_s(newpath, 1024, path);
		wcscat_s(newpath, 1024, L":");
#else
    _logger->debug("11");
		wcscpy(newpath, path);
    _logger->debug("12");
		wcscat(newpath, L":");
#endif
    _logger->debug("13");
		auto d = std::wstring(dir.begin(), dir.end());
    _logger->debug("14");
#if LAUNCHER_WINDOWS
		wcscat_s(newpath, 1024, d.c_str());
		wcscat_s(newpath, 1024, L":.");
		wcstombs_s(&ret, dst, newpath, sizeof(dst));
#else
		wcscat(newpath, d.c_str());
    _logger->debug("15");
		wcscat(newpath, L":.");
    _logger->debug("16");
		std::wcstombs(dst, newpath, sizeof(dst));
    _logger->debug("17");
#endif
		std::wprintf(L"NEWPATH: %sl\n", newpath);
    _logger->debug("18");
    PySys_SetPath(newpath);
    _logger->debug("19");
#else
		char *path, *newpath;
		path = Py_GetPath();
		newpath = new char[strlen(path) + dir.length() + 6];
#if LAUNCHER_LINUX || LAUNCHER_MACOS
		strcpy(newpath, path);
    _logger->debug("20");
		strcat(newpath, ":");
    _logger->debug("21");
		strcat(newpath, dir.c_str());
    _logger->debug("22");
		strcat(newpath, ":.");
    _logger->debug("23");
#else
		strcpy_s(newpath, 1024, path);
		strcpy_s(newpath, 1024, ":");
		strcpy_s(newpath, 1024, dir.c_str());
		strcpy_s(newpath, 1024, ":.");
#endif
		PySys_SetPath(newpath);
    _logger->debug("24");
		delete[] newpath;
    _logger->debug("25");
#endif
		// ":." for unix, or ";." for windows 
	}
_logger->debug("26");
}

SubutaiLauncher::SL::~SL()
{

}

void SubutaiLauncher::SL::open(const std::string& path)
{
	//auto p = path;
	auto settings = Session::instance()->getSettings();
	std::string p = settings->getTmpPath().c_str();
	_logger->trace("SL::Opening tmp path: %s", p);
	p.append(PATH_DELIM);
	p.append(path);
	p.append(".py");
	_logger->trace("SL::Open path %s", p);

	FileSystem fs(_dir);
	//    l->info() << "FileSytem fs(_dir): " << _dir << std::endl;

	//    FileSystem fs();
	_logger->trace("SL::open dir: %s", _dir);

	std::string f = path;
	f.append(".py");
	_logger->trace("SL::open file: %s", f);
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

	_logger->debug("SL::Executing %s [Module: %s]", _name, module);
	execute();
}

void SubutaiLauncher::SL::execute()
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
	_logger->information("Starting script execution: %s", _name);

	if (_name == NULL)
	{
		ncenter->stop();
		_logger->error("SL::execute: Empty module name: %s", _module);
		PyErr_Print();
		throw SLException("Empty module name", 12);
	}
	PyObject* sysPath = PySys_GetObject((char*)"path");
	if (sysPath == NULL)
	{
		_logger->error("Failed to extract path from python system");
	}
	else
	{
		_logger->trace("SL::execute syspath: %s, dir: %s", sysPath, _dir);

#if PY_MAJOR_VERSION >= 3
		PyObject* tmpPath = PyUnicode_FromString(_dir.c_str());
		//PyObject* tmpPath = PyUnicode_DecodeFSDefault(_dir.c_str());
#else
		PyObject* tmpPath = PyString_FromString(_dir.c_str());
#endif
		PyList_Append(sysPath, tmpPath);
	}

	_module = PyImport_Import(_name);
	if (!_module) {
		_logger->error("Can't find specified module");
		ncenter->stop();
		PyErr_Print();
		throw SLException("Cannot find specified module", 7);
	}
	Py_XDECREF(_name);

	if (_module == NULL || _module == 0) {
		ncenter->stop();
		_logger->debug("SL::execute Can't find module %s", _module);
		throw SLException("Cannot find specified module", 7);
	}

	//Py_DECREF(_name);
    /*
	Py_DECREF(PyImport_ImportModule("threading"));
	PyEval_InitThreads();
    */

	PyObject *pFunc, *pArgs, *pValue;
	if (!(_module == NULL || _module == 0))
	{
		pFunc = PyObject_GetAttrString(_module, "subutaistart");

		if (pFunc && PyCallable_Check(pFunc))
		{
			_logger->debug("subutaistart() entry point was not found");
			PyErr_Print();
			pArgs = PyTuple_New(0);
			pValue = PyObject_CallObject(pFunc, pArgs);
			Py_DECREF(pArgs);
			if (pValue != NULL)
			{
#if PY_MAJOR_VERSION >= 3
				_exitCode = PyLong_AsLong(pValue);
#else
				_exitCode = PyInt_AsLong(pValue);
#endif
				Py_DECREF(pValue);
			}
			else
			{
				Py_DECREF(pFunc);
				Py_DECREF(_module);
				PyErr_Print();
				ncenter->stop();
				_logger->error("Execution exit code: %ld", _exitCode);
				PyErr_Print();
				pErrorText = "Script execution failed";
				//throw SLException("Script execution failed", 5);
			}
		}
		else
		{
			if (PyErr_Occurred()) PyErr_Print();
			ncenter->stop();
			throw SLException("Cannot find subutaistart() function", 6);
		}
		Py_XDECREF(pFunc);
		Py_DECREF(_module);
		}
	else
	{
		PyErr_Print();
		ncenter->stop();
		PyErr_Print();
		throw SLException("Cannot find specified module", 7);
	}
	_logger->information("Script execution completed without any errors: %ld", _exitCode);
	ncenter->stop();
	_running = false;
}

std::thread SubutaiLauncher::SL::executeInThread()
{
	_logger->debug("Starting execution in thread");
	return std::thread([=] { execute(); });
}

std::thread SubutaiLauncher::SL::executeInThread(const std::string& module)
{
	_logger->debug("Starting execution in thread with specific module");
	return std::thread([=] { execute(module); });
}

long SubutaiLauncher::SL::exitCode() {
	return _exitCode;
}

bool SubutaiLauncher::SL::running()
{
	return _running;
}

