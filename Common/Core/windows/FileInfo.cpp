
#include <direct.h>
#include "FileInfo.h"
#include <stdio.h>
#include <Shlwapi.h>
#include "Path.h"
#include <regex>
#include "Core/StringUtilities.h"
#include <sys/stat.h>

#ifndef FILENAME_MAX
#define FILENAME_MAX 260
#endif

using namespace std;
using namespace Realisim;
	using namespace Core;

namespace
{
	const char kNativeDirSeparator('\\');
}

//-----------------------------------------------------------------------------

FileInfo::FileInfo()
	: mCached(true)
{
	
}

//-----------------------------------------------------------------------------

FileInfo::FileInfo(const std::string & filePath)
	: mCached(true)
{
	setFile(filePath);
}

//-----------------------------------------------------------------------------

FileInfo::~FileInfo()
{

}

//-----------------------------------------------------------------------------

bool FileInfo::exists() const
{
	bool r = mCache.mExists;
	if (!isCached())
	{
		struct stat localBuffer;
		mCache.mExists = (stat(getCanonicalFilePath().c_str(), &localBuffer) == 0);
		r = mCache.mExists;
	}	
	return r;
}

//-----------------------------------------------------------------------------

string FileInfo::extractFileName(const string& iPath) const
{
	string r;
	size_t lastSlash = iPath.find_last_of(kNativeDirSeparator);
	if (lastSlash != std::string::npos)
		r = iPath.substr(lastSlash + 1);
	return r;
}

//-----------------------------------------------------------------------------
// Returns the file's absolute path. It will include the file name if it has one.
// if the file name had a trailing slash, it will be kept.
//
std::string FileInfo::getAbsoluteFilePath() const
{
	string r = getFilePath();
	if( !isAbsolute() )
	{
		r = Path::join(Path::getCurrentWorkingDirectory(), getFilePath());
	}

	return r;
}

//-----------------------------------------------------------------------------
// Returns the file's absolute path. It does not include the file name
// It will never contain a terminating directory separator.
//
std::string FileInfo::getAbsolutePath() const
{
	std::string path = getAbsoluteFilePath();

	// if it's a file, we remove the filename.
	path = removeFileName(path);
	path = removeTrailingSlash(path);
	
	return path;
}

//-----------------------------------------------------------------------------
// Returns the base name of the file without the path. Base name includes all
// file character not including the first '.'
//
std::string FileInfo::getBaseName() const
{
	string r = getFilePath();

	size_t lastSlash = r.find_last_of(kNativeDirSeparator);
	if (lastSlash != std::string::npos)
		r = r.substr(lastSlash + 1);
	size_t firstDot = r.find_first_of(".");
	if (firstDot != std::string::npos)
		r = r.substr(0, firstDot);

	return r;
}

//-----------------------------------------------------------------------------
// returns an absolute and canonical path with no '.' or '..': ie: the path is resolved.
// furthermore, there will be a trailing slash if there was one intially (see filePath())
//
std::string FileInfo::getCanonicalFilePath() const
{
	string r = mCache.mCanonicalFilePath;
	if (!isCached())
	{		
		r = Path::resolve(getAbsoluteFilePath());

		mCache.mCanonicalFilePath = r;
	}
	return r;	
}

//-----------------------------------------------------------------------------
// As cononicalFilePath() except, the filename will be removed and there will
// be no trailing slash.
//
std::string FileInfo::getCanonicalPath() const
{
	std::string path = getCanonicalFilePath();

	// if it's a file, we remove the filename.
	//
	path = removeFileName(path);
	path = removeTrailingSlash(path);
	
	return path;
}

//-----------------------------------------------------------------------------
// Returns the complete base name of the file without the path.
//
// The complete base name consists of all characters in the file up to(but not including) the last '.' character.
//
std::string FileInfo::getCompleteBaseName() const
{
	std::string r = getFilePath();
	size_t lastSlash = r.find_last_of(kNativeDirSeparator);
	if (lastSlash != std::string::npos)
		r = r.substr(lastSlash + 1);
	size_t lastDot = r.find_last_of(".");
	if (lastDot != std::string::npos)
		r = r.substr(0, lastDot);
	
	if (isDir())
	{
		r = "";
	}
	return r;
}

//-----------------------------------------------------------------------------

