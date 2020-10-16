
#include <cassert>
#include "Core/StatisticsTree.h"
#include "Core/StringUtilities.h"

using namespace Realisim;
    using namespace Core;
using namespace std;

namespace
{
    char kSeparator = '/';
}

//-----------------------------------------------------------------------------
StatisticsTree::StatisticsTree() : 
    mKeepSamples(false),
    mpRoot(new Node())
{}

//-----------------------------------------------------------------------------
StatisticsTree::~StatisticsTree()
{
    delete mpRoot;
    mpRoot = nullptr;
}

//-----------------------------------------------------------------------------
// add value iValue to key iKey. if key iKey, does not exists, it will be
// created
void StatisticsTree::add(const string& iKey, double iValue)
{
    Node *n = findKey(iKey);
    if(n == nullptr)
    { n = addKey(iKey); }
    assert(n != nullptr);
    
    n->mStats.add(iValue);
}

//-----------------------------------------------------------------------------
// This method will add key iKey if missing and returns a pointer
// to the node corresponding with the key.
//
StatisticsTree::Node* StatisticsTree::addKey(const string& iKey)
{
    Node *n = mpRoot;
    
    string keyToAdd;
    vector<string> keys = toVector(iKey, kSeparator);
    for(size_t i = 0; i < keys.size(); ++i)
    {
        const string key = keys[i];
        
        // add key to the complete key set.
        keyToAdd += keyToAdd.empty() ? key : (kSeparator + key);
        mKeys.insert(keyToAdd);
        
        // add key to the tree.
        auto it = n->mChilds.find(key);
        if(it != n->mChilds.end())
        {
            // the key is found.
            n = it->second.get();
        }
        else
        {
            // the key was not found...
            // add a new node and configure it.
            //
            unique_ptr<Node> child(new Node);
            child->mKey = key;
            child->mStats.keepSamples(isKeepingSamples());
            auto it2 = n->mChilds.insert(make_pair(key, std::move(child)));
            n = it2.first->second.get();
        }
    }
    
    return n;
}

//-----------------------------------------------------------------------------
// clear all data and reset to constructor state
//
void StatisticsTree::clear()
{
    delete mpRoot;
    mpRoot = new Node();
    
    mKeys.clear();
}

//-----------------------------------------------------------------------------
// return a pointer to the node corresponding to key iKey. It will return
// nullptr if the key is not found.
//
const StatisticsTree::Node* StatisticsTree::findKey(const string& iKey) const
{
    Node *n = mpRoot;
    
    vector<string> keys = toVector(iKey, kSeparator);
    for(size_t i = 0; i < keys.size() && n != nullptr; ++i)
    {
        auto it = n->mChilds.find(keys[i]);
        if(it != n->mChilds.end())
        {
            n = it->second.get();
        }
        else
        { n = nullptr; }
    }
    
    return n;
}

//-----------------------------------------------------------------------------
// see findKey() above
//
StatisticsTree::Node* StatisticsTree::findKey(const string& iKey)
{
    const Node *n = const_cast<const StatisticsTree*>(this)->findKey(iKey);
    return const_cast<Node*>(n);
}

//-----------------------------------------------------------------------------
double StatisticsTree::getAverage(const string& iKey) const
{
    double r = 0.0;
    const Node *n = findKey(iKey);
    if(n != nullptr)
    {
        r = n->mStats.average();
    }
    return r;
}

//-----------------------------------------------------------------------------
// returns a set of all keys contained in the tree.
//
const set<string>& StatisticsTree::getKeys() const
{
    return mKeys;
}

//-----------------------------------------------------------------------------
double StatisticsTree::getMaximum(const string& iKey) const
{
    double r = 0.0;
    const Node *n = findKey(iKey);
    if(n != nullptr)
    {
        r = n->mStats.maximum();
    }
    return r;
}

//-----------------------------------------------------------------------------
double StatisticsTree::getMinimum(const string& iKey) const
{
    double r = 0.0;
    const Node *n = findKey(iKey);
    if(n != nullptr)
    {
        r = n->mStats.minimum();
    }
    return r;
}
//-----------------------------------------------------------------------------
// returns the number of keys in the tree.
//
int StatisticsTree::getNumberOfKeys() const
{ return (int)mKeys.size(); }

//-----------------------------------------------------------------------------
// vector<double> StatisticsTree::getSamples(const string& iKey) const
//{}

//-----------------------------------------------------------------------------
double StatisticsTree::getStandardDeviation(const string& iKey) const
{
    double r = 0.0;
    const Node *n = findKey(iKey);
    if(n != nullptr)
    {
        r = n->mStats.standardDeviation();
    }
    return r;
}

//-----------------------------------------------------------------------------
bool StatisticsTree::hasChilds(const std::string& iKey) const
{
    bool r = false;
    const Node *n = findKey(iKey);
    if(n)
    {
        return n->mChilds.size() > 0;
    }
    return r;
}

//-----------------------------------------------------------------------------
bool StatisticsTree::hasKey(const string& iKey) const
{ return findKey(iKey) != nullptr; }

//-----------------------------------------------------------------------------
bool StatisticsTree::isKeepingSamples() const
{return mKeepSamples;}

//-----------------------------------------------------------------------------
void StatisticsTree::keepSamples(bool iK) 
{mKeepSamples = iK;}

//-----------------------------------------------------------------------------
// string toCsv() const{}

//-----------------------------------------------------------------------------
//--- StatisticsTree::Node
//-----------------------------------------------------------------------------
StatisticsTree::Node::Node() :
    mKey(),
    mStats(),
    mChilds()
{}

