#ifndef __SL_FUNCTIONS_H__
#define __SL_FUNCTIONS_H__

#include <string>
#include <thread>
#include <Python.h>

#include "Downloader.h"
#include "Session.h"
#include "Install.h"
#include "NotificationCenter.h"
#include "Environment.h"

namespace SubutaiLauncher {

    static char const* sl_filename = "";
    static char const* sl_tmpdir = "";
    static char const* sl_string = "";
    static char const* sl_desc = "";
    static char const* sl_destination = "";
    static char const* sl_type = "";
    static char const* sl_mh = "";



    static char* download_keywords[] = {(char*)"filename", NULL};
    static char* tmpdir_keywords[] = {(char*)"tmpdir", NULL};
    static char* string_keywords[] = {(char*)"string", NULL};
    static char* vb_keywords[] = {(char*)"type", (char*)"mh", NULL};
    static char* desc_keywords[] = {(char*)"string", (char*)"desc", NULL};

    static PyObject* SL_HelloWorld(PyObject* self, PyObject* args) {
        return Py_BuildValue("s", "Hello, World!");
    }

    static PyObject* SL_Debug(PyObject* self, PyObject* args) {
        return Py_BuildValue("s", "Debug");
    }

    static PyObject* SL_Version(PyObject* self, PyObject* args) {
        return Py_BuildValue("s", "Version: 0.1.0");
    }

    static PyObject* SL_GetMasterVersion(PyObject* self, PyObject* args) {
        return Py_BuildValue("s", "4.0.15");
    }

    static PyObject* SL_GetDevVersion(PyObject* self, PyObject* args) {
        return Py_BuildValue("s", "4.0.16");
    }

    static PyObject* SL_getDistro(PyObject* self, PyObject* args) {
	Environment e;
        std::string distro = e.distroOS("-c");
        return Py_BuildValue("s", distro);
    }

    static PyObject* SL_Shutdown(PyObject* self, PyObject* args) {
        Session::instance()->getNotificationCenter()->add(SCRIPT_FINISHED);
        return Py_BuildValue("i", 1);
    }

    static PyObject* SL_CheckDirectories(PyObject* self, PyObject* args) {
        auto settings = Session::instance()->getSettings();
        auto tmpDir = settings->getTmpPath();
        auto installDir = settings->getInstallationPath();

        try {

            FileSystem fs("/");
            if (!fs.isFileExists(installDir)) {
                fs.createDirectory(installDir);
            }
            FileSystem ifs(installDir);
            if (!ifs.isFileExists("bin"))
            {
                ifs.createDirectory("bin");
            }
            if (!ifs.isFileExists("etc"))
            {
                ifs.createDirectory("etc");
            }
            if (!ifs.isFileExists("log"))
            {
                ifs.createDirectory("log");
            }
            if (!ifs.isFileExists("resources"))
            {
                ifs.createDirectory("resources");
            }
            if (!ifs.isFileExists("lib"))
            {
                ifs.createDirectory("lib");
            }

        } catch (SubutaiException e) {
            return Py_BuildValue("i", 0);
        }

        return Py_BuildValue("i", 1);
    }

    static PyObject* SL_Download(PyObject* self, PyObject* args, PyObject* keywords) {
        if (!PyArg_ParseTupleAndKeywords(args, keywords, "s|i", download_keywords, &sl_filename))
            return NULL;
        std::printf("Requested download of a file: %s\n", sl_filename);
        auto downloader = Session::instance()->getDownloader();
	PyErr_Print();
        downloader->setFilename(sl_filename);
        if (!downloader->retrieveFileInfo()) {
            std::printf("Failed to retrieve file data");
        } else {
            Session::instance()->getNotificationCenter()->add(DOWNLOAD_STARTED);
            std::printf("File info retrieved\n");
            auto t = downloader->download();
            t.detach();
        }
        return Py_BuildValue("s", sl_filename);
    }

    static PyObject* SL_IsDownloaded(PyObject* self, PyObject* args) {
        auto downloader = Session::instance()->getDownloader();
        if (downloader->isDone())
        {
            Session::instance()->getNotificationCenter()->add(DOWNLOAD_FINISHED);
            return Py_BuildValue("i", 1);
        }
        else 
        {
            return Py_BuildValue("i", 0);
        }
    }

    static PyObject* SL_GetProgress(PyObject* self, PyObject* args) {
        auto downloader = Session::instance()->getDownloader();
        auto percent = downloader->getPercent();
        return Py_BuildValue("i", percent);
    }

    static PyObject* SL_GetTmpDir(PyObject* self, PyObject* args) {
        auto settings = Session::instance()->getSettings();
        auto path = settings->getTmpPath().c_str();
        return Py_BuildValue("s", path);
    }

    static PyObject* SL_GetInstallDir(PyObject* self, PyObject* args) {
        auto settings = Session::instance()->getSettings();
        auto path = settings->getInstallationPath().c_str();
        return Py_BuildValue("s", path);
    }

