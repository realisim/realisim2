/* utilities.cpp */

#include "Audio/utilities.h"
#include <cmath>

namespace Realisim
{
namespace Audio
{
    
    //------------------------------------------------------------------------------
    /*
     f: frequence a génerer en hertz
     s: sampling rate en Hertz
     d: durée en seconde
     af: format audio - voir AudioInterface.h
     
     Par exemple, pour faire un "la" de 1 sec avec un sampling rate de 44.1KHz:
     generateSoundBuffer( 440, 1.0, 44100 )
     
     8 bits: 0 à 255
     16 bits: -32768 à 32767
     */
    std::string generateSoundBuffer(int f,
                                    int s,
                                    double d )
    {
        const uint32_t sizeInBytes = (uint32_t)(d*s);
        char* data = new char[sizeInBytes];
        const float twoPi = 2*3.1415926f;
        for( int i = 0; i < d * s; ++i )
        {
            const float sineSample = ((i/(float)s) * f * twoPi);
            data[i] = (char)(sinf( fmodf( sineSample, twoPi ) ) * 127) + 127;
        }
     
        std::string r(data, sizeInBytes);
        delete[] data;
        return r;
    }
    
    
} //fin de namespace
}