
#pragma once

#include "IReflectable.h"
#include <sstream>
#include <vector>

//---------------------------------------------------------------------------------------------------------------------
// BinaryWriterVisitor
//---------------------------------------------------------------------------------------------------------------------
class BinaryWriterVisitor {

public:
    BinaryWriterVisitor();
    ~BinaryWriterVisitor();

    template<typename Described>
    void visit(Described& iDescribed) {
        mOss.str().clear();

        iDescribed.describe(*this);
    }

    void Name(const std::string& iName);

    void field(const std::string& iName, uint8_t& iV);
    void field(const std::string& iName, uint16_t& iV);
    void field(const std::string& iName, uint32_t& iV);
    void field(const std::string& iName, uint64_t& iV);

    void field(const std::string& iName, std::vector<uint32_t>& iV);

    // catch all IReflectable childs
    template <class T, typename std::enable_if<std::is_base_of<IReflectable, T>::value, int>::type = 0>
    void field(const std::string& iName, T& iV)
    {
        (void)iName;
        visit(iV);
    }

    // catch all std::vector of all derived from IReflectable
    // std::vector<IReflectable&>...
    template<typename T,
        typename std::enable_if<std::is_base_of<IReflectable, T>::value, int>::type = 0 >
        void field(const std::string& iName, std::vector<T>& iValues) {
        (void)iName;
        
        // write number of elements
        writeInteger((uint32_t)iValues.size());
        for (auto& v : iValues) {
            visit(v);
        }
    }

    std::string getString() const { return mOss.str(); }

protected:
    template<typename T> void writeInteger(const T& iV);

    std::ostringstream mOss;
};

//----------------------------
template<typename T> 
void BinaryWriterVisitor::writeInteger(const T& iV) {
    mOss.write((char*)&iV, sizeof(T));
}

//---------------------------------------------------------------------------------------------------------------------
// BinaryReaderVisitor
//---------------------------------------------------------------------------------------------------------------------
class BinaryReaderVisitor {

public:
    BinaryReaderVisitor();
    ~BinaryReaderVisitor();

    template<typename Described>
    void visit(Described& iDescribed) {
        iDescribed.describe(*this);
    }

    void Name(const std::string& iName);

    void field(const std::string& iName, uint8_t& iV);
    void field(const std::string& iName, uint16_t& iV);
    void field(const std::string& iName, uint32_t& iV);
    void field(const std::string& iName, uint64_t& iV);

    void field(const std::string& iName, std::vector<uint32_t>& iV);

    // catch all IReflectable childs
    template <class T, typename std::enable_if<std::is_base_of<IReflectable, T>::value, int>::type = 0>
    void field(const std::string& iName, T& iV)
    {
        (void)iName;
        visit(iV);
    }

    // catch all std::vector of all derived from IReflectable
    // std::vector<IReflectable&>...
    template<typename T,
        typename std::enable_if<std::is_base_of<IReflectable, T>::value, int>::type = 0 >
        void field(const std::string& iName, std::vector<T>& iValues) {
        (void)iName;
        // readnumber of elements
        uint32_t size = 0;
        readInteger(size);

        iValues.resize(size);
        for (auto& v : iValues) {
            visit(v);
        }
    }

    void setBinaryPayload(const std::string& iPayload);

protected:
    template<typename T> void readInteger(const T& iV);

    std::istringstream mIss;
};

//----------------------------
template<typename T>
void BinaryReaderVisitor::readInteger(const T& iV) {
    mIss.read((char*)&iV, sizeof(T));
}