    static PyObject* SL_SetTmpDir(PyObject* self, PyObject* args, PyObject* keywords) {
        if (!PyArg_ParseTupleAndKeywords(args, keywords, "s|i", tmpdir_keywords, &sl_tmpdir))
            return NULL;
        Session::instance()->getDownloader()->setOutputDirectory(sl_tmpdir);
        return Py_BuildValue("i", 1);
    }

    static PyObject* SL_NewConfiguration(PyObject* self, PyObject* args, PyObject* keywords) {
        if (!PyArg_ParseTupleAndKeywords(args, keywords, "s|i", string_keywords, &sl_string))
            return NULL;
        Session::instance()->getConfManager()->addConfig(sl_string);
        return Py_BuildValue("i", 1);
    }

    static PyObject* SL_SetConfigurationDesc(PyObject* self, PyObject* args, PyObject* keywords) {
        if (!PyArg_ParseTupleAndKeywords(args, keywords, "s|s", desc_keywords, &sl_string, &sl_desc))
            return NULL;
        Session::instance()->getConfManager()->addDesc(sl_string, sl_desc);
        return Py_BuildValue("i", 1);
    }

    static PyObject* SL_SetConfigurationFile(PyObject* self, PyObject* args, PyObject* keywords) {
        if (!PyArg_ParseTupleAndKeywords(args, keywords, "s|s", desc_keywords, &sl_string, &sl_desc))
            return NULL;
        Session::instance()->getConfManager()->addFile(sl_string, sl_desc);
        return Py_BuildValue("i", 1);
    }

    static PyObject* SL_SSHPeer(PyObject* self, PyObject* args, PyObject* keywords) {
        if (!PyArg_ParseTupleAndKeywords(args, keywords, "s|i", string_keywords, &sl_string))
            return NULL;
        Session::instance()->getConfManager()->addConfig(sl_string);
        return Py_BuildValue("i", 1);
    }

    static PyObject* SL_Install(PyObject* self, PyObject* args, PyObject* keywords) {
        if (!PyArg_ParseTupleAndKeywords(args, keywords, "s|s", string_keywords, &sl_filename, &sl_destination))
            return NULL;
        Install i;
        i.setFilename(sl_filename);
        i.setSrcPath(Session::instance()->getDownloader()->getOutputDirectory());
        i.setDstPath(sl_destination);
        try {
            i.preInstall();
            i.install();
            i.postInstall();
        } catch (SubutaiException &e) {
            return Py_BuildValue("i", 1);

        }
        return Py_BuildValue("i", 0);
    }


    static PyObject* SL_UnInstall(PyObject* self, PyObject* args, PyObject* keywords) {
        if (!PyArg_ParseTupleAndKeywords(args, keywords, "s|s", string_keywords, &sl_filename, &sl_destination))
            return NULL;
        Install i;
        i.setFilename(sl_filename);
        try {
            i.unInstall();
            } catch (SubutaiException &e) {
            return Py_BuildValue("i", 1);

        }
        return Py_BuildValue("i", 0);
    }

    // Notification messages

    static PyObject* SL_RaiseError(PyObject* self, PyObject* args, PyObject* keywords) {
        if (!PyArg_ParseTupleAndKeywords(args, keywords, "s|i", string_keywords, &sl_string))
            return NULL;
        Session::instance()->getNotificationCenter()->notificationRaised(N_ERROR, sl_string);
        return Py_BuildValue("i", 1);
    }

    static PyObject* SL_RaiseWarning(PyObject* self, PyObject* args, PyObject* keywords) {
        if (!PyArg_ParseTupleAndKeywords(args, keywords, "s|i", string_keywords, &sl_string))
            return NULL;
        Session::instance()->getNotificationCenter()->notificationRaised(N_WARNING, sl_string);
        return Py_BuildValue("i", 1);
    }

    static PyObject* SL_RaiseInfo(PyObject* self, PyObject* args, PyObject* keywords) {
        if (!PyArg_ParseTupleAndKeywords(args, keywords, "s|i", string_keywords, &sl_string))
            return NULL;
        Session::instance()->getNotificationCenter()->notificationRaised(N_INFO, sl_string);
        return Py_BuildValue("i", 1);
    }

    static PyObject* SL_VBox(PyObject* self, PyObject* args, PyObject* keywords) {
        if (!PyArg_ParseTupleAndKeywords(args, keywords, "s|i", string_keywords, &sl_string))
            return NULL;

        Log::instance()->logger()->debug() << "PyObject VBox: " << sl_string << std::endl;

        VirtualBox vb;
	std::string out = vb.execute(sl_string);
	int found = out.find("error");
        if (found == std::string::npos)
	    return Py_BuildValue("i", 1);
	return Py_BuildValue("i", 0);
    }

    static PyObject* SL_cloneVM(PyObject* self, PyObject* args) {

        VirtualBox vb;
        vb.cloneVM();
        return Py_BuildValue("i", 1);
    }

