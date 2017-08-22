#ifndef __SL_FUNCTIONS_H__
#define __SL_FUNCTIONS_H__

#include "Vars.h"



#include <string>
#include <thread>
#include <Python.h>
#include "Poco/Process.h"
#include "Poco/StringTokenizer.h"
#include "ThreadWrapper.h"

#if LAUNCHER_WINDOWS
#include <Windows.h>
#endif

#include "Downloader.h"
#include "Session.h"
#include "NotificationCenter.h"
#include "Environment.h"
#include "VirtualBox.h"
#include "RootProcess.h"
#include "SS.h"

#ifndef __attribute_used__
#define __attribute_used__
#endif

namespace SubutaiLauncher 
{

    // TODO: Refactor and extend arguments
    static char const* sl_filename = "";
    static char const* sl_tmpdir = "";
    static char const* sl_string = "";
    static char const* sl_desc = "";

    //static double const* sl_double;
    static double sl_double;

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
    static char* double_keywords[] = {(char*)"double", NULL};

    // ========================================================================

    static PyObject* SL_HelloWorld(PyObject* self, PyObject* args)
    {
        if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }
        //Poco::Logger::get("subutai").information("SL_HelloWorld");
        return Py_BuildValue("s", "Hello, World!");
    }

    // ========================================================================

    static PyObject* SL_GetScheme(PyObject* self, PyObject* args)
    {
        if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }
        //std::printf("GetScheme: %s\n", BUILD_SCHEME);
        //Poco::Logger::get("subutai").information("SL_GetScheme");
        return Py_BuildValue("s", "Hello, World!");
    }

    // ========================================================================

    static PyObject* SL_Debug(PyObject* self, PyObject* args)
    {
        if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }
        //Poco::Logger::get("subutai").information("SL_Debug");
        return Py_BuildValue("s", "Debug");
    }

    // ========================================================================

    static PyObject* SL_Version(PyObject* self, PyObject* args)
    {
        if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }
        //Poco::Logger::get("subutai").information("SL_Version");
        return Py_BuildValue("s", "Version: 0.1.0");
    }

    // ========================================================================

    static PyObject* SL_GetMasterVersion(PyObject* self, PyObject* args)
    {
        if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }
        //Poco::Logger::get("subutai").information("SL_GetMasterVersion");
        return Py_BuildValue("s", "4.0.15");
    }

    // ========================================================================

    static PyObject* SL_GetDevVersion(PyObject* self, PyObject* args)
    {
        if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }
        //Poco::Logger::get("subutai").information("SL_GetDevVersion");
        return Py_BuildValue("s", "4.0.16");
    }

    // ========================================================================

    static PyObject* SL_Shutdown(PyObject* self, PyObject* args)
    {
        if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }
        Poco::Logger::get("subutai").trace("SL_Shutdown");
        Session::instance()->getNotificationCenter()->add(SCRIPT_FINISHED);
        return Py_BuildValue("i", 1);
    }

    // ========================================================================

    static PyObject* SL_CheckDirectories(PyObject* self, PyObject* args)
    {
        if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }
        //Poco::Logger::get("subutai").information("SL_CheckDirectories");
        Session::instance()->getNotificationCenter()->add(SCRIPT_FINISHED);
        auto settings = Session::instance()->getSettings();
        auto tmpDir = settings->getTmpPath();
        auto installDir = settings->getInstallationPath();

        try
        {
            FileSystem fs("/");
            if (!fs.isFileExists(installDir))
            {
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

        }
        catch (SubutaiException e)
        {
            return Py_BuildValue("i", 0);
        }

        return Py_BuildValue("i", 1);
    }

    // ========================================================================

    static PyObject* SL_Download(
            PyObject* self,
            PyObject* args,
            PyObject* keywords)
    {
        if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }
        //Poco::Logger::get("subutai").information("SL_Download");
        if (!PyArg_ParseTupleAndKeywords(
                    args, keywords, "s", download_keywords, &sl_filename))
        {
            return NULL;
        }
        Poco::Logger::get("subutai").trace("SL_Download ~ %s", std::string(sl_filename));
        auto downloader = Session::instance()->getDownloader();
        PyErr_Print();
        downloader->setFilename(sl_filename);
        if (!downloader->retrieveFileInfo())
        {
            Session::instance()
                ->getNotificationCenter()
                ->notificationRaised(
                        N_ERROR,
                        Poco::DynamicAny("Failed to retrieve file data"));
        }
        else
        {
            Session::instance()->getNotificationCenter()->add(DOWNLOAD_STARTED);
            std::printf("File info retrieved\n");
            auto t = downloader->download();
            t.detach();
        }
        return Py_BuildValue("s", sl_filename);
    }

    // ========================================================================

    static PyObject* SL_IsDownloaded(PyObject* self, PyObject* args)
    {
        if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }
        //Poco::Logger::get("subutai").information("SL_IsDownloaded");
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


    static PyObject* SL_GetBytesDownload(PyObject* self, PyObject* args)
    {
        auto downloader = Session::instance()->getDownloader();
        long b = downloader->getBytesDownload();
        Poco::Logger::get("subutai").trace("~~~~~~~~~~~~~~~~~~~~~~~~~~~~ %ld", b);
        return Py_BuildValue("l", b);
    }

    static PyObject* SL_GetPercentDownload(PyObject* self, PyObject* args)
    {
        auto downloader = Session::instance()->getDownloader();
        int p = downloader->getPercentDownload();
        return Py_BuildValue("i", p);
    }

    // ========================================================================

    static PyObject* SL_GetProgress(PyObject* self, PyObject* args)
    {
        if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }
        //Poco::Logger::get("subutai").information("SL_GetProgress");
        auto downloader = Session::instance()->getDownloader();
        auto percent = downloader->getPercent();
        return Py_BuildValue("i", percent);
    }

    // ========================================================================

    static PyObject* SL_GetTmpDir(PyObject* self, PyObject* args)
    {
        if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }
        Poco::Logger::get("subutai").trace("SL_GetTmpDir");
        auto settings = Session::instance()->getSettings();
        auto path = settings->getTmpPath();
        Poco::Logger::get("subutai").trace("SL_GetTmpDir ~ %s", settings->getTmpPath());
        return Py_BuildValue("s", path.c_str());
    }

    // ========================================================================

    static PyObject* SL_GetInstallDir(PyObject* self, PyObject* args)
    {
        if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }
        auto settings = Session::instance()->getSettings();
        //auto path = settings->getInstallationPath().c_str();
        std::string path = settings->getInstallationPath();
        Poco::Logger::get("subutai").trace("SL_GetInstallDir ~ %s", settings->getInstallationPath());
        return Py_BuildValue("s", path.c_str());
    }

    // ========================================================================

    static PyObject* SL_SetTmpDir(PyObject* self, PyObject* args, PyObject* keywords)
    {
        if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }
        if (!PyArg_ParseTupleAndKeywords(args, keywords, "s|i", tmpdir_keywords, &sl_tmpdir))
            return NULL;
        Session::instance()->getDownloader()->setOutputDirectory(sl_tmpdir);
        return Py_BuildValue("i", 1);
    }

    // ========================================================================

    // Notification messages

    // ========================================================================

    static PyObject* SL_RaiseError(PyObject* self, PyObject* args, PyObject* keywords)
    {
        if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }
        if (!PyArg_ParseTupleAndKeywords(args, keywords, "s", string_keywords, &sl_string))
            return NULL;
        Poco::Logger::get("subutai").trace("SL_RaiseError ~ %s", std::string(sl_string));
        Poco::Dynamic::Var v = sl_string;
        Session::instance()->getNotificationCenter()->notificationRaised(N_ERROR, v);
        return Py_BuildValue("i", 1);
    }

    // ========================================================================

    static PyObject* SL_RaiseWarning(PyObject* self, PyObject* args, PyObject* keywords)
    {
        if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }
        if (!PyArg_ParseTupleAndKeywords(args, keywords, "s", string_keywords, &sl_string))
            return NULL;
        Poco::Logger::get("subutai").trace("SL_RaiseWarning ~ %s", std::string(sl_string));
        Poco::Dynamic::Var v = sl_string;
        Session::instance()->getNotificationCenter()->notificationRaised(N_WARNING, v);
        return Py_BuildValue("i", 1);
    }

    // ========================================================================

    static PyObject* SL_RaiseInfo(PyObject* self, PyObject* args, PyObject* keywords)
    {
        if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }
        if (!PyArg_ParseTupleAndKeywords(args, keywords, "s", string_keywords, &sl_string))
            return NULL;
        Poco::Logger::get("subutai").trace("SL_RaiseInfo ~ %s", std::string(sl_string));
        Poco::Dynamic::Var v = sl_string;
        Session::instance()->getNotificationCenter()->notificationRaised(N_INFO, v);
        return Py_BuildValue("i", 1);
    }

    // ========================================================================
    
    static PyObject* SL_IsVBoxInstalled(PyObject* self, PyObject* args)
    {
        if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }
        Poco::Logger::get("subutai").trace("SL_IsVBoxInstalled");

        VirtualBox vb;
        if (vb.findInstallation()) return Py_BuildValue("i", 0);
        return Py_BuildValue("i", 1);
    }
    
    // ========================================================================

    static PyObject* SL_VBox(PyObject* self, PyObject* args, PyObject* keywords)
    {
        if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }
        if (!PyArg_ParseTupleAndKeywords(args, keywords, "s", string_keywords, &sl_string))
            return NULL;

        Poco::Logger::get("subutai").trace("SL_VBox ~ %s", std::string(sl_string));
        VirtualBox vb;
        std::string out = vb.execute(sl_string);
        size_t found = out.find("error");
        if (found == std::string::npos)
            return Py_BuildValue("i", 1);
        return Py_BuildValue("i", 0);
    }

    // ========================================================================

    static PyObject* SL_VBoxS(PyObject* self, PyObject* args, PyObject* keywords)
    {
        if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }
        if (!PyArg_ParseTupleAndKeywords(args, keywords, "s", string_keywords, &sl_string))
            return NULL;

        Poco::Logger::get("subutai").trace("SL_VBoxS ~ %s", std::string(sl_string));
        VirtualBox vb;
        int exitStatus = 0;
        std::string out = vb.execute(sl_string, exitStatus);
        return Py_BuildValue("i", exitStatus);
    }

    // ========================================================================

    static PyObject* SL_GetDefaultRoutingInterface(PyObject* self, PyObject* args)
    {
        if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }
        Poco::Logger::get("subutai").trace("SL_GetDefaultRoutingInterface");
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
        if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }
        if (!PyArg_ParseTupleAndKeywords(args, keywords, "s", string_keywords, &sl_string))
            return NULL;
        Poco::Logger::get("subutai").trace("SL_GetVBoxBridgedInterface ~ %s", std::string(sl_string));
        VirtualBox vb;
        return Py_BuildValue("s", vb.getBridgedInterface(sl_string).c_str());
    }

    // ========================================================================

    static PyObject* SL_GetVBoxHostOnlyInterface(
            PyObject* self,
            PyObject* args
            )
    {
        if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }
        Poco::Logger::get("subutai").trace("SL_GetVBoxHostOnlyInterface");

        VirtualBox vb;
        return Py_BuildValue("s", vb.getHostOnlyAdapter().c_str());
    }

    // ========================================================================

    static PyObject* SL_SetSSHCredentials(
            PyObject* self,
            PyObject* args,
            PyObject* keywords
            )
    {
        if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }
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
        Poco::Logger::get("subutai").trace("SL_SetSSHCredentials ~ user: %s, host: %s:%d", std::string(ssh_user), std::string(ssh_host), ssh_port);

        Session::instance()->setSSHCredentials(ssh_user, ssh_pass, ssh_host, (long)ssh_port);
        return Py_BuildValue("i", 0);
    }

    // ========================================================================

    static PyObject* SL_TestSSH(PyObject* self, PyObject* args)
    {
        if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }
        Poco::Logger::get("subutai").trace("SL_TestSSH");
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
        if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }
        Poco::Logger::get("subutai").trace("SL_InstallSSHKey");
        auto s = Session::instance();

        SSH *p = new SubutaiLauncher::SSH();
        p->setHost(s->getSSHHostname(), s->getSSHPort());
        p->setUsername(s->getSSHUser(), s->getSSHPass());
        p->connect();
        if (!p->isConnected())
        {
            p->disconnect();
            delete p;
            return Py_BuildValue("i", 1);
        }
        p->authenticate();
        if (!p->isAuthenticated())
        {
            p->disconnect();
            delete p;
            return Py_BuildValue("i", 1);
        }

        auto key = SSH::getPublicKey();
        if (key == "")
        {
            p->disconnect();
            delete p;
            return Py_BuildValue("i", 1);
        }
        std::string cmd = "echo '";
        cmd.append(key);
        cmd.append("' >> /home/subutai/.ssh/authorized_keys");
        auto output = p->execute(cmd);
        p->disconnect();
        delete p;

        return Py_BuildValue("i", 0);
    }

    // ========================================================================

    static PyObject* SL_SSHRun(PyObject* self, PyObject* args, PyObject* keywords)
    {
        if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }
        if (!PyArg_ParseTupleAndKeywords(
                    args,
                    keywords,
                    "s",
                    string_keywords,
                    &sl_string
                    ))
            return NULL;
        Poco::Logger::get("subutai").trace("SL_SSHRun ~ %s", std::string(sl_string));

        auto s = Session::instance();

        SSH *p = new SubutaiLauncher::SSH();
        p->setHost(s->getSSHHostname(), s->getSSHPort());
        p->setUsername(s->getSSHUser(), s->getSSHPass());
        p->connect();
        p->authenticate();
        auto ret = p->execute(sl_string);
        p->disconnect();
        delete p;

        return Py_BuildValue("i", 0);
    }

    // ========================================================================
    // Unused
    // ========================================================================

    static PyObject* SL_StartProcedure(PyObject* self, PyObject* args, PyObject* keywords)
    {
        if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }
        if (!PyArg_ParseTupleAndKeywords(args, keywords, "s", string_keywords, &sl_string))
            return NULL;
        Poco::Logger::get("subutai").trace("SL_StartProcedure ~ %s", std::string(sl_string));
        return Py_BuildValue("i", 0);
    }

    // ========================================================================
    // Unused
    // ========================================================================

    static PyObject* SL_StopProcedure(PyObject* self, PyObject* args)
    {
        if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }
        return Py_BuildValue("i", 0);
    }

    // ========================================================================
    // VM
    // ========================================================================

    static PyObject* SL_CheckVMExists(PyObject* self, PyObject* args, PyObject* keywords)
    {
        if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }
        if (!PyArg_ParseTupleAndKeywords(args, keywords, "s", string_keywords, &sl_string))
            return NULL;
        Poco::Logger::get("subutai").trace("SL_CheckVMExists ~ %s", std::string(sl_string));

        VirtualBox vb;

        if (vb.isMachineExists(sl_string))
            return Py_BuildValue("i", 0);
        return Py_BuildValue("i", 1);
    }

    // ========================================================================

    static PyObject* SL_CheckVMRunning(PyObject* self, PyObject* args, PyObject* keywords)
    {
        if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }
        if (!PyArg_ParseTupleAndKeywords(args, keywords, "s", string_keywords, &sl_string))
            return NULL;
        Poco::Logger::get("subutai").trace("SL_CheckVMRunning~ %s", std::string(sl_string));

        VirtualBox vb;

        if (vb.isMachineRunning(sl_string))
            return Py_BuildValue("i", 0);
        return Py_BuildValue("i", 1);
    }

    // ========================================================================

    static PyObject* SL_AddStatus(PyObject* self, PyObject* args, PyObject* keywords)
    {
        if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }
        if (!PyArg_ParseTupleAndKeywords(args, keywords, "s", string_keywords, &sl_string)) {
            return NULL;
        }

        std::string status(sl_string);
        Session::instance()->addStatus(status);
        return Py_BuildValue("i", 0);
    }

    // ========================================================================

    static PyObject* SL_GetDownloadProgress(PyObject* self, PyObject* args)
    {
        if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }
        auto pProgress = Session::instance()->getDownloader()->getPercent();
        return Py_BuildValue("d", pProgress);
    }

    // ========================================================================

    static PyObject* SL_SetProgress(PyObject* self, PyObject* args, PyObject* keywords)
    {
        if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }
        if (!PyArg_ParseTupleAndKeywords(args, keywords, "d", double_keywords, &sl_double))
            return NULL;

        std::printf("DOUBLE: %f\n", sl_double);
        Poco::Dynamic::Var pProgress = sl_double;
        Session::instance()->getNotificationCenter()->notificationRaised(N_DOUBLE_DATA, pProgress);
        return Py_BuildValue("i", 0);
    }

    // ========================================================================

    static PyObject* SL_GetFileSize(PyObject* self, PyObject* args, PyObject* keywords)
    {
        if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }
        if (!PyArg_ParseTupleAndKeywords(args, keywords, "s", string_keywords, &sl_string))
            return NULL;

        auto pDownloader = Session::instance()->getDownloader();
        pDownloader->setFilename(sl_string);
        long result = -1;
        if (pDownloader->retrieveFileInfo()) 
        {
            auto pInfo = pDownloader->info();
            result = pInfo.size;
            return Py_BuildValue("l", result);
        }
        return Py_BuildValue("l", 0);
    }

    // ========================================================================

    static PyObject* SL_MakeLink(PyObject* self, PyObject* args, PyObject* keywords)
    {
        if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }
        //Poco::Logger::get("subutai").information("SL_MakeLink");
        if (!PyArg_ParseTupleAndKeywords(args, keywords, "ss", desc_keywords, &sl_string, &sl_desc))
            return NULL;

