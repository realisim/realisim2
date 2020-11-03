#pragma once


#include "Math/VectorI.h"
#include <vector>

namespace Realisim
{
namespace Math
{
    // This class is simply an interface on a 2d array.
    // The intention is to ease readability when using 2d arrays.
    //
    // Here's an example:
    //
    //  //Classic array using std::vector, could be int *pIntArray = new int[sx*sy]
    //  // or anything else... maybe an int** abc = new int[][]....
    //
    //  const int sx = 5, sy = 5;
    //  std::vector<int> intArray(sx*sy, 0);
    //  
    //  // set diagonal to 1;
    //  intArray[0*sx + 0] = 1;
    //  intArray[1*sx + 1] = 1;
    //  intArray[2*sx + 2] = 1;
    //  intArray[3*sx + 3] = 1;
    //  intArray[4*sx + 4] = 1;
    //  
    //  
    //  //With Grid2d
    //  Simthetiq::Math::Grid2d<int> grid(sx, sy, 0);
    //  grid[0][0] = 1;
    //  grid[1][1] = 1;
    //  grid[2][2] = 1;
    //  grid[3][3] = 1;
    //  grid[4][4] = 1;
    //  
    //  //or
    //  grid[ Math::Vector2i(0,0) ] = 1;
    //  grid[ Math::Vector2i(1,1) ] = 1;
    //  grid[ Math::Vector2i(2,2) ] = 1;
    //  grid[ Math::Vector2i(3,3) ] = 1;
    //  grid[ Math::Vector2i(4,4) ] = 1;
    //
    //
    // Implementation Notes:
    //
    //  The class keeps the default value for the resize function. Indeed, 
    //  when resizing, the default value passed to the constructor (or directyl the 
    //  resize function) will be used.

    template<typename T>
    class Grid2d
    {
    public:
        Grid2d();
        Grid2d(const Grid2d<T>&) = default;
        Grid2d<T>& operator=(const Grid2d<T>&) = default;
        Grid2d(int iSizeX, int iSizeY, const T& iDefaultValue = T());
        Grid2d(const Vector2i& iSize, const T& iDefaultValue = T());

        const T& at(int iX, int iY) const;
        const T& at(const Vector2i& iSize) const;
        Vector2i getSize() const;
        bool isInBounds(int iX, int iY) const;
        bool isInBounds(const Vector2i& iSize) const;
        std::vector<T>& operator[](int);
        const std::vector<T>& operator[](int) const;
        T& operator[](const Vector2i& iSize);
        const T& operator[](const Vector2i& iSize) const;

        void resize(int iSizeX, int iSizeY);
        void resize(int iSizeX, int iSizeY, const T& iDefaultValue);
        void resize(const Vector2i& iSize);
        void resize(const Vector2i& iSize, const T& iDefaultValue);
        
    protected:
        Vector2i mSize;
        std::vector< std::vector<T> > mData;
        T mDefaultValue;
    };

    //-------------------------------------------------------------------------
    template<typename T>
    Grid2d<T>::Grid2d() : mSize(0, 0), mData(), mDefaultValue()
    {}

    //-------------------------------------------------------------------------
    template<typename T>
    Grid2d<T>::Grid2d(int iSizeX, int iSizeY, const T& iDefaultValue /*= T()*/) : 
        mSize(0, 0),
        mData(),
        mDefaultValue(iDefaultValue)
    {
        resize(iSizeX, iSizeY, iDefaultValue);
    }

    //-------------------------------------------------------------------------
    template<typename T>
    Grid2d<T>::Grid2d(const Vector2i& iSize, const T& iDefaultValue /*= T()*/) : 
        mSize(0, 0),
        mData(),
        mDefaultValue(iDefaultValue)
    {
        resize(iSize, iDefaultValue);
    }

    //-------------------------------------------------------------------------
    template<typename T>
    const T& Grid2d<T>::at(int iX, int iY) const
    {
        const T *r = &mDefaultValue;
        if ( isInBounds(iX,iY) )
        {
            r = &mData.at(iX).at(iY);
        }
        return *r;
    }

    //-------------------------------------------------------------------------
    template<typename T>
    const T& Grid2d<T>::at(const Vector2i& iIndex) const
    {
        return at(iIndex.x(), iIndex.y());
    }

    //-------------------------------------------------------------------------
    template<typename T>
    Vector2i Grid2d<T>::getSize() const
    {
        return mSize;
    }

    //-------------------------------------------------------------------------
    template<typename T>
    bool Grid2d<T>::isInBounds(int iX, int iY) const
    {
        return (iX >= 0 && iX < mSize.x() &&
                iY >= 0 && iY < mSize.y() );
    }

    //-------------------------------------------------------------------------
    template<typename T>
    bool Grid2d<T>::isInBounds(const Vector2i& iSize) const
    {
        return isInBounds(iSize.x(), iSize.y());
    }

    //-------------------------------------------------------------------------
    template<typename T>
    std::vector<T>& Grid2d<T>::operator[](int iX)
    {
        return mData[iX];
    }

    //-------------------------------------------------------------------------
    template<typename T>
    const std::vector<T>& Grid2d<T>::operator[](int iX) const
    {
        return mData[iX];
    }

    //-------------------------------------------------------------------------
    template<typename T>
    T& Grid2d<T>::operator[](const Vector2i& iIndex)
    {
        return mData[iIndex.x()][iIndex.y()];
    }

    //-------------------------------------------------------------------------
    template<typename T>
    const T& Grid2d<T>::operator[](const Vector2i& iIndex) const
    {
        return mData[iIndex.x()][iIndex.y()];
    }

    //-------------------------------------------------------------------------
    template<typename T>
    void Grid2d<T>::resize(int iX, int iY)
    {
        resize(iX, iY, T());
    }

    //-------------------------------------------------------------------------
    template<typename T>
    void Grid2d<T>::resize(int iX, int iY, const T& iDefaultValue)
    {
        mSize.set(iX, iY);
        mDefaultValue = iDefaultValue;

        mData.resize(iX);
        for (auto& y : mData)
        {
            y.resize(iY, iDefaultValue);
        }
    }

    //-------------------------------------------------------------------------
    template<typename T>
    void Grid2d<T>::resize(const Vector2i& iSize)
    {
        resize(iSize.x(), iSize.y(), T());
    }

    //-------------------------------------------------------------------------
    template<typename T>
    void Grid2d<T>::resize(const Vector2i& iSize, const T& iDefaultValue)
    {
        resize(iSize.x(), iSize.y(), iDefaultValue);
    }
}
}