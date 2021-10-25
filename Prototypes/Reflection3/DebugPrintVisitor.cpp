
#include "DebugPrintVisitor.h"


//---------------------------------------------------------------------------------------------------------------------
DebugPrintVisitor::DebugPrintVisitor() {}

//---------------------------------------------------------------------------------------------------------------------
DebugPrintVisitor::~DebugPrintVisitor() {}

//---------------------------------------------------------------------------------------------------------------------
void DebugPrintVisitor::Name(const std::string& iName)
{
    mOss << "Name: " << iName << "\n";
}

//---------------------------------------------------------------------------------------------------------------------
void DebugPrintVisitor::field(const std::string& iName, uint8_t& iV)
{
    mOss << "field: " << iName << "type: uint8_t" << "Value: " << std::to_string(iV) << "\n";
}

//---------------------------------------------------------------------------------------------------------------------
void DebugPrintVisitor::field(const std::string& iName, uint16_t& iV)
{
    mOss << "field: " << iName << "type: uint16_t" << "Value: " << std::to_string(iV) << "\n";
}

//---------------------------------------------------------------------------------------------------------------------
void DebugPrintVisitor::field(const std::string& iName, uint32_t& iV)
{
    mOss << "field: " << iName << "type: uint32_t" << "Value: " << std::to_string(iV) << "\n";
}

//---------------------------------------------------------------------------------------------------------------------
void DebugPrintVisitor::field(const std::string& iName, uint64_t& iV)
{
    mOss << "field: " << iName << "type: uint64_t" << "Value: " << std::to_string(iV) << "\n";
}


//---------------------------------------------------------------------------------------------------------------------
void DebugPrintVisitor::field(const std::string& iName, std::vector<uint32_t>& iValues)
{
    mOss << "field: " << iName << "\n";
    mOss << "    values [";

    if (!iValues.empty())
    {
        for (auto& v : iValues) {
            mOss << std::to_string(v) << ",";
        }
        mOss.seekp(-1, std::ios_base::end);
    }
    
    mOss << "]\n";
}