
#include "BinaryVisitor.h"


//---------------------------------------------------------------------------------------------------------------------
// BinaryWriterVisitor
//---------------------------------------------------------------------------------------------------------------------
BinaryWriterVisitor::BinaryWriterVisitor() {}

//---------------------------------------------------------------------------------------------------------------------
BinaryWriterVisitor::~BinaryWriterVisitor() {}

//---------------------------------------------------------------------------------------------------------------------
void BinaryWriterVisitor::Name(const std::string& iName)
{
    (void)iName;
}

//---------------------------------------------------------------------------------------------------------------------
void BinaryWriterVisitor::field(const std::string& iName, uint8_t& iV)
{
    (void)iName;
    writeInteger(iV);
}

//---------------------------------------------------------------------------------------------------------------------
void BinaryWriterVisitor::field(const std::string& iName, uint16_t& iV)
{
    (void)iName;
    writeInteger(iV);
}

//---------------------------------------------------------------------------------------------------------------------
void BinaryWriterVisitor::field(const std::string& iName, uint32_t& iV)
{
    (void)iName;
    writeInteger(iV);
}

//---------------------------------------------------------------------------------------------------------------------
void BinaryWriterVisitor::field(const std::string& iName, uint64_t& iV)
{
    (void)iName;
    writeInteger(iV);
}


//---------------------------------------------------------------------------------------------------------------------
void BinaryWriterVisitor::field(const std::string& iName, std::vector<uint32_t>& iV)
{
    (void)iName;
    writeInteger((uint32_t)iV.size());
    for (auto& v : iV) {
        writeInteger(v);
    }
}


//---------------------------------------------------------------------------------------------------------------------
// BinaryReaderVisitor
//---------------------------------------------------------------------------------------------------------------------
BinaryReaderVisitor::BinaryReaderVisitor() {}

//---------------------------------------------------------------------------------------------------------------------
BinaryReaderVisitor::~BinaryReaderVisitor() {}

//---------------------------------------------------------------------------------------------------------------------
void BinaryReaderVisitor::Name(const std::string& iName)
{
    (void)iName;
}

//---------------------------------------------------------------------------------------------------------------------
void BinaryReaderVisitor::field(const std::string& iName, uint8_t& iV)
{
    (void)iName;
    readInteger(iV);
}

//---------------------------------------------------------------------------------------------------------------------
void BinaryReaderVisitor::field(const std::string& iName, uint16_t& iV)
{
    (void)iName;
    readInteger(iV);
}

//---------------------------------------------------------------------------------------------------------------------
void BinaryReaderVisitor::field(const std::string& iName, uint32_t& iV)
{
    (void)iName;
    readInteger(iV);
}

//---------------------------------------------------------------------------------------------------------------------
void BinaryReaderVisitor::field(const std::string& iName, uint64_t& iV)
{
    (void)iName;
    readInteger(iV);
}


//---------------------------------------------------------------------------------------------------------------------
void BinaryReaderVisitor::field(const std::string& iName, std::vector<uint32_t>& iV)
{
    (void)iName;
    uint32_t size = 0;
    readInteger(size);

    uint32_t v = 0;
    iV.resize(size);
    for (size_t i = 0; i < size; ++i) {
        readInteger(v);
        iV[i] = v;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void BinaryReaderVisitor::setBinaryPayload(const std::string& iPayload)
{
    mIss = std::istringstream(iPayload, std::ios_base::binary);
}