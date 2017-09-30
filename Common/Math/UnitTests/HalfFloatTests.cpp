#include "gtest/gtest.h"
#include "Half/half.hpp"
#include <cmath>

TEST(Half, tryingOut)
{
    using half_float::half;
    using namespace half_float::literal;

    {
        half a(3.4_h), b(5.0_h);
        half c = a * b;
        c += 3;
        //std::cout << c;
        EXPECT_TRUE(c == 19.9844_h);
    }
    
    {
        const int s = 512;
        half a[s];
        for (int i = 0; i < s; ++i)
        { 
            a[i] = (half)(2.2f*i);
            std::cout << a[i] << " ";
        }

        for (int i = 0; i < s; ++i)
        {
            EXPECT_TRUE(a[i] == (half)(2.2f*i));
        }
    }
}