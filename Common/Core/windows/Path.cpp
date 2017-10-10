
#include "Core/StringUtilities.h"
#include <cstdarg>
#include "Path.h"
#include <regex>
#include <sstream>
#include <direct.h>
#include <Windows.h>



#include <direct.h>
#include <Windows.h>

using namespace std;
using namespace Realisim;
	using namespace Core;

namespace
{

	const char kNativeDirSeparator('\\');
	const string kAllDirSeparators("/\\");

}

//-----------------------------------------------------------------------------
// Returns the file path of the executable currently being executed.
//
string Path::getApplicationFilePath()
{
    HMODULE hModule = GetModuleHandleW(NULL);
    WCHAR path[MAX_PATH];
    GetModuleFileNameW(hModule, path, MAX_PATH);
    wstring w(path);
    return string(w.begin(), w.end());
}

//-----------------------------------------------------------------------------

string Path::getCurrentWorkingDirectory()
{
	string r;
	char currentPath[FILENAME_MAX];

    if (_getcwd(currentPath, sizeof(currentPath)))
    {
        currentPath[sizeof(currentPath) - 1] = '\0';
        //r = sanitize(currentPath);
        r = currentPath;
    }

	return r;
}

//-----------------------------------------------------------------------------

string Path::join(const string& iPath0, const string& iPath1)
{
    return sanitize(iPath0 + "\\" + iPath1);
}

//-----------------------------------------------------------------------------

string Path::resolve(const string & path)
{
	string r;
	// resolve path. We remove . and resolve ..
	//
	vector<string> resolved;
	vector<string> tokens = Realisim::Core::toVector(path, kNativeDirSeparator);
	for (size_t i = 0; i < tokens.size(); ++i)
	{
		if (tokens[i] == ".")
		{
			//skip
		}
		else if (tokens[i] == ".." && tokens.size() > 0)
		{
			resolved.pop_back();
		}
		else
		{
			resolved.push_back(tokens[i]);
		}
	}

	r = Core::fromVector(resolved, kNativeDirSeparator);
	return r;
}

//-----------------------------------------------------------------------------

string Path::sanitize(const string & path)
{
	// convert all separator to native ones
	// 1- split the path in tokens with both / and \.
	// 2- remove empty token.
	//	2.1 A few exceptions to removing empty tokens...
	//			Keep the first 2 (if both empty) as they represent a network path '\\server\folder\'
	//	2.2	Keep the last one as it represents the folderPath 'c:\\some\thing\'
	//   want to break a folder path in the form c:\a\b\c\
	// 3- from the tokens bake the string with native separator
	//
	vector<string> tokens = Realisim::Core::toVector(path, kAllDirSeparators);

	vector<string> resolve;
	for (size_t i = 0; i < tokens.size(); ++i)
	{
		if (!tokens[i].empty() || i == tokens.size() - 1 ||
            (tokens[i].empty() && (i == 0 || (i == 1 && tokens[0].empty() ) ) ) ) // keep first empty, second empty if first was also empty
		{
			resolve.push_back(tokens[i]);
		}
	}

	string result = Realisim::Core::fromVector(resolve, kNativeDirSeparator);

	// sanitize Windows network path
	string pattern = "\\\\\?\\";
	size_t start = result.find(pattern, 0);
	if (start == 0)
		result = result.substr(start + 4);

	pattern = "UNC\\";
	start = result.find(pattern, 0);
	if (start == 0)
		result = "\\\\" + result.substr(start + 4);

	return result;
}

//-----------------------------------------------------------------------------
void Path::setCurrentWorkingDirectory(const std::string & path)
{
	string p = sanitize(path);
	_chdir(path.c_str());
}