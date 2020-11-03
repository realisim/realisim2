#include "Color.h"

using namespace Realisim;
using namespace Core;
using namespace half_float;

namespace
{
    //-------------------------------------------------------------------------
    // Here we normalize the input value iValue. 
    //
    // All unsigned type will be in the range [0.0, 1.0]
    // All signed type will be in the range [-0.5, 0.5]
    // The reason for the signed type to be between [-0.5, 0.5] is to enable
    // arithmetic operation on normalized value.
    //
    // For example:
    //   int8_t c = 0; will be 0.0 normalized.
    //   uint8_t d = 0; will be 0.0 normalized.
    //   thus: c+d = 0; normalized and unnormalized
    //
    //   If signed values were map to [0, 1], things would be different
    //   int8_t c = 0; will be 0.5 normalized.
    //   uint8_t d = 0; will be 0.0 normalized.
    //   thus: c+d = 0.5; thus 0 if unnormalized as int8_t, but 127 if unnormalized as uint8_t;
    //
    // Limitations: The largest representable type is uint32. To properly 
    //   normalize an uint32, we promote to int64 and store the result in
    //   double.    
    //
#pragma warning( push )
#pragma warning( disable : 4127 )
    template<class T>
    double normalize(T iValue)
    {
        const int64_t vMin = std::numeric_limits<T>::min();
        const int64_t vMax = std::numeric_limits<T>::max();
        const int64_t d = vMax - vMin;
        const double oneOverD = 1.0/(double)d;

        const int64_t n = iValue - vMin;
        double r = n * oneOverD;

        if(vMin < 0)
        { r -= 0.5; }

        return r;
    }
#pragma warning( pop )

    //-------------------------------------------------------------------------
    template<class T> inline
    void setRgbNormalized(Color *oColor, T iR, T iG, T iB)
    { oColor->setF64(normalize(iR), normalize(iG), normalize(iB), oColor->getAlpha()); }

    //-------------------------------------------------------------------------
    template<class T> inline
    void setRgbaNormalized(Color *oColor, T iR, T iG, T iB, T iA)
    { oColor->setF64(normalize(iR), normalize(iG), normalize(iB), normalize(iA)); }

    //-------------------------------------------------------------------------
    // see normalize.
    //
#pragma warning( push )
#pragma warning( disable : 4127 )
    template<class T> inline
    T unnormalize(double iValue)
    {
        const int64_t vMin = std::numeric_limits<T>::min();
        const int64_t vMax = std::numeric_limits<T>::max();
        const int64_t d = vMax - vMin;

        if(vMin < 0)
        { iValue += 0.5; }

        return (T)(vMin + (iValue * d));
    }
#pragma warning( pop )

    //-------------------------------------------------------------------------
    template<class T> inline
    ColorRgb<T> getRgbUnnormalized(const Color *iC)
    {
        return ColorRgb<T>(unnormalize<T>(iC->getRed()),
            unnormalize<T>(iC->getGreen()),
            unnormalize<T>(iC->getBlue()));
    }

    //-------------------------------------------------------------------------
    template<class T> inline
    ColorRgba<T> getRgbaUnnormalized(const Color *iC)
    {
        return ColorRgba<T>(unnormalize<T>(iC->getRed()),
            unnormalize<T>(iC->getGreen()),
            unnormalize<T>(iC->getBlue()),
            unnormalize<T>(iC->getAlpha()));
    }

}

//------------------------------------------------------------------------------
Color::Color() : ColorRgbaF64()
{}

//------------------------------------------------------------------------------
Color::Color(const ColorRgba<double>& iC) : ColorRgbaF64(iC)
{}

//------------------------------------------------------------------------------
Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) :
    ColorRgbaF64(normalize(r), normalize(g), normalize(b), normalize(a))
{}

//------------------------------------------------------------------------------
Color::Color(int8_t r, int8_t g, int8_t b, int8_t a) :
    ColorRgbaF64(normalize(r), normalize(g), normalize(b), normalize(a))
{}

//------------------------------------------------------------------------------
Color::Color(uint16_t r, uint16_t g, uint16_t b, uint16_t a) :
    ColorRgbaF64(normalize(r), normalize(g), normalize(b), normalize(a))
{}

