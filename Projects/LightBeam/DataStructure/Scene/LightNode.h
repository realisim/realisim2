#pragma once

#include "Interfaces.h"


namespace Realisim
{
namespace LightBeam
{
    class LightNode : public ISceneNode, public ILightNode
    {
    public:
        LightNode();
        LightNode(const LightNode&) = default;
        LightNode& operator=(const LightNode&) = default;
        virtual ~LightNode();

        virtual double computeLi() const override;
        
    protected:
    };

}
}