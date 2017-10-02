#pragma once

#include "Math/Vector.h"

namespace Realisim
{
namespace Geometry
{
    // This class represent a rectangle.
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
    //
    //      A is the bottomLeft corner
    //      B is the topRight corner
    //
    // An invalid rectangle is a rectangle with a negative area.
    // For a rectangle to be valid, the top right corner must be greater than
    // the bottom left corner.
    //
    // When invalid, most funtions with return values cannot be trusted.
    //
    class Rectangle
    {
    public:
        Rectangle();
        Rectangle( const Math::Vector2& iBottomLeft, const Math::Vector2& iTopRight );
        Rectangle( const Math::Vector2& iBottomLeft, double iWidth, double iHeight );
        Rectangle( const Rectangle& ) = default;
        Rectangle& operator=( const Rectangle& ) = default;
        ~Rectangle();

        bool contains( const Math::Vector2& iPos, bool iProper = false ) const;
        bool contains(const Rectangle& iRect, bool iProper = false) const;
        double getArea() const;
        Math::Vector2 getCenter() const;
        
        double getBottom() const;
        Math::Vector2 getBottomLeft() const;
        Math::Vector2 getBottomRight() const;
        double getHeight() const;
        double getLeft() const;
        Math::Vector2 getPoint(int iIndex) const;
        double getRight() const;
        Math::Vector2 getSize() const;
        double getTop() const;
        Math::Vector2 getTopLeft() const;
        Math::Vector2 getTopRight() const;
        double getWidth() const;
        bool isValid() const;
        bool intersects(const Rectangle& iR) const;
        Rectangle intersected(const Rectangle& iR) const;
        void set( const Math::Vector2& iBottomLeft, const Math::Vector2& iTopRight );
        void set( const Math::Vector2& iBottomLeft, double iWidth, double iHeight );
        void setBottom(double);
        void setBottomLeft(const Math::Vector2& iBottomLeft);
        void setBottomRight(const Math::Vector2& iBottomRight);
        void setHeight(double iV);
        void setLeft(double iV);
        void setRight(double iV);
        void setSize( const Math::Vector2& iSize );
        void setTop(double iV);
        void setTopLeft(const Math::Vector2& iTopLeft);
        void setTopRight(const Math::Vector2& iTopRight);
        void setWidth(double iV);
        Rectangle united(const Rectangle& iR) const;

    protected:
        void validate();

        bool mIsValid;
        Math::Vector2 mBottomLeft;
        Math::Vector2 mTopRight;
    };

}
}