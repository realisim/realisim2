/*
 * Path.h
 */

#pragma once 

#include <string>

namespace Realisim
{
namespace Core
{
	class Path
	{
	public:
        static std::string getApplicationFilePath();
		static std::string getCurrentWorkingDirectory();
		static std::string join(const std::string& iPath0, const std::string& iPath1);
		static std::string resolve(const std::string & path);
		static std::string sanitize(const std::string & path);
		static void setCurrentWorkingDirectory(const std::string & path);
	};
}
}


