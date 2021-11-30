#pragma once

#include <set>
#include <string>
#include <vector>

namespace Realisim
{
namespace ThreeD
{

class SceneNode
{
    public:
        // WARNING:
        //  Do not change the order of this enum as it is serialized.
        //
        enum class NodeType { ntSceneNode = 0, ntUserDefined = 10000 };

        SceneNode();
        explicit SceneNode(int iNt); // enables userdefine nodes
        SceneNode(const SceneNode&) = delete;
        SceneNode& operator=(const SceneNode&) = delete;
        virtual ~SceneNode();

        void addChild(SceneNode*);
        void clearChilds();
        uint32_t countNodes() const;
        template<class T> T* findFirstParentOfType();
        SceneNode* findNode(uint32_t iId);
        SceneNode* findNode(const std::string& iName);
        SceneNode* getChild(int iIndex);
        uint32_t getId() const;
        std::string getName() const;
        NodeType getNodeType() const;
        void getNodesOfType(NodeType, std::vector< SceneNode*>& listOfNodes);
        int getNumberOfChilds() const;
        SceneNode* getParent();
        SceneNode* removeChild(int iIndex);
        SceneNode* removeNode(uint32_t iId);
        void setName(const std::string& iName);
        static std::string toString(NodeType);
        virtual void update(double iTimeSinceStartupInSeconds);

    protected:
        //--- data
        static uint32_t mIdCounter;
        uint32_t mId;
        int mNodeType;
        std::string mName;

        SceneNode* mpParent;
        std::vector<SceneNode*> mChilds; //owned.
};

//-----------------------------------------------------------------------------------------------------------------
template<class T>
T* SceneNode::findFirstParentOfType()
{
    T* r = nullptr;

    SceneNode *p = getParent();
    while (p && r == nullptr)
    {
        r = dynamic_cast<T*>(p);
        p = p->getParent();
    }

    return r;
}

}
}