/* AudioInterface.h */

#pragma once

#include "Math/Vector.h"

#ifdef  __APPLE__
#include "OpenAL/al.h"
#include "OpenAL/alc.h"
#endif //  __APPLE__

#ifdef _WIN32
#include "al.h"
#include "alc.h"
#endif // _WIN32


#include <string>
#include <vector>

namespace Realisim
{
namespace Audio
{
    
    class AudioInterface
    {
    public:
        AudioInterface();
        virtual ~AudioInterface();
        
        enum sourceState { ssInitial, ssPlaying, ssPaused, ssStopped };
        enum sourceType { stUndetermined, stStatic, stStreaming };
        enum supportedFileType{ sftWav };
        enum format{ fMono8, fMono16, fStereo8, fStereo16 };
        
        int addBuffer();
        int addSource();
        void attachBufferToSource( int, int );
        void detachBufferFromSource(int iSourceId);
        std::string getAndClearLastErrors() const;
        int getBufferBitsPerSample( int iBufferId ) const;
        int getBufferFrequency( int iBufferId ) const;
        int getBufferId( int iBufferId ) const;
        double getBufferLengthInSeconds( int iBufferId ) const;
        int getBufferNumberOfChannels( int iBufferId ) const;
        int getBufferSize( int iBufferId ) const; //in bytes
        double getListenerGain() const;
        Math::Vector3 getListenerPosition() const;
        Math::Vector3 getListenerVelocity() const;
        Math::Vector3 getListenerLookDirection() const;
        Math::Vector3 getListenerUp() const;
        int getNumberOfBuffers() const;
        int getNumberOfQueuedBuffersAtSource( int ) const;
        int getNumberOfSources() const;
        double getSourceConeInnerAngle(int) const;
        double getSourceConeOutterAngle(int) const;
        double getSourceConeOutterGain(int) const;
        int getSourceCurrentBuffer( int ) const;
        Math::Vector3 getSourceDirection( int ) const;
        double getSourceGain( int ) const;
        int getSourceId( int iIndex );
        double getSourceMaximumGain( int ) const;
        double getSourceMinimumGain( int ) const;
        double getSourceOffsetInBytes( int ) const;
        double getSourceOffsetInSamples( int ) const;
        double getSourceOffsetInSeconds( int ) const;
        double getSourcePitchShift( int ) const;
        Math::Vector3 getSourcePosition( int ) const;
        Math::Vector3 getSourceVelocity( int ) const;
        sourceState getSourceState( int ) const;
        sourceType getSourceType( int ) const;
        bool hasError() const;
        bool isSourceDirectional( int ) const; //AL_DIRECTION != 0.0
        bool isSourceLooping( int ) const;
        bool isSourceRelative( int ) const;
        bool isSourceReadyToStream( int ) const;
        bool isSourceReadyToPlayStaticBuffer( int ) const;
        bool isVerbose() const;
        void pauseSource( int );
        void pauseSources( std::vector<int> );
        void playSource( int );
        void playSources( std::vector<int> );
        void removeBuffer( int );
        void removeSource( int );
        void rewindSource( int );
        void rewindSources( std::vector<int> );
        //void queueBufferToSource( int, int );
        //void queueBuffersToSource( vector<int>, int );
        void setBufferData( int iBufferId, const unsigned char* iData, int iDataSize, format iFormat, int iFreq );
        void setBufferData( int iBufferId, const std::string& iData, format iFormat, int iFreq );
        void setListenerGain( double );
        void setListenerPosition( Math::Vector3 );
        void setListenerVelocity( Math::Vector3 );
        void setListenerOrientation( Math::Vector3, Math::Vector3 );
        void setSourceAsLooping( int, bool );
        void setSourceAsRelative( int, bool );
        void setAsVerbose( bool );
        void setSourceConeInnerAngle( int, double );
        void setSourceConeOuterAngle( int, double );
        void setSourceConeOuterGain( int, double );
        void setSourceDirection( int, Math::Vector3 );
        void setSourceGain( int, double );
        void setSourceGainBounds( int, double, double );
        void setSourceOffsetInBytes( int, double );
        void setSourceOffsetInSamples( int, double );
        void setSourceOffsetInSeconds( int, double );
        void setSourcePitchShift( int, double );
        void setSourcePosition( int, Math::Vector3 );
        void setSourceVelocity( int, Math::Vector3 );
        void stopSource( int );
        void stopSources( std::vector<int> );
        //void unqueueBufferFromSource( int, int );
        //void unqueueBuffersFromSource( vector<int>, int );
        
    protected:
        void addError( const std::string& ) const;
        void checkForAlError(const std::string& iM = std::string() ) const;
        void checkForAlcError(ALCdevice *ipD, const std::string& iM = std::string()) const;
        
        //std::string getVersion() const;
        //int getMajorVersion() const;
        //int getMinorVersion();
        //std::string getRenderer();
        
        ALCdevice* mpDevice;
        ALCcontext* mpContext;
        mutable std::string mErrors;
        bool mIsVerbose;
        std::vector<int> mSources;
        std::vector<int> mBuffers;
    };
    
}
}
