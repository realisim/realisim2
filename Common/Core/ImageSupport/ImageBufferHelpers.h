
#pragma once

#include "Core/Color.h"
#include "Core/Image.h"
#include "Core/ImageInternalFormat.h"
#include <stdint.h>

namespace Realisim
{
namespace Core
{
    Core::Image convertToInternalFormat(const Core::Image& iInput, ImageInternalFormat iOutput, bool iNormalize, double iMinValue, double iMaxValue);
    Color getColorFromImageBuffer(const char *ipPtr, ImageInternalFormat iIif);
    int64_t indexToPeriodicIndex(int iInput, int iMaxValue);
    uint64_t indexToClampedToEdgeIndex(int iInput, int iMaxValue);
    void setColorToImageBuffer(char *ptr, ImageInternalFormat iIif, const Color& iCol);
}
}