#if LAUNCHER_LINUX
        RootProcess* rp = new RootProcess();
        char cmd[1024];
        std::sprintf(cmd, "ln -s %s %s", sl_string, sl_desc);
        rp->addCommand(std::string(cmd));
        rp->execute("Install executable");
        delete rp;
        return Py_BuildValue("i", 0);
#elif LAUNCHER_MACOS
        Poco::Process::Args symlinkArgs;
        symlinkArgs.push_back("-s");
        symlinkArgs.push_back(sl_string);
        symlinkArgs.push_back(sl_desc);
        auto ph = Poco::Process::launch("ln", symlinkArgs);
        int exitCode = ph.wait();
        return Py_BuildValue("i", exitCode);
#else
        return Py_BuildValue("i", 1);
#endif

    }

    // ========================================================================

    static PyObject* SL_Log(PyObject* self, PyObject* args, PyObject* keywords)
    {
        if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }
        if (!PyArg_ParseTupleAndKeywords(args, keywords, "ss", desc_keywords, &sl_string, &sl_desc))
            return NULL;

        Poco::Logger::get("subutai").information("SL %s: %s", std::string(sl_string), std::string(sl_desc));
        return Py_BuildValue("i", 0);
    }

    // ========================================================================

    static PyObject* SL_RegisterService(PyObject* self, PyObject* args, PyObject* keywords)
    {
        if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }
        if (!PyArg_ParseTupleAndKeywords(args, keywords, "ss", desc_keywords, &sl_string, &sl_desc))
            return NULL;

        Poco::Logger::get("subutai").trace("SL_RegisterService ~ %s %s", std::string(sl_string), std::string(sl_desc));

        // sl_string - name of service
        // sl_desc - path_to_exe|arguments
        std::vector<std::string> pArgs;
        std::string pPath = "";

        Poco::StringTokenizer st(std::string(sl_desc), "|", Poco::StringTokenizer::TOK_TRIM | Poco::StringTokenizer::TOK_IGNORE_EMPTY);
        for (auto it = st.begin(); it != st.end(); it++)
        {
            if (it == st.begin())
            {
                pPath = (*it);
                continue;
            }
            pArgs.push_back((*it));
        }

        Environment e;
        bool rc = e.registerService(std::string(sl_string), pPath, pArgs);
        if (rc)
        {
            return Py_BuildValue("i", 0);
        }
        return Py_BuildValue("i", 1);
    }

    // ========================================================================

    static PyObject* SL_UnregisterService(PyObject* self, PyObject* args, PyObject* keywords)
    {
        if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }
        if (!PyArg_ParseTupleAndKeywords(args, keywords, "s", string_keywords, &sl_string))
            return NULL;

        Poco::Logger::get("subutai").trace("SL_UnregisterService ~ %s", std::string(sl_string));

        Environment e;
        bool rc = e.unregisterService(std::string(sl_string));
        if (rc)
        {
            return Py_BuildValue("i", 0);
        }
        return Py_BuildValue("i", 1);
    }

    // ========================================================================

    static PyObject* SL_CreateDesktopShortcut(PyObject* self, PyObject* args, PyObject* keywords)
    {
        if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }
        if (!PyArg_ParseTupleAndKeywords(args, keywords, "ss", desc_keywords, &sl_string, &sl_desc))
            return NULL;
        Poco::Logger::get("subutai").trace("SL_CreateDesktopShortcut ~ %s %s", std::string(sl_string), std::string(sl_desc));

        Environment e;
        e.CreateShortcut(std::string(sl_string), std::string(sl_desc));
        return Py_BuildValue("i", 0);
    }

    // ========================================================================

    static PyObject* SL_UpdatePath(PyObject* self, PyObject* args)
    {
        if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }
        Environment e;
        e.updatePath(Session::instance()->getSettings()->getInstallationPath() + "bin");
        return Py_BuildValue("i", 0);
    }

    // ========================================================================

    static PyObject* SL_ProcessKill(PyObject* self, PyObject* args, PyObject* keywords)
    {

        if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }
        if (!PyArg_ParseTupleAndKeywords(args, keywords, "s", string_keywords, &sl_string))
            return NULL;

        Poco::Logger::get("subutai").trace("SL_ProcessKill ~ %s", std::string(sl_string));

        Environment e;
        bool rc = e.killProcess(std::string(sl_string));
        if (rc)
        {
            return Py_BuildValue("i", 0);
        }
        return Py_BuildValue("i", 1);
    }

    // ========================================================================

    static PyObject* SL_GetRemoteFileSize(PyObject* self, PyObject* args, PyObject* keywords)
    {
        if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }
        if (!PyArg_ParseTupleAndKeywords(args, keywords, "s", string_keywords, &sl_string))
            return NULL;

        auto downloader = Session::instance()->getDownloader();

        downloader->setFilename(std::string(sl_string));
        if (!downloader->retrieveFileInfo())
        {
            Session::instance()
                ->getNotificationCenter()
                ->notificationRaised(
                        N_ERROR,
                        Poco::DynamicAny("Failed to retrieve file data"));
            return Py_BuildValue("i", -1);
        }
        auto info = downloader->info();

        return Py_BuildValue("i", info.size);
    }

    // ========================================================================

    static PyObject* SL_GetRemoteTemplateSize(PyObject* self, PyObject* args, PyObject* keywords)
    {
        if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }
        if (!PyArg_ParseTupleAndKeywords(args, keywords, "s", string_keywords, &sl_string))
            return NULL;

        auto downloader = Session::instance()->getDownloader();

        downloader->setFilename(std::string(sl_string));
        if (!downloader->retrieveTemplateInfo())
        {
            Session::instance()
                ->getNotificationCenter()
                ->notificationRaised(
                        N_ERROR,
                        Poco::DynamicAny("Failed to retrieve file data"));
            return Py_BuildValue("i", -1);
        }
        auto info = downloader->info();

        return Py_BuildValue("i", info.size);
    }

    // ========================================================================

    static PyObject* SL_GetPeerFileSize(PyObject* self, PyObject* args, PyObject* keywords)
    {
        if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }
        if (!PyArg_ParseTupleAndKeywords(args, keywords, "s", string_keywords, &sl_string))
            return NULL;

        auto s = Session::instance();

        SSH *p = new SubutaiLauncher::SSH();
        p->setHost(s->getSSHHostname(), s->getSSHPort());
        p->setUsername(s->getSSHUser(), s->getSSHPass());
        p->connect();
        p->authenticate();
        std::string cmd("ls -l ");
        cmd.append(sl_string);
        cmd.append(" | awk '{print $5}'");
        auto ret = p->execute(sl_string);
        p->disconnect();
        delete p;
        Poco::Dynamic::Var v(ret);
        return Py_BuildValue("i", v.convert<int>());
    }

    // ========================================================================

    static PyObject* SL_GetVBoxPath(PyObject* self, PyObject* args)
    {
        VirtualBox vb;
        if (!vb.findInstallation())
        {
            return Py_BuildValue("s", "");
        }

        std::string pLocation = vb.getBinaryLocation();
        return Py_BuildValue("s", pLocation.c_str());
    }

    // ========================================================================

    static PyObject* SL_GetCoreNum(PyObject* self, PyObject* args)
    {
        if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }

        int num = Session::instance()->getSettings()->getCoreNum();
        return Py_BuildValue("i", num);
    }

    // ========================================================================

    static PyObject* SL_GetMemSize(PyObject* self, PyObject* args)
    {
        if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }

        int size = Session::instance()->getSettings()->getMemSize();
        return Py_BuildValue("i", size);
    }

    // ========================================================================

    static PyObject* SL_GetPeerIP(PyObject* self, PyObject* args)
    {
        if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }
        Poco::Logger::get("subutai").trace("SL_GetPeerIP");
        auto s = Session::instance();

        SSH *p = new SubutaiLauncher::SSH();
        p->setHost(s->getSSHHostname(), s->getSSHPort());
        p->setUsername(s->getSSHUser(), s->getSSHPass());
        p->connect();
        p->authenticate();
        auto ret = p->execute("sudo subutai info ipaddr");
        p->disconnect();
        delete p;
        return Py_BuildValue("s", ret.c_str());
    }

    // ========================================================================

    static PyObject* SL_IsPeerReady(PyObject* self, PyObject* args, PyObject* keywords)
    {
        if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }
        Poco::Logger::get("subutai").trace("SL_IsPeerReady");
        if (!PyArg_ParseTupleAndKeywords(args, keywords, "s", string_keywords, &sl_string))
            return NULL;

        try
        {
            SS* subutai = new SS(std::string(sl_string));
            if (subutai->checkPeerInstall())
            {
                delete subutai;
                return Py_BuildValue("i", 0);
            }
            delete subutai;
            return Py_BuildValue("i", 1);
        }
        catch (std::exception& e)
        {
            return Py_BuildValue("i", 2);
        }
    }

    // ========================================================================
    //
    static PyObject* SL_GetOSVersionNumber(PyObject* self, PyObject* args)
    {
        if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }
        Poco::Logger::get("subutai").trace("SL_GetPeerIP");

        Environment env;
        std::string version = env.versionNumber();
        return Py_BuildValue("s", version.c_str());
    }

    // ========================================================================

