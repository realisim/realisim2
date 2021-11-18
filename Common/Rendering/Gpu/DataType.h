
#pragma once
#include <string>
#include "Core/ImageInternalFormat.h"
//#include "Core/ImageSupport/DdsImage.h"
namespace Realisim
{
namespace Rendering
{
    enum BufferDataUsage { bduUndefined = 0, bduStaticDraw, bduDynamicDraw, bduStreamDraw , bduDynamicCopy, bduStaticCopy,bduStreamCopy};
    enum DataType { dtUndefined=0, dtUnsignedByte, dtByte, dtUnsignedShort, dtShort, dtUnsignedInteger, dtInteger, dtHalfFloat, dtFloat, dtDouble, dtUnsignedInteger24_8 };

    enum DrawMode { dmUndefined=0, dmPoints, dmLineStrip, dmLineLoop, dmLines, dmLineStripAdjacency, dmLineAdjacency, dmTriangleStrip,
        dmTriangleFan, dmTriangles, dmTriangleStripAdjacency, dmTrianglesAdjacency, dmPatches, dmQuads};

    enum BufferBindingTarget { bbtUndefined = 0, bbtArrayBuffer, bbtElementArrayBuffer, bbtUniformBuffer, bbtShaderStorageBuffer, bbtTransformFeedbackBuffer,bbtAtomicCounterBuffer,
        bbtQueryBuffer,
        bbtTextureBuffer,
        bbtDrawIndirectBuffer,
        bbtDispatchIndirectBuffer
    };

    enum TextureInternalFormat {
        tifR8, tifRg8, tifRgb8, tifRgba8, 
        tifSrgb8, tifSrgba8, 
        tifR16, tifRg16, tifRgb16, tifRgba16,
        tifR16f, tifRg16f, tifRgb16f, tifRgba16f,
        tifR32f, tifRg32f, tifRgb32f, tifRgba32f,
        tifR32i, tifRg32i, tifRgb32i, tifRgba32i,
        tifR32ui, tifRg32ui, tifRgb32ui, tifRgba32ui,
        // DDS - dxt1 - dxt3 - dxt5 - bc1 - bc3 - bc5
        tifCompressedRgbDxt1, tifCompressedSrgbDxt1, tifCompressedRgbaDxt1, tifCompressedRgbaDxt3,
        tifCompressedRgbaDxt5, tifCompressedSrgbaDxt1, tifCompressedSrgbaDxt3, tifCompressedSrgbaDxt5,

        tifDepthInteger16,
        tifDepthInteger24,
        tifDepthInteger32,
        tifDepthFloat32,
        tifDepthInteger24StencilInteger8,
        tifDepthFloat32StencilInteger8,
        tifStencilIndexedInteger4,
        tifStencilIndexedInteger8,
        tifStencilIndexedInteger16
    };

    enum TextureFilter {
        tfNearest, tfLinear, tfNearestMipmapNearest, tfLinearMipmapNearest,
        tfNearestMipmapLinear, tfLinearMipmapLinear, tfCount
    };

    enum TextureFormat { tfRed, tfRg, tfRgb, tfRgba,
        tfBgr, tfBgra,// preferred format for transfer on Windows
		tfStencil, tfDepth, tfDepthStencil,
        tfRgba_integer,
        tfBgra_integer // preferred format for transfer on Windows
    };

    enum TextureTarget { tt2d,tt2dMultisampled,tt2dArray,tt3d, ttCubeMapPositiveX, ttCubeMapNegativeX, ttCubeMapPositiveY, ttCubeMapNegativeY,
        ttCubeMapPositiveZ, ttCubeMapNegativeZ };

    enum TextureWrapMode {twmRepeat, twmClampToBorder, twmClampToEdge, twmMirroredRepeat, twmCount};

	enum ShaderType{stFragment = 0, stVertex, stGeometry, stTesselationControl,
            stTesselationEvaluation,stComputeShader, stCount};

    enum FrameBufferAttachementType
    {
        fbaColor0=0,
        fbaColor1,
        fbaColor2,
        fbaColor3,
        fbaColor4,
        fbaColor5,
        fbaColor6,
        fbaColor7,
        fbaColor8,
        fbaColor9,
        fbaColor10,
        fbaColor11,
        fbaColor12,
        fbaColor13,
        fbaColor14,
        fbaColor15,
        fbaDepthOnly,
        fbaStencilOnly,
        fbaDepthStencil,
        fbaCount,
		fbaUnknown
    };

    enum BlendEquation
    {
        beAdditive=0,
        beSubstract,
        beReverseSubstract,
        beMin,
        beMax,
        beCount
    };

    enum BlendFunction
    {
        bfZero=0,
        bfOne,
        bfSourceColor,
        bfOneMinusSourceColor,
        bfDestinationColor,
        bfOneMinusDestinationColor,
        bfSourceAlpha,
        bfOneMinusSourceAlpha,
        bfDestinationAlpha,
        bfOneMinusDestinationAlpha,        
        bfConstantColor,
        bfOneMinusConstantColor,
        bfConstantAlpha,
        bfOneMinusConstantAlpha,
        bfSourceAlphaSaturate,        
        bfSource1Color,
        bfOneMinusSource1Color,
        bfSource1Alpha,
        bfOneMinusSource1Alpha,
        bfCount
    };

    int getSizeOf(DataType iDt);
    bool isACompressedFormat(TextureInternalFormat iInternalFormat);
    int toGlBufferBindingTarget(BufferBindingTarget iBbt);
    int toGlBufferDataUsage(BufferDataUsage iBdu);
    int toGlDataType(DataType iDt);
    int toGlDrawMode(DrawMode iDm);

    int toGlBlendFunction(BlendFunction iBf);
    int toGlBlendEquation(BlendEquation iBe);

    int toGlTextureInternalFormat(TextureInternalFormat iV);
    int toGlTextureFilter(TextureFilter iV);
    int toGlTextureFormat(TextureFormat iV);
    int toGlTextureTarget(TextureTarget iV);
    int toGlTextureWrapMode(TextureWrapMode iV);

	int toGlShaderType(ShaderType iSt);

    int toGlFrameBufferAttachement(FrameBufferAttachementType o);

	std::string toString(FrameBufferAttachementType o);
    std::string toString(TextureFilter iV);
    std::string toString(TextureWrapMode iV);

    struct TextureFormatDefinition
    {
        TextureInternalFormat   textureInternalFormat;
        DataType                type;
        TextureFormat           textureFormat;
    };
    TextureFormatDefinition   toTextureFormatDefinition(Realisim::Core::ImageInternalFormat o, bool iGammaCorrected);
    //TextureFormatDefinition   toTextureFormatDefinition(Realisim::Core::DdsImage::CompressionType compression,bool gammaCorrected);
}
}