//------------------------------------------------------------------------------
Color::Color(int16_t r, int16_t g, int16_t b, int16_t a) :
    ColorRgbaF64(normalize(r), normalize(g), normalize(b), normalize(a))
{}

//------------------------------------------------------------------------------
Color::Color(uint32_t r, uint32_t g, uint32_t b, uint32_t a) :
    ColorRgbaF64(normalize(r), normalize(g), normalize(b), normalize(a))
{}

//------------------------------------------------------------------------------
Color::Color(int32_t r, int32_t g, int32_t b, int32_t a) :
    ColorRgbaF64(normalize(r), normalize(g), normalize(b), normalize(a))
{}

//------------------------------------------------------------------------------
Color::Color(float r, float g, float b, float a) :
    ColorRgbaF64(r, g, b, a)
{}

//------------------------------------------------------------------------------
Color::Color(double r, double g, double b, double a) :
    ColorRgbaF64(r, g, b, a)
{}

//------------------------------------------------------------------------------
uint8_t Color::getAlphaUint8() const
{ return unnormalize<uint8_t>(getAlpha()); }

//------------------------------------------------------------------------------
int8_t Color::getAlphaInt8() const
{ return unnormalize<int8_t>(getAlpha()); }

//------------------------------------------------------------------------------
uint16_t Color::getAlphaUint16() const
{ return unnormalize<uint16_t>(getAlpha()); }

//------------------------------------------------------------------------------
int16_t Color::getAlphaInt16() const
{ return unnormalize<int16_t>(getAlpha()); }

//------------------------------------------------------------------------------
uint32_t Color::getAlphaUint32() const
{ return unnormalize<uint32_t>(getAlpha()); }

//------------------------------------------------------------------------------
int32_t Color::getAlphaInt32() const
{ return unnormalize<int32_t>(getAlpha()); }

//------------------------------------------------------------------------------
half Color::getAlphaF16() const
{ return (half)( (float)getAlpha()) ;}

//------------------------------------------------------------------------------
float Color::getAlphaF32() const
{ return (float)getAlpha(); }

//------------------------------------------------------------------------------
uint8_t Color::getBlueUint8() const
{ return unnormalize<uint8_t>(getBlue()); }

//------------------------------------------------------------------------------
int8_t Color::getBlueInt8() const
{ return unnormalize<int8_t>(getBlue()); }

//------------------------------------------------------------------------------
uint16_t Color::getBlueUint16() const
{ return unnormalize<uint16_t>(getBlue()); }

//------------------------------------------------------------------------------
int16_t Color::getBlueInt16() const
{ return unnormalize<int16_t>(getBlue()); }

//------------------------------------------------------------------------------
uint32_t Color::getBlueUint32() const
{ return unnormalize<uint32_t>(getBlue()); }

//------------------------------------------------------------------------------
int32_t Color::getBlueInt32() const
{ return unnormalize<int32_t>(getBlue()); }

//------------------------------------------------------------------------------
half Color::getBlueF16() const
{ return (half)((float)getBlue()); }

//------------------------------------------------------------------------------
float Color::getBlueF32() const
{ return (float)getBlue(); }

//------------------------------------------------------------------------------
uint8_t Color::getGreenUint8() const
{ return unnormalize<uint8_t>(getGreen()); }

//------------------------------------------------------------------------------
int8_t Color::getGreenInt8() const
{ return unnormalize<int8_t>(getGreen()); }

//------------------------------------------------------------------------------
uint16_t Color::getGreenUint16() const
{ return unnormalize<uint16_t>(getGreen()); }

//------------------------------------------------------------------------------
int16_t Color::getGreenInt16() const
{ return unnormalize<int16_t>(getGreen()); }

//------------------------------------------------------------------------------
uint32_t Color::getGreenUint32() const
{ return unnormalize<uint32_t>(getGreen()); }

//------------------------------------------------------------------------------
int32_t Color::getGreenInt32() const
{ return unnormalize<int32_t>(getGreen()); }

//------------------------------------------------------------------------------
half Color::getGreenF16() const
{ return (half)((float)getGreen()); }

//------------------------------------------------------------------------------
float Color::getGreenF32() const
{ return (float)getGreen(); }

//------------------------------------------------------------------------------
uint8_t Color::getRedUint8() const
{ return unnormalize<uint8_t>(getRed()); }