#if LAUNCHER_LINUX 
    static PyObject* SL_AddSystemdUnit(PyObject* self, PyObject* args, PyObject* keywords)
    {
        if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }
        Poco::Logger::get("subutai").trace("SL_AddSystemdUnit");
        if (!PyArg_ParseTupleAndKeywords(args, keywords, "ss", desc_keywords, &sl_string, &sl_desc))
            return NULL;

        char cmd[1024];
        std::sprintf(cmd, "cp %s %s/%s", sl_desc, "/etc/systemd/system/", sl_string);

        RootProcess* rp = new RootProcess();
        rp->addCommand(std::string(cmd));
        std::sprintf(cmd, "systemctl enable %s", sl_string);
        rp->addCommand(std::string(cmd));
        std::sprintf(cmd, "systemctl start %s", sl_string);
        rp->addCommand(std::string(cmd));
        rp->execute("Install systemd unit");
        delete rp;
        return Py_BuildValue("i", 0);
    }

    // ========================================================================

    static PyObject* SL_RemoveSystemdUnit(PyObject* self, PyObject* args, PyObject* keywords)
    {
        if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }
        Poco::Logger::get("subutai").trace("SL_RemoveSystemdUnit");
        if (!PyArg_ParseTupleAndKeywords(args, keywords, "s", string_keywords, &sl_string))
            return NULL;

        char cmd[1024];

        RootProcess* rp = new RootProcess();
        std::sprintf(cmd, "systemctl stop %s", sl_string);
        rp->addCommand(std::string(cmd));
        std::sprintf(cmd, "systemctl disable %s", sl_string);
        rp->addCommand(std::string(cmd));
        std::sprintf(cmd, "rm -rf /etc/systemd/system/%s", sl_string);
        rp->addCommand(std::string(cmd));
        rp->execute("Uninstall P2P systemd unit");
        delete rp;
        return Py_BuildValue("i", 0);
    }

    // ========================================================================

    static PyObject* SL_DesktopFileInstall(PyObject* self, PyObject* args, PyObject* keywords)
    {
        if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }
        Poco::Logger::get("subutai").trace("SL_DesktopFileInstall");
        if (!PyArg_ParseTupleAndKeywords(args, keywords, "s", string_keywords, &sl_string))
            return NULL;

        char cmd[1024];

        RootProcess* rp = new RootProcess();
        std::sprintf(cmd, "desktop-file-install %s", sl_string);
        rp->addCommand(std::string(cmd));
        rp->execute("Setup Tray");
        delete rp;
        return Py_BuildValue("i", 0);
    }

    // ========================================================================

    static PyObject* SL_InstallVBox(PyObject* self, PyObject* args, PyObject* keywords)
    {
        if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }
        Poco::Logger::get("subutai").trace("SL_InstallVBox");
        if (!PyArg_ParseTupleAndKeywords(args, keywords, "s", string_keywords, &sl_string))
            return NULL;

        char cmd[1024];

        RootProcess* rp = new RootProcess();
        std::sprintf(cmd, "dpkg -i %s", sl_string);
        rp->addCommand(std::string(cmd));
        rp->addCommand("modprobe vboxnetadp");
        rp->addCommand("modprobe vboxpci");
        rp->execute("Setup VirtualBox");
        delete rp;
        return Py_BuildValue("i", 0);
    }
