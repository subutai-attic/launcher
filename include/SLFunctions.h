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
#include "VirtualBox.h"

namespace SubutaiLauncher {

    static char const* sl_filename = "";
    static char const* sl_tmpdir = "";
    static char const* sl_string = "";
    static char const* sl_desc = "";
    static char const* sl_destination = "";

    // SSH
    static char const* ssh_user;        // SSH User
    static char const* ssh_pass;        // SSH Password
    static char const* ssh_host;        // SSH Host
    static int const* ssh_port;         // SSH Port
    static char const* ssh_cmd;         // SSH Command
    static int const* ssh_hostcheck;    // SSH Hostcheck
    static char* ssh_keywords[] = {
        (char*)"user", 
        (char*)"pass", 
        (char*)"host", 
        (char*)"port", 
        (char*)"cmd", 
        (char*)"hostcheck", 
        NULL};


    // ========================================================================

    static char* download_keywords[] = {(char*)"filename", NULL};
    static char* tmpdir_keywords[] = {(char*)"tmpdir", NULL};
    static char* string_keywords[] = {(char*)"string", NULL};
    static char* desc_keywords[] = {(char*)"string", (char*)"desc", NULL};

    // ========================================================================

    static PyObject* SL_HelloWorld(PyObject* self, PyObject* args) {
        return Py_BuildValue("s", "Hello, World!");
    }

    static PyObject* SL_GetScheme(PyObject* self, PyObject* args) {
        std::printf("GetScheme: %s\n", BUILD_SCHEME);
        return Py_BuildValue("s", "Hello, World!");
    }

    // ========================================================================

    static PyObject* SL_Debug(PyObject* self, PyObject* args) {
        return Py_BuildValue("s", "Debug");
    }

    // ========================================================================

    static PyObject* SL_Version(PyObject* self, PyObject* args) {
        return Py_BuildValue("s", "Version: 0.1.0");
    }

    // ========================================================================

    static PyObject* SL_GetMasterVersion(PyObject* self, PyObject* args) {
        return Py_BuildValue("s", "4.0.15");
    }

    // ========================================================================

    static PyObject* SL_GetDevVersion(PyObject* self, PyObject* args) {
        return Py_BuildValue("s", "4.0.16");
    }

    // ========================================================================

    static PyObject* SL_getDistro(PyObject* self, PyObject* args) {
        Environment e;
        std::string distro = e.distroOS("-c");
        return Py_BuildValue("s", distro);
    }

    // ========================================================================

    static PyObject* SL_Shutdown(PyObject* self, PyObject* args) {
        Session::instance()->getNotificationCenter()->add(SCRIPT_FINISHED);
        return Py_BuildValue("i", 1);
    }

    // ========================================================================

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

    // ========================================================================

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

    // ========================================================================

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

    // ========================================================================

    static PyObject* SL_GetProgress(PyObject* self, PyObject* args) {
        auto downloader = Session::instance()->getDownloader();
        auto percent = downloader->getPercent();
        return Py_BuildValue("i", percent);
    }

    // ========================================================================

    static PyObject* SL_GetTmpDir(PyObject* self, PyObject* args) {
        auto settings = Session::instance()->getSettings();
        auto path = settings->getTmpPath().c_str();
        return Py_BuildValue("s", path);
    }

    // ========================================================================

    static PyObject* SL_GetInstallDir(PyObject* self, PyObject* args) {
        auto settings = Session::instance()->getSettings();
        auto path = settings->getInstallationPath().c_str();
        return Py_BuildValue("s", path);
    }

    // ========================================================================

    static PyObject* SL_SetTmpDir(PyObject* self, PyObject* args, PyObject* keywords) {
        if (!PyArg_ParseTupleAndKeywords(args, keywords, "s|i", tmpdir_keywords, &sl_tmpdir))
            return NULL;
        Session::instance()->getDownloader()->setOutputDirectory(sl_tmpdir);
        return Py_BuildValue("i", 1);
    }

    // ========================================================================

    static PyObject* SL_NewConfiguration(PyObject* self, PyObject* args, PyObject* keywords) {
        if (!PyArg_ParseTupleAndKeywords(args, keywords, "s|i", string_keywords, &sl_string))
            return NULL;
        Session::instance()->getConfManager()->addConfig(sl_string);
        return Py_BuildValue("i", 1);
    }

