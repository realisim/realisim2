#include "Light.h"

using namespace Realisim;
    using namespace LightBeam;
    using namespace Math;

//-----------------------------------------------------------------------------
Light::Light() : mType(tDirectionnal),
    mAttenuationType(atNone),
    mPosition(),
    mDirection(1, 1, 1)
{ mDirection.normalize(); }

//-----------------------------------------------------------------------------
Light::~Light()
{}

//-----------------------------------------------------------------------------
Light::AttenuationType Light::getAttenuationType() const
{ return mAttenuationType; }

//-----------------------------------------------------------------------------
Math::Vector3 Light::getDirection() const
{ return mDirection; }

//-----------------------------------------------------------------------------
Math::Vector3 Light::getPosition() const
{ return mPosition; }

//-----------------------------------------------------------------------------
Light::Type Light::getType() const
{ return mType; }

//-----------------------------------------------------------------------------
void Light::setAttenuationType(AttenuationType iType)
{ mAttenuationType = iType; }

//-----------------------------------------------------------------------------
void Light::setDirection(const Math::Vector3& iD)
{
    mDirection = iD;
    mDirection.normalize();
}

//-----------------------------------------------------------------------------
void Light::setPosition(const Math::Vector3& iPos)
{ mPosition = iPos; }

//-----------------------------------------------------------------------------
void Light::setType(Type iType)
{ mType = iType; }