std::string FileInfo::getCompleteSuffix() const
{
	std::string path = getFilePath();
	size_t lastSlash = path.find_last_of(kNativeDirSeparator);
	if (lastSlash != std::string::npos)
		path = path.substr(lastSlash + 1);
	size_t firstDot = path.find_first_of(".");
	if (firstDot != std::string::npos)
		return path.substr(firstDot + 1);
	return "";
}

//-----------------------------------------------------------------------------

std::time_t FileInfo::getCreationTime() const
{
	time_t r = mCache.mCreationTime;
	if (!isCached())
	{
		struct stat localBuffer;
		if (stat(getCanonicalFilePath().c_str(), &localBuffer) == 0)
		{
            r = mCache.mCreationTime = localBuffer.st_ctime;
		}
	}
	return r;
}

//-----------------------------------------------------------------------------
// Returns the complete filename (with all suffixes)
//
std::string FileInfo::getFileName() const
{
	std::string r = getFilePath();
	r = extractFileName(r);	
	return r;
}

//-----------------------------------------------------------------------------

std::string FileInfo::getFilePath() const
{
	return mFilePath;
}

//-----------------------------------------------------------------------------

std::time_t FileInfo::getLastModificationTime() const
{
	time_t r = mCache.mLastModificationTime;
	if (!isCached())
	{
		struct stat localBuffer;
		if (stat(getCanonicalFilePath().c_str(), &localBuffer) == 0)
		{
            r = mCache.mLastModificationTime = localBuffer.st_mtime;
		}
	}
	return r;
}

//-----------------------------------------------------------------------------
// Returns the file's path. This doesn't include the file name.
//
// Note that, if this object is given a path ending in a slash, the name of
// the file is considered empty and this function will return the entire path.
//
std::string FileInfo::getPath() const
{
	std::string path = getFilePath();

	path = removeFileName(path);
	path = removeTrailingSlash(path);
	
	return path;
}

//-----------------------------------------------------------------------------
// returns file size in bytes
//
unsigned long long FileInfo::getSize() const
{
	unsigned long long r = mCache.mFileSize;
	if (!isCached())
	{
		struct stat localBuffer;
		if (stat(getCanonicalFilePath().c_str(), &localBuffer) == 0)
		{
            r = mCache.mFileSize = localBuffer.st_size;
		}		
	}
	return r;
}

//-----------------------------------------------------------------------------

std::string FileInfo::getSuffix() const
{
	std::string path = mFilePath;

	size_t lastSlash = path.find_last_of(kNativeDirSeparator);
	if (lastSlash != std::string::npos)
		path = path.substr(lastSlash + 1);
	size_t lastDot = path.find_last_of(".");
	if (lastDot != std::string::npos)
		return path.substr(lastDot + 1);
	return "";
}

//-----------------------------------------------------------------------------
// Returns the target of the simlink.
// If it is not a simlink, it will return it's own path.
//
std::string FileInfo::getSymlinkTarget() const
{
	string r = mCache.mSymlinkTarget;

	if (!isCached())
	{
		if (exists())
		{
			char target[FILENAME_MAX];

			DWORD flagsAttr = FILE_ATTRIBUTE_DIRECTORY | FILE_FLAG_BACKUP_SEMANTICS;
			HANDLE file = CreateFile(getCanonicalFilePath().c_str(), 0, FILE_SHARE_READ, NULL, OPEN_EXISTING, flagsAttr, NULL);
			if (file != INVALID_HANDLE_VALUE)
			{
				GetFinalPathNameByHandle(file, target, FILENAME_MAX, FILE_NAME_NORMALIZED);
				CloseHandle(file);
				r = mCache.mSymlinkTarget = Path::sanitize(target);
			}
		}
	}

	return r;
}

//-----------------------------------------------------------------------------

bool FileInfo::isAbsolute() const
{
	//match a single drive letter followed by a colon at the begining of the string
	const regex driveRe("^[a-zA-Z]{1}:");

	//match a single drive letter followed by a colon
	const regex networkriveRe("^\\\\");

	const bool isAbsoluteDrive = regex_search(getFilePath(), driveRe);
	const bool isAbsoluteNetwork = regex_search(getFilePath(), networkriveRe);

	return isAbsoluteDrive || isAbsoluteNetwork;
}

//-----------------------------------------------------------------------------

bool FileInfo::isCached() const
{
	return mCached;
}

//-----------------------------------------------------------------------------

