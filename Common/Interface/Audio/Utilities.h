/*  sound/utilities.h */
#include "Audio/AudioInterface.h"
#include <string>

namespace Realisim
{
namespace Audio 
{

    std::string generateSoundBuffer( int iFreq, int iSampling, double iDuration,
                                   AudioInterface::format = AudioInterface::fMono8 );

}
}