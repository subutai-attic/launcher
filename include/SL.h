#ifndef __SL_H__
#define __SL_H__

#include "Vars.h"

#include <string>
#include <Python.h>
#include "frameobject.h"
#include "SubutaiException.h"
#include "SLException.h"
#include "FileSystem.h"
#include "Session.h"

#include "Poco/Logger.h"
#include "Poco/String.h"
#include "Poco/Format.h"

#if LAUNCHER_LINUX
#include <sys/stat.h>
#elif LAUNCHER_WINDOWS
//#error Not Implemented for this platform
#elif LAUNCHER_MACOS
//#error Not Implemented for this platform
#else
#error Unknown Platform
#endif

namespace SubutaiLauncher {

	class SL {
	public:
		SL(const std::string& dir = "/");
		~SL();
		void open(const std::string& path);
		void execute();
		void execute(std::string module);
		std::thread executeInThread();
		std::thread executeInThread(const std::string& module);
		long exitCode();
		bool running();
	private:
        int handleTraceback(PyTracebackObject* tb, long limit);
        int tracebackLine(PyObject *filename, int lineno, PyObject *name);
		bool _running;
		Poco::Logger* _logger;
		PyObject* _name;
		PyObject* _module;
		long _exitCode;
		std::string _dir;
	};

}

#endif