bool FileInfo::isDir() const
{
	bool r = mCache.mIsDir;
	if (!isCached())
	{
		struct stat localBuffer;
		if (stat(getCanonicalFilePath().c_str(), &localBuffer) == 0)
		{
			r = mCache.mIsDir = ((localBuffer.st_mode & _S_IFDIR) > 0);
		}
	}
	return r;
}

//-----------------------------------------------------------------------------

bool FileInfo::isFile() const
{
	bool r = mCache.mIsFile;
	if (!isCached())
	{
		struct stat localBuffer;
		if (stat(getCanonicalFilePath().c_str(), &localBuffer) == 0)
		{
			r = mCache.mIsFile = ((localBuffer.st_mode & _S_IFREG) > 0);
		}
	}
	return r;
}

//-----------------------------------------------------------------------------

bool FileInfo::isRelative() const
{
	return (getFilePath().compare(getAbsoluteFilePath()) != 0);
}

//-----------------------------------------------------------------------------

bool FileInfo::isRoot() const
{
	std::string path = getCanonicalPath();

	return isAbsolute() && path.size() > 0 &&
		(path[path.size() - 1] == ':' || path.find_last_of("\\") == 1);
}

//-----------------------------------------------------------------------------

bool FileInfo::isSymlink() const
{
	bool r = mCache.mIsSymlink;
	if (!isCached())
	{
		const string cfp = getCanonicalFilePath();
		if (exists())
		{
			r = mCache.mIsSymlink = ((GetFileAttributes(cfp.c_str()) & FILE_ATTRIBUTE_REPARSE_POINT) > 0);
		}
	}
	return r;
}

//-----------------------------------------------------------------------------
// Converts the file's path to an absolute path if it is not already in that
// form. Returns true to indicate that the path was converted; otherwise
// returns false to indicate that the path was already absolute.
bool FileInfo::makeAbsolute()
{
	bool r = false;
	if (!isAbsolute())
	{
		mFilePath = getAbsoluteFilePath();
		r = true;
	}
	return r;
}

//-----------------------------------------------------------------------------

void FileInfo::refresh()
{
	// early out
	if (!isCached()) { return; }

	// temporarily remove cached flag so we can call methods
	// to recompute the cached value.
	mCached = false;
	mCache.clear();

	getCanonicalFilePath();
	if (exists())
	{
		isSymlink();
		isDir();
		isFile();
		getSize();
		getCreationTime();
		getLastModificationTime();
		getSymlinkTarget();
	}

	mCached = true;
}

//-----------------------------------------------------------------------------

string FileInfo::removeFileName(const std::string& iPath) const
{
	string r = iPath;
	size_t lastSlash = iPath.find_last_of(kNativeDirSeparator);

	// is in the string and not the trailing slash itself
	// remove file name and leave trailing slash.
	//
	if (lastSlash != std::string::npos && lastSlash < iPath.size() - 1)
		r = iPath.substr(0, lastSlash + 1);
	return r;
}

//-----------------------------------------------------------------------------

string FileInfo::removeTrailingSlash(const string& iPath) const
{
	string r = iPath;

	// make sure there is no trailling directory separator.
	// unless this is a network path starting with '\\'
	//
	size_t lastSlash = iPath.find_last_of(kNativeDirSeparator);
	if (lastSlash != std::string::npos && 
		lastSlash > 1 && //prevent removing tailing slash from network path
		lastSlash == iPath.size() - 1)
		r = iPath.substr(0, lastSlash);

	return r;
}

//-----------------------------------------------------------------------------

void FileInfo::setCached(bool cached)
{
	if (cached != isCached())
	{
		mCached = cached;
		if (mCached)
			refresh();
	}
}

//-----------------------------------------------------------------------------

void FileInfo::setFile(const std::string & filePath)
{
    mFilePath = Path::sanitize(filePath);
	refresh();
}

//-----------------------------------------------------------------------------

void FileInfo::setFile(const std::string & dirpath, const std::string & filename)
{
	std::string path = Path::join(dirpath, filename);
	setFile(path);
}


//-----------------------------------------------------------------------------
//--- FileInfo::Cache
//-----------------------------------------------------------------------------
FileInfo::Cache::Cache()
{
	clear();
}

//-----------------------------------------------------------------------------
void FileInfo::Cache::clear()	
{
	mCanonicalFilePath = "";
	mExists = false;
	mIsSymlink = false;
	mIsDir = false;
	mIsFile = false;
	mFileSize = 0;
	mCreationTime = 0;
	mLastModificationTime = 0;
	mSymlinkTarget = "";
}