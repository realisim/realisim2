
#pragma once

#include "Core/ByteArray.h"
#include <istream>
#include <ostream>
#include <sstream>
#include <stdint.h>
#include <string>

namespace Realisim
{
namespace Core
{
    class StreamUtility
    {
    public:
        StreamUtility() = default;
        StreamUtility(const StreamUtility&) = delete;
        StreamUtility& operator=(const StreamUtility&) = delete;
        ~StreamUtility() = default;
        
        enum endianness{ eBigEndian=0, eLittleEndian };
        
        static endianness getLocalMachineByteOrder();
        endianness getStreamFormat() const {return mStreamFormat;}
        bool readBytes(std::istream& iStream, size_t iNumberOfBytesToRead, char *oV);
        bool readBytes(std::istream& iStream, size_t iNumberOfBytesToRead, std::string *oV);
        bool readChar(std::istream& iStream, size_t iNumberOfCharToRead, std::string *oV);
        bool readDouble(std::istream& iStream, double *oV);
        bool readFloat32(std::istream& iStream, float *oV);
        bool readInt8(std::istream& iStream, int8_t *oV);
        bool readInt16(std::istream& iStream, int16_t *oV);
        bool readInt32(std::istream& iStream, int32_t *oV);
        bool readInt64(std::istream& iStream, int64_t *oV);
        bool readUint8(std::istream& iStream, uint8_t *oV);
        bool readUint16(std::istream& iStream, uint16_t *oV);
        bool readUint32(std::istream& iStream, uint32_t *oV);
        bool readUint64(std::istream& iStream, uint64_t *oV);

        void setStreamFormat(endianness);

        void write(std::ostream& iStream, ByteArray);
        void write(std::ostream& iStream, int8_t);
        void write(std::ostream& iStream, int16_t);
        void write(std::ostream& iStream, int32_t);
        void write(std::ostream& iStream, int64_t);
        void write(std::ostream& iStream, uint8_t);
        void write(std::ostream& iStream, uint16_t);
        void write(std::ostream& iStream, uint32_t);
        void write(std::ostream& iStream, uint64_t);
        void write(std::ostream& iStream, float);
        void write(std::ostream& iStream, double);
        void write(std::ostream& iStream, size_t iNumberOfBytesToWrite, const char *ipData);
        void writeBytes(std::ostream& iStream, const std::string& iData);
        void writeChar(std::ostream& iStream, const std::string& iData);
        
    protected:
        bool needsSwapping() const;
        void swapBytes2(void* iV);
        void swapBytes4(void* iV);
        void swapBytes8(void* iV);
        
        endianness mStreamFormat;
    };

    //-------------------------------------------------------------------------
    // utility function
    //-------------------------------------------------------------------------
	std::istringstream makeIStringStream(ByteArray);
    ByteArray readFromFile(const std::string& iFilenamePath);
    bool writeToFile(ByteArray iBa, const std::string& iFilenamePath);
}
}