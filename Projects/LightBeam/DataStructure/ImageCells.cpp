
#include <cassert>
#include "ImageCells.h"

using namespace Realisim;
    using namespace Core;
    using namespace Geometry;
    using namespace LightBeam;
    using namespace Math;

//-----------------------------------------------------------------------------
//  ImageCells
//-----------------------------------------------------------------------------    
ImageCells::ImageCells()
{}

//-----------------------------------------------------------------------------
void ImageCells::clear()
{
    mCoverage = Rectangle();
    mRgba.clear();
    mDepth.clear();
}

//-----------------------------------------------------------------------------
Color ImageCells::getCellColor(const Math::Vector2i& iCell) const
{ return mRgba.getPixelColor(iCell); }

//-----------------------------------------------------------------------------
Geometry::Rectangle ImageCells::getCellCoverage(const Math::Vector2i& iCellIndex) const
{
    const Rectangle& r = getCoverage();
    const double w = r.getWidth() / getWidthInCells();
    const double h = r.getHeight() / getHeightInCells();
    
    return Rectangle(
        Vector2(iCellIndex.x() * w, iCellIndex.y() * h),
                     w, h );
}

//-----------------------------------------------------------------------------
const Rectangle& ImageCells::getCoverage() const
{ return mCoverage; }

//-----------------------------------------------------------------------------
double ImageCells::getCellDepth(const Math::Vector2i& iCell) const
{ return mDepth.getPixelColor(iCell).getRed(); }

//-----------------------------------------------------------------------------
int ImageCells::getHeightInCells() const
{ return mRgba.getHeight(); }

//-----------------------------------------------------------------------------
int ImageCells::getWidthInCells() const
{ return mRgba.getWidth(); }

//-----------------------------------------------------------------------------
Math::Vector2i ImageCells::getSizeInCells() const
{ return mRgba.getSizeInPixels(); }

//-----------------------------------------------------------------------------
bool ImageCells::isValid()
{
    return mCoverage.isValid() &&
        mRgba.getSizeInPixels().normSquare() > 0.0;
}

//-----------------------------------------------------------------------------
void ImageCells::set(const Math::Vector2i& iSizeInCells,
                     const Geometry::Rectangle& iCoverage)
{
    clear();
    setSize(iSizeInCells);
    setCoverage(iCoverage);
}

//-----------------------------------------------------------------------------
void ImageCells::setCellValue(const Vector2i& iCell,
    const Core::Color& iC,
    double iDepth)
{
    mRgba.setPixelColor(iCell, iC);
    
    Color depth(iDepth, 0.0, 0.0, 0.0);
    mDepth.setPixelColor(iCell, depth);
}

//-----------------------------------------------------------------------------
void ImageCells::setSize(const Math::Vector2i& iSizeInCells)
{
    mRgba.set(iSizeInCells, Core::iifRgbaUint8);
    mDepth.set(iSizeInCells, Core::iifRF32);
}

//-----------------------------------------------------------------------------
void ImageCells::setCoverage(const Geometry::Rectangle& iR)
{
    mCoverage = iR;
}




