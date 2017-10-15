
#pragma once

#include <vector>

namespace Realisim
{
namespace Core
{
    std::string fromVector(std::vector<std::string>& iInput, char iSeparator);
    std::vector<std::string> toVector(const std::string& iInput, char iSeparator);
	std::vector<std::string> toVector(const std::string& iInput, const std::string& iSeparatorList);
}
}