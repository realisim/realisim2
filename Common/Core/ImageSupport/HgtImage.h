#pragma once

#include "IImageReader.h"
#include "Math/Vector.h"
#include "Math/VectorI.h"
#include <stdint.h>

namespace Realisim
{
namespace Core
{
    // Most of the documention is here:
    // https://dds.cr.usgs.gov/srtm/version2_1/Documentation/SRTM_Topo.pdf 
    // althought it is quite clear once understood, I will try to explain a
    // bit more over here.
    //
    // http://poniesandlight.co.uk/notes/creating_normal_maps_from_nasa_depth_data/
    //
    // Hgt image do not have headers... So we will try to guess what type
    // of data is contained by using facts about srtm
    //
    // SRTM1:
    //  3601 x 3601 samples
    //  2 bytes per sample
    //  1 channel
    //  file size = 3601 x 3601 x 2
    //  middle of lowerLeft sample Geodetic coordinate is given in the filename
    //  sampling is at 1 arc seconds -> 1/3600 in lat/long
    //
    // SRTM3:
    //  1201 x 1201 samples
    //  2 bytes per sample
    //  1 channel
    //  file size = 1201x 1201 x 2
    //  middle of lowerLeft sample Geodetic coordinate is given in the filename
    //  sampling is at 3 arc seconds -> 3/3600 in lat/long
    //
    // Given these facts, when calling loadHeader() we can determine if it is
    // an srtm1 or srtm3, in which case the image will be valid (see isValid).
    // 
    // It is also possible to configure HgtImage to read arbitrary samples by
    // using method setCustomType().
    //
    // Origin of the image:
    //  The lower left Geodetic coordinate refer to the geometric center
    //  of the lower left sample
    //
    //-------------------------------------------------------------------------
    // Dependecies
    //-------------------------------------------------------------------------
    // none
    //
    class HgtImage : public IImageReader
    {
    public:
        enum type { tUndefined, tCustom, tSrtm1, tSrtm3, tBlueMarbleSrtmRamp2 };

        HgtImage();
        HgtImage(const HgtImage&) = default;
        HgtImage& operator=(const HgtImage&) = default;
        ~HgtImage();

        void clear();
        
        virtual ImageInternalFormat getInternalFormat() const override;
        static Math::Vector2 getLowerLeftCornerFromFileName(const std::string& iFilename, bool *oSuccess);
        Math::Vector2 getGeoTiffLowerLeftCorner() const;
        Math::Vector2 getGeoTiffUpperRightCorner() const;
        double getResolutionPerSampleInDegrees() const;
        type getType() const;
        virtual void load() override;
        virtual void loadHeader() override;       
        void setCustomType(int iNumberOfSamplesPerLine, int iNumberOfLines, double iResolutionPerSampleInDegree,
                           Math::Vector2 iLowerLeftCorner);
        
    protected:
        void flipVertical();
        type mType;
        
        //-- data read from header.
        double mResolutionPerSampleInDegrees;

        //-- data read from image name
        Math::Vector2 mGeoTiffLowerLeftCorner;
        Math::Vector2 mGeoTiffUpperRightCorner;
    };

}
}