//------------------------------------------------------------------------------
int8_t Color::getRedInt8() const
{ return unnormalize<int8_t>(getRed()); }

//------------------------------------------------------------------------------
uint16_t Color::getRedUint16() const
{ return unnormalize<uint16_t>(getRed()); }

//------------------------------------------------------------------------------
int16_t Color::getRedInt16() const
{ return unnormalize<int16_t>(getRed()); }

//------------------------------------------------------------------------------
uint32_t Color::getRedUint32() const
{ return unnormalize<uint32_t>(getRed()); }

//------------------------------------------------------------------------------
int32_t Color::getRedInt32() const
{ return unnormalize<int32_t>(getRed()); }

//------------------------------------------------------------------------------
half Color::getRedF16() const
{ return (half)((float)getRed()); }

//------------------------------------------------------------------------------
float Color::getRedF32() const
{ return (float)getRed(); }

//------------------------------------------------------------------------------
ColorRgbUint8 Color::getRgbUint8() const
{ return getRgbUnnormalized<uint8_t>(this); }

//------------------------------------------------------------------------------
ColorRgbInt8 Color::getRgbInt8() const
{ return getRgbUnnormalized<int8_t>(this); }

//------------------------------------------------------------------------------
ColorRgbUint16 Color::getRgbUint16() const
{ return getRgbUnnormalized<uint16_t>(this); }

//------------------------------------------------------------------------------
ColorRgbInt16 Color::getRgbInt16() const
{ return getRgbUnnormalized<int16_t>(this); }

//------------------------------------------------------------------------------
ColorRgbUint32 Color::getRgbUint32() const
{ return getRgbUnnormalized<uint32_t>(this); }

//------------------------------------------------------------------------------
ColorRgbInt32 Color::getRgbInt32() const
{ return getRgbUnnormalized<int32_t>(this); }

//------------------------------------------------------------------------------
ColorRgbF16 Color::getRgbF16() const
{ return ColorRgbF16(getRedF16(), getGreenF16(), getBlueF16()) ;}

//------------------------------------------------------------------------------
ColorRgbF32 Color::getRgbF32() const
{ return ColorRgbF32(getRedF32(), getGreenF32(), getBlueF32()); }

//------------------------------------------------------------------------------
ColorRgbaUint8 Color::getRgbaUint8() const
{ return getRgbaUnnormalized<uint8_t>(this); }

//------------------------------------------------------------------------------
ColorRgbaInt8 Color::getRgbaInt8() const
{ return getRgbaUnnormalized<int8_t>(this); }

//------------------------------------------------------------------------------
ColorRgbaUint16 Color::getRgbaUint16() const
{ return getRgbaUnnormalized<uint16_t>(this); }

//------------------------------------------------------------------------------
ColorRgbaInt16 Color::getRgbaInt16() const
{ return getRgbaUnnormalized<int16_t>(this); }

//------------------------------------------------------------------------------
ColorRgbaUint32 Color::getRgbaUint32() const
{ return getRgbaUnnormalized<uint32_t>(this); }

//------------------------------------------------------------------------------
ColorRgbaInt32 Color::getRgbaInt32() const
{ return getRgbaUnnormalized<int32_t>(this); }

//------------------------------------------------------------------------------
ColorRgbaF16 Color::getRgbaF16() const
{ return ColorRgbaF16(getRedF16(), getGreenF16(), getBlueF16(), getAlphaF16()); }

//------------------------------------------------------------------------------
ColorRgbaF32 Color::getRgbaF32() const
{ return ColorRgbaF32(getRedF32(), getGreenF32(), getBlueF32(), getAlphaF32()); }

//------------------------------------------------------------------------------
void Color::setUint8(uint8_t r, uint8_t g, uint8_t b, uint8_t a /*=0*/)
{ setRgbaNormalized(this, r, g, b, a); }

//------------------------------------------------------------------------------
void Color::setInt8(int8_t r, int8_t g, int8_t b, int8_t a /*=0*/)
{ setRgbaNormalized(this, r, g, b, a); }

//------------------------------------------------------------------------------
void Color::setUint16(uint16_t r, uint16_t g, uint16_t b, uint16_t a /*=0*/)
{ setRgbaNormalized(this, r, g, b, a); }