    // ========================================================================

    static PyObject* SL_SetConfigurationDesc(PyObject* self, PyObject* args, PyObject* keywords) {
        if (!PyArg_ParseTupleAndKeywords(args, keywords, "s|s", desc_keywords, &sl_string, &sl_desc))
            return NULL;
        Session::instance()->getConfManager()->addDesc(sl_string, sl_desc);
        return Py_BuildValue("i", 1);
    }

    // ========================================================================

    static PyObject* SL_SetConfigurationFile(PyObject* self, PyObject* args, PyObject* keywords) {
        if (!PyArg_ParseTupleAndKeywords(args, keywords, "s|s", desc_keywords, &sl_string, &sl_desc))
            return NULL;
        Session::instance()->getConfManager()->addFile(sl_string, sl_desc);
        return Py_BuildValue("i", 1);
    }

    // ========================================================================

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


    // ========================================================================

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

    // ========================================================================

    // Notification messages

    // ========================================================================

    static PyObject* SL_RaiseError(PyObject* self, PyObject* args, PyObject* keywords) {
        if (!PyArg_ParseTupleAndKeywords(args, keywords, "s|i", string_keywords, &sl_string))
            return NULL;
        Session::instance()->getNotificationCenter()->notificationRaised(N_ERROR, sl_string);
        return Py_BuildValue("i", 1);
    }

    // ========================================================================

    static PyObject* SL_RaiseWarning(PyObject* self, PyObject* args, PyObject* keywords) {
        if (!PyArg_ParseTupleAndKeywords(args, keywords, "s|i", string_keywords, &sl_string))
            return NULL;
        Session::instance()->getNotificationCenter()->notificationRaised(N_WARNING, sl_string);
        return Py_BuildValue("i", 1);
    }

    // ========================================================================

    static PyObject* SL_RaiseInfo(PyObject* self, PyObject* args, PyObject* keywords) {
        if (!PyArg_ParseTupleAndKeywords(args, keywords, "s|i", string_keywords, &sl_string))
            return NULL;
        Session::instance()->getNotificationCenter()->notificationRaised(N_INFO, sl_string);
        return Py_BuildValue("i", 1);
    }

    // ========================================================================

    static PyObject* SL_VBox(PyObject* self, PyObject* args, PyObject* keywords) {
        if (!PyArg_ParseTupleAndKeywords(args, keywords, "s|i", string_keywords, &sl_string))
            return NULL;

        Log::instance()->logger()->debug() << "PyObject VBox: " << sl_string << std::endl;

        VirtualBox vb;
        std::string out = vb.execute(sl_string);
        size_t found = out.find("error");
        if (found == std::string::npos)
            return Py_BuildValue("i", 1);
        return Py_BuildValue("i", 0);
    }

    // ========================================================================

    static PyObject* SL_VBoxS(PyObject* self, PyObject* args, PyObject* keywords) {
        if (!PyArg_ParseTupleAndKeywords(args, keywords, "s|i", string_keywords, &sl_string))
            return NULL;

        Log::instance()->logger()->debug() << "PyObject VBox: " << sl_string << std::endl;

        VirtualBox vb;
        int exitStatus = 0;
        std::string out = vb.execute(sl_string, exitStatus);
        return Py_BuildValue("i", exitStatus);
    }

    // ========================================================================

    static PyObject* SL_cloneVM(PyObject* self, PyObject* args) {

        VirtualBox vb;
        vb.cloneVM();
        return Py_BuildValue("i", 1);
    }

    // ========================================================================

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

    // ========================================================================

    static PyObject* SL_runAutobuild(PyObject* self, PyObject* args) {
        Log::instance()->logger()->debug() << "runAutobuild: " <<  std::endl;

        VirtualBox vb;
        vb.runAutobuild();
        Log::instance()->logger()->debug() << "runAutobuild: after run "  << std::endl;
        return Py_BuildValue("i", 1);
    }

    // ========================================================================

    static PyObject* SL_GetDefaultRoutingInterface(PyObject* self, PyObject* args) {
        Log::instance()->logger()->debug() << "SL_GetDefaultRoutingInterface" << std::endl;

        Environment env;

        return Py_BuildValue("s", env.getDefaultGateway().c_str());
    }

