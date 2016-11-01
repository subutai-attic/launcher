#ifndef __SL_FUNCTIONS_H__
#define __SL_FUNCTIONS_H__

#include <string>
#include <thread>
#include <Python.h>

#include "Downloader.h"
#include "Session.h"
#include "Install.h"

namespace SubutaiLauncher {

    static char const* sl_filename = "";
    static char const* sl_tmpdir = "";
    static char const* sl_string = "";
    static char const* sl_desc = "";
    static char const* sl_destination = "";


    static char* download_keywords[] = {"filename", NULL};
    static char* tmpdir_keywords[] = {"tmpdir", NULL};
    static char* string_keywords[] = {"string", NULL};
    static char* desc_keywords[] = {"string", "desc", NULL};

    static PyObject* SL_HelloWorld(PyObject* self, PyObject* args) {
        return Py_BuildValue("s", "Hello, World!");
    }

    static PyObject* SL_Debug(PyObject* self, PyObject* args) {
        return Py_BuildValue("s", "Debug");
    }

    static PyObject* SL_Version(PyObject* self, PyObject* args) {
        return Py_BuildValue("s", "Version: 0.1.0");
    }

    static PyObject* SL_Download(PyObject* self, PyObject* args, PyObject* keywords) {
        if (!PyArg_ParseTupleAndKeywords(args, keywords, "s|i", download_keywords, &sl_filename))
            return NULL;
        std::printf("Requested download of a file: %s\n", sl_filename);
        auto downloader = Session::instance()->getDownloader();
        downloader->setFilename(sl_filename);
        if (!downloader->retrieveFileInfo()) {
            std::printf("Failed to retrieve file data");
        } else {
            std::printf("File info retrieved\n");
            auto t = downloader->download();
            t.detach();
        }
        return Py_BuildValue("s", sl_filename);
    }

    static PyObject* SL_IsDownloaded(PyObject* self, PyObject* args) {
        auto downloader = Session::instance()->getDownloader();
        if (downloader->isDone())
            return Py_BuildValue("i", 1);
        else 
            return Py_BuildValue("i", 0);
    }

    static PyObject* SL_GetProgress(PyObject* self, PyObject* args) {
        auto downloader = Session::instance()->getDownloader();
        auto percent = downloader->getPercent();
        return Py_BuildValue("i", percent);
    }

    static PyObject* SL_GetTmpDir(PyObject* self, PyObject* args) {
        auto settings = Session::instance()->getSettings();
        auto path = settings->getTmpPath();
        return Py_BuildValue("s", path);
    }

    static PyObject* SL_GetInstallDir(PyObject* self, PyObject* args) {
        auto settings = Session::instance()->getSettings();
        auto path = settings->getInstallationPath();
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


    static PyMethodDef SubutaiSLMethods[] = {
        {"download", (PyCFunction)SL_Download, METH_VARARGS | METH_KEYWORDS, "Downloads a file from Subutai CDN"},
        {"install", (PyCFunction)SL_Install, METH_VARARGS | METH_KEYWORDS, "Installs a file"},
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
