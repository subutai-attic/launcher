#ifndef __SL_FUNCTIONS_H__
#define __SL_FUNCTIONS_H__

#include <string>
#include <thread>
#include <Python.h>

#include "Downloader.h"
#include "Session.h"

namespace SubutaiLauncher {

    static char* filename = "";

    static char* download_keywords[] = {"filename", NULL};

    static PyObject* SLHelloWorld(PyObject* self, PyObject* args) {
        return Py_BuildValue("s", "Hello, World!");
    }

    static PyObject* SLDebug(PyObject* self, PyObject* args) {
        return Py_BuildValue("s", "Debug");
    }

    static PyObject* SLVersion(PyObject* self, PyObject* args) {
        return Py_BuildValue("s", "Version: 0.1.0");
    }

    static PyObject* SLDownload(PyObject* self, PyObject* args, PyObject* keywords) {
        if (!PyArg_ParseTupleAndKeywords(args, keywords, "s|i", download_keywords, &filename))
            return NULL;
        std::printf("Requested download of a file: %s\n", filename);
        auto downloader = Session::instance()->getDownloader();
        downloader->setFilename(filename);
        if (!downloader->retrieveFileInfo()) {
            std::printf("Failed to retrieve file data");
        } else {
            std::printf("File info retrieved\n");
            auto t = downloader->download();
            t.detach();
        }
        return Py_BuildValue("s", filename);
    }

    static PyObject* SLIsDownloaded(PyObject* self, PyObject* args) {
        auto downloader = Session::instance()->getDownloader();
        if (downloader->isDone())
        return Py_BuildValue("i", 1);
        else 
        return Py_BuildValue("i", 0);
    }

    static PyObject* SLGetProgress(PyObject* self, PyObject* args) {
        auto downloader = Session::instance()->getDownloader();
        auto percent = downloader->getPercent();
        return Py_BuildValue("i", percent);
    }

    static PyMethodDef SubutaiSLMethods[] = {
        {"download", (PyCFunction)SLDownload, METH_VARARGS | METH_KEYWORDS, "Downloads a file from Subutai CDN"},
        {"isDownloadComplete", SLIsDownloaded, METH_VARARGS, "Returns bool"},
        {"getProgress", SLGetProgress, METH_VARARGS, "Returns bool"},
        {"hello", SLHelloWorld, METH_VARARGS, "Hello World method of subutai scripting language"},
        {"debug", SLDebug, METH_VARARGS, "Shows debug information about current launcher instance and environment"},
        {"version", SLVersion, METH_VARARGS, "Display launcher version"},
        {NULL, NULL, 0, NULL}
    };

};

#endif
