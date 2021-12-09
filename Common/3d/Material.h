#pragma once

#include <array>
#include "Core/Color.h"
#include "Core/Image.h"
#include <map>

namespace Realisim
{
namespace ThreeD
{
    // 
    // Note: 
    //
   
    //  
    class Material
    {
    public:
        Material();
        Material(const Material&) = default;
        Material& operator=(const Material&) = default;
        virtual ~Material();

        enum ImageLayer {ilAmbient=0, ilDiffuse, ilSpecular, ilNormal, ilNumberOfLayers, ilundefined};
        enum ReflectanceModel {rmDiffuse = 1 << 0, 
            rmGlossySpecular = 1 << 1,
            rmPerfectSpecular = 1 << 2, 
            rmAll = rmDiffuse | rmGlossySpecular | rmPerfectSpecular };

        bool operator==(const Material&) const;

        void addImageLayer(ImageLayer iLayer, const std::string &iImagePath);
        float getAlpha() const;
        const Core::ColorRgbF32& getAmbientColor()  const;
        const Core::ColorRgbF32& getDiffuseColor()  const;
        const Core::ColorRgbF32& getEmissiveColor()  const;
        const std::string& getImagePath(ImageLayer) const;
        int getReflectanceModel() const { return mReflectanceModel; }
        float getShininess() const;
        const Core::ColorRgbF32& getSpecularColor() const;
        bool hasImageLayer(ImageLayer) const;
        bool hasReflectanceModel(ReflectanceModel iV) const;
        void setAlpha(float iV);
        void setAmbientColor(const Core::ColorRgbF32& iCol);
        void setDiffuseColor(const Core::ColorRgbF32& iCol);
        void setEmissiveColor(const Core::ColorRgbF32& iCol);
        void setReflectanceModel(int iV);
        void setShininess(float iV);
        void setSpecularColor(const Core::ColorRgbF32& iCol);

    protected:
        int mReflectanceModel;
        Core::ColorRgbF32 mAmbientColor;
        Core::ColorRgbF32 mDiffuseColor;
        Core::ColorRgbF32 mEmissiveColor;
        Core::ColorRgbF32 mSpecularColor;
        float mShininess;
        float mAlpha;

        std::array<std::string, ilNumberOfLayers> mImageLayerToImagePath;
    };
}
}