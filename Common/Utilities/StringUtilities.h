
#pragma once

#include <vector>

namespace Realisim
{
namespace Utilities
{
    std::string fromVector(std::vector<std::string>& iInput, char iSeparator);
    int toInt(const std::string& iInput, bool *oSucess = nullptr);
    unsigned int toUnsignedInt(const std::string& iInput, bool *oSucess = nullptr);
    std::string toString(int iV);
    std::string toString(unsigned int iV);
    std::vector<std::string> toVector(const std::string& iInput, char iSeparator);
	std::vector<std::string> toVector(const std::string& iInput, const std::string& iSeparatorList);
}
}