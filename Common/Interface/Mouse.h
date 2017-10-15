#pragma once

#include "Math/VectorI.h"
#include <string>

namespace Realisim
{
namespace Interface
{
    //-------------------------------------------------------------------------
    /* This class is a modelization of the mouse.
       Buttons state (pressed or released) and position in pixel is available.

       A global state for the mouse is also available via getState() method.
       The mouse can be:
            sIdle: not moving
            sMoving: moving
            sDragging, moved at least 3 pixels with left button pressed.
    */
    class Mouse
    {
        public:
            Mouse();
            Mouse(const Mouse&) = default;
            Mouse& operator=(const Mouse&) = default;
            ~Mouse();

            enum Button {bLeft = 0, bMiddle, bRight, bCount};
            enum State {sIdle, sMoving, sDragging};
            
            Math::Vector2i getAndClearDelta();
            Math::Vector2i getDelta() const;
            Math::Vector2i getDragDelta() const;
            Math::Vector2i getPosition() const;
            Math::Vector2i getPositionWhenClicked() const;
            State getState() const;
            bool isButtonPressed(Button iButton) const;
            void setButtonPressed(Button iButton);
            void setButtonReleased(Button iButton);
            void setPosition(const Math::Vector2i& iPos);
            void setPosition(int iX, int iY);
            std::string toString() const;

        protected:
            void updateState();

            Math::Vector2i mPosition;
            Math::Vector2i mPositionWhenClicked;
            Math::Vector2i mDelta;
            bool mButtonPressed[bCount];
            State mState;
    };
}
}