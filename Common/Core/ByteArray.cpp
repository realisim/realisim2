
#include "ByteArray.h"
#include <cassert>

using namespace Realisim;
using namespace Core;
using namespace std;

ByteArray::ByteArray() : mpGuts(nullptr)
{
    makeGuts();
}

//-------------------------------------------------------
ByteArray::ByteArray(const char* iData, int iLength /*= -1*/) :
mpGuts(nullptr)
{
    makeGuts();
    if(iData != nullptr)
    {
        if(iLength > 0)
        { mpGuts->mData = string(iData, iLength); }
        else
        { mpGuts->mData = iData; }
    }
    
}

//-------------------------------------------------------
ByteArray::ByteArray(const std::string& iData) :
mpGuts(nullptr)
{
    makeGuts();
    mpGuts->mData = iData;
}

//-------------------------------------------------------
ByteArray::ByteArray(const ByteArray& iOther) :
mpGuts(nullptr)
{
    shareGuts(iOther.mpGuts);
}

//-------------------------------------------------------
ByteArray& ByteArray::operator=(const ByteArray& iOther)
{
    shareGuts(iOther.mpGuts);
    return *this;
}

//-------------------------------------------------------
ByteArray& ByteArray::operator=(const char* iData)
{
    detachGuts();
    mpGuts->mData = iData;
    return *this;
}

//-------------------------------------------------------
ByteArray::~ByteArray()
{
    deleteGuts();
}
    
//-------------------------------------------------------
ByteArray& ByteArray::append(const char* iData)
{
    detachGuts();
    mpGuts->mData += iData;
    return *this;
}

//-------------------------------------------------------
ByteArray& ByteArray::append(const char* iData, int iLength)
{
    detachGuts();
    mpGuts->mData += string(iData, iLength);
    return *this;
}

//-------------------------------------------------------
ByteArray& ByteArray::append(const std::string& iData)
{
    detachGuts();
    mpGuts->mData += iData;
    return *this;
}

//-------------------------------------------------------
ByteArray& ByteArray::append(const ByteArray& iOther)
{
    detachGuts();
    // we recreate a string to remove the trailing \0 from iOther.
    mpGuts->mData += string(iOther.mpGuts->mData.c_str(), iOther.size());
    return *this;
}

//-------------------------------------------------------
char ByteArray::at(size_t i) const
{
    return mpGuts->mData.at(i);
}

//-------------------------------------------------------
size_t ByteArray::capacity() const
{
    return mpGuts->mData.capacity();
}

//-------------------------------------------------------
// ensures that iLength is within bounds of the current data buffer.
//
size_t ByteArray::capLength(int iLength) const
{
    size_t l = (size_t)iLength;
    if( l > size() )
    { l = size(); }
    return l;
}
//-------------------------------------------------------
// Removes all data from mData and will release allocated
// memory. Thus empty() will be true. Size will be 0 and
// capacity will be 0.
//
void ByteArray::clear()
{
    detachGuts();
    // We do not use std::string::clear() as it does not
    // deallocate memory. It only removes data.
    mpGuts->mData = string();
}

//-------------------------------------------------------
// Returns a const pointer to the data. This is guaranteed
// to never make a copy of the data.
//
const char* ByteArray::constData() const
{
    // not using c_str() because it adds a '\0'
    //
    return &(mpGuts->mData[0]);
}

//-------------------------------------------------------
// Returns a const reference to the data. This is guaranteed
// to never make a copy of the data. see method nonConstString() for
// a non const reference with a possible deep copy.
//
const std::string& ByteArray::constString() const
{
    return mpGuts->mData;
}

//-------------------------------------------------------
// returns a pointer to the data. When the refCount is greater than 1,
// a deep copy will be performed. When the refCount is 1, no copy is performed.
//
char* ByteArray::data()
{
    detachGuts();
    return &(mpGuts->mData[0]);
}

//-------------------------------------------------------
void ByteArray::deleteGuts()
{    
    if(!mpGuts) {return;}
    // delete gust when the refcount reach 0.
    // In this case, it means that refCount value was 1 prior
    // to the call to atomic_fetch_sub.
    //
    if( atomic_fetch_sub( &(mpGuts->mRefCount), 1 ) == 1 )
    {
        delete mpGuts;
        mpGuts = nullptr;
    }
}

//-------------------------------------------------------
// This is where the copy-on-write happens!
// When refcount is greater than 1, this will perform
// a deep copy of the existing one thus giving a new
// instance owned with ref count set to 1.
//
void ByteArray::detachGuts()
{
    //early out.
    if(!mpGuts) return;

    // only perform a deep copy if ref count is greater
    // than one
    if( atomic_fetch_sub( &(mpGuts->mRefCount), 1 ) > 1 )
    {
        //perform a deep copy
        mpGuts = new Guts(*mpGuts);
    }
    else
    {
        // increment the refcount back to 1 since
        // we are the only onwer of this guts.
        atomic_fetch_add( &(mpGuts->mRefCount), 1 );
    }
}

//-------------------------------------------------------
ByteArray& ByteArray::fill(char iChar, int iLength/*= -1*/)
{
    size_t l = capLength(iLength);
    
    auto begin = mpGuts->mData.begin();
    auto end = begin + l;
    
    std::fill(begin, end, iChar);
    return *this;
}

////-------------------------------------------------------
//ByteArray ByteArray::fromRawData(const char *iData, int iSize)
//{
//    return ByteArray();
//}

