
#pragma once

#include "IReflectable.h"
#include <sstream>
#include <vector>

class DebugPrintVisitor {

public:
    DebugPrintVisitor();
    ~DebugPrintVisitor();

    void clear() {
        mOss.str(std::string());
    }

    template<typename Described>
    void visit(Described& iDescribed) {
        iDescribed.describe(*this);
    }

    void Name(const std::string& iName);

    void field(const std::string& iName, uint8_t& iV);
    void field(const std::string& iName, uint16_t& iV);
    void field(const std::string& iName, uint32_t& iV);
    void field(const std::string& iName, uint64_t& iV);

    void field(const std::string& iName, std::vector<uint32_t>& iValues);


    // catch all IReflectable childs
    template <class T, typename std::enable_if<std::is_base_of<IReflectable, T>::value, int>::type = 0>
    void field(const std::string& iName, T& iV)
    {
        mOss << "Field class IReflectable: " << iName << "\n";
        visit(iV);
    }

    // catch all std::vector of all derived from IReflectable
    // std::vector<IReflectable&>...
    template<typename T,
        typename std::enable_if<std::is_base_of<IReflectable, T>::value, int>::type = 0 >
        void field(const std::string& iName, std::vector<T>& iValues) {

        mOss << "Field  std::vector<IReflectable>: " << iName << "\n";
        mOss << "values [";
        for (auto& v : iValues) {
            visit(v);
        }
        mOss << "]\n";
    }

    std::string getString() const { return mOss.str(); }

protected:
    std::ostringstream mOss;
};

//----------------------------