#ifndef __SL_FUNCTIONS_H__
#define __SL_FUNCTIONS_H__

#include <string>
#include <thread>
#include <Python.h>

#include "Downloader.h"

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
        auto downloader = new SubutaiDownloader(std::string(filename));
        if (!downloader->retrieveFileInfo()) {
            std::printf("Failed to retrieve file data");
        } else {
            downloader->download();
        }
        return Py_BuildValue("s", filename);
    }

    static PyMethodDef SubutaiSLMethods[] = {
        {"download", (PyCFunction)SLDownload, METH_VARARGS | METH_KEYWORDS, "Downloads a file from Subutai CDN"},
        {"hello", SLHelloWorld, METH_VARARGS, "Hello World method of subutai scripting language"},
        {"debug", SLDebug, METH_VARARGS, "Shows debug information about current launcher instance and environment"},
        {"version", SLVersion, METH_VARARGS, "Display launcher version"},
        {NULL, NULL, 0, NULL}
    };

};

#endif