//------------------------------------------------------------------------------
void Color::setInt16(int16_t r, int16_t g, int16_t b, int16_t a /*=0*/)
{ setRgbaNormalized(this, r, g, b, a); }

//------------------------------------------------------------------------------
void Color::setUint32(uint32_t r, uint32_t g, uint32_t b, uint32_t a /*=0*/)
{ setRgbaNormalized(this, r, g, b, a); }

//------------------------------------------------------------------------------
void Color::setInt32(int32_t r, int32_t g, int32_t b, int32_t a /*=0*/)
{ setRgbaNormalized(this, r, g, b, a); }

//------------------------------------------------------------------------------
void Color::setF16(half r, half g, half b, half a /*=0*/)
{ mRed = (double)r; mGreen = (double)g, mBlue = (double)b, mAlpha = (double)a; }

//------------------------------------------------------------------------------
void Color::setF32(float r, float g, float b, float a /*=0*/)
{ mRed = (double)r; mGreen = (double)g, mBlue = (double)b, mAlpha = (double)a; }

//------------------------------------------------------------------------------
void Color::setF64(double r, double g, double b, double a /*=0*/)
{ mRed = r; mGreen = g, mBlue = b, mAlpha = a; }

//------------------------------------------------------------------------------
void Color::setAlphaUint8(uint8_t a)
{ mAlpha = normalize(a); }

//------------------------------------------------------------------------------
void Color::setAlphaInt8(int8_t a)
{ mAlpha = normalize(a); }

//------------------------------------------------------------------------------
void Color::setAlphaUint16(uint16_t a)
{ mAlpha = normalize(a); }

//------------------------------------------------------------------------------
void Color::setAlphaInt16(int16_t a)
{ mAlpha = normalize(a); }

//------------------------------------------------------------------------------
void Color::setAlphaUint32(uint32_t a)
{ mAlpha = normalize(a); }

//------------------------------------------------------------------------------
void Color::setAlphaInt32(int32_t a)
{ mAlpha = normalize(a); }

//------------------------------------------------------------------------------
void Color::setAlphaF16(half r)
{ mAlpha = r; }

//------------------------------------------------------------------------------
void Color::setAlphaF32(float a)
{ mAlpha = a; }

//------------------------------------------------------------------------------
void Color::setBlueUint8(uint8_t b)
{ mBlue = normalize(b); }

//------------------------------------------------------------------------------
void Color::setBlueInt8(int8_t b)
{ mBlue = normalize(b); }

//------------------------------------------------------------------------------
void Color::setBlueUint16(uint16_t b)
{ mBlue = normalize(b); }

//------------------------------------------------------------------------------
void Color::setBlueInt16(int16_t b)
{ mBlue = normalize(b); }

//------------------------------------------------------------------------------
void Color::setBlueUint32(uint32_t b)
{ mBlue = normalize(b); }

//------------------------------------------------------------------------------
void Color::setBlueInt32(int32_t b)
{ mBlue = normalize(b); }

//------------------------------------------------------------------------------
void Color::setBlueF16(half r)
{ mBlue = r; }

//------------------------------------------------------------------------------
void Color::setBlueF32(float b)
{ mBlue = b; }

//------------------------------------------------------------------------------
void Color::setGreenUint8(uint8_t g)
{ mGreen = normalize(g); }

//------------------------------------------------------------------------------
void Color::setGreenInt8(int8_t g)
{ mGreen = normalize(g); }

//------------------------------------------------------------------------------
void Color::setGreenUint16(uint16_t g)
{ mGreen = normalize(g); }

//------------------------------------------------------------------------------
void Color::setGreenInt16(int16_t g)
{ mGreen = normalize(g); }

//------------------------------------------------------------------------------
void Color::setGreenUint32(uint32_t g)
{ mGreen = normalize(g); }

//------------------------------------------------------------------------------
void Color::setGreenInt32(int32_t g)
{ mGreen = normalize(g); }

//------------------------------------------------------------------------------
void Color::setGreenF16(half r)
{ mGreen = r;}

//------------------------------------------------------------------------------
void Color::setGreenF32(float g)
{ mGreen = g; }

//------------------------------------------------------------------------------
void Color::setRedUint8(uint8_t r)
{ mRed = normalize(r); }

