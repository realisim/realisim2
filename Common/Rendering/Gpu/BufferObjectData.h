#pragma once

#include <vector>
#include <memory>

#include "DataType.h"

namespace Realisim
{
    namespace Rendering
    {
        //******************************************************
        // BufferObjectData is an abstract class to handle buffer object data structure on the GPU
        // You must use one of the 2 children class and give a pointer of type BufferObjectData to a BufferObject instance
        // 
        // You add attributes to a structure definition by calling addAttribute()
        //
        // BufferObjectDataInterleaved handles complex data structure, mixing different data type, calling addAttribute() multiple times will add attributes to the data structure
        // BufferObjectDataPlain handles a data made of only one data type, calling addAttribute() multiple times will overwrite the current data structure
        // 
        // When you finished to add attributes, you must call computeOffset() to finish the setup
        //******************************************************
        class BufferObjectData
        {
        public:
            struct InterleaveData
            {
                InterleaveData();
                InterleaveData(DataType data,int iNumberOfComponents,int iLayoutLocation);
                InterleaveData(const InterleaveData&) = default;
                InterleaveData& operator=(const InterleaveData&) = default;
                ~InterleaveData() = default;

                DataType mDataType;
                int mNumberOfComponents;
                int mOffsetInBytes;
                int mLayoutLocationIndex;
            };

            BufferObjectData();
            virtual ~BufferObjectData();

            virtual void addAttribute(DataType mDataType,int mNumberOfComponents,int mLayoutLocationIndex) = 0;

            virtual DataType getDataType(int iIndex) const = 0;

            virtual int getLayoutLocationIndex(int) const = 0;
            virtual int getNumberOfAttributes() const = 0;
            virtual int getNumberOfComponents(int) const = 0;            
            virtual int getOffsetInBytes(int) const = 0;
            virtual int getSizeInBytes() const = 0;

            virtual int pad(int alignement);

        protected:
            virtual void computeOffset() = 0;
        };

        class BufferObjectDataInterleaved : public BufferObjectData
        {        
        public:
            BufferObjectDataInterleaved();
            virtual ~BufferObjectDataInterleaved();

            virtual void addAttribute(DataType mDataType, int mNumberOfComponents, int mLayoutLocationIndex);
            
            virtual DataType getDataType(int iIndex) const final;
            virtual int getLayoutLocationIndex(int iIndex) const final;
            virtual int getNumberOfAttributes() const final;
            virtual int getNumberOfComponents(int iIndex) const final;
            virtual int getOffsetInBytes(int iIndex) const final;
            virtual int getSizeInBytes() const final;
        protected:
            std::vector<BufferObjectData::InterleaveData>  mInterleaveData;
            virtual void computeOffset() final;
        };

        class BufferObjectDataPlain : public BufferObjectData
        {
        public:
            BufferObjectDataPlain();

            virtual ~BufferObjectDataPlain();

            virtual void addAttribute(DataType mDataType, int mNumberOfComponents, int mLayoutLocationIndex);

            virtual DataType getDataType(int iIndex) const final;
            virtual int getLayoutLocationIndex(int iIndex) const final;
            virtual int getNumberOfAttributes() const final;
            virtual int getNumberOfComponents(int iIndex) const final;
            virtual int getOffsetInBytes(int iIndex) const final;
            virtual int getSizeInBytes() const final;
        protected:
            BufferObjectData::InterleaveData mData;
            virtual void computeOffset() final;
        };
    }
}
