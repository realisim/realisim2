
#include <algorithm>
#include <cassert>
#include "Core/ImageSupport/ImageBufferHelpers.h"
#include "Half/half.hpp"

namespace Realisim
{
    namespace Core
    {
        //---------------------------------------------------------------------
        //
        //
        Core::Image convertToInternalFormat(const Core::Image& iInput,
            ImageInternalFormat iOutput, bool iNormalize, double iInputMinValue, double iInputMaxValue)
        {
            const int w = iInput.getWidth(), h = iInput.getHeight();
            Image o;
            o.setData(w, h, iOutput, nullptr);

            Math::Vector2i p;
            Color c;
            const double d = 1.0 / (iInputMaxValue - iInputMinValue);
            for (int y = 0; y < h; ++y)
                for (int x = 0; x < w; ++x)
                {
                    p.set(x, y);
                    c = iInput.getPixelColor(p);

                    if (iNormalize)
                    {
                        c.set( (c.getRed() - iInputMinValue) * d,
                            (c.getGreen() - iInputMinValue) * d,
                            (c.getBlue() - iInputMinValue) * d,
                            (c.getAlpha() - iInputMinValue) * d);
                    }

                    o.setPixelColor( p, c );
                }
            return o;
        }

        //---------------------------------------------------------------------
        Color getColorFromImageBuffer(const char *ipPtr, ImageInternalFormat iIif)
        {
            using namespace half_float;
            Color r;

            switch (iIif)
            {
            case iifUndefined: break;
            case iifRUint8: r.setRedUint8(*((uint8_t*)ipPtr)); break;
            case iifRInt8: r.setRedInt8(*ipPtr); break;
            case iifRUint16: r.setRedUint16(*((uint16_t*)ipPtr)); break;
            case iifRInt16: r.setRedInt16(*((int16_t*)ipPtr)); break;
            case iifRF16: r.setRedF16(*((half*)ipPtr)); break;
            case iifRUint32: r.setRedUint32(*((uint32_t*)ipPtr)); break;
            case iifRInt32: r.setRedInt32(*((int32_t*)ipPtr)); break;
            case iifRF32: r.setRedF32(*((float*)ipPtr));  break;
            case iifRgbUint8:
            {
                uint8_t *typedPtr = (uint8_t*)ipPtr;
                r.setUint8(typedPtr[0], typedPtr[1], typedPtr[2]);
            }break;
            case iifRgbInt8:
            {
                int8_t *typedPtr = (int8_t*)ipPtr;
                r.setInt8(typedPtr[0], typedPtr[1], typedPtr[2]);
            }break;
            case iifRgbUint16:
            {
                uint16_t *typedPtr = (uint16_t*)ipPtr;
                r.setUint16(typedPtr[0], typedPtr[1], typedPtr[2]);
            }break;
            case iifRgbInt16:
            {
                int16_t *typedPtr = (int16_t*)ipPtr;
                r.setInt16(typedPtr[0], typedPtr[1], typedPtr[2]);
            }break;
            case iifRgbF16:
            {
                half *typedPtr = (half*)ipPtr;
                r.setF16(typedPtr[0], typedPtr[1], typedPtr[2]);
            }break;
            case iifRgbUint32:
            {
                uint32_t *typedPtr = (uint32_t*)ipPtr;
                r.setUint32(typedPtr[0], typedPtr[1], typedPtr[2]);
            }break;
            case iifRgbInt32:
            {
                int32_t *typedPtr = (int32_t*)ipPtr;
                r.setInt32(typedPtr[0], typedPtr[1], typedPtr[2]);
            }break;
            case iifRgbF32:
            {
                float *typedPtr = (float*)ipPtr;
                r.setF32(typedPtr[0], typedPtr[1], typedPtr[2]);
            }break;
            case iifRgbaUint8:
            {
                uint8_t *typedPtr = (uint8_t*)ipPtr;
                r.setUint8(typedPtr[0], typedPtr[1], typedPtr[2], typedPtr[3]);
            }break;
            case iifRgbaInt8:
            {
                int8_t *typedPtr = (int8_t*)ipPtr;
                r.setInt8(typedPtr[0], typedPtr[1], typedPtr[2], typedPtr[3]);
            }break;
            case iifRgbaUint16:
            {
                uint16_t *typedPtr = (uint16_t*)ipPtr;
                r.setUint16(typedPtr[0], typedPtr[1], typedPtr[2], typedPtr[3]);
            }break;
            case iifRgbaInt16:
            {
                int16_t *typedPtr = (int16_t*)ipPtr;
                r.setInt16(typedPtr[0], typedPtr[1], typedPtr[2], typedPtr[3]);
            }break;
            case iifRgbaF16:
            {
                half *typedPtr = (half*)ipPtr;
                r.setF16(typedPtr[0], typedPtr[1], typedPtr[2], typedPtr[3]);
            }break;
            case iifRgbaUint32:
            {
                uint32_t *typedPtr = (uint32_t*)ipPtr;
                r.setUint32(typedPtr[0], typedPtr[1], typedPtr[2], typedPtr[3]);
            }break;
            case iifRgbaInt32:
            {
                int32_t *typedPtr = (int32_t*)ipPtr;
                r.setInt32(typedPtr[0], typedPtr[1], typedPtr[2], typedPtr[3]);
            }break;
            case iifRgbaF32:
            {
                float *typedPtr = (float*)ipPtr;
                r.setF32(typedPtr[0], typedPtr[1], typedPtr[2], typedPtr[3]);
            }break;
            default: assert(0); break;
            }

            return r;
        }
        