#endif

    // ========================================================================

    static __attribute_used__ PyObject* SL_RegisterPlugin(PyObject* self, PyObject* args)
    {
#if LAUNCHER_WINDOWS
		if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }
		Poco::Logger::get("subutai").trace("SL_RegisterPlugin");
        Environment e;
        if (e.writeE2ERegistry(""))
        {
            return Py_BuildValue("i", 0);
        }
        else
        {
            return Py_BuildValue("i", 1);
        }
#else
        return Py_BuildValue("i", 1);
#endif
    }

	// ========================================================================
#if LAUNCHER_WINDOWS
	static __attribute_used__ PyObject* SL_IsChromeInstalled(PyObject* self, PyObject* args)
	{
		if (Session::instance()->isTerminating()) { return Py_BuildValue("i", 0); }
		Poco::Logger::get("subutai").trace("SL_IsChromeInsatlled");
		HKEY root = HKEY_LOCAL_MACHINE;
		HKEY tmp_key;

		int res = 0;

		if ((res = RegOpenKeyExW(root, L"SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Google Chrome",
			0, KEY_ALL_ACCESS, &tmp_key)) != ERROR_SUCCESS) {
			Poco::Logger::get("subutai").error("SL_IsChromeInsatlled: %d -> %d", res, GetLastError());
			return Py_BuildValue("i", 1);
		}
		Poco::Logger::get("subutai").information("Google Chrome was found");
		return Py_BuildValue("i", 0);
	}
