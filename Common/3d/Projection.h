
#pragma once

#include "Math/Vector.h"

namespace Realisim
{
namespace TreeD
{
    class Projection
    {
    public:
        Projection();
        Projection(const Projection&) = default;
        Projection& operator=(const Projection&) = default;
        ~Projection() = default;

        enum Type{ tOrthogonal = 0, tPerspective };
      
        double bottom() const;
        double far() const;
        double left() const;
        double height() const;
        double near() const;
        double right() const;
        void setBottom(double);
        void setFar(double);
        void setLeft(double);
        void setNear(double);
        void setOrthoProjection(double iVisibleGlUnit, double iNear, double iFar);
        void setOrthoProjection(double, double, double, double);
        void setPerspectiveProjection(double iFov, double iRatio, double iNear, double iFar);
        void setProjection(double iLeft, double iRight, double iBottom, double iTop, double iNear, double iFar, Projection::Type);
        void setRight(double);
        void setTop(double);
        Math::Vector2 size() const;
        double top() const;
        Type type() const;
        double width() const;
        double zoom() const;
        
    protected:
        double mLeft;
        double mRight;
        double mBottom;
        double mTop;
        double mNear;
        double mFar;
        double mZoomFactor; // a mettre dans camera...
        Type mType;
    };
}
}
