
#pragma once

#include "Math/Matrix.h"
#include "Math/Vector.h"

namespace Realisim
{
namespace Rendering
{
    // This class presents an interface to control a projection matrix. It supports
    // orthogonal and perspective matrices.
    //
    // The function type() return the type of projection matrix that will be computed
    // by method projectionMatrix(). Type can be tOrthogonal or tPerspective.
    //
    // There are 2 ways to specify the parameters of the projection:        
    //
    //        1- setProjection(), were all parameters (left, right, bottom, top, near, far 
    //        and type) must be specified. This method is quite convenient for 
    //        orthogonal projection.
    //
    //        2- setPerspectiveProjection(), is a bit more convenient for perspective
    //        projection since the angle in degree of field of view can be specified.
    //
    // The projection matrix is computed and returned by method projectionMatrix().
    //
    // Methods left(), right(), bottom(), top() return the value of the projection
    // at the near plane.
    //
    // Method nearPlane() and farPlane() return the distance of the near and far plane.
    //
    class Projection
    {
    public:
        enum Type{ tOrthogonal = 0, tPerspective };
        
        Projection();
		Projection(double iFovX, double iRatio, double iNear, double iFar);
        Projection(double iLeft, double iRight, double iBottom, double iTop, double iNear, double iFar, Projection::Type);
        Projection(const Projection&) = default;
        Projection& operator=(const Projection&) = default;
        ~Projection() = default;
      
        double getBottom() const;
        double getFar() const;
		double getHeight() const;
        double getLeft() const;
        double getNear() const;
        Math::Matrix4 getProjectionMatrix() const;
        double getRight() const;
		Math::Vector2 getSize() const;
		double getTop() const;
		Type getType() const;
		double getWidth() const;
		bool operator==(const Projection& iP);
		bool operator!=(const Projection& iP);
        void setBottom(double);
        void setFar(double);
        void setLeft(double);
        void setNear(double);
		void setPerspectiveProjection(double iFovX, double iFovY, double iNear, double iFar);
		void setPerspectiveProjectionWithRatio(double iFovX, double iRatio, double iNear, double iFar);
        void setProjection(double iLeft, double iRight, double iBottom, double iTop, double iNear, double iFar, Projection::Type);
        void setRight(double);
        void setTop(double);
		//Box getBox() const;
        
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
