
#include <cassert>
#include "Core/Unused.h"
#include <queue>
#include "SceneNode.h"

using namespace Realisim;
    using namespace ThreeD;
using namespace std;

//--- init static
uint32_t SceneNode::mIdCounter = 0;
uint32_t SceneNode::mInvalidId = std::numeric_limits<uint32_t>::max();

//---------------------------------------------------------------------------------------------------------------------
SceneNode::SceneNode() :
    mId(mIdCounter++),
    mName(""),
    mpParent(nullptr),
    mChilds(),
    mNodeType((int)NodeType::ntSceneNode)
{
}

//---------------------------------------------------------------------------------------------------------------------
SceneNode::SceneNode(int iNt) :
    mId(mIdCounter++),
    mName(""),
    mpParent(nullptr),
    mChilds(),
    mNodeType(iNt)
{}

//---------------------------------------------------------------------------------------------------------------------
SceneNode::~SceneNode()
{
    for (size_t i = 0; i < mChilds.size(); ++i)
    {
        delete mChilds[i];
    }
    mChilds.clear();
}

//---------------------------------------------------------------------------------------------------------------------
void SceneNode::addChild(SceneNode* ipNode)
{
    assert(ipNode != nullptr);
    ipNode->mpParent = this;
    mChilds.push_back(ipNode);

    // propagate the active flag... not too sure about this one...
    // ipNode->setActive(isActive());
}

//---------------------------------------------------------------------------------------------------------------------
// This method will remove (without delete) all childs of current node. The main usage of this would be when reparenting
// childs from a node.
//
// IE:  Reparenting node c,d,e from B to A and then deleting B.
// 
//      a->b->c,d,e
//
//      a->addChild(c);
//      a->addChild(d);
//      a->addChild(e);
//      toRemove = a->removeChild(b->getId());
//      toRemove->clearChilds();
//      delete toRemove;
//
void SceneNode::clearChilds()
{
    mChilds.clear();
}

//---------------------------------------------------------------------------------------------------------------------
uint32_t SceneNode::countNodes() const
{
    uint32_t r = 0;

    queue<const SceneNode*> q;
    q.push(this);
    while (!q.empty())
    {
        r++;

        const SceneNode * n = q.front();
        q.pop();

        for (int i = 0; i < n->getNumberOfChilds(); ++i)
        { q.push( const_cast<SceneNode*>(n)->getChild(i)); }
    }

    return r;
}

//---------------------------------------------------------------------------------------------------------------------
SceneNode* SceneNode::findNode(uint32_t iId)
{
    // TODO 
    // mettre ununordered map static au lieu de breadth first search...
    //

    //breadth first search    
    
    SceneNode *searchedNode = nullptr;

    queue<SceneNode*> q;
    q.push(this);

    SceneNode *n = nullptr;
    while (!q.empty() && searchedNode == nullptr)
    {
        n = q.front();
        q.pop();

        if(n->getId() == iId)
        { searchedNode = n; }

        for (int i = 0; i < n->getNumberOfChilds(); ++i)
        {
            q.push(n->getChild(i));
        }
    }

    return searchedNode;
}

//---------------------------------------------------------------------------------------------------------------------
// search the tree for a node with name iName.
//
// it will return the first one found or nullptr
//
SceneNode* SceneNode::findNode(const std::string& iName)
{
    SceneNode *searchedNode = nullptr;

    queue<SceneNode*> q;
    q.push(this);

    SceneNode *n = nullptr;
    while (!q.empty() && searchedNode == nullptr)
    {
        n = q.front();
        q.pop();

        if(n->getName() == iName)
        { searchedNode = n; }

        for (int i = 0; i < n->getNumberOfChilds(); ++i)
        {
            q.push(n->getChild(i));
        }
    }

    return searchedNode;
}

//---------------------------------------------------------------------------------------------------------------------
SceneNode* SceneNode::getChild(int iIndex)
{
    SceneNode *r = nullptr;
    if(iIndex >= 0 && iIndex < getNumberOfChilds())
    { r = mChilds[iIndex]; }
    return r;
}

//---------------------------------------------------------------------------------------------------------------------
uint32_t SceneNode::getId() const
{
    return mId;
}

//---------------------------------------------------------------------------------------------------------------------
string SceneNode::getName() const
{
    return mName;
}

//---------------------------------------------------------------------------------------------------------------------
SceneNode::NodeType SceneNode::getNodeType() const
{
    return (SceneNode::NodeType)mNodeType;
}

//---------------------------------------------------------------------------------------------------------------------
int SceneNode::getNumberOfChilds() const
{
    return (int)mChilds.size();
}

//---------------------------------------------------------------------------------------------------------------------
SceneNode* SceneNode::getParent()
{
    return mpParent;
}

//---------------------------------------------------------------------------------------------------------------------
void SceneNode::getNodesOfType(NodeType type, std::vector< SceneNode*>& listOfNodes)
{
    if (getNodeType() == type)
    {
        listOfNodes.push_back(this);
    }

    const int numChilds = getNumberOfChilds();
    for (int i = 0; i < numChilds; ++i)
    {
        getChild(i)->getNodesOfType(type, listOfNodes);
    }
}

//---------------------------------------------------------------------------------------------------------------------
// removes node (and all of its childs) at child iIndex (if present) from the hierarchy.
// the child in question is returned (nullptr if no child was removed).
// The handling of the removed childs (deletion most probably) is left to the caller.
//
SceneNode* SceneNode::removeChild(int iIndex)
{
    SceneNode *r = getChild(iIndex);
    if (r)
    {
        mChilds.erase(mChilds.begin() + iIndex);
    }
    return r;
}

//---------------------------------------------------------------------------------------------------------------------
// removes node (and all of its childs) with id iId (if present) from the hierarchy.
// the child in question is returned (nullptr if no child was removed).
// The handling of the removed childs (deletion most probably) is left to the caller.
//
SceneNode* SceneNode::removeNode(uint32_t iId)
{
    SceneNode *r = findNode(iId);
    if (r)
    {
        SceneNode *p = r->mpParent;
        if (p)
        {
            auto it = std::find(p->mChilds.begin(), p->mChilds.end(), r);
            if (it != p->mChilds.end())
            {
                p->mChilds.erase(it);
            }
        }
    }
    return r;
}

//---------------------------------------------------------------------------------------------------------------------
void SceneNode::setName(const std::string& iName)
{
    mName = iName;
}

//---------------------------------------------------------------------------------------------------------------------
string SceneNode::toString(NodeType iType)
{
    string r="undefined node type";

    switch (iType)
    {
        case NodeType::ntSceneNode: r = "Scene"; break;
        case NodeType::ntUserDefined: r = "UserDefined"; break;
        default: assert(0); break;
    }
    return r;
}

//---------------------------------------------------------------------------------------------------------------------
void SceneNode::update(double iTimeSinceStartupInSeconds)
{
    UNUSED(iTimeSinceStartupInSeconds);
}