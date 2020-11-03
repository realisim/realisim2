#pragma once

#include <algorithm>
#include "Math/CommonMath.h"
#include <memory>

namespace Realisim
{
    namespace Math
    {
        // clamps iV between iMin and iMax
        //
        template<typename T>
        static T clamp(T iV, T iMin, T iMax)
        {
            return std::min(std::max(iV, iMin), iMax);
        }

        // linearly interpolate between iBegin and iEnd, iFactor must be in range [0, 1]
        //
        template<typename T>
        inline T lerp(const T& iBegin, const T& iEnd, double iFactor)
        {
            return iBegin + (iEnd - iBegin)*iFactor;
        }

        // returns the (linear interpolation) of iValue given
        // a range of [iBegin, iEnd]
        //
        template<typename T>
        inline T inverseLerp(const T& iBegin, const T& iEnd, double iValue)
        {
            return (iValue - iBegin) / std::max((iEnd - iBegin), -std::numeric_limits<double>::max()); 
        }

        // returns the normalized value (linear interpolation) of iValue given
        // a range of [iBegin, iEnd]
        //
        template<typename T>
        inline T inverseLerpClamp(const T& iBegin, const T& iEnd, double iValue)
        {
            return clamp(inverseLerp(iBegin, iEnd, iValue), 0.0, 1.0); 
        }

        template<typename T>
        inline T biLerp(const T& iP00, const T& iP10, const T& iP11, const T& iP01,
            double iFractX, double iFractY)
        {
            const T r0 = lerp(iP00, iP10, iFractX);
            const T r1 = lerp(iP01, iP11, iFractX);
            return lerp(r0, r1, iFractY);
        }

        inline double smoothstep(double x) {
            // Evaluate polynomial
            return x * x * (3.0 - 2.0 * x);
        }

        inline double smoothstep(double edge0, double edge1, double x) {
            // Scale, bias and saturate x to 0..1 range
            x = inverseLerpClamp(edge0, edge1, x);
            // Evaluate polynomial
            return x * x * (3.0 - 2.0 * x);
        }

        // https://en.wikipedia.org/wiki/Sinc_function
        // sinc is the cardinal sine function, it is used to do a window filtering, mimicing a normal distribution
        inline static float sincf(const float x)
        {
            if(fabs(x) < 0.0001f)
            {
                return 1.0f + x*x*(-1.0f/6.0f + x*x*1.0f/120.0f);
            }
            else
            {
                return sin(x) / x;
            }
        }

        // lanczos resampling
        // https://en.wikipedia.org/wiki/Image_scaling
        // this class constructs the resampling filter kernel
        class FilterKernelLanczos
        {
        protected:
            std::unique_ptr<float[]>    kernel;
            int kernelSize;

        public:
            FilterKernelLanczos(int size = 5)
                : kernelSize(size)
            {
                kernel.reset(new float[kernelSize*kernelSize]);

                float center = ((float)kernelSize)*0.5f;
                float halfSize = center;

                float s = halfSize*0.5f;

                for(int j=0;j<kernelSize;++j)
                {
                    float y = (float)j - center;
                    for(int i=0;i<kernelSize;++i)
                    {
                        float x = (float)i - center;

                        float d = sqrt(x*x + y*y);

                        // lanczos
                        kernel[i + j*kernelSize] = std::max(sincf((float)M_PI*s*d)*sincf((float)M_PI*0.333f*d),0.f);
                    }
                }
            }
            ~FilterKernelLanczos()
            {
            }

            int getSize()
            {
                return kernelSize;
            }

            float evaluate(int i,int j)
            {
                return kernel[i+j*kernelSize];
            }
        };
    }
}
