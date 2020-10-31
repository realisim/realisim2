#pragma once

namespace Realisim
{
    namespace Core
    {
        enum ImageInternalFormat { iifUndefined,
                    iifRUint8, iifRInt8, iifRUint16, iifRInt16, iifRF16, iifRUint32, iifRInt32, iifRF32,
                    iifRgbUint8, iifRgbInt8, iifRgbUint16, iifRgbInt16, iifRgbF16, iifRgbUint32, iifRgbInt32, iifRgbF32,
                    iifRgbaUint8, iifRgbaInt8, iifRgbaUint16, iifRgbaInt16, iifRgbaF16, iifRgbaUint32, iifRgbaInt32, iifRgbaF32,
                };

        int getBytesPerChannel(ImageInternalFormat);
        int getNumberOfChannels(ImageInternalFormat);
    }
}