    static PyObject* SL_runScripts(PyObject* self, PyObject* args, PyObject* keywords) {
        
    //	if (!PyArg_ParseTupleAndKeywords(args, keywords, "s|i", vb_keywords, &sl_type, &sl_mh))
    //        return NULL;

    	if (!PyArg_ParseTupleAndKeywords(args, keywords, "s|s", desc_keywords, &sl_string, &sl_desc))
            return NULL;

//	if (!PyArg_ParseTupleAndKeywords(args, keywords, "s|i", string_keywords, &sl_string))
//            return NULL;

        //Log::instance()->logger()->debug() << "runScripts: " << sl_type << " ifMH: " << sl_mh << std::endl;

        VirtualBox vb;
        //Log::instance()->logger()->debug() << "runScripts: vb " << sl_type << std::endl;
        vb.runScripts(sl_string, sl_desc);
        //Log::instance()->logger()->debug() << "runScripts: after run " << sl_desc << std::endl;
        return Py_BuildValue("i", 1);
    }

    static PyObject* SL_runAutobuild(PyObject* self, PyObject* args) {
        Log::instance()->logger()->debug() << "runAutobuild: " <<  std::endl;

        VirtualBox vb;
        vb.runAutobuild();
        Log::instance()->logger()->debug() << "runAutobuild: after run "  << std::endl;
        return Py_BuildValue("i", 1);
    }

    // Module bindings

    static PyMethodDef SubutaiSLMethods[] = {
        {"download", (PyCFunction)SL_Download, METH_VARARGS | METH_KEYWORDS, "Downloads a file from Subutai CDN"},
        {"install", (PyCFunction)SL_Install, METH_VARARGS | METH_KEYWORDS, "Installs a file"},
        {"uninstall", (PyCFunction)SL_UnInstall, METH_VARARGS | METH_KEYWORDS, "Uninstalls a file"},
        {"setTmpDir", (PyCFunction)SL_SetTmpDir, METH_VARARGS | METH_KEYWORDS, "Sets tmp output directory"},
        {"NewConfiguration", (PyCFunction)SL_NewConfiguration, METH_VARARGS | METH_KEYWORDS, "Creates a new configuration with a given name"},
        {"SetConfigurationDesc", (PyCFunction)SL_SetConfigurationDesc, METH_VARARGS | METH_KEYWORDS, "Sets a description to a given configuration"},
        {"SetConfigurationFile", (PyCFunction)SL_SetConfigurationFile, METH_VARARGS | METH_KEYWORDS, "Sets a python script to a given configuration"},
        {"isDownloadComplete", SL_IsDownloaded, METH_VARARGS, "Returns 1 if current download has been completed"},
        {"GetTmpDir", SL_GetTmpDir, METH_VARARGS, "Returns tmp directory"},
        {"GetInstallDir", SL_GetInstallDir, METH_VARARGS, "Returns installation directory"},
        {"getProgress", SL_GetProgress, METH_VARARGS, "Returns bool"},
        {"hello", SL_HelloWorld, METH_VARARGS, "Hello World method of subutai scripting language"},
        {"debug", SL_Debug, METH_VARARGS, "Shows debug information about current launcher instance and environment"},
        {"version", SL_Version, METH_VARARGS, "Display launcher version"},
        {"CheckDirectories", SL_CheckDirectories, METH_VARARGS, "Display launcher version"},
        {"RaiseError", (PyCFunction)SL_RaiseError, METH_VARARGS | METH_KEYWORDS, "Raising error"},
        {"RaiseWarning", (PyCFunction)SL_RaiseWarning, METH_VARARGS | METH_KEYWORDS, "Raising warning"},
        {"RaiseInfo", (PyCFunction)SL_RaiseInfo, METH_VARARGS | METH_KEYWORDS, "Raising info"},
        {"VBox", (PyCFunction)SL_VBox, METH_VARARGS | METH_KEYWORDS, "Tells vboxmanage to do something"},
	{"cloneVM", (PyCFunction)SL_cloneVM, METH_VARARGS | METH_KEYWORDS, "clones VM"},
        {"runScripts", (PyCFunction)SL_runScripts, METH_VARARGS | METH_KEYWORDS, "Configure peer"},
        {"Shutdown", SL_Shutdown, METH_VARARGS, "Finalizes the script"},
        {"GetMasterVersion", SL_GetMasterVersion, METH_VARARGS, "Returns master version of a product"},
        {"GetDevVersion", SL_GetDevVersion, METH_VARARGS, "Returns dev version of a product"},
        {"getDistro", SL_getDistro, METH_VARARGS, "Returns OS distro"},
        {"runAutobuild", SL_runAutobuild, METH_VARARGS, "temporary - runs autobuild script"},
        {NULL, NULL, 0, NULL}
    };

#if PY_MAJOR_VERSION >= 3
    static PyModuleDef SubutaiModule = {
        PyModuleDef_HEAD_INIT, "subutai", NULL, -1, SubutaiSLMethods, 
        NULL, NULL, NULL, NULL
    };

    static PyObject* PyInit_Subutai(void)
    {
        return PyModule_Create(&SubutaiModule);
    }
#endif


}

#endif
