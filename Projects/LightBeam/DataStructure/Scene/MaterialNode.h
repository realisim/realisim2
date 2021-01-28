#pragma once

#include "3d/Material.h"
#include "DataStructure/Scene/Interfaces.h"


namespace Realisim
{
namespace LightBeam
{
    class MaterialNode : public ISceneNode
    {
    public:
        MaterialNode();
        MaterialNode(const MaterialNode&) = default;
        MaterialNode& operator=(const MaterialNode&) = default;
        virtual ~MaterialNode();
        
        const Core::Image& getDiffuse() const;
        const ThreeD::Material& getMaterial() const;
        void setDiffuse(const Core::Image&);
        void setMaterial(const ThreeD::Material& iV);

    protected:
        ThreeD::Material mMaterial;

        Core::Image mDiffuse;
    };

}
}