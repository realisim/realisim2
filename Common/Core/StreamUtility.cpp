
#include "StreamUtility.h"
#include <fstream>

using namespace std;
using namespace Realisim;
using namespace Core;

namespace
{
    const StreamUtility::endianness __localByteOrder = StreamUtility::eLittleEndian; //Windows machine are little endian...
}

//-------------------------------------------------------------------------
StreamUtility::endianness StreamUtility::getLocalMachineByteOrder()
{
    return __localByteOrder;
}

//-------------------------------------------------------------------------
bool StreamUtility::needsSwapping() const
{
    return __localByteOrder != getStreamFormat();
}

//-------------------------------------------------------------------------
// read iNumberOfBytesToRead into oV.
// oV must be preallocated at the correct size.
//
bool StreamUtility::readBytes(std::istream& iStream, size_t iNumberOfBytesToRead, char *oV)
{
    iStream.read(oV, iNumberOfBytesToRead);
    return iStream.good();
}

//-------------------------------------------------------------------------
bool StreamUtility::readBytes(std::istream& iStream, size_t iNumberOfBytesToRead, std::string *oV)
{
    oV->resize(iNumberOfBytesToRead);
    iStream.read(&(*oV)[0], iNumberOfBytesToRead);
    
    return iStream.good();
}

//-------------------------------------------------------------------------
bool StreamUtility::readChar(std::istream& iStream, size_t iNumberOfCharToRead, std::string *oV)
{
    char *c = new char[iNumberOfCharToRead];
    iStream.read(c, iNumberOfCharToRead);

    //no swap involved in reading chars...

    *oV = string(c);
    delete[] c;

    return iStream.good();
}

//-------------------------------------------------------------------------
bool StreamUtility::readDouble(std::istream& iStream, double *oV)
{
    iStream.read( (char*)oV, sizeof(double) );

    if(needsSwapping()){ swapBytes8((void*)oV); }

    return iStream.good();
}

//-------------------------------------------------------------------------
bool StreamUtility::readFloat32(std::istream& iStream, float *oV)
{
    iStream.read( (char*)oV, sizeof(float) );
    
    if(needsSwapping()){ swapBytes4((void*)oV); }
    
    return iStream.good();
}

//-------------------------------------------------------------------------
bool StreamUtility::readInt8(istream& iStream, int8_t *oV)
{
    iStream.read( (char*)oV, sizeof(int8_t) );
    return iStream.good();
}

//-------------------------------------------------------------------------
bool StreamUtility::readInt16(istream& iStream, int16_t *oV)
{
    iStream.read( (char*)oV, sizeof(int16_t) );

    if(needsSwapping()){ swapBytes2((void*)oV); }

    return iStream.good();
}

//-------------------------------------------------------------------------
bool StreamUtility::readInt32(std::istream& iStream, int32_t *oV)
{
    iStream.read( (char*)oV, sizeof(int32_t) );

    if(needsSwapping()){ swapBytes4((void*)oV); }

    return iStream.good();
}

//-------------------------------------------------------------------------
bool StreamUtility::readInt64(std::istream& iStream, int64_t *oV)
{
    iStream.read((char*)oV, sizeof(int64_t));

    if (needsSwapping()) { swapBytes8((void*)oV); }

    return iStream.good();
}

//-------------------------------------------------------------------------
bool StreamUtility::readUint8(std::istream& iStream, uint8_t *oV)
{
    iStream.read( (char*)oV, sizeof(uint8_t) );
    return iStream.good();
}

//-------------------------------------------------------------------------
bool StreamUtility::readUint16(istream& iStream, uint16_t *oV)
{
    iStream.read( (char*)oV, sizeof(uint16_t) );

    if(needsSwapping()){ swapBytes2((void*)oV); }

    return iStream.good();
}

//-------------------------------------------------------------------------
bool StreamUtility::readUint32(istream& iStream, uint32_t *oV)
{
    iStream.read( (char*)oV, sizeof(uint32_t) );
    
    if(needsSwapping()){ swapBytes4((void*)oV); }
    
    return iStream.good();
}

//-------------------------------------------------------------------------
bool StreamUtility::readUint64(istream& iStream, uint64_t *oV)
{
    iStream.read((char*)oV, sizeof(uint64_t));

    if (needsSwapping()) { swapBytes8((void*)oV); }

    return iStream.good();
}

//-------------------------------------------------------------------------
void StreamUtility::setStreamFormat(endianness iV)
{ mStreamFormat = iV; }

//-------------------------------------------------------------------------
void StreamUtility::swapBytes2(void* iV)
{ 
    char in[2], out[2];
    memcpy(in, iV, 2);
    out[0]  = in[1];
    out[1]  = in[0];
    memcpy(iV, out, 2);
}

//-------------------------------------------------------------------------
void StreamUtility::swapBytes4(void* iV)
{ 
    char in[4], out[4];
    memcpy(in, iV, 4);
    out[0]  = in[3];
    out[1]  = in[2];
    out[2]  = in[1];
    out[3]  = in[0];
    memcpy(iV, out, 4);
}