    // ========================================================================

    static PyObject* SL_GetVBoxBridgedInterface(
            PyObject* self, 
            PyObject* args, 
            PyObject* keywords
            ) 
    {
        Log::instance()->logger()->debug() << "SL_GetVBoxBridgedInterface" << std::endl;
        if (!PyArg_ParseTupleAndKeywords(args, keywords, "s|i", string_keywords, &sl_string))
            return NULL;
        VirtualBox vb;

        return Py_BuildValue("s", vb.getBridgedInterface(sl_string).c_str());
    }

    // ========================================================================

    static PyObject* SL_SetSSHCredentials(
            PyObject* self, 
            PyObject* args, 
            PyObject* keywords
            ) 
    {
        if (!PyArg_ParseTupleAndKeywords(
                    args, 
                    keywords, 
                    "sssi|si", 
                    ssh_keywords, 
                    &ssh_user, 
                    &ssh_pass,
                    &ssh_host, 
                    &ssh_port, 
                    &ssh_cmd, 
                    &ssh_hostcheck))
            return NULL;

        Session::instance()->setSSHCredentials(ssh_user, ssh_pass, ssh_host, (long)ssh_port);
        return Py_BuildValue("i", 0);
    }

    // ========================================================================

    static PyObject* SL_TestSSH(PyObject* self, PyObject* args) 
    {
        // Attempt to establish SSH connection
        Log::instance()->logger()->debug() << "SL_TestSSH" << std::endl;

        SSH *p = new SubutaiLauncher::SSH();
        auto s = Session::instance();
        p->setHost(s->getSSHHostname(), s->getSSHPort());
        p->connect();
        int ret = 1;
        if (p->isConnected())
            ret = 0;

        p->disconnect();
        delete p;

        return Py_BuildValue("i", ret);
    }

    // ========================================================================

    static PyObject* SL_InstallSSHKey(PyObject* self, PyObject* args) 
    {
        // Attempt to establish SSH connection
        Log::instance()->logger()->debug() << "SL_InstallSSHKey" << std::endl;

        auto s = Session::instance();

        SSH *p = new SubutaiLauncher::SSH();
        p->setHost(s->getSSHHostname(), s->getSSHPort());
        p->setUsername(s->getSSHUser(), s->getSSHPass());
        p->connect();
        if (!p->isConnected()) {
            p->disconnect();
            delete p;
            return Py_BuildValue("i", 1);
        }
        p->authenticate();
        if (!p->isAuthenticated()) {
            p->disconnect();
            delete p;
            return Py_BuildValue("i", 1);
        }

        auto key = SSH::getPublicKey();
        if (key == "") {
            p->disconnect();
            delete p;
            return Py_BuildValue("i", 1);
        }
        std::string cmd = "echo '";
        cmd.append(key);
        cmd.append("' >> /home/subutai/.ssh/authorized_keys");
        auto output = p->execute(cmd);
        Log::instance()->logger()->debug() << "SL_InstallSSHKey: " << output << std::endl;
        p->disconnect();
        delete p;

        return Py_BuildValue("i", 0);
    }

    // ========================================================================

    static PyObject* SL_SSHRun(PyObject* self, PyObject* args, PyObject* keywords) 
    {
        Log::instance()->logger()->debug() << "SL_SSHRun" << std::endl;
        if (!PyArg_ParseTupleAndKeywords(
                    args, 
                    keywords, 
                    "s", 
                    string_keywords, 
                    &sl_string 
                    ))
            return NULL;

        auto s = Session::instance();

        SSH *p = new SubutaiLauncher::SSH();
        p->setHost(s->getSSHHostname(), s->getSSHPort());
        p->setUsername(s->getSSHUser(), s->getSSHPass());
        p->connect();
        p->authenticate();
        auto ret = p->execute(sl_string);
        p->disconnect();
        delete p;
        Log::instance()->logger()->info() << "SL_SSHRun: " << ret << std::endl;

        return Py_BuildValue("i", 0);
    }

    // ========================================================================

    static PyObject* SL_StartProcedure(PyObject* self, PyObject* args, PyObject* keywords) 
    {
        Log::instance()->logger()->info() << "Starting procedure" << std::endl;
        if (!PyArg_ParseTupleAndKeywords(args, keywords, "s", string_keywords, &sl_string))
            return NULL;
        return Py_BuildValue("i", 0);
    }

