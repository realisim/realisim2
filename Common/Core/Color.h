#pragma once

#include "Half/half.hpp"
#include <vector>
#include <stdint.h>

namespace Realisim
{
namespace Core
{    
    //-------------------------------------------------------------------------
    // ColorRgb
    //-------------------------------------------------------------------------
    // This class represents an RGB color. It is quite straight forward.
    //
    // getXXX to get desired channel value.
    // setXXX to set.
    //
    // The template is there to allow for typedef like ColorRgb8, ColorRgb16 
    // and ColorRgbF. (see typedef at bottom of file).
    //
    template<typename T>
    class ColorRgb
    {
    public:
        ColorRgb();
        ColorRgb(T iR, T iG, T iB);
        ColorRgb(const ColorRgb&) = default;
        ColorRgb& operator=(const ColorRgb&) = default;
        ~ColorRgb() {};

        T getBlue() const;
        T getGreen() const;
        T getRed() const;
        bool operator==(const ColorRgb<T>& iC) const;
        bool operator!=(const ColorRgb<T>& iC) const;
        void set(T iR, T iG, T iB);
        void setBlue(T iV);
        void setGreen(T iV);
        void setRed(T iV);

    protected:
        T mRed;
        T mGreen;
        T mBlue;
    };

    //-------------------------------------------------------------------------
    template<typename T>
    ColorRgb<T>::ColorRgb() :
        mRed((T)0),
        mGreen((T)0),
        mBlue((T)0)
    {}

    //-------------------------------------------------------------------------
    template<typename T>
    ColorRgb<T>::ColorRgb(T iR, T iG, T iB) :
        mRed(iR),
        mGreen(iG),
        mBlue(iB)
    {}

    //-------------------------------------------------------------------------
    template<typename T>
    T ColorRgb<T>::getBlue() const
    { return mBlue; }

    //-------------------------------------------------------------------------
    template<typename T>
    T ColorRgb<T>::getGreen() const
    { return mGreen; }

    //-------------------------------------------------------------------------
    template<typename T>
    T ColorRgb<T>::getRed() const
    { return mRed; }

    //-------------------------------------------------------------------------
    template<typename T>
    bool ColorRgb<T>::operator==(const ColorRgb<T>& iC) const
    {
        return mRed == iC.getRed() &&
            mGreen == iC.getGreen() &&
            mBlue == iC.getBlue();
    }

    //-------------------------------------------------------------------------
    template<typename T>
    bool ColorRgb<T>::operator!=(const ColorRgb<T>& iC) const
    {
        return !operator==(iC);
    }

    //-------------------------------------------------------------------------
    template<typename T>
    void ColorRgb<T>::set(T iR, T iG, T iB)
    {
        mRed = iR;
        mGreen = iG;
        mBlue = iB;
    }

    //-------------------------------------------------------------------------
    template<typename T>
    void ColorRgb<T>::setBlue(T iV)
    { mBlue = iV; }

    //-------------------------------------------------------------------------
    template<typename T>
    void ColorRgb<T>::setGreen(T iV)
    { mGreen = iV; }

    //-------------------------------------------------------------------------
    template<typename T>
    void ColorRgb<T>::setRed(T iV)
    { mRed = iV; }

    //-------------------------------------------------------------------------
    // ColorRgba
    //-------------------------------------------------------------------------
    // See ColorRgb for details.
    //
    // Exactly the same class but with an additionnal alpha channel. The reason
    // why ColorRgba is not inherited from ColorRgb is simply because the class
    // is small and it was quite annoying in the debugger to have rgb separated
    // a. Apart from that, there is no good reason to not inherit from ColorRgb
    // in the following manner.
    //
    // template<typename T> class ColorRgba : publi ColorRgb<T>
    //  make sure to have destructor virtual in ColorRgb
    //
    //
    template<typename T>
    class ColorRgba
    {
    public:
        ColorRgba();
        ColorRgba(T iR, T iG, T iB, T iA);
        ColorRgba(const ColorRgba&) = default;
        ColorRgba& operator=(const ColorRgba&) = default;
        virtual ~ColorRgba() {};

