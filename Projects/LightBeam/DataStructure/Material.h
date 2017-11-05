#pragma once

#include "Core/Color.h"

namespace Realisim
{
namespace LightBeam
{
    class Material
    {
    public:
        Material();
        Material(const Material&) = default;
        Material& operator=(const Material&) = default;
        ~Material() = default;

        const Core::Color& getColor() const;
        double getGlossFactor() const;
        double getSpecularFactor() const;
        
        void setColor(const Core::Color&);
        void setGlossFactor(double);
        void setSpecularFactor(double);
        
    protected:
        double mSpecular;
        double mGloss;
        Core::Color mColor;
    };
}
}