        //---------------------------------------------------------------------
        // Maybe this should be inlined
        //
        int64_t indexToPeriodicIndex(int iInput, int iMaxValue)
        {
            // for negative index...
            //
            // Lets suppose a period of 3;
            //
            // continuous indices -8 -7 -6 -5  -4 -3 -2 -1   0  1  2  3   4  5  6  7
            // wrapped indices     0  1  2  3 | 0  1  2  3 | 0  1  2  3 | 0  1  2  3
            //
            // so negatve index must be made absolute, then wrapped to period and
            // substracted from period.
            // A special case is needed when modulo gives 0. See -4 or -8.
            //
            if (iInput < 0)
            {
                iInput = std::abs(iInput);
                iInput %= iMaxValue;
                iInput = iInput == 0 ? iMaxValue : iInput;
                iInput = iMaxValue - iInput;
            }
            else
            {
                iInput = iInput % iMaxValue;
            }

            return iInput;
        }

        //---------------------------------------------------------------------
        // Maybe this should be inlined
        //
        uint64_t indexToClampedToEdgeIndex(int iInput, int iMaxValue)
        {
            uint64_t r = std::min(std::max(0, iInput), iMaxValue - 1);
            return r;
        }


        //---------------------------------------------------------------------
        void setColorToImageBuffer(char *ptr, ImageInternalFormat iIif, const Color& iCol)
        {
            using namespace half_float;

            switch (iIif)
            {
            case iifUndefined: break;
            case iifRUint8: *((uint8_t*)ptr) = iCol.getRedUint8(); break;
            case iifRInt8: *ptr = iCol.getRedInt8(); break;
            case iifRUint16: *((uint16_t*)ptr) = iCol.getRedUint16(); break;
            case iifRInt16: *((int16_t*)ptr) = iCol.getRedInt16(); break;
            case iifRF16: *((half*)ptr) = iCol.getRedF16(); break;
            case iifRUint32: *((uint32_t*)ptr) = iCol.getRedUint32(); break;
            case iifRInt32: *((int32_t*)ptr) = iCol.getRedInt32(); break;
            case iifRF32: *((float*)ptr) = iCol.getRedF32(); break;
            case iifRgbUint8:
            {
                uint8_t *typedPtr = (uint8_t*)ptr;
                typedPtr[0] = iCol.getRedUint8();
                typedPtr[1] = iCol.getGreenUint8();
                typedPtr[2] = iCol.getBlueUint8();
            }break;
            case iifRgbInt8:
            {
                int8_t *typedPtr = (int8_t*)ptr;
                typedPtr[0] = iCol.getRedInt8();
                typedPtr[1] = iCol.getGreenInt8();
                typedPtr[2] = iCol.getBlueInt8();
            }break;
            case iifRgbUint16:
            {
                uint16_t *typedPtr = (uint16_t*)ptr;
                typedPtr[0] = iCol.getRedUint16();
                typedPtr[1] = iCol.getGreenUint16();
                typedPtr[2] = iCol.getBlueUint16();
            }break;
            case iifRgbInt16:
            {
                int16_t *typedPtr = (int16_t*)ptr;
                typedPtr[0] = iCol.getRedInt16();
                typedPtr[1] = iCol.getGreenInt16();
                typedPtr[2] = iCol.getBlueInt16();
            }break;
            case iifRgbF16:
            {
                half *typedPtr = (half*)ptr;
                typedPtr[0] = iCol.getRedF16();
                typedPtr[1] = iCol.getGreenF16();
                typedPtr[2] = iCol.getBlueF16();
            }break;
            case iifRgbUint32:
            {
                uint32_t *typedPtr = (uint32_t*)ptr;
                typedPtr[0] = iCol.getRedUint32();
                typedPtr[1] = iCol.getGreenUint32();
                typedPtr[2] = iCol.getBlueUint32();
            }break;
            case iifRgbInt32:
            {
                int32_t *typedPtr = (int32_t*)ptr;
                typedPtr[0] = iCol.getRedInt32();
                typedPtr[1] = iCol.getGreenInt32();
                typedPtr[2] = iCol.getBlueInt32();
            }break;
            case iifRgbF32:
            {
                float *typedPtr = (float*)ptr;
                typedPtr[0] = iCol.getRedF32();
                typedPtr[1] = iCol.getGreenF32();
                typedPtr[2] = iCol.getBlueF32();
            }break;
            case iifRgbaUint8:
            {
                uint8_t *typedPtr = (uint8_t*)ptr;
                typedPtr[0] = iCol.getRedUint8();
                typedPtr[1] = iCol.getGreenUint8();
                typedPtr[2] = iCol.getBlueUint8();
                typedPtr[3] = iCol.getAlphaUint8();
            }break;
            case iifRgbaInt8:
            {
                int8_t *typedPtr = (int8_t*)ptr;
                typedPtr[0] = iCol.getRedInt8();
                typedPtr[1] = iCol.getGreenInt8();
                typedPtr[2] = iCol.getBlueInt8();
                typedPtr[3] = iCol.getAlphaInt8();
            }break;
            case iifRgbaUint16:
            {
                uint16_t *typedPtr = (uint16_t*)ptr;
                typedPtr[0] = iCol.getRedUint16();
                typedPtr[1] = iCol.getGreenUint16();
                typedPtr[2] = iCol.getBlueUint16();
                typedPtr[3] = iCol.getAlphaUint16();
            }break;
            case iifRgbaInt16:
            {
                int16_t *typedPtr = (int16_t*)ptr;
                typedPtr[0] = iCol.getRedInt16();
                typedPtr[1] = iCol.getGreenInt16();
                typedPtr[2] = iCol.getBlueInt16();
                typedPtr[3] = iCol.getAlphaInt16();
            }break;
            case iifRgbaF16:
            {
                half *typedPtr = (half*)ptr;
                typedPtr[0] = iCol.getRedF16();
                typedPtr[1] = iCol.getGreenF16();
                typedPtr[2] = iCol.getBlueF16();
                typedPtr[3] = iCol.getAlphaF16();
            }break;
            case iifRgbaUint32:
            {
                uint32_t *typedPtr = (uint32_t*)ptr;
                typedPtr[0] = iCol.getRedUint32();
                typedPtr[1] = iCol.getGreenUint32();
                typedPtr[2] = iCol.getBlueUint32();
                typedPtr[3] = iCol.getAlphaUint32();
            }break;
            case iifRgbaInt32:
            {
                int32_t *typedPtr = (int32_t*)ptr;
                typedPtr[0] = iCol.getRedInt32();
                typedPtr[1] = iCol.getGreenInt32();
                typedPtr[2] = iCol.getBlueInt32();
                typedPtr[3] = iCol.getAlphaInt32();
            }break;
            case iifRgbaF32:
            {
                float *typedPtr = (float*)ptr;
                typedPtr[0] = iCol.getRedF32();
                typedPtr[1] = iCol.getGreenF32();
                typedPtr[2] = iCol.getBlueF32();
                typedPtr[3] = iCol.getAlphaF32();
            }break;
            default: assert(0); break;
            }
        }
    }
}
