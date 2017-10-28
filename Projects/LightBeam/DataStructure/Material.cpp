#include "Material.h"

using namespace Realisim;
    using namespace LightBeam;

//-----------------------------------------------------------------------------
Material::Material() :
    mSpecular(0.0),
    mGloss(0.0),
    mColor(1.0, 1.0, 1.0, 1.0)
{}

//-----------------------------------------------------------------------------
const Core::Color& Material::getColor() const
{
    return mColor;
}

//-----------------------------------------------------------------------------
double Material::getGlossFactor() const
{ return mGloss; }

//-----------------------------------------------------------------------------
double Material::getSpecularFactor() const
{ return mSpecular; }

//-----------------------------------------------------------------------------
void Material::setColor(const Core::Color& iC)
{ mColor = iC; }

//-----------------------------------------------------------------------------
void Material::setGlossFactor(double iV)
{ mGloss = iV; }

//-----------------------------------------------------------------------------
void Material::setSpecularFactor(double iV)
{ mSpecular = iV; }

//-----------------------------------------------------------------------------
