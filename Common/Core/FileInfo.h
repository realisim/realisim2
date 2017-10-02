

#pragma once

#include <ctime>
#include <string>

namespace Realisim
{
namespace Core
{

	// explain c:\\windows\\system32\\mspaint.exe
	// explain c:\\windows/system32\\mspaint.exe
	// filePath() is sanitized, all method return sanitized path.
	// all methods, except filePath(), return resolved path.

	// who/how resolved path...
	// mFilePath and filepath() always returns a sanitized path.
	//
	// Note: using this class requires linkage to Shlwapi on windows
	//
	class FileInfo
	{
	public:
		FileInfo();
		FileInfo(const std::string &filePath);
		FileInfo(const FileInfo &fileinfo) = default;
		FileInfo& operator=(const FileInfo &fileinfo) = default;
		~FileInfo();

		bool exists() const;
        std::string getAbsoluteFilePath() const;
        std::string getAbsolutePath() const;
        std::string getBaseName() const;
        std::string getCanonicalFilePath() const;
        std::string getCanonicalPath() const;
        std::string getCompleteBaseName() const;
        std::string getCompleteSuffix() const;
        std::time_t getCreationTime() const;
        std::string getFileName() const;
        std::string getFilePath() const;
        std::time_t getLastModificationTime() const;
        std::string getPath() const;
        unsigned long long getSize() const;
        std::string getSuffix() const;
        std::string getSymlinkTarget() const;
		bool isAbsolute() const;
		bool isCached() const;
		bool isDir() const;
		bool isFile() const;
		bool isRelative() const;
		bool isRoot() const;
		bool isSymlink() const;
		bool makeAbsolute();
		void refresh();
		void setCached(bool cached);
		void setFile(const std::string &filePath);
		void setFile(const std::string &dirpath, const std::string &filename);

	protected:
		std::string extractFileName(const std::string&) const;
		std::string removeFileName(const std::string&) const;
		std::string removeTrailingSlash(const std::string&) const;

		//--- data
		struct Cache
		{			
			Cache();
			Cache(const Cache&) = default;
			Cache& operator=(const Cache&) = default;
			~Cache() = default;
			void clear();

			std::string mCanonicalFilePath;
			bool mExists;
			bool mIsSymlink;
			bool mIsDir;
			bool mIsFile;
			unsigned long long mFileSize;
			std::time_t mCreationTime;
			std::time_t mLastModificationTime;
			std::string mSymlinkTarget;
		};

		std::string mFilePath;
		bool mCached;
		mutable Cache mCache;
	};
}
}
	