        T getAlpha() const;
        T getBlue() const;
        T getGreen() const;
        T getRed() const;
        bool operator==(const ColorRgba<T>& iC) const;
        bool operator!=(const ColorRgba<T>& iC) const;
        void set(T iR, T iG, T iB, T iA);
        void setAlpha(T iV);
        void setBlue(T iV);
        void setGreen(T iV);
        void setRed(T iV);

    protected:
        T mRed;
        T mGreen;
        T mBlue;
        T mAlpha;
    };

    //-------------------------------------------------------------------------
    template<typename T>
    ColorRgba<T>::ColorRgba() :
        mRed((T)0),
        mGreen((T)0),
        mBlue((T)0),
        mAlpha((T)0)
    {}

    //-------------------------------------------------------------------------
    template<typename T>
    ColorRgba<T>::ColorRgba(T iR, T iG, T iB, T iA) :
        mRed(iR),
        mGreen(iG),
        mBlue(iB),
        mAlpha(iA) 
    {}

    //-------------------------------------------------------------------------
    template<typename T>
    T ColorRgba<T>::getAlpha() const
    { return mAlpha; }

    //-------------------------------------------------------------------------
    template<typename T>
    T ColorRgba<T>::getBlue() const
    { return mBlue; }

    //-------------------------------------------------------------------------
    template<typename T>
    T ColorRgba<T>::getGreen() const
    { return mGreen; }

    //-------------------------------------------------------------------------
    template<typename T>
    T ColorRgba<T>::getRed() const
    { return mRed; }

    //-------------------------------------------------------------------------
    template<typename T>
    bool ColorRgba<T>::operator==(const ColorRgba<T>& iC) const
    {
        return mRed == iC.getRed() &&
            mGreen == iC.getGreen() &&
            mBlue == iC.getBlue() &&
            mAlpha == iC.getAlpha();
    }

    //-------------------------------------------------------------------------
    template<typename T>
    bool ColorRgba<T>::operator!=(const ColorRgba<T>& iC) const
    {
        return !operator==(iC);
    }

    //-------------------------------------------------------------------------
    template<typename T>
    void ColorRgba<T>::set(T iR, T iG, T iB, T iA)
    {
        mRed = iR;
        mGreen = iG;
        mBlue = iB;
        mAlpha = iA;
    }

    //-------------------------------------------------------------------------
    template<typename T>
    void ColorRgba<T>::setAlpha(T iV)
    { mAlpha = iV; }

    //-------------------------------------------------------------------------
    template<typename T>
    void ColorRgba<T>::setBlue(T iV)
    { mBlue = iV; }

    //-------------------------------------------------------------------------
    template<typename T>
    void ColorRgba<T>::setGreen(T iV)
    { mGreen = iV; }

    //-------------------------------------------------------------------------
    template<typename T>
    void ColorRgba<T>::setRed(T iV)
    { mRed = iV; }

    typedef ColorRgb<uint8_t> ColorRgbUint8;
    typedef ColorRgb<int8_t> ColorRgbInt8;
    typedef ColorRgb<uint16_t> ColorRgbUint16;
    typedef ColorRgb<int16_t> ColorRgbInt16;
    typedef ColorRgb<uint32_t> ColorRgbUint32;
    typedef ColorRgb<int32_t> ColorRgbInt32;
    typedef ColorRgb<half_float::half> ColorRgbF16;
    typedef ColorRgb<float> ColorRgbF32;

    typedef ColorRgba<uint8_t> ColorRgbaUint8;
    typedef ColorRgba<int8_t> ColorRgbaInt8;
    typedef ColorRgba<uint16_t> ColorRgbaUint16;
    typedef ColorRgba<int16_t> ColorRgbaInt16;
    typedef ColorRgba<uint32_t> ColorRgbaUint32;
    typedef ColorRgba<int32_t> ColorRgbaInt32;
    typedef ColorRgba<half_float::half> ColorRgbaF16;
    typedef ColorRgba<float> ColorRgbaF32;
    typedef ColorRgba<double> ColorRgbaF64;

    //-------------------------------------------------------------------------
    // Color
    //-------------------------------------------------------------------------
    // This class is an abstraction of ColorRgba<double>... it is a convenience class
    // to manipulate the color with abstraction of the underlying color resolution.
    //
    // Color will present the color in a normalized rgba format. This means that
    // get function such as getAlpha(), getRed(), getGreen() and getBlue() will return
    // a normalized double.
    //
    // Get:
    //  In order to get a specific type, the appropriate get must be called.
    //  Function such as getAlpha[SpecificType]() will return a non normalized value.
    //
    //  Convenient function such as getRgb[SpecificType]() will return a non normalized 
    //  RGB triplet.
    //  see also getRgba[SpecificType]()
    //
    // Set:
    //  There are many ways to set the color.
    //  Constructors will accept all support type (see below) in rgb or rgba form.
    //        Ex: Color r(1, 2, 3);
    //            Color r(0.2, 0.4, 0.8, 1.0);
    //
    //  It is also possible to set with an existing RGB triple or RGBA quadruplet.
    //      Ex: Color c(ColorRgbUint8(1,2,3));
    //          Color c(ColorRgbaUint8(1,2,3));
    //
    //  Supported type:
    //      int8, uint8
    //      int16, uint16
    //      int32, uint32
    //      float16 ? -> need support... see https://github.com/acgessler/half_float
    //      float32
    //      rgbInt8, rgbUint8, rgbInt16, rgbUint16, rgbInt32, rgbUint32
    //      rgbaInt8, rgbaUint8, rgbaInt16, rgbaUint16, rgbaInt32, rgbaUint32
    //
    // Limitations:
    //      The largest representable normalized type is uint32. see normalize
    //      function in .cpp
    //
    class Color : public ColorRgba<double>
    {
    public:
        Color();
        Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0);
        Color(int8_t r, int8_t g, int8_t b, int8_t a = 0);
        Color(uint16_t r, uint16_t g, uint16_t b, uint16_t a = 0);
        Color(int16_t r, int16_t g, int16_t b, int16_t a = 0);
        Color(uint32_t r, uint32_t g, uint32_t b, uint32_t a = 0);
        Color(int32_t r, int32_t g, int32_t b, int32_t a = 0);
        //Color(half r, half g, half b, half a);
        Color(float r, float g, float b, float a = 0.0f);
        Color(double r, double g, double b, double a = 0.0);

        Color(const Color&) = default;
        Color& operator=(const Color&) = default;
        virtual ~Color() = default;

        uint8_t getAlphaUint8() const;
        int8_t getAlphaInt8() const;
        uint16_t getAlphaUint16() const;
        int16_t getAlphaInt16() const;
        uint32_t getAlphaUint32() const;
        int32_t getAlphaInt32() const;
        half_float::half getAlphaF16() const;
        float getAlphaF32() const;

        uint8_t getBlueUint8() const;
        int8_t getBlueInt8() const;
        uint16_t getBlueUint16() const;
        int16_t getBlueInt16() const;
        uint32_t getBlueUint32() const;
        int32_t getBlueInt32() const;
        half_float::half getBlueF16() const;
        float getBlueF32() const;

        uint8_t getGreenUint8() const;
        int8_t getGreenInt8() const;
        uint16_t getGreenUint16() const;
        int16_t getGreenInt16() const;
        uint32_t getGreenUint32() const;
        int32_t getGreenInt32() const;
        half_float::half getGreenF16() const;
        float getGreenF32() const;

        uint8_t getRedUint8() const;
        int8_t getRedInt8() const;
        uint16_t getRedUint16() const;
        int16_t getRedInt16() const;
        uint32_t getRedUint32() const;
        int32_t getRedInt32() const;
        half_float::half getRedF16() const;
        float getRedF32() const;

        ColorRgbUint8 getRgbUint8() const;
        ColorRgbInt8 getRgbInt8() const;
        ColorRgbUint16 getRgbUint16() const;
        ColorRgbInt16 getRgbInt16() const;
        ColorRgbUint32 getRgbUint32() const;
        ColorRgbInt32 getRgbInt32() const;
        ColorRgbF16 getRgbF16() const;
        ColorRgbF32 getRgbF32() const;

        ColorRgbaUint8 getRgbaUint8() const;
        ColorRgbaInt8 getRgbaInt8() const;
        ColorRgbaUint16 getRgbaUint16() const;
        ColorRgbaInt16 getRgbaInt16() const;
        ColorRgbaUint32 getRgbaUint32() const;
        ColorRgbaInt32 getRgbaInt32() const;
        ColorRgbaF16 getRgbaF16() const;
        ColorRgbaF32 getRgbaF32() const;
        
        void setAlphaUint8(uint8_t a);
        void setAlphaInt8(int8_t a);
        void setAlphaUint16(uint16_t a);
        void setAlphaInt16(int16_t a);
        void setAlphaUint32(uint32_t a);
        void setAlphaInt32(int32_t a);
        void setAlphaF16(half_float::half a);
        void setAlphaF32(float a);

        void setBlueUint8(uint8_t a);
        void setBlueInt8(int8_t a);
        void setBlueUint16(uint16_t a);
        void setBlueInt16(int16_t a);
        void setBlueUint32(uint32_t a);
        void setBlueInt32(int32_t a);
        void setBlueF16(half_float::half a);
        void setBlueF32(float a);

        void setGreenUint8(uint8_t a);
        void setGreenInt8(int8_t a);
        void setGreenUint16(uint16_t a);
        void setGreenInt16(int16_t a);
        void setGreenUint32(uint32_t a);
        void setGreenInt32(int32_t a);
        void setGreenF16(half_float::half a);
        void setGreenF32(float a);

        void setRedUint8(uint8_t a);
        void setRedInt8(int8_t a);
        void setRedUint16(uint16_t a);
        void setRedInt16(int16_t a);
        void setRedUint32(uint32_t a);
        void setRedInt32(int32_t a);
        void setRedF16(half_float::half a);
        void setRedF32(float a);

        void setUint8(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0);
        void setInt8(int8_t r, int8_t g, int8_t b, int8_t a = 0);
        void setUint16(uint16_t r, uint16_t g, uint16_t b, uint16_t a = 0);
        void setInt16(int16_t r, int16_t g, int16_t b, int16_t a = 0);
        void setUint32(uint32_t r, uint32_t g, uint32_t b, uint32_t a = 0);
        void setInt32(int32_t r, int32_t g, int32_t b, int32_t a = 0);
        void setF16(half_float::half r, half_float::half g, half_float::half b, half_float::half a = (half_float::half)0.0f);
        void setF32(float r, float g, float b, float a = 0.0f);
        void setF64(double r, double g, double b, double a = 0.0);

        void setRgb(const ColorRgbUint8&);
        void setRgb(const ColorRgbInt8&);
        void setRgb(const ColorRgbUint16&);
        void setRgb(const ColorRgbInt16&);
        void setRgb(const ColorRgbUint32&);
        void setRgb(const ColorRgbInt32&);
        void setRgb(const ColorRgbF16&);
        void setRgb(const ColorRgbF32&);

        void setRgba(const ColorRgbaUint8&);
        void setRgba(const ColorRgbaInt8&);
        void setRgba(const ColorRgbaUint16&);
        void setRgba(const ColorRgbaInt16&);
        void setRgba(const ColorRgbaUint32&);
        void setRgba(const ColorRgbaInt32&);
        void setRgba(const ColorRgbaF16&);
        void setRgba(const ColorRgbaF32&);

    protected:
    };
}
}

