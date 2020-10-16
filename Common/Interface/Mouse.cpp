
#include "Mouse.h"
#include <sstream>

using namespace Realisim;
using namespace Interface;

namespace
{
    const Math::Vector2i kInvalidPosition(-1, -1);
    const double kDraggingDistance = 3; //pixels.
}

//-------------------------------------------------------------------------
Mouse::Mouse() :
    mPosition(kInvalidPosition),
    mPositionWhenClicked(kInvalidPosition),
    mDelta(),
    mButtonPressed(),
    mState(sIdle)
{
    for (int i = 0; i < bCount; ++i)
    {
        mButtonPressed[i] = false;
    }
}

//-------------------------------------------------------------------------
Mouse::~Mouse()
{}

//-------------------------------------------------------------------------
// Return the mouse movement delta and then clears the delta.
//
Math::Vector2i Mouse::getAndClearDelta()
{
    Math::Vector2i r = mDelta;
    setPosition(getPosition());
    return r;
}

//-------------------------------------------------------------------------
Math::Vector2i Mouse::getDelta() const
{
    return mDelta; 
}

//-------------------------------------------------------------------------
// Returns the total delta for the current drag.
//
Math::Vector2i Mouse::getDragDelta() const
{
    Math::Vector2i r;
    if (getState() == sDragging)
    {
        return getPosition() - getPositionWhenClicked();
    }
    return r;
}

//-------------------------------------------------------------------------
Math::Vector2i Mouse::getPosition() const
{
    return mPosition;
}

//-------------------------------------------------------------------------
// Returns the mouse position when the left button was pressed.
//
Math::Vector2i Mouse::getPositionWhenClicked() const
{
    return mPositionWhenClicked;
}

//-------------------------------------------------------------------------
Mouse::State Mouse::getState() const
{
    return mState;
}

//-------------------------------------------------------------------------
bool Mouse::isButtonPressed(Button iButton) const
{
    bool r = false;
    if (iButton >= 0 && iButton < bCount)
    {
        r = mButtonPressed[(int)iButton];
    }
    return r;
}

//-------------------------------------------------------------------------
void Mouse::setButtonPressed(Button iButton)
{
    if (iButton >= 0 && iButton < bCount)
    {
        mButtonPressed[(int)iButton] = true;

        if (iButton == bLeft)
        {
            mPositionWhenClicked = getPosition();
        }
        updateState();
    }
}

//-------------------------------------------------------------------------
void Mouse::setButtonReleased(Button iButton)
{
    if (iButton >= 0 && iButton < bCount)
    {
        mButtonPressed[(int)iButton] = false;
        
        if (iButton == bLeft)
        {
            mPositionWhenClicked = kInvalidPosition;
        }
        updateState();
    }
}

//-------------------------------------------------------------------------
void Mouse::setPosition(const Math::Vector2i& iPos)
{
    if (getPosition() != kInvalidPosition)
    {
        mDelta = iPos - mPosition;
    }
    
    mPosition = iPos;
    updateState();
}

//-------------------------------------------------------------------------
void Mouse::setPosition(int iX, int iY)
{
    setPosition(Math::Vector2i(iX, iY));
}

//-------------------------------------------------------------------------
void Mouse::updateState()
{
    switch (getState())
    {
    case sIdle:
        if (getDelta().normSquared() > 0)
        {
            mState = sMoving;
        }
        break;

    case sMoving:
        if (isButtonPressed(bLeft) && (getPosition() - getPositionWhenClicked()).norm() >= kDraggingDistance)
        {
            mState = sDragging;
        }
        break;

    case sDragging:
        if (!isButtonPressed(bLeft))
        {
            mState = sMoving;
            if (getDelta().normSquared() == 0)
            {
                mState = sIdle;
            }
        }
        break;

    default: break;
    }
}

//-------------------------------------------------------------------------
std::string Mouse::toString() const
{
    std::ostringstream oss;
    oss << "--- Mouse ---" << "\n";
    oss << "position: " << getPosition().toString() << "\n";
    oss << "position when clicked: " << getPositionWhenClicked().toString() << "\n";
    oss << "delta: " << getDelta().toString() << "\n";

    oss << "Button pressed: ";
    for(int i = 0; i < bCount; ++i)
    {
        oss << i << " " << (isButtonPressed((Button)i) ? "true, " : "false, ");
    }
    oss << "\n";

    oss << "State: " << mState << "\n";

    return oss.str();
}