    // ========================================================================

    static PyObject* SL_StopProcedure(PyObject* self, PyObject* args) 
    {
        Log::instance()->logger()->info() << "Stoping procedure" << std::endl;
        return Py_BuildValue("i", 0);
    }

    // ========================================================================
    // VM
    // ========================================================================

    static PyObject* SL_CheckVMExists(PyObject* self, PyObject* args, PyObject* keywords) 
    {
        Log::instance()->logger()->debug() << "SL_CheckVMExists" << std::endl;
        if (!PyArg_ParseTupleAndKeywords(args, keywords, "s", string_keywords, &sl_string))
            return NULL;

        VirtualBox vb;

        if (vb.isMachineExists(sl_string))
            return Py_BuildValue("i", 0);
        return Py_BuildValue("i", 1);
    }

    static PyObject* SL_CheckVMRunning(PyObject* self, PyObject* args, PyObject* keywords) 
    {
        Log::instance()->logger()->debug() << "SL_CheckVMRunning" << std::endl;
        if (!PyArg_ParseTupleAndKeywords(args, keywords, "s", string_keywords, &sl_string))
            return NULL;

        VirtualBox vb;

        if (vb.isMachineRunning(sl_string))
            return Py_BuildValue("i", 0);
        return Py_BuildValue("i", 1);
    }

    // ========================================================================
    // Module bindings
    // ========================================================================

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
        {"VBoxS", (PyCFunction)SL_VBoxS, METH_VARARGS | METH_KEYWORDS, "Tells vboxmanage to do something and returns status"},
        {"cloneVM", (PyCFunction)SL_cloneVM, METH_VARARGS | METH_KEYWORDS, "clones VM"},
        {"runScripts", (PyCFunction)SL_runScripts, METH_VARARGS | METH_KEYWORDS, "Configure peer"},
        {"Shutdown", SL_Shutdown, METH_VARARGS, "Finalizes the script"},
        {"GetMasterVersion", SL_GetMasterVersion, METH_VARARGS, "Returns master version of a product"},
        {"GetDevVersion", SL_GetDevVersion, METH_VARARGS, "Returns dev version of a product"},
        {"getDistro", SL_getDistro, METH_VARARGS, "Returns OS distro"},
        {"runAutobuild", SL_runAutobuild, METH_VARARGS, "temporary - runs autobuild script"},
        // New version
        //{"ImportVirtualMachine", SL_importVirtualMachine, METH_VARARGS | METH_KEYWORDS, "Import a virtual machine into VB"},
        {"GetDefaultRoutingInterface", SL_GetDefaultRoutingInterface, METH_VARARGS, "Returns name of default network interface"},
        {"GetVBoxBridgedInterface", (PyCFunction)SL_GetVBoxBridgedInterface, METH_VARARGS | METH_KEYWORDS, "Returns name of default network interface"},
        {"SetSSHCredentials", (PyCFunction)SL_SetSSHCredentials, METH_VARARGS | METH_KEYWORDS, "Set SSH Connection credentials"},
        {"TestSSH", (PyCFunction)SL_TestSSH, METH_VARARGS, "Test if SSH connection is alive"},
        {"InstallSSHKey", (PyCFunction)SL_InstallSSHKey, METH_VARARGS, "Install SSH public key"},
        {"SSHRun", (PyCFunction)SL_SSHRun, METH_VARARGS | METH_KEYWORDS, "Run command over SSH"},
        {"CheckVMExists", (PyCFunction)SL_CheckVMExists, METH_VARARGS | METH_KEYWORDS, "Checks if VM with this name exists"},
        {"CheckVMRunning", (PyCFunction)SL_CheckVMRunning, METH_VARARGS | METH_KEYWORDS, "Checks if VM with this name running"},
        {"GetScheme", SL_GetScheme, METH_VARARGS, "Returns current scheme: production, master, dev"},
        {"StartProcedure", (PyCFunction)SL_StartProcedure, METH_VARARGS | METH_KEYWORDS, "Starts install/update/remove procedure"},
        {"StopProcedure", SL_StopProcedure, METH_VARARGS, "Stops install/update/remove procedure"},
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