//-------------------------------------------------------------------------
void StreamUtility::swapBytes8(void* iV)
{
    char in[8], out[8];
    memcpy(in, iV, 8);
    out[0]  = in[7];
    out[1]  = in[6];
    out[2]  = in[5];
    out[3]  = in[4];
    out[4]  = in[3];
    out[5]  = in[2];
    out[6]  = in[1];
    out[7]  = in[0];
    memcpy(iV, out, 8);
}

//-------------------------------------------------------------------------
void StreamUtility::write(std::ostream& iStream, ByteArray iV)
{
    iStream.write(iV.constData(), iV.size());
}

//-------------------------------------------------------------------------
void StreamUtility::write(std::ostream& iStream, int8_t iV)
{
    iStream.write((char*)&iV, sizeof(int8_t));
}

//-------------------------------------------------------------------------
void StreamUtility::write(std::ostream& iStream, int16_t iV)
{
    if (needsSwapping()) { swapBytes2((void*)&iV); }
    iStream.write((char*)&iV, sizeof(int16_t));
}

//-------------------------------------------------------------------------
void StreamUtility::write(std::ostream& iStream, int32_t iV)
{
    if (needsSwapping()) { swapBytes4((void*)&iV); }
    iStream.write((char*)&iV, sizeof(int32_t));
}

//-------------------------------------------------------------------------
void StreamUtility::write(std::ostream& iStream, int64_t iV)
{
    if (needsSwapping()) { swapBytes8((void*)&iV); }
    iStream.write((char*)&iV, sizeof(int64_t));
}

//-------------------------------------------------------------------------
void StreamUtility::write(std::ostream& iStream, uint8_t iV)
{
    iStream.write((char*)&iV, sizeof(uint8_t));
}

//-------------------------------------------------------------------------
void StreamUtility::write(std::ostream& iStream, uint16_t iV)
{
    if (needsSwapping()) { swapBytes2((void*)&iV); }
    iStream.write((char*)&iV, sizeof(uint16_t));
}

//-------------------------------------------------------------------------
void StreamUtility::write(std::ostream& iStream, uint32_t iV)
{
    if (needsSwapping()) { swapBytes4((void*)&iV); }
    iStream.write((char*)&iV, sizeof(uint32_t));
}

//-------------------------------------------------------------------------
void StreamUtility::write(std::ostream& iStream, uint64_t iV)
{
    if (needsSwapping()) { swapBytes8((void*)&iV); }
    iStream.write((char*)&iV, sizeof(uint64_t));
}

//-------------------------------------------------------------------------
void StreamUtility::write(std::ostream& iStream, float iV)
{
    if (needsSwapping()) { swapBytes4((void*)&iV); }
    iStream.write((char*)&iV, sizeof(float));
}

//-------------------------------------------------------------------------
void StreamUtility::write(std::ostream& iStream, double iV)
{
    if (needsSwapping()) { swapBytes8((void*)&iV); }
    iStream.write((char*)&iV, sizeof(double));
}

//-------------------------------------------------------------------------
void StreamUtility::write(std::ostream& iStream, size_t iNumberOfBytesToWrite, const char *ipData)
{
    iStream.write(ipData, iNumberOfBytesToWrite);
}

//-------------------------------------------------------------------------
// This method will write iData as is, bytes by bytes. see writeChar to
// write null terminated strings.
//
void StreamUtility::writeBytes(std::ostream& iStream, const std::string& iData)
{
    iStream.write(iData.c_str(), iData.size());
}

//-------------------------------------------------------------------------
// This method will write iData as if it was a null terminated string.
//
void StreamUtility::writeChar(std::ostream& iStream, const std::string& iData)
{
    //+1 to account for the \0 of .c_str()
    iStream.write(iData.c_str(), iData.size()+1);
}

//-------------------------------------------------------------------------
// utility function
//-------------------------------------------------------------------------
namespace Realisim
{
    namespace Core
    {

        std::istringstream makeIStringStream(ByteArray iBa)
        {
            return istringstream(iBa.constString(), stringstream::binary);
        }

        ByteArray readFromFile(const std::string& iFilenamePath)
        {
            ByteArray r;
            ifstream ifs;
            ifs.open(iFilenamePath, ios::in | ios::binary);
            if (ifs.is_open())
            {
                std::streampos fsize = 0;
                fsize = ifs.tellg();
                ifs.seekg(0, ios::end);
                fsize = ifs.tellg() - fsize;
                ifs.seekg(0, ios::beg);

                StreamUtility su;
                su.setStreamFormat(StreamUtility::eLittleEndian);
                su.readBytes(ifs, fsize, &r.nonConstString());

                ifs.close();
            }
            return r;
        }

        bool writeToFile(ByteArray iBa, const std::string& iFilenamePath)
        {
            bool r = false;
            ofstream ofs;
            ofs.open(iFilenamePath, ios::out | ios::binary | ios::trunc);
            if (ofs.is_open())
            {
                StreamUtility su;
                su.setStreamFormat(StreamUtility::eLittleEndian);
                su.write(ofs, iBa);
                ofs.flush();
                ofs.close();
            }
            return r;
        }

    }
}