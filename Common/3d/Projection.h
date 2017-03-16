
#pragma once

#include "Math/Matrix.h"
#include "Math/Vector.h"

namespace Realisim
{
namespace TreeD
{
    class Projection
    {
    public:
        enum Type{ tOrthogonal = 0, tPerspective };
        
        Projection();
        Projection(double iLeft, double iRight, double iBottom, double iTop, double iNear, double iFar, Projection::Type);
        Projection(const Projection&) = default;
        Projection& operator=(const Projection&) = default;
        ~Projection() = default;
      
        double bottom() const;
        double farPlane() const;
        double left() const;
        double height() const;
        double nearPlane() const;
		Math::Matrix4 projectionMatrix() const;
        double right() const;
        void setBottom(double);
        void setFarPlane(double);
        void setLeft(double);
        void setNearPlane(double);
        void setPerspectiveProjection(double iFov, double iRatio, double iNear, double iFar);
        void setProjection(double iLeft, double iRight, double iBottom, double iTop, double iNear, double iFar, Projection::Type);
        void setRight(double);
        void setTop(double);
        Math::Vector2 size() const;
        double top() const;
        Type type() const;
        double width() const;
        
    protected:
        double mLeft;
        double mRight;
        double mBottom;
        double mTop;
        double mNear;
        double mFar;
        Type mType;
    };
}
}
