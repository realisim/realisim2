
#include "Material.h"

using namespace Realisim;
    using namespace ThreeD;
using namespace std;

//---------------------------------------------------------------------------------------------------------------------
Material::Material() :
    mReflectanceModel(rmDiffuse | rmGlossySpecular),
    mAmbientColor(1.0f, 1.0f, 1.0f),
    mDiffuseColor(1.0f, 1.0f, 1.0f),
    mEmissiveColor(0, 0, 0),
    mSpecularColor(0.0f, 0.0f, 0.0f),
    mShininess(10.0f),
    mAlpha(1.0f)
{
}

//---------------------------------------------------------------------------------------------------------------------
Material::~Material()
{}

//---------------------------------------------------------------------------------------------------------------------
bool Material::operator==(const Material& iM) const
{
    return mAmbientColor == iM.mAmbientColor &&
        mDiffuseColor == iM.mDiffuseColor &&
        mEmissiveColor == iM.mEmissiveColor &&
        mSpecularColor == iM.mSpecularColor &&
        mShininess == iM.mShininess &&
        mAlpha == iM.mAlpha;
}

//---------------------------------------------------------------------------------------------------------------------
void Material::addImageLayer(ImageLayer iLayer, const std::string &iImagePath)
{
    mImageLayerToImagePath[iLayer] = iImagePath;
}

//---------------------------------------------------------------------------------------------------------------------
float Material::getAlpha() const
{
    return mAlpha;
}

//---------------------------------------------------------------------------------------------------------------------
const Core::ColorRgbF32& Material::getAmbientColor()  const
{
    return mAmbientColor;
}

//---------------------------------------------------------------------------------------------------------------------
const Core::ColorRgbF32& Material::getDiffuseColor()  const
{
    return mDiffuseColor;
}

//---------------------------------------------------------------------------------------------------------------------
const Core::ColorRgbF32& Material::getEmissiveColor()  const
{
    return mEmissiveColor;
}

//---------------------------------------------------------------------------------------------------------------------
const string& Material::getImagePath(ImageLayer iIndex) const
{
    return mImageLayerToImagePath[iIndex];
}

//---------------------------------------------------------------------------------------------------------------------
float Material::getShininess() const
{
    return mShininess;
}

//---------------------------------------------------------------------------------------------------------------------
const Core::ColorRgbF32& Material::getSpecularColor() const
{
    return mSpecularColor;
}

//---------------------------------------------------------------------------------------------------------------------
bool Material::hasImageLayer(ImageLayer iIndex) const
{
    return !mImageLayerToImagePath[iIndex].empty();
}

//---------------------------------------------------------------------------------------------------------------------
bool Material::hasReflectanceModel(ReflectanceModel iV) const
{
    return (iV & mReflectanceModel) > 0;
}

//---------------------------------------------------------------------------------------------------------------------
void Material::setAlpha(float iV)
{
    mAlpha = iV;
}

//---------------------------------------------------------------------------------------------------------------------
void Material::setAmbientColor(const Core::ColorRgbF32& iCol)
{
    mAmbientColor = iCol;
}

//---------------------------------------------------------------------------------------------------------------------
void Material::setDiffuseColor(const Core::ColorRgbF32& iCol)
{
    mDiffuseColor = iCol;
}

//---------------------------------------------------------------------------------------------------------------------
void Material::setEmissiveColor(const Core::ColorRgbF32& iCol)
{
    mEmissiveColor = iCol;
}

//---------------------------------------------------------------------------------------------------------------------
void Material::setReflectanceModel(int iV)
{
    mReflectanceModel = iV;
}

//---------------------------------------------------------------------------------------------------------------------
void Material::setShininess(float iV)
{
    mShininess = iV;
}

//---------------------------------------------------------------------------------------------------------------------
void Material::setSpecularColor(const Core::ColorRgbF32& iCol)
{
    mSpecularColor = iCol;
}