#endif

    // ========================================================================
    // Module bindings
    // ========================================================================

    static PyMethodDef SubutaiSLMethods[] = {
        { "download", (PyCFunction)SL_Download, METH_VARARGS | METH_KEYWORDS, "Downloads a file from Subutai CDN" },
        //{ "GetDownloadResult", SL_GetDownloadResult, METH_VARARGS, "Returns download last error" },
        { "setTmpDir", (PyCFunction)SL_SetTmpDir, METH_VARARGS | METH_KEYWORDS, "Sets tmp output directory" },
        { "isDownloadComplete", SL_IsDownloaded, METH_VARARGS, "Returns 1 if current download has been completed" },
        { "GetBytesDownload", SL_GetBytesDownload, METH_VARARGS, "Returns number of bytes downloaded" },
        { "GetPercentDownload", SL_GetPercentDownload, METH_VARARGS, "Returns percents of download complete" },
        { "GetTmpDir", SL_GetTmpDir, METH_VARARGS, "Returns tmp directory" },
        { "GetInstallDir", SL_GetInstallDir, METH_VARARGS, "Returns installation directory" },
        { "getProgress", SL_GetProgress, METH_VARARGS, "Returns bool" },
        { "hello", SL_HelloWorld, METH_VARARGS, "Hello World method of subutai scripting language" },
        { "debug", SL_Debug, METH_VARARGS, "Shows debug information about current launcher instance and environment" },
        { "version", SL_Version, METH_VARARGS, "Display launcher version" },
        { "CheckDirectories", SL_CheckDirectories, METH_VARARGS, "Display launcher version" },
        { "RaiseError", (PyCFunction)SL_RaiseError, METH_VARARGS | METH_KEYWORDS, "Raising error" },
        { "RaiseWarning", (PyCFunction)SL_RaiseWarning, METH_VARARGS | METH_KEYWORDS, "Raising warning" },
        { "RaiseInfo", (PyCFunction)SL_RaiseInfo, METH_VARARGS | METH_KEYWORDS, "Raising info" },
        { "IsVBoxInstalled", SL_IsVBoxInstalled, METH_VARARGS, "Returns 0 if vbox is installed" },
        { "VBox", (PyCFunction)SL_VBox, METH_VARARGS | METH_KEYWORDS, "Tells vboxmanage to do something" },
        { "VBoxS", (PyCFunction)SL_VBoxS, METH_VARARGS | METH_KEYWORDS, "Tells vboxmanage to do something and returns status" },
        { "Shutdown", SL_Shutdown, METH_VARARGS, "Finalizes the script" },
        { "GetMasterVersion", SL_GetMasterVersion, METH_VARARGS, "Returns master version of a product" },
        { "GetDevVersion", SL_GetDevVersion, METH_VARARGS, "Returns dev version of a product" },
        // New version
        //{"ImportVirtualMachine", SL_importVirtualMachine, METH_VARARGS | METH_KEYWORDS, "Import a virtual machine into VB"},
        { "GetDefaultRoutingInterface", SL_GetDefaultRoutingInterface, METH_VARARGS, "Returns name of default network interface" },
        { "GetVBoxBridgedInterface", (PyCFunction)SL_GetVBoxBridgedInterface, METH_VARARGS | METH_KEYWORDS, "Returns name of default network interface" },
        { "GetVBoxHostOnlyInterface", (PyCFunction)SL_GetVBoxHostOnlyInterface, METH_VARARGS, "Returns name of the VB HO interface" },
        { "SetSSHCredentials", (PyCFunction)SL_SetSSHCredentials, METH_VARARGS | METH_KEYWORDS, "Set SSH Connection credentials" },
        { "TestSSH", (PyCFunction)SL_TestSSH, METH_VARARGS, "Test if SSH connection is alive" },
        { "InstallSSHKey", (PyCFunction)SL_InstallSSHKey, METH_VARARGS, "Install SSH public key" },
        { "SSHRun", (PyCFunction)SL_SSHRun, METH_VARARGS | METH_KEYWORDS, "Run command over SSH" },
        { "CheckVMExists", (PyCFunction)SL_CheckVMExists, METH_VARARGS | METH_KEYWORDS, "Checks if VM with this name exists" },
        { "CheckVMRunning", (PyCFunction)SL_CheckVMRunning, METH_VARARGS | METH_KEYWORDS, "Checks if VM with this name running" },
        { "GetScheme", SL_GetScheme, METH_VARARGS, "Returns current scheme: production, master, dev" },
        { "StartProcedure", (PyCFunction)SL_StartProcedure, METH_VARARGS | METH_KEYWORDS, "Starts install/update/remove procedure" },
        { "StopProcedure", SL_StopProcedure, METH_VARARGS, "Stops install/update/remove procedure" },
        { "AddStatus", (PyCFunction)SL_AddStatus, METH_VARARGS | METH_KEYWORDS, "Add status" },
        //{"RootCommand", (PyCFunction)SL_RootCommand, METH_VARARGS | METH_KEYWORDS, "Executes root command"},
        { "MakeLink", (PyCFunction)SL_MakeLink, METH_VARARGS | METH_KEYWORDS, "Executes ln -s on root behalf" },
        { "GetDownloadProgress", SL_GetDownloadProgress, METH_VARARGS, "Return percentage of download" },
        { "SetProgress", (PyCFunction)SL_SetProgress, METH_VARARGS | METH_KEYWORDS, "Sets action percentage" },
        { "GetFileSize", (PyCFunction)SL_GetFileSize, METH_VARARGS | METH_KEYWORDS, "Gets remote file size" },
        { "log", (PyCFunction)SL_Log, METH_VARARGS | METH_KEYWORDS, "Writes to log" },
        { "RegisterService", (PyCFunction)SL_RegisterService, METH_VARARGS | METH_KEYWORDS, "Win32: Register new service" },
        { "UnregisterService", (PyCFunction)SL_UnregisterService, METH_VARARGS | METH_KEYWORDS, "Win32: Unregister service" },
        { "CreateDesktopShortcut", (PyCFunction)SL_CreateDesktopShortcut, METH_VARARGS | METH_KEYWORDS, "Creates a shortcut on a desktop" },
        { "UpdatePath", SL_UpdatePath, METH_VARARGS, "Updates path variables on windows" },
        { "ProcessKill", (PyCFunction)SL_ProcessKill, METH_VARARGS | METH_KEYWORDS, "Kills a Windows process" },
        { "GetRemoteFileSize", (PyCFunction)SL_GetRemoteFileSize, METH_VARARGS | METH_KEYWORDS, "Retrieves a file size for kurjun file" },
        { "GetRemoteTemplateSize", (PyCFunction)SL_GetRemoteTemplateSize, METH_VARARGS | METH_KEYWORDS, "Retrieves a file size for kurjun file" },
        { "GetPeerFileSize", (PyCFunction)SL_GetPeerFileSize, METH_VARARGS | METH_KEYWORDS, "Retrieves a file size for a file inside a peer over SSH" },
        { "GetVBoxPath", SL_GetVBoxPath, METH_VARARGS, "Returns path to a vboxmanage binary" },
        { "GetCoreNum", SL_GetCoreNum, METH_VARARGS, "Returns choosen amount of cores" },
        { "GetMemSize", SL_GetMemSize, METH_VARARGS, "Return amount of memory" },
        { "GetPeerIP", SL_GetPeerIP, METH_VARARGS, "Returns Peer IP address" },
        { "IsPeerReady", (PyCFunction)SL_IsPeerReady, METH_VARARGS | METH_KEYWORDS, "Returns 0 if peer is ready or 1 if it's not" },
        { "GetOSVersionNumber", SL_GetOSVersionNumber, METH_VARARGS, "Returns number of OS version" },
#if LAUNCHER_LINUX
        { "AddSystemdUnit", (PyCFunction)SL_AddSystemdUnit, METH_VARARGS | METH_KEYWORDS, "Adds new systemd unit" },
        { "RemoveSystemdUnit", (PyCFunction)SL_RemoveSystemdUnit, METH_VARARGS | METH_KEYWORDS, "Removes systemd unit" },
        { "DesktopFileInstall", (PyCFunction)SL_DesktopFileInstall, METH_VARARGS | METH_KEYWORDS, "Runs desktop-file-install" },
        { "InstallVBox", (PyCFunction)SL_InstallVBox, METH_VARARGS | METH_KEYWORDS, "Install virtualbox as root" },
#endif
#if LAUNCHER_WINDOWS
        { "RegisterPlugin", SL_RegisterPlugin, METH_VARARGS, "Registers a plugin in windows registry" },
		{ "IsChromeInstalled", SL_IsChromeInstalled, METH_VARARGS, "Checks whether chrome web browser installed or not" },
#endif
        { NULL, NULL, 0, NULL }
    };

#if PY_MAJOR_VERSION >= 3
    static PyModuleDef SubutaiModule = {
        PyModuleDef_HEAD_INIT, "subutai", NULL, -1, SubutaiSLMethods,
        NULL, NULL, NULL, NULL
    };

    static __attribute_used__ PyObject* PyInit_Subutai(void)
    {
        return PyModule_Create(&SubutaiModule);
    }
#endif


}

#endif
