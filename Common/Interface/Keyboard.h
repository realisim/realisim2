#pragma once

#include "Key.h"
#include <map>
#include <string>

namespace Realisim
{
namespace Interface
{
    //-------------------------------------------------------------------------
    /* 
    This class is a modelization of a keyboard.  
    
    It stores which keys are pressed and give access to common modifiers (shift, 
    control, alt).

    Key are defined in Interface::Key.
    */
    class Keyboard
    {
    public:
        Keyboard();
        Keyboard(const Keyboard&) = default;
        Keyboard& operator=(const Keyboard&) = default;
        ~Keyboard();

        enum Modifiers{mNone = 0x0, mShift = 0x1, mAlt = 0x10, mControl = 0x100};

        bool hasModifiers() const;
        bool isKeyPressed(Key iKey) const;
        Modifiers getModifiers() const;
        void setKeyPressed(Key iKey);
        void setKeyReleased(Key iKey);
        std::string toString() const;

    protected:
        Modifiers asModifier(Key iKey);

        std::map<Key, bool> mKeys;
        Modifiers mModifiers;
    };
}
}