#pragma once

#include "Core/Color.h"
#include "Core/Image.h"
#include "Geometry/Rectangle.h"
#include "Math/Vector.h"

namespace Realisim
{
namespace LightBeam
{
    class ImageCells
    {
    public:
        ImageCells();
        ImageCells(const ImageCells&) = default;
        ImageCells& operator=(const ImageCells&) = default;
        ~ImageCells() = default;

        void clear();
        Core::Color getCellColor(const Math::Vector2i& iCell) const;
        Geometry::Rectangle getCellCoverage(const Math::Vector2i& iCellIndex) const;
        const Geometry::Rectangle& getCoverage() const;
        double getCellDepth(const Math::Vector2i& iCell) const;
        int getHeightInCells() const;
        int getWidthInCells() const;
        Math::Vector2i getSizeInCells() const;
        bool isValid();
        void set(const Math::Vector2i& iSizeInCells, const Geometry::Rectangle& iCoverage);
        void setCellValue(const Math::Vector2i& iCell, const Core::Color& iC, double iDepth);
        void setSize(const Math::Vector2i& iSizeInCells);
        void setCoverage(const Geometry::Rectangle&);

    protected:
        Geometry::Rectangle mCoverage;
        Core::Image mRgba;
        Core::Image mDepth;
    };
}
}