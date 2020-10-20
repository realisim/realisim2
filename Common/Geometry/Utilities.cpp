#include "Geometry/Utilities.h"
#include "Math/IsEqual.h"

using namespace Realisim;
    using namespace Math;

     //---------------------------------------------------------------------------
    Math::Vector3 Realisim::Geometry::getPerpendicularVector(const Math::Vector3& iV)
    {
        /*afin d'obtenir un vecteur perpendiculaire, le produit scalaire doit donner
        0. donc
        1- (ax, by, cz) * (dx, ey, fz) = 0
        2- ( a*d + b*e + c*z ) = 0
        si d = b et que e = -a et que z = 0,
        3- a*b + b*(-a) + 0 = 0
        Bref, en permuttant deux valeurs et en inversant une des deux et remplacant
        la troisieme par 0, on obtient toujours un vecteur perpendiculaire.*/
        Math::Vector3 r(1.0, 0.0, 0.0);
        if (!Math::isEqual(iV.x(), 0.0))
            r = Math::Vector3(iV.y(), -iV.x(), 0.0);
        else if (!Math::isEqual(iV.y(), 0.0))
            r = Math::Vector3(-iV.y(), iV.x(), 0.0);
        else if (!Math::isEqual(iV.z(), 0.0))
            r = Math::Vector3(0.0, iV.z(), -iV.y());
        return r;
    }