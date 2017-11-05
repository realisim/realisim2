#include "LightNode.h"

using namespace Realisim;
    using namespace LightBeam;

//----------------------------------------------------------------------
LightNode::LightNode() : ISceneNode(ntLight),
    ILightNode()
{}

//----------------------------------------------------------------------
LightNode::~LightNode()
{}

//----------------------------------------------------------------------
double LightNode::computeLi() const
{ return 1.0; }