//-------------------------------------------------------
//ByteArra& insert(int iPos, const char*)
//{}

//-------------------------------------------------------
//ByteArra& insert(int iPos, const char*, int iSize); 
//{}

//-------------------------------------------------------
//ByteArra& insert(int iPos, const ByteArray&)
//{}

//-------------------------------------------------------
bool ByteArray::isEmpty() const
{
    return mpGuts->mData.empty();
}

//-------------------------------------------------------
// bool ByteArray::isNull() const
// {

// }

//-------------------------------------------------------
void ByteArray::makeGuts()
{
    assert(mpGuts == nullptr);
    mpGuts = new Guts();
}

//-------------------------------------------------------
// Returns a subpart of the data starting at byte iPos with a lenght of iLength.
// If iLenght is -1, it will return a buffer starting at iPos to the end.
//
ByteArray ByteArray::mid(size_t iPos, int iLength /*= -1*/) const
{
    return ByteArray( mpGuts->mData.substr(iPos,iLength) );
}

//-------------------------------------------------------
// see method constString()
//
std::string& ByteArray::nonConstString()
{
    detachGuts();
    return mpGuts->mData;
}

//-------------------------------------------------------
// Equality is perfomed on the data, not on the value
// of mpGuts.
//
bool ByteArray::operator==(const ByteArray& iOther) const
{
    return mpGuts->mData == iOther.mpGuts->mData;
}

//-------------------------------------------------------
bool ByteArray::operator!=(const ByteArray& iOther) const
{
    return mpGuts->mData != iOther.mpGuts->mData;
}

//-------------------------------------------------------
ByteArray ByteArray::operator+(const ByteArray& iOther) const
{
    // we recreate strings using size() to gurarantee
    // all data (even \0) are taken.
    //
    return ByteArray(
        string(mpGuts->mData.c_str(), size()) +
        string(iOther.mpGuts->mData.c_str(), iOther.size()));
}

//-------------------------------------------------------
ByteArray ByteArray::operator+(const char* iData)
{
    // we recreate strings using size() to gurarantee
    // all data (even \0) are taken.
    //
    return ByteArray(
                     string(mpGuts->mData.c_str(), size()) +
                     iData );
}

//-------------------------------------------------------
ByteArray ByteArray::operator+(const std::string& iData)
{
    // we recreate strings using size() to gurarantee
    // all data (even \0) are taken.
    //
    return ByteArray(
                     string(mpGuts->mData.c_str(), size()) +
                     iData );
}

//-------------------------------------------------------
ByteArray& ByteArray::operator+=(const ByteArray& iOther)
{
    detachGuts();
    // we recreate strings using size() to gurarantee
    // all data (even \0) are taken.
    //
    mpGuts->mData += string(iOther.mpGuts->mData.c_str(), iOther.size());
    return *this;
}

//-------------------------------------------------------
ByteArray& ByteArray::operator+=(const char* iData)
{
    detachGuts();
    mpGuts->mData += iData;
    return *this;
}

//-------------------------------------------------------
ByteArray& ByteArray::operator+=(const std::string& iData)
{
    detachGuts();
    mpGuts->mData += iData;
    return *this;
}

//-------------------------------------------------------
char ByteArray::operator[](size_t iIndex) const
{
    return mpGuts->mData[iIndex];
}

//-------------------------------------------------------
char& ByteArray::operator[](size_t iIndex)
{
    detachGuts();
    return mpGuts->mData[iIndex];
}

//-------------------------------------------------------
//ByteArra& prepend(const char*)
//{}

//-------------------------------------------------------
//ByteArra& prepend(const char*, int iSize); 
//{}

//-------------------------------------------------------
//ByteArra& prepend(const ByteArray&)
//{}

//-------------------------------------------------------
//ByteArra& remove(int iPos, int iLength = -1)
//{}

//-------------------------------------------------------
int ByteArray::refCount() const
{
    return mpGuts->mRefCount;
}

//-------------------------------------------------------
void ByteArray::reserve(size_t iSize)
{
    detachGuts();
    mpGuts->mData.reserve(iSize);
}

//-------------------------------------------------------
void ByteArray::resize(size_t iSize)
{
    detachGuts();
    mpGuts->mData.resize(iSize);
}

//-------------------------------------------------------
void ByteArray::set(const char *ipData, size_t iLength)
{
    detachGuts();

    if (ipData != nullptr && iLength > 0)
    {
        mpGuts->mData = string(ipData, iLength);
    }
}

//-------------------------------------------------------
void ByteArray::set(const std::string &iData)
{
    detachGuts();
    mpGuts->mData = iData;
}

//-------------------------------------------------------
void ByteArray::set(const ByteArray &iData)
{
    detachGuts();
    shareGuts(iData.mpGuts);
}

//-------------------------------------------------------
void ByteArray::shareGuts(Guts *ipShared)
{
    //if same guts do nothing
    if (mpGuts != ipShared)
    {
        if (atomic_fetch_add(&(ipShared->mRefCount), 1) > 0)
        {
            deleteGuts();
            mpGuts = ipShared;
        }
    }
}

//-------------------------------------------------------
size_t ByteArray::size() const
{
    return mpGuts->mData.size();
}

//-------------------------------------------------------
// ByteArra::Guts
//-------------------------------------------------------
ByteArray::Guts::Guts() : 
mRefCount(1),
mData()
{}

// deep copy
//
ByteArray::Guts::Guts(const Guts& iOther) : 
mRefCount(1),
mData( iOther.mData )
{}
