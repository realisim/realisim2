
#include <cassert>
#include "ImageInternalFormat.h"

namespace Realisim
{
    
namespace Core
{
    //-------------------------------------------------------------------------
    int getBytesPerChannel(ImageInternalFormat iIf)
    {
        int r = 0;

        switch (iIf)
        {
         case iifUndefined: r = 0; break;
         case iifRUint8: r = 1; break;
         case iifRInt8: r = 1; break;
         case iifRUint16: r = 2; break;
         case iifRInt16: r = 2; break;
         case iifRF16: r = 2; break;
         case iifRUint32: r = 4; break;
         case iifRInt32: r = 4; break;
         case iifRF32: r = 4; break;
         case iifRgbUint8: r = 1; break;
         case iifRgbInt8: r = 1; break;
         case iifRgbUint16: r = 2; break;
         case iifRgbInt16: r = 2; break;
         case iifRgbF16: r = 2; break;
         case iifRgbUint32: r = 4; break;
         case iifRgbInt32: r = 4; break;
         case iifRgbF32: r = 4; break;
         case iifRgbaUint8: r = 1; break;
         case iifRgbaInt8: r = 1; break;
         case iifRgbaUint16: r = 2; break;
         case iifRgbaInt16: r = 2; break;
         case iifRgbaF16: r = 2; break;
         case iifRgbaUint32: r = 4; break;
         case iifRgbaInt32: r = 4; break;
         case iifRgbaF32: r = 4; break;
        default: assert(0); break;
        }

        return r;
    }

    //-------------------------------------------------------------------------
    int getNumberOfChannels(ImageInternalFormat iIf)
    {
        int r = 0;

        switch (iIf)
        {
        case iifUndefined: r = 0; break;
        case iifRUint8: r = 1; break;
        case iifRInt8: r = 1; break;
        case iifRUint16: r = 1; break;
        case iifRInt16: r = 1; break;
        case iifRF16: r = 1; break;
        case iifRUint32: r = 1; break;
        case iifRInt32: r = 1; break;
        case iifRF32: r = 1; break;
        case iifRgbUint8: r = 3; break;
        case iifRgbInt8: r = 3; break;
        case iifRgbUint16: r = 3; break;
        case iifRgbInt16: r = 3; break;
        case iifRgbF16: r = 3; break;
        case iifRgbUint32: r = 3; break;
        case iifRgbInt32: r = 3; break;
        case iifRgbF32: r = 3; break;
        case iifRgbaUint8: r = 4; break;
        case iifRgbaInt8: r = 4; break;
        case iifRgbaUint16: r = 4; break;
        case iifRgbaInt16: r = 4; break;
        case iifRgbaF16: r = 4; break;
        case iifRgbaUint32: r = 4; break;
        case iifRgbaInt32: r = 4; break;
        case iifRgbaF32: r = 4; break;
        default: assert(0); break;
        }

        return r;
    }
}
}