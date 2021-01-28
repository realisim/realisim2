#include "MaterialNode.h"

using namespace Realisim;
    using namespace LightBeam;

//----------------------------------------------------------------------
MaterialNode::MaterialNode() : ISceneNode(ntMaterial)
{}

//----------------------------------------------------------------------
MaterialNode::~MaterialNode()
{}

//----------------------------------------------------------------------
const Core::Image& MaterialNode::getDiffuse() const
{ return mDiffuse; }

//----------------------------------------------------------------------
const ThreeD::Material& MaterialNode::getMaterial() const
{ return mMaterial; }

//----------------------------------------------------------------------
void MaterialNode::setDiffuse(const Core::Image& iV)
{ mDiffuse = iV; }

//----------------------------------------------------------------------
void MaterialNode::setMaterial(const ThreeD::Material& iV)
{ mMaterial = iV; }
