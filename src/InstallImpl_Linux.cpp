#include "InstallImpl_Linux.h"

#if LAUNCHER_LINUX
SubutaiLauncher::InstallImpl::InstallImpl()
{

}

void SubutaiLauncher::InstallImpl::preInstall()
{
	FileSystem fs;
	fs.setPath(_srcPath);
	if (!fs.isFileExists(_filename))
	{
		throw SubutaiException("Can't install. Source file was not found");
	}
}

void SubutaiLauncher::InstallImpl::install()
{
	char* sp;
	char* dp;
	std::sprintf(sp, "%s%s%s", _srcPath.c_str(), PATH_DELIM, _filename.c_str());
	std::sprintf(dp, "%s%s%s", _dstPath.c_str(), PATH_DELIM, _filename.c_str());
	FileSystem fs;
	fs.copyFile(std::string(sp), std::string(dp));
}

void SubutaiLauncher::InstallImpl::postInstall()
{
	// Make a link to a file in PATH

	std::vector<std::string> binPath;
	binPath.push_back("/usr/bin");
	binPath.push_back("/usr/local/bin");
	binPath.push_back("/bin");

	auto env = new Environment();
	SubutaiString pathVar(env->getVar("PATH", ""));
	std::vector<std::string> path;
	pathVar.split(':', path);
	std::string pathCandidate = "";
	for (auto it = path.begin(); it != path.end(); it++)
	{
		for (auto p = binPath.begin(); p != binPath.end(); p++)
		{
			if ((*it).compare((*p)) == 0)
			{
				pathCandidate = (*it);
			}
		}
	}
	if (pathCandidate == "") {
		throw new SubutaiException("No directory was found in the path");
	}

	char* sp;
	char* dp;
	std::sprintf(sp, "%s%s%s", _srcPath.c_str(), PATH_DELIM, _filename.c_str());
	std::sprintf(dp, "%s%s%s", pathCandidate.c_str(), PATH_DELIM, _filename.c_str());
	int rc = symlink(sp, dp);
}
#endif
