#pragma once

#include "AxisAlignedBoundingBox.h"
#include "Math/Vector.h"
#include "Mesh.h"
#include <vector>

namespace Realisim
{
namespace Geometry
{
    // This class represent a RectangularPrism.
    //
    // The coordinate system assumption is as follow:
    //
    //                  --------B
    //          Y       |       |
    //          ^       |       |
    //          |       |       |
    //          |       A--------
    //          |     
    //          ---------> X
    //         /
    //        /
    //       Z
    //
    //      A is the bottomLeft corner in 3d on the far Z plane (min coord)
    //      B is the topRight corner in 3d on the near Z plane (max coord)
    //
    // An invalid RectangularPrism is a RectangularPrism with a negative area.
    // For a RectangularPrism to be valid, the top right corner must be greater than
    // the bottom left corner.
    //
    // When invalid, most funtions with return values cannot be trusted.
    //
    class RectangularPrism
    {
    public:
        RectangularPrism();
        RectangularPrism( const Math::Vector3& iBottomLeft, const Math::Vector3& iTopRight );
        RectangularPrism( const Math::Vector3& iBottomLeft, double iWidth, double iHeight, double iDepth );
        RectangularPrism( const RectangularPrism& ) = default;
        RectangularPrism& operator=( const RectangularPrism& ) = default;
        ~RectangularPrism();

        enum PointIndex{ piNearBottomLeft = 0, piNearBottomRight, piNearTopRight, piNearTopLeft,
            piFarBottomLeft, piFarBottomRight, piFarTopRight, piFarTopLeft };

        bool contains( const Math::Vector3& iPos, bool iProper = false ) const;
        bool contains(const RectangularPrism& iRect, bool iProper = false) const;
               
        double getBottom() const;
        Math::Vector3 getCenter() const;
        double getDepth() const;
        double getFar() const;
        Math::Vector3 getFarBottomLeft() const;
        Math::Vector3 getFarBottomRight() const;
        Math::Vector3 getFarTopLeft() const;
        Math::Vector3 getFarTopRight() const;
        double getHeight() const;
        double getLeft() const;
        double getNear() const;
        Math::Vector3 getNearBottomLeft() const;
        Math::Vector3 getNearBottomRight() const;
        Math::Vector3 getNearTopLeft() const;
        Math::Vector3 getNearTopRight() const;
        Math::Vector3 getPoint(int iIndex) const;
        std::vector<Math::Vector3> getPoints() const;
        double getRight() const;
        Math::Vector3 getSize() const;
        double getTop() const;
        double getVolume() const;
        double getWidth() const;
        bool isValid() const;
        bool intersects(const RectangularPrism& iR) const;
        RectangularPrism intersected(const RectangularPrism& iR) const;
        Mesh makeMesh() const;
        void set( const Math::Vector3& iBottomLeft, const Math::Vector3& iTopRight );
        void set( const Math::Vector3& iBottomLeft, double iWidth, double iHeight, double iDepth );
        void set(const AxisAlignedBoundingBox& iAabb);
        void setBottom(double);
        void setDepth(double iV);
        void setFar(double iV);
        void setFarBottomLeft(const Math::Vector3&);
        void setFarBottomRight(const Math::Vector3&);
        void setFarTopLeft(const Math::Vector3&);
        void setFarTopRight(const Math::Vector3&);
        void setHeight(double iV);
        void setLeft(double iV);
        void setNear(double iV);
        void setNearBottomLeft(const Math::Vector3&);
        void setNearBottomRight(const Math::Vector3&);
        void setNearTopLeft(const Math::Vector3&);
        void setNearTopRight(const Math::Vector3&);
        void setRight(double iV);
        void setSize( const Math::Vector3& iSize );
        void setTop(double iV);
        void setWidth(double iV);
        RectangularPrism united(const RectangularPrism& iR) const;

    protected:
        void validate();

        bool mIsValid;
        Math::Vector3 mBottomLeft;
        Math::Vector3 mTopRight;
    };

}
}