//------------------------------------------------------------------------------
void Color::setRedInt8(int8_t r)
{ mRed = normalize(r); }

//------------------------------------------------------------------------------
void Color::setRedUint16(uint16_t r)
{ mRed = normalize(r); }

//------------------------------------------------------------------------------
void Color::setRedInt16(int16_t r)
{ mRed = normalize(r); }

//------------------------------------------------------------------------------
void Color::setRedUint32(uint32_t r)
{ mRed = normalize(r); }

//------------------------------------------------------------------------------
void Color::setRedInt32(int32_t r)
{ mRed = normalize(r); }

//------------------------------------------------------------------------------
void Color::setRedF16(half r)
{ mRed = r;}

//------------------------------------------------------------------------------
void Color::setRedF32(float r)
{ mRed = r; }

//------------------------------------------------------------------------------
void Color::setRgb(const ColorRgbUint8& iV)
{ setRgbNormalized(this, iV.getRed(), iV.getGreen(), iV.getBlue()); }

//------------------------------------------------------------------------------
void Color::setRgb(const ColorRgbInt8& iV)
{ setRgbNormalized(this, iV.getRed(), iV.getGreen(), iV.getBlue()); }

//------------------------------------------------------------------------------
void Color::setRgb(const ColorRgbUint16& iV)
{ setRgbNormalized(this, iV.getRed(), iV.getGreen(), iV.getBlue()); }

//------------------------------------------------------------------------------
void Color::setRgb(const ColorRgbInt16& iV)
{ setRgbNormalized(this, iV.getRed(), iV.getGreen(), iV.getBlue()); }

//------------------------------------------------------------------------------
void Color::setRgb(const ColorRgbUint32& iV)
{ setRgbNormalized(this, iV.getRed(), iV.getGreen(), iV.getBlue()); }

//------------------------------------------------------------------------------
void Color::setRgb(const ColorRgbInt32& iV)
{ setRgbNormalized(this, iV.getRed(), iV.getGreen(), iV.getBlue()); }

//------------------------------------------------------------------------------
void Color::setRgb(const ColorRgbF16& iV)
{ setF16(iV.getRed(), iV.getGreen(), iV.getBlue(), (half_float::half)(float)getAlpha()); }

//------------------------------------------------------------------------------
void Color::setRgb(const ColorRgbF32& iV)
{ setF32(iV.getRed(), iV.getGreen(), iV.getBlue(), (float)getAlpha()); }

//------------------------------------------------------------------------------
void Color::setRgba(const ColorRgbaUint8& iV)
{ setRgbaNormalized(this, iV.getRed(), iV.getGreen(), iV.getBlue(), iV.getAlpha()); }

//------------------------------------------------------------------------------
void Color::setRgba(const ColorRgbaInt8& iV)
{ setRgbaNormalized(this, iV.getRed(), iV.getGreen(), iV.getBlue(), iV.getAlpha()); }

//------------------------------------------------------------------------------
void Color::setRgba(const ColorRgbaUint16& iV)
{ setRgbaNormalized(this, iV.getRed(), iV.getGreen(), iV.getBlue(), iV.getAlpha()); }

//------------------------------------------------------------------------------
void Color::setRgba(const ColorRgbaInt16& iV)
{ setRgbaNormalized(this, iV.getRed(), iV.getGreen(), iV.getBlue(), iV.getAlpha()); }

//------------------------------------------------------------------------------
void Color::setRgba(const ColorRgbaUint32& iV)
{ setRgbaNormalized(this, iV.getRed(), iV.getGreen(), iV.getBlue(), iV.getAlpha()); }

//------------------------------------------------------------------------------
void Color::setRgba(const ColorRgbaInt32& iV)
{ setRgbaNormalized(this, iV.getRed(), iV.getGreen(), iV.getBlue(), iV.getAlpha()); }

//------------------------------------------------------------------------------
void Color::setRgba(const ColorRgbaF16& iV)
{ setF16(iV.getRed(), iV.getGreen(), iV.getBlue(), iV.getAlpha()); }

//------------------------------------------------------------------------------
void Color::setRgba(const ColorRgbaF32& iV)
{ set(iV.getRed(), iV.getGreen(), iV.getBlue(), iV.getAlpha()); }
