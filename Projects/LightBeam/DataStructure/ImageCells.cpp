
#include <cassert>
#include "ImageCells.h"

using namespace Realisim;
    using namespace Geometry;
    using namespace LightBeam;
    using namespace Math;

namespace
{
    int kNumberOfChilds = 4;
}

//-----------------------------------------------------------------------------
//  ImageCells::Node
//-----------------------------------------------------------------------------    
ImageCells::Node::Node() :
    mpParent(nullptr),
    mChilds(),
    mDepth(0.0)

{}

//-----------------------------------------------------------------------------
ImageCells::Node::~Node()
{
    // delete all childs
    for(int i = 0; i < mChilds.size(); ++i)
    { delete mChilds[i]; }
    mChilds.clear();
}

//-----------------------------------------------------------------------------
const Core::Color& ImageCells::Node::getColor() const
{ return mRgba; }

//-----------------------------------------------------------------------------
const Rectangle& ImageCells::Node::getCoverage() const
{ return mCoverage; }

//-----------------------------------------------------------------------------
double ImageCells::Node::getDepth() const
{ return mDepth; }

//-----------------------------------------------------------------------------
void ImageCells::Node::setColor(const Core::Color& iC)
{ mRgba = iC; }

//-----------------------------------------------------------------------------
void ImageCells::Node::setDepth(double iV)
{ mDepth = iV; }

//-----------------------------------------------------------------------------
// Split the current node in 4 and fills in childs.
//
// The split is done in the following layout. It follows the convention of
// origin in lower left corner.
//
//  |--------------|
//  |    3  |   2  |
//  |--------------|
//  |    0  |   1  |
//  |--------------|
//
void ImageCells::Node::split()
{
    if (mChilds.empty())
    {
        mChilds.resize(kNumberOfChilds);
        mChilds.shrink_to_fit(); //ensure we have only the requested allocated size!!!

        for (int i = 0; i < kNumberOfChilds; ++i)
        {
            Node *n = new Node();
            n->mpParent = this;

            const Rectangle parentCoverage = getCoverage();
            const double halfParentWidth = parentCoverage.getWidth() / 2.0;
            const double halfParentHeight = parentCoverage.getHeight() / 2.0;
            Vector2 bottomLeft;
            switch (i)
            {
            case 0: 
                bottomLeft = parentCoverage.getBottomLeft();
                break;
            case 1: 
                bottomLeft = parentCoverage.getBottomLeft() +
                    Vector2(halfParentWidth, 0);
                break;
            case 2: 
                bottomLeft = parentCoverage.getBottomLeft() +
                    Vector2(halfParentWidth, halfParentHeight);
                break;
            case 3:
                bottomLeft = parentCoverage.getBottomLeft() +
                    Vector2(0, halfParentHeight);
                break;
            default: assert(0); break;
            }

            Rectangle coverage(bottomLeft, halfParentWidth, halfParentHeight);
            n->mCoverage = coverage;

            mChilds[i] = n;
        }
    }
}

//-----------------------------------------------------------------------------
//  ImageCells
//-----------------------------------------------------------------------------    
ImageCells::ImageCells() :
    mpRoot(new Node())
{}

//-----------------------------------------------------------------------------
void ImageCells::clear()
{
    assert(mpRoot);
    delete mpRoot;
    mpRoot = new Node();
}

//-----------------------------------------------------------------------------
ImageCells::Node* ImageCells::getRoot()
{ return mpRoot; }


//-----------------------------------------------------------------------------
// Returns the total number of nodes that would be present in a fully extended 
// tree at level iLevel.
//
uint64_t ImageCells::getNumberOfNodeAtDepthLevel(int iLevel)
{
    // n = (4^(l+1) - 1) / 3
    uint64_t a = (uint64_t)pow((uint64_t)4, (uint64_t)(iLevel + 1));
    return (a - 1) / 3;
}

//-----------------------------------------------------------------------------
void ImageCells::setCoverage(double iX, double iY,
                                      double iW, double iH)
{
    clear();
    mpRoot->mCoverage = Rectangle(Vector2(iX, iY), iW, iH);
}




