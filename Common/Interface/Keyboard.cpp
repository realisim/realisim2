
#include <iomanip>
#include "Keyboard.h"
#include <sstream>

using namespace Realisim;
using namespace Interface;

//-----------------------------------------------------------------------------
Keyboard::Keyboard() :
    mKeys(),
    mModifiers(mNone)
{}

//-----------------------------------------------------------------------------
Keyboard::~Keyboard()
{}

//-----------------------------------------------------------------------------
Keyboard::Modifiers Keyboard::asModifier(Key iKey)
{
    Modifiers r = mNone;
    switch(iKey)
    {
        case Key_Control: r = mControl; break;
        case Key_Shift: r = mShift; break;
        case Key_Alt: r = mAlt; break;
        default: break;
    }
    return r;
}

//-----------------------------------------------------------------------------
bool Keyboard::hasModifiers() const
{
    return getModifiers() != mNone;
}

//-----------------------------------------------------------------------------
bool Keyboard::isKeyPressed(Key iKey) const
{
    bool r = false;
    auto it = mKeys.find(iKey);
    if(it != mKeys.end())
    {
        r = it->second;
    }
    return r;
}

//-----------------------------------------------------------------------------
Keyboard::Modifiers Keyboard::getModifiers() const
{
    return mModifiers;
}

//-----------------------------------------------------------------------------
void Keyboard::setKeyPressed(Key iKey)
{
    mKeys[iKey] = true;

    mModifiers = (Modifiers)( (int)mModifiers | (int)asModifier(iKey) );
}

//-----------------------------------------------------------------------------
void Keyboard::setKeyReleased(Key iKey)
{
    mKeys[iKey] = false;
    mModifiers = (Modifiers)((int)mModifiers & ~(int)asModifier(iKey));
}

//-----------------------------------------------------------------------------
std::string Keyboard::toString() const
{
    std::ostringstream oss;

    oss << "--- Keyboard ---\n";
    oss << "key pressed: ";
    for (auto k : mKeys)
    {
        if (k.second)
        {
            oss << k.first << " ";
        }
    }
    oss << "\n";

    oss << "Modifiers: ";
    oss << "0x" << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << (int)getModifiers();

    return oss.str();
}