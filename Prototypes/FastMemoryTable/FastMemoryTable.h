
#pragma once

#include <vector>
#include <unordered_map>

//---------------------------------------------------------------------------------------------------------------------
template<typename T>
class  SearchIndex {
public:
    SearchIndex() = default;
    virtual ~SearchIndex() = default;

    virtual uint64_t makeHash(const T*) = 0;
    virtual bool compare(const T* ipLhs, const T* ipRhs) = 0;
};

//---------------------------------------------------------------------------------------------------------------------
template<typename T>
class FastMemoryTable
{
public:
    FastMemoryTable();
    ~FastMemoryTable();

    int addSearchIndex(SearchIndex<T>*);
    std::vector<uint64_t> equalRange(int iSearchIndex, const T&) const;
    uint64_t findFirst(int iSearchIndex, const T&) const;

    const T& getData(size_t iIndex) const;
    T& getDataRef(size_t iIndex);

    void push_back(const T&);
    void reserve(int iNumberOfItems);

protected:
    std::vector<T*> mData;
    static T mDummyData;
    SearchIndex<T>* mpSearchIndex;
    std::unordered_multimap<uint64_t, uint64_t> mSearchIndexation;

    uint8_t* mpStartOfRawMemory;
    uint8_t* mpAddressOfNextItem;
    size_t mSizeInBytesOfRawMemory;
    size_t mAvailableRawMemoryInBytes;
};

//---------------------------------------------------------------------------------------------------------------------
//static initialization
template<typename T> T FastMemoryTable<T>::mDummyData;

//---------------------------------------------------------------------------------------------------------------------
template<typename T>
FastMemoryTable<T>::FastMemoryTable() :
    mpSearchIndex(nullptr),
    mpStartOfRawMemory(nullptr),
    mpAddressOfNextItem(nullptr),
    mSizeInBytesOfRawMemory(0),
    mAvailableRawMemoryInBytes(0)
{
}

//---------------------------------------------------------------------------------------------------------------------
template<typename T>
FastMemoryTable<T>::~FastMemoryTable() {
    
    //call destructor on all elements
    for (auto& d : mData) {
        d->~T();
    }
    mData.clear();

    if (mpSearchIndex) {
        delete mpSearchIndex;
        mpSearchIndex = nullptr;
    }

    if (mpStartOfRawMemory != nullptr) {
        delete[] mpStartOfRawMemory;
        mpStartOfRawMemory = nullptr;
        mpAddressOfNextItem = nullptr;
    }
    mSizeInBytesOfRawMemory = 0;
    mAvailableRawMemoryInBytes = 0;
}

//---------------------------------------------------------------------------------------------------------------------
template<typename T>
int FastMemoryTable<T>::addSearchIndex(SearchIndex<T>* ipSearchIndex)
{
    mpSearchIndex = ipSearchIndex;
    return 0;
}

//---------------------------------------------------------------------------------------------------------------------
template<typename T>
std::vector<uint64_t> FastMemoryTable<T>::equalRange(int iSearchIndex, const T& iValue) const {
    std::vector<uint64_t> r;
    if (iSearchIndex == 0)
    {
        const uint64_t index = mpSearchIndex->makeHash(&iValue);
        const auto range = mSearchIndexation.equal_range(index);
        for (auto it = range.first; it != range.second; ++it)
        {
            if (mpSearchIndex->compare(&iValue, mData[it->second]))
            {
                r.push_back(it->second);
            }
        }
            
    }
    return r;
}

//---------------------------------------------------------------------------------------------------------------------
template<typename T>
uint64_t FastMemoryTable<T>::findFirst(int iSearchIndex, const T& iValue) const {
    uint64_t r = (uint64_t )-1;
    if (iSearchIndex == 0)
    {
        const uint64_t index = mpSearchIndex->makeHash(&iValue);
        const auto it = mSearchIndexation.find(index);
        if (it != mSearchIndexation.end() && mpSearchIndex->compare(&iValue, mData[it->second]))
            r = it->second;
    }
    return r;
}


//---------------------------------------------------------------------------------------------------------------------
template<typename T>
const T& FastMemoryTable<T>::getData(size_t iIndex) const {
    const T* r = &mDummyData;
    if (iIndex < mData.size())
        r = mData[iIndex];
    return *r;
}

//---------------------------------------------------------------------------------------------------------------------
template<typename T>
T& FastMemoryTable<T>::getDataRef(size_t iIndex) {
    const T* r = &mDummyData;
    if (iIndex < mData.size())
        r = mData[iIndex];
    return *r;
}


//---------------------------------------------------------------------------------------------------------------------
template<typename T>
void FastMemoryTable<T>::push_back(const T& iValue) {
    const int sizeoft = sizeof(T);
    if (mAvailableRawMemoryInBytes >= sizeoft)
    {
        T* ptr = new(mpAddressOfNextItem) T(iValue);
        mpAddressOfNextItem += sizeoft;
        mAvailableRawMemoryInBytes -= sizeoft;
        mData.push_back(ptr);

        if (mpSearchIndex) {
            const uint64_t r = mpSearchIndex->makeHash(ptr);
            auto it = mSearchIndexation.find(r);
            if (it != mSearchIndexation.end()) {
                mSearchIndexation.insert(it, std::make_pair(r, (uint64_t)(mData.size() - 1)) );
            } else {
                mSearchIndexation.insert(mSearchIndexation.begin(), std::make_pair(r, (uint64_t)(mData.size() - 1)));
            }
            
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
template<typename T>
void FastMemoryTable<T>::reserve(int iNumberOfItems) {
    size_t requiredSizeInBytes = iNumberOfItems * sizeof(T);
    if (mpStartOfRawMemory == nullptr)
    {
        mpStartOfRawMemory = new uint8_t[requiredSizeInBytes];
        mpAddressOfNextItem = mpStartOfRawMemory;
        mSizeInBytesOfRawMemory = requiredSizeInBytes;
        mAvailableRawMemoryInBytes = requiredSizeInBytes;
    }
}