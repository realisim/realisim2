#pragma once

#include "Math/Vector.h"
namespace Realisim { namespace Math { class Vector2i; } }
#include "Mesh.h"

namespace Realisim
{
namespace Geometry
{
	/*This class represent a mathematical definition of a plane.
	
	  It can be constructed from a point an a normal or by 3 points.

	  The plane can be invalid, meaning it is mathematically unusable. For
	  example, when 3 colinear points are given...

      The normal obtained via getNormal is always normalized.
	*/
    class Plane
    {
    public:
        Plane();
        Plane( const Math::Vector3& p, const Math::Vector3& n);
		Plane(const Math::Vector3& p0, const Math::Vector3& p1, const Math::Vector3& p2);
        Plane( const Plane& ) = default;
        Plane& operator= ( const Plane& ) = default;
        ~Plane();

        double distance(const Math::Vector3& p) const;        
        Math::Vector3 getNormal() const;
		//getParametricFrom(double *a, double *b, double *c, double *d) const;
        Math::Vector3 getPoint() const;
		bool isValid() const;
		Mesh makeMesh(const Math::Vector2& iSizeInMeter) const;
        void set(const Math::Vector3& p, const Math::Vector3& n);
        void set(const Math::Vector3& p0, const Math::Vector3& p1, const Math::Vector3& p2);

    protected:
		void validate();

		bool mIsValid;
        Math::Vector3 mPoint;
        Math::Vector3 mNormal;
	};
}
}