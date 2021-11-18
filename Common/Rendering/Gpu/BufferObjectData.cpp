
#include "BufferObjectData.h"
#include <cmath>

using namespace Realisim;
using namespace Rendering;

//----------------------------------------------------------------------------------
#pragma region InterleaveData
BufferObjectData::InterleaveData::InterleaveData() :
    mDataType(DataType::dtUndefined),
    mNumberOfComponents(0),
    mOffsetInBytes(0),
    mLayoutLocationIndex(0)
{
}

BufferObjectData::InterleaveData::InterleaveData(DataType data,
    int iSize,
    int iLayoutLocation) :
    mDataType(data),
    mNumberOfComponents(iSize),
    mOffsetInBytes(0),
    mLayoutLocationIndex(iLayoutLocation)
{
}
#pragma endregion
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
#pragma region BufferObjectData
BufferObjectData::BufferObjectData()
{
}

BufferObjectData::~BufferObjectData()
{
}

int BufferObjectData::pad(int alignement)
{
    int size = getSizeInBytes();
    int n = (int)std::ceil((float)size/(float)alignement);

    int padding = (n*alignement - size);

    // we need padding, so lets add a padding infos in the data structure
    if(padding)
        this->addAttribute(DataType::dtUnsignedByte,padding,-1);

    return padding;
}
#pragma endregion
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
#pragma region BufferObjectDataInterleaved
BufferObjectDataInterleaved::BufferObjectDataInterleaved()
    : BufferObjectData()
{
}

BufferObjectDataInterleaved::~BufferObjectDataInterleaved()
{
}

void BufferObjectDataInterleaved::addAttribute(DataType mDataType,
    int mNumberOfComponents,
    int mLayoutLocationIndex)
{
    BufferObjectData::InterleaveData data;
    data.mDataType = mDataType;
    data.mNumberOfComponents = mNumberOfComponents;
    data.mLayoutLocationIndex = mLayoutLocationIndex;

    mInterleaveData.push_back(data);

    this->computeOffset();
}

void BufferObjectDataInterleaved::computeOffset()
{
    int offset = 0;
    for(auto& attrib : mInterleaveData)
    {
        attrib.mOffsetInBytes = offset;
        offset += attrib.mNumberOfComponents*getSizeOf(attrib.mDataType);
    }
}

int BufferObjectDataInterleaved::getNumberOfAttributes() const
{
    return (int)mInterleaveData.size();
}

int BufferObjectDataInterleaved::getLayoutLocationIndex(int iIndex) const
{
    int r = -1;
    if(iIndex >= 0 && iIndex < (int)mInterleaveData.size())
    {
        r = mInterleaveData[iIndex].mLayoutLocationIndex;
    }
    return r;
}
int BufferObjectDataInterleaved::getOffsetInBytes(int iIndex) const
{
    int r = 0;
    if(iIndex >= 0 && iIndex < (int)mInterleaveData.size())
    {
        r = mInterleaveData[iIndex].mOffsetInBytes;
    }
    return r;
}

int BufferObjectDataInterleaved::getNumberOfComponents(int iIndex) const
{
    int r = 1;
    if(iIndex >= 0 && iIndex < (int)mInterleaveData.size())
    {
        r = mInterleaveData[iIndex].mNumberOfComponents;
    }
    return r;
}
DataType BufferObjectDataInterleaved::getDataType(int iIndex) const
{
    DataType r = DataType::dtUndefined;
    if(iIndex >= 0 && iIndex < (int)mInterleaveData.size())
    {
        r = mInterleaveData[iIndex].mDataType;
    }
    return r;
}

int BufferObjectDataInterleaved::getSizeInBytes() const
{
    int r = 0;
    for(size_t i = 0; i < mInterleaveData.size(); ++i)
    {
        r += mInterleaveData[i].mNumberOfComponents*getSizeOf(mInterleaveData[i].mDataType);
    }

    return r;
}
#pragma endregion
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
#pragma region BufferObjectDataPlain
BufferObjectDataPlain::BufferObjectDataPlain()
{
}

BufferObjectDataPlain::~BufferObjectDataPlain()
{
}

int BufferObjectDataPlain::getSizeInBytes() const
{
    return mData.mNumberOfComponents*getSizeOf(mData.mDataType);
}

int BufferObjectDataPlain::getNumberOfAttributes() const
{
    return 1;
}

void BufferObjectDataPlain::addAttribute(
    DataType mDataType,
    int mNumberOfComponents,
    int mLayoutLocationIndex)
{
    mData.mDataType = mDataType;
    mData.mNumberOfComponents = mNumberOfComponents;
    mData.mLayoutLocationIndex = mLayoutLocationIndex;
    
    this->computeOffset();
}

void BufferObjectDataPlain::computeOffset()
{
    mData.mOffsetInBytes = 0;
}

int BufferObjectDataPlain::getLayoutLocationIndex(int iIndex) const
{
    (void)iIndex;
    return mData.mLayoutLocationIndex;
}
int BufferObjectDataPlain::getOffsetInBytes(int iIndex) const
{
    (void)iIndex;
    return mData.mOffsetInBytes;
}
int BufferObjectDataPlain::getNumberOfComponents(int iIndex) const
{
    (void)iIndex;
    return mData.mNumberOfComponents;
}
DataType BufferObjectDataPlain::getDataType(int iIndex) const
{
    (void)iIndex;
    return mData.mDataType;
}
#pragma endregion
//----------------------------------------------------------------------------------