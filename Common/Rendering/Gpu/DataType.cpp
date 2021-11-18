

#include <cassert>
#include "Half/half.hpp"
#include "Rendering/Gpu/DataType.h"
#include "Rendering/Gpu/OpenGlHeaders.h"
//#include "Core/ImageSupport/DdsImage.h"

using namespace Realisim;

//-----------------------------------------------------------------------------
int Rendering::getSizeOf(DataType iDt)
{
    int r = 0;
    switch (iDt)
    {
    case dtUnsignedByte: r = 1; break;
    case dtByte: r = 1; break;
    case dtUnsignedShort: r = sizeof(unsigned short); break;
    case dtShort: r = sizeof(short); break;
    case dtUnsignedInteger: r = sizeof(unsigned int); break;
    case dtInteger: r = sizeof(int); break;
    case dtHalfFloat: r = sizeof(half_float::half); break;
    case dtFloat: r = sizeof(float); break;
    case dtDouble: r = sizeof(double); break;
    case dtUnsignedInteger24_8: r = sizeof(unsigned int); break;
    default: assert(0); break;
    }

    return r;
}

//-----------------------------------------------------------------------------
bool Rendering::isACompressedFormat(TextureInternalFormat iInternalFormat)
{
    bool r = false;

    switch (iInternalFormat)
    {
    case tifCompressedRgbDxt1: r = true; break;
    case tifCompressedSrgbDxt1: r = true; break;
    case tifCompressedRgbaDxt1: r = true; break;
    case tifCompressedRgbaDxt3: r = true; break;
    case tifCompressedRgbaDxt5: r = true; break;
    case tifCompressedSrgbaDxt1: r = true; break;
    case tifCompressedSrgbaDxt3: r = true; break;
    case tifCompressedSrgbaDxt5: r = true; break;
    default: break;
    }

    return r;
}

//-----------------------------------------------------------------------------
int Rendering::toGlBufferDataUsage(BufferDataUsage iBdu)
{
    int r = 0;
    switch (iBdu)
    {
    case bduUndefined: r = 0; break;
    case bduStaticDraw: r = GL_STATIC_DRAW; break;
    case bduDynamicDraw: r = GL_DYNAMIC_DRAW; break;
    case bduStreamDraw: r = GL_STREAM_DRAW; break;
    case bduDynamicCopy: r = GL_DYNAMIC_COPY; break;
    case bduStaticCopy: r = GL_STATIC_COPY; break;
    case bduStreamCopy: r = GL_STREAM_COPY; break;

    default: assert(0); break;
    }
    return r;
}

//-----------------------------------------------------------------------------
int Rendering::toGlBufferBindingTarget(BufferBindingTarget iBbt)
{
    int target = 0;
    switch (iBbt)
    {
    case bbtArrayBuffer: target = GL_ARRAY_BUFFER; break;	// VBO
    case bbtElementArrayBuffer: target = GL_ELEMENT_ARRAY_BUFFER;  break;	// IBO
    case bbtUniformBuffer: target = GL_UNIFORM_BUFFER;  break;	// UBO
    case bbtShaderStorageBuffer: target = GL_SHADER_STORAGE_BUFFER;  break;	// SSO
    // case bbtTransformFeedbackBuffer: target = GL_TRANSFORM_FEEDBACK_BUFFER;  break;	// TFB
    // case bbtAtomicCounterBuffer: target = GL_ATOMIC_COUNTER_BUFFER;  break;	// atomic counter
    // case bbtQueryBuffer: target = GL_QUERY_BUFFER;  break;
    // case bbtTextureBuffer: target = GL_TEXTURE_BUFFER;  break;
    // case bbtDrawIndirectBuffer: target = GL_DRAW_INDIRECT_BUFFER;  break;
    // case bbtDispatchIndirectBuffer: target = GL_DISPATCH_INDIRECT_BUFFER;  break;
    default: assert(0); break;
    }
    return target;
}

//-----------------------------------------------------------------------------
int Rendering::toGlDataType(DataType iDt)
{
    int dt = 0;
    switch (iDt)
    {
    case dtUndefined: dt = 0; break;
    case dtUnsignedByte: dt = GL_UNSIGNED_BYTE; break;
    case dtByte: dt = GL_BYTE; break;
    case dtUnsignedShort: dt = GL_UNSIGNED_SHORT; break;
    case dtShort: dt = GL_SHORT; break;
    case dtUnsignedInteger: dt = GL_UNSIGNED_INT; break;
    case dtInteger: dt = GL_INT; break;
    case dtHalfFloat: dt = GL_HALF_FLOAT; break;
    case dtFloat: dt = GL_FLOAT; break;
    case dtDouble: dt = GL_DOUBLE; break;
    case dtUnsignedInteger24_8: dt = GL_UNSIGNED_INT_24_8; break;
    default: assert(0); break;
    }
    return dt;
}

//-----------------------------------------------------------------------------
int Rendering::toGlDrawMode(DrawMode iDm)
{
    int dm = 0;
    switch (iDm)
    {
    case dmUndefined: dm = 0; break;
    case dmPoints: dm = GL_POINTS; break;
    case dmLineStrip: dm = GL_LINE_STRIP; break;
    case dmLineLoop: dm = GL_LINE_LOOP; break;
    case dmLines: dm = GL_LINES; break;
    case dmLineStripAdjacency: dm = GL_LINE_STRIP_ADJACENCY; break;
    case dmLineAdjacency: dm = GL_LINES_ADJACENCY; break;
    case dmTriangleStrip: dm = GL_TRIANGLE_STRIP; break;
    case dmTriangleFan: dm = GL_TRIANGLE_FAN; break;
    case dmTriangles: dm = GL_TRIANGLES; break;
    case dmTriangleStripAdjacency: dm = GL_TRIANGLE_STRIP_ADJACENCY; break;
    case dmTrianglesAdjacency: dm = GL_TRIANGLES_ADJACENCY; break;
    case dmPatches: dm = GL_PATCHES; break;
        //case dmQuads: dm = GL_QUADS; break; not supported in core 4.1 and up
    default: assert(0); break;
    }
    return dm;
}

//-----------------------------------------------------------------------------
int Rendering::toGlTextureInternalFormat(TextureInternalFormat iV)
{
    int r = 0;
    switch (iV)
    {
    case tifR8: r = GL_R8; break;
    case tifRg8: r = GL_RG8; break;
    case tifRgb8: r = GL_RGB8; break;
    case tifRgba8: r = GL_RGBA8; break;
    case tifSrgb8: r = GL_SRGB8; break;
    case tifSrgba8: r = GL_SRGB8_ALPHA8; break;
    case tifR16: r = GL_R16; break;
    case tifRg16: r = GL_RG16; break;
    case tifRgb16: r = GL_RGB16; break;
    case tifRgba16: r = GL_RGBA16; break;
    case tifR16f: r = GL_R16F; break;
    case tifRg16f: r = GL_RG16F; break;
    case tifRgb16f: r = GL_RGB16F; break;
    case tifRgba16f: r = GL_RGBA16F; break;
    case tifR32f: r = GL_R32F; break;
    case tifRg32f: r = GL_RG32F; break;
    case tifRgb32f: r = GL_RGB32F; break;
    case tifRgba32f: r = GL_RGBA32F; break;
    case tifR32i: r = GL_R32I; break;
    case tifRg32i: r = GL_RG32I; break;
    case tifRgb32i: r = GL_RGB32I; break;
    case tifRgba32i: r = GL_RGBA32I; break;
    case tifR32ui: r = GL_R32UI; break;
    case tifRg32ui: r = GL_RG32UI; break;
    case tifRgb32ui: r = GL_RGB32UI; break;
    case tifRgba32ui: r = GL_RGBA32UI; break;
    case tifCompressedRgbDxt1: r = GL_COMPRESSED_RGB_S3TC_DXT1_EXT; break;
    case tifCompressedSrgbDxt1: r = GL_COMPRESSED_SRGB_S3TC_DXT1_EXT; break;
    case tifCompressedRgbaDxt1: r = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT; break;
    case tifCompressedRgbaDxt3: r = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT; break;
    case tifCompressedRgbaDxt5: r = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; break;
    case tifCompressedSrgbaDxt1: r = GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT; break;
    case tifCompressedSrgbaDxt3: r = GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT; break;
    case tifCompressedSrgbaDxt5: r = GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT; break;
    case tifDepthInteger16: r = GL_DEPTH_COMPONENT16; break;
    case tifDepthInteger24: r = GL_DEPTH_COMPONENT24; break;
    case tifDepthInteger32: r = GL_DEPTH_COMPONENT32; break;
    case tifDepthFloat32: r = GL_DEPTH_COMPONENT32F; break;
    case tifDepthInteger24StencilInteger8: r = GL_DEPTH24_STENCIL8; break;
    case tifDepthFloat32StencilInteger8: r = GL_DEPTH32F_STENCIL8; break;
    case tifStencilIndexedInteger4: r = GL_STENCIL_INDEX4; break;
    case tifStencilIndexedInteger8: r = GL_STENCIL_INDEX8; break;
    case tifStencilIndexedInteger16: r = GL_STENCIL_INDEX16; break;
    default: assert(0); break;
    }
    return r;
}

//-----------------------------------------------------------------------------
int Rendering::toGlTextureFilter(TextureFilter iV)
{
    int r = 0;
    switch (iV)
    {
    case tfNearest: r = GL_NEAREST; break;
    case tfLinear: r = GL_LINEAR; break;
    case tfNearestMipmapNearest: r = GL_NEAREST_MIPMAP_NEAREST; break;
    case tfLinearMipmapNearest: r = GL_LINEAR_MIPMAP_NEAREST; break;
    case tfNearestMipmapLinear: r = GL_NEAREST_MIPMAP_LINEAR; break;
    case tfLinearMipmapLinear: r = GL_LINEAR_MIPMAP_LINEAR; break;
    default: assert(0); break;
    }
    return r;
}

//-----------------------------------------------------------------------------
int Rendering::toGlTextureFormat(TextureFormat iV)
{
    int r = 0;
    switch (iV)
    {
    case tfRed: r = GL_RED; break;
    case tfRg: r = GL_RG; break;
    case tfRgb: r = GL_RGB; break;
    case tfRgba: r = GL_RGBA; break;
    case tfBgr: r = GL_BGR; break;
    case tfBgra: r = GL_BGRA; break;
    case tfStencil: r = GL_STENCIL_INDEX; break;
    case tfDepth: r = GL_DEPTH_COMPONENT; break;
    case tfDepthStencil: r = GL_DEPTH_STENCIL; break;
    case tfRgba_integer: r = GL_RGBA_INTEGER; break;
    case tfBgra_integer: r = GL_BGRA_INTEGER; break;

    default: assert(0); break;
    }
    return r;
}

//-----------------------------------------------------------------------------
int Rendering::toGlTextureTarget(TextureTarget iV)
{
    int r = 0;
    switch (iV)
    {
    case tt2d: r = GL_TEXTURE_2D; break;
    case tt2dMultisampled: r = GL_TEXTURE_2D_MULTISAMPLE; break;
    case tt2dArray: r = GL_TEXTURE_2D_ARRAY; break;
    case tt3d: r = GL_TEXTURE_3D; break;
    case ttCubeMapPositiveX: r = GL_TEXTURE_CUBE_MAP_POSITIVE_X; break;
    case ttCubeMapNegativeX: r = GL_TEXTURE_CUBE_MAP_NEGATIVE_X; break;
    case ttCubeMapPositiveY: r = GL_TEXTURE_CUBE_MAP_POSITIVE_Y; break;
    case ttCubeMapNegativeY: r = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y; break;
    case ttCubeMapPositiveZ: r = GL_TEXTURE_CUBE_MAP_POSITIVE_Z; break;
    case ttCubeMapNegativeZ: r = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z; break;
    default: assert(0); break;
    }
    return r;
}

//-----------------------------------------------------------------------------
std::string Rendering::toString(FrameBufferAttachementType o)
{
    std::string r = "unknown";
    switch (o)
    {
    case fbaColor0: r = "fbaColor0"; break;
    case fbaColor1: r = "fbaColor1"; break;
    case fbaColor2: r = "fbaColor2"; break;
    case fbaColor3: r = "fbaColor3"; break;
    case fbaColor4: r = "fbaColor4"; break;
    case fbaColor5: r = "fbaColor5"; break;
    case fbaColor6: r = "fbaColor6"; break;
    case fbaColor7: r = "fbaColor7"; break;
    case fbaColor8: r = "fbaColor8"; break;
    case fbaColor9: r = "fbaColor9"; break;
    case fbaColor10: r = "fbaColor10"; break;
    case fbaColor11: r = "fbaColor11"; break;
    case fbaColor12: r = "fbaColor12"; break;
    case fbaColor13: r = "fbaColor13"; break;
    case fbaColor14: r = "fbaColor14"; break;
    case fbaColor15: r = "fbaColor15"; break;
    case fbaDepthOnly: r = "fbaDepthOnly"; break;
    case fbaStencilOnly: r = "fbaStencilOnly"; break;
    case fbaDepthStencil: r = "fbaDepthStencil"; break;
    default: assert(0); break;
    }
    return r;
}

//-----------------------------------------------------------------------------
std::string Rendering::toString(TextureFilter iV)
{
    std::string r = "unknown";

    switch (iV)
    {
    case tfNearest: r = "nearest"; break;
    case tfLinear: r = "linear"; break;
    case tfNearestMipmapNearest: r = "nearest mipmap nearest"; break;
    case tfLinearMipmapNearest: r = "linear mipmap nearest"; break;
    case tfNearestMipmapLinear: r = "nearest mipmap linear"; break;
    case tfLinearMipmapLinear: r = "linear mipmap linear"; break;
    default: break;
    }

    return r;
}

//-----------------------------------------------------------------------------
std::string Rendering::toString(TextureWrapMode iV)
{
    std::string r = "unknown";

    switch (iV)
    {
    case twmRepeat: r = "repeat"; break;
    case twmClampToBorder: r = "clamp to border"; break;
    case twmClampToEdge: r = "clamp to edge"; break;
    case twmMirroredRepeat: r = "mirrored repeat"; break;
    default: break;
    }

    return r;
}

//-----------------------------------------------------------------------------
int Rendering::toGlFrameBufferAttachement(FrameBufferAttachementType o)
{
    int r = 0;
    switch (o)
    {
    case fbaColor0: r = GL_COLOR_ATTACHMENT0; break;
    case fbaColor1: r = GL_COLOR_ATTACHMENT1; break;
    case fbaColor2: r = GL_COLOR_ATTACHMENT2; break;
    case fbaColor3: r = GL_COLOR_ATTACHMENT3; break;
    case fbaColor4: r = GL_COLOR_ATTACHMENT4; break;
    case fbaColor5: r = GL_COLOR_ATTACHMENT5; break;
    case fbaColor6: r = GL_COLOR_ATTACHMENT6; break;
    case fbaColor7: r = GL_COLOR_ATTACHMENT7; break;
    case fbaColor8: r = GL_COLOR_ATTACHMENT8; break;
    case fbaColor9: r = GL_COLOR_ATTACHMENT9; break;
    case fbaColor10: r = GL_COLOR_ATTACHMENT10; break;
    case fbaColor11: r = GL_COLOR_ATTACHMENT11; break;
    case fbaColor12: r = GL_COLOR_ATTACHMENT12; break;
    case fbaColor13: r = GL_COLOR_ATTACHMENT13; break;
    case fbaColor14: r = GL_COLOR_ATTACHMENT14; break;
    case fbaColor15: r = GL_COLOR_ATTACHMENT15; break;
    case fbaDepthOnly: r = GL_DEPTH_ATTACHMENT; break;
    case fbaStencilOnly: r = GL_STENCIL_ATTACHMENT; break;
    case fbaDepthStencil: r = GL_DEPTH_STENCIL_ATTACHMENT; break;
    default: assert(0); break;
    }
    return r;
}

//-----------------------------------------------------------------------------
int Rendering::toGlTextureWrapMode(TextureWrapMode iV)
{
    int r = 0;
    switch (iV)
    {
    case twmRepeat: r = GL_REPEAT; break;
    case twmClampToBorder: r = GL_CLAMP_TO_BORDER; break;
    case twmClampToEdge: r = GL_CLAMP_TO_EDGE; break;
    case twmMirroredRepeat: r = GL_MIRRORED_REPEAT; break;
    default: assert(0); break;
    }
    return r;
}

//-----------------------------------------------------------------------------
int Rendering::toGlShaderType(ShaderType iSt)
{
    int shaderType = 0;
    switch (iSt)
    {
    case stFragment: shaderType = GL_FRAGMENT_SHADER; break;
    case stVertex: shaderType = GL_VERTEX_SHADER; break;
    case stGeometry: shaderType = GL_GEOMETRY_SHADER; break;
    case stTesselationControl: shaderType = GL_TESS_CONTROL_SHADER; break;
    case stTesselationEvaluation: shaderType = GL_TESS_EVALUATION_SHADER; break;
    case stComputeShader: shaderType = GL_COMPUTE_SHADER; break;
    default: break;
    }
    return shaderType;
}

int Rendering::toGlBlendFunction(BlendFunction iBf)
{
    int func = 0;
    switch (iBf)
    {
    case bfZero: func = GL_ZERO; break;
    case bfOne: func = GL_ONE; break;
    case bfSourceColor: func = GL_SRC_COLOR; break;
    case bfOneMinusSourceColor: func = GL_ONE_MINUS_SRC_COLOR; break;
    case bfDestinationColor: func = GL_DST_COLOR; break;
    case bfOneMinusDestinationColor: func = GL_ONE_MINUS_DST_COLOR; break;
    case bfSourceAlpha: func = GL_SRC_ALPHA; break;
    case bfOneMinusSourceAlpha: func = GL_ONE_MINUS_SRC_ALPHA; break;
    case bfDestinationAlpha: func = GL_DST_ALPHA; break;
    case bfOneMinusDestinationAlpha: func = GL_ONE_MINUS_DST_ALPHA; break;
    case bfConstantColor: func = GL_CONSTANT_COLOR; break;
    case bfOneMinusConstantColor: func = GL_ONE_MINUS_CONSTANT_COLOR; break;
    case bfConstantAlpha: func = GL_CONSTANT_ALPHA; break;
    case bfOneMinusConstantAlpha: func = GL_ONE_MINUS_CONSTANT_ALPHA; break;
    case bfSourceAlphaSaturate: func = GL_SRC_ALPHA_SATURATE; break;
    case bfSource1Color: func = GL_SRC1_COLOR; break;
    case bfOneMinusSource1Color: func = GL_ONE_MINUS_SRC1_COLOR; break;
    case bfSource1Alpha: func = GL_SRC1_ALPHA; break;
    case bfOneMinusSource1Alpha: func = GL_ONE_MINUS_SRC1_ALPHA; break;

    default: break;
    }
    return func;
}

int Rendering::toGlBlendEquation(BlendEquation iBe)
{
    int eq = 0;
    switch (iBe)
    {
    case beAdditive: eq = GL_FUNC_ADD; break;
    case beSubstract: eq = GL_FUNC_SUBTRACT; break;
    case beReverseSubstract: eq = GL_FUNC_REVERSE_SUBTRACT; break;
    case beMin: eq = GL_MIN; break;
    case beMax: eq = GL_MAX; break;
    default: break;
    }
    return eq;
}

//Rendering::TextureFormatDefinition   Rendering::toTextureFormatDefinition(Core::DdsImage::CompressionType compression, bool gammaCorrected)
//{
//    Rendering::TextureFormatDefinition data;
//    data.textureInternalFormat = Rendering::TextureInternalFormat::tifRgba8;
//    data.type = Rendering::DataType::dtUndefined;
//    data.textureFormat = Rendering::TextureFormat::tfRgba;

//    switch (compression)
//    {
//    case Core::DdsImage::CompressionType::ctDxt1:
//        data.textureInternalFormat = gammaCorrected ? Rendering::TextureInternalFormat::tifCompressedSrgbDxt1 : Rendering::TextureInternalFormat::tifCompressedRgbDxt1;
//        data.type = Rendering::DataType::dtUnsignedByte;
//        data.textureFormat = Rendering::TextureFormat::tfRgb;
//        break;
//    case Core::DdsImage::CompressionType::ctDxt3:
//        data.textureInternalFormat = gammaCorrected ? Rendering::TextureInternalFormat::tifCompressedSrgbaDxt3 : Rendering::TextureInternalFormat::tifCompressedRgbaDxt3;
//        data.type = Rendering::DataType::dtUnsignedByte;
//        data.textureFormat = Rendering::TextureFormat::tfRgba;
//        break;

//    case Core::DdsImage::CompressionType::ctDxt5:
//        data.textureInternalFormat = gammaCorrected ? Rendering::TextureInternalFormat::tifCompressedSrgbaDxt5 : Rendering::TextureInternalFormat::tifCompressedRgbaDxt5;
//        data.type = Rendering::DataType::dtUnsignedByte;
//        data.textureFormat = Rendering::TextureFormat::tfRgba;
//        break;

//    case Core::DdsImage::CompressionType::ctNone:
//        // this is a bit of a hack... 
//        // Dds, supports an non compressed format which will always be bgra...
//        // The datatype might not always be dtUnsignedByte. To support the correct datatype,
//        // we would have to pass a bit more info to this function.
//        //
//        data.textureInternalFormat = gammaCorrected ? Rendering::TextureInternalFormat::tifSrgba8 : Rendering::TextureInternalFormat::tifRgba8;
//        data.type = Rendering::DataType::dtUnsignedByte;
//        data.textureFormat = Rendering::TextureFormat::tfBgra;
//    default:
//        break;
//    }

//    return data;

//}

Rendering::TextureFormatDefinition   Rendering::toTextureFormatDefinition(Core::ImageInternalFormat o, bool iGammaCorrected)
{
    Rendering::TextureFormatDefinition data;
    data.textureInternalFormat = iGammaCorrected ? Rendering::TextureInternalFormat::tifSrgba8 : Rendering::TextureInternalFormat::tifRgba8;
    data.type = Rendering::DataType::dtUnsignedByte;
    data.textureFormat = Rendering::TextureFormat::tfRgba;

    switch (o)
    {
    case Core::ImageInternalFormat::iifRUint8:
        data.textureInternalFormat = Rendering::TextureInternalFormat::tifR8;
        data.type = Rendering::DataType::dtUnsignedByte;
        data.textureFormat = Rendering::TextureFormat::tfRed;
        break;
    case Core::ImageInternalFormat::iifRInt8:
        data.textureInternalFormat = Rendering::TextureInternalFormat::tifR8;
        data.type = Rendering::DataType::dtByte;
        data.textureFormat = Rendering::TextureFormat::tfRed;
        break;

    case Core::ImageInternalFormat::iifRUint16:
        data.textureInternalFormat = Rendering::TextureInternalFormat::tifR16;
        data.type = Rendering::DataType::dtUnsignedShort;
        data.textureFormat = Rendering::TextureFormat::tfRed;
        break;

    case Core::ImageInternalFormat::iifRInt16:
        data.textureInternalFormat = Rendering::TextureInternalFormat::tifR16;
        data.type = Rendering::DataType::dtShort;
        data.textureFormat = Rendering::TextureFormat::tfRed;
        break;

    case Core::ImageInternalFormat::iifRF16:
        data.textureInternalFormat = Rendering::TextureInternalFormat::tifR16f;
        data.type = Rendering::DataType::dtHalfFloat;
        data.textureFormat = Rendering::TextureFormat::tfRed;
        break;

    case Core::ImageInternalFormat::iifRUint32:
        data.textureInternalFormat = Rendering::TextureInternalFormat::tifR32ui;
        data.type = Rendering::DataType::dtUnsignedInteger;
        data.textureFormat = Rendering::TextureFormat::tfRed;
        break;

    case Core::ImageInternalFormat::iifRInt32:
        data.textureInternalFormat = Rendering::TextureInternalFormat::tifR32i;
        data.type = Rendering::DataType::dtInteger;
        data.textureFormat = Rendering::TextureFormat::tfRed;
        break;

    case Core::ImageInternalFormat::iifRF32:
        data.textureInternalFormat = Rendering::TextureInternalFormat::tifR32f;
        data.type = Rendering::DataType::dtFloat;
        data.textureFormat = Rendering::TextureFormat::tfRed;
        break;

    case Core::ImageInternalFormat::iifRgbUint8:
        data.textureInternalFormat = iGammaCorrected ? Rendering::TextureInternalFormat::tifSrgb8 : Rendering::TextureInternalFormat::tifRgb8;
        data.type = Rendering::DataType::dtUnsignedByte;
        data.textureFormat = Rendering::TextureFormat::tfRgb;
        break;

    case Core::ImageInternalFormat::iifRgbInt8:
        data.textureInternalFormat = iGammaCorrected ? Rendering::TextureInternalFormat::tifSrgb8 : Rendering::TextureInternalFormat::tifRgb8;
        data.type = Rendering::DataType::dtByte;
        data.textureFormat = Rendering::TextureFormat::tfRgb;
        break;

    case Core::ImageInternalFormat::iifRgbUint16:
        data.textureInternalFormat = Rendering::TextureInternalFormat::tifRgb16;
        data.type = Rendering::DataType::dtUnsignedShort;
        data.textureFormat = Rendering::TextureFormat::tfRgb;
        break;

    case Core::ImageInternalFormat::iifRgbInt16:
        data.textureInternalFormat = Rendering::TextureInternalFormat::tifRgb16;
        data.type = Rendering::DataType::dtShort;
        data.textureFormat = Rendering::TextureFormat::tfRgb;
        break;

    case Core::ImageInternalFormat::iifRgbF16:
        data.textureInternalFormat = Rendering::TextureInternalFormat::tifRgb16f;
        data.type = Rendering::DataType::dtHalfFloat;
        data.textureFormat = Rendering::TextureFormat::tfRgb;
        break;

    case Core::ImageInternalFormat::iifRgbUint32:
        data.textureInternalFormat = Rendering::TextureInternalFormat::tifRgb32ui;
        data.type = Rendering::DataType::dtUnsignedInteger;
        data.textureFormat = Rendering::TextureFormat::tfRgb;
        break;

    case Core::ImageInternalFormat::iifRgbInt32:
        data.textureInternalFormat = Rendering::TextureInternalFormat::tifRgb32i;
        data.type = Rendering::DataType::dtInteger;
        data.textureFormat = Rendering::TextureFormat::tfRgb;
        break;

    case Core::ImageInternalFormat::iifRgbF32:
        data.textureInternalFormat = Rendering::TextureInternalFormat::tifRgb32f;
        data.type = Rendering::DataType::dtFloat;
        data.textureFormat = Rendering::TextureFormat::tfRgb;
        break;

    case Core::ImageInternalFormat::iifRgbaUint8:
        data.textureInternalFormat = iGammaCorrected ? Rendering::TextureInternalFormat::tifSrgba8 : Rendering::TextureInternalFormat::tifRgba8;
        data.type = Rendering::DataType::dtUnsignedByte;
        data.textureFormat = Rendering::TextureFormat::tfRgba;
        break;

    case Core::ImageInternalFormat::iifRgbaInt8:
        data.textureInternalFormat = iGammaCorrected ? Rendering::TextureInternalFormat::tifSrgba8 : Rendering::TextureInternalFormat::tifRgba8;
        data.type = Rendering::DataType::dtByte;
        data.textureFormat = Rendering::TextureFormat::tfRgba;
        break;

    case Core::ImageInternalFormat::iifRgbaUint16:
        data.textureInternalFormat = Rendering::TextureInternalFormat::tifRgba16;
        data.type = Rendering::DataType::dtUnsignedShort;
        data.textureFormat = Rendering::TextureFormat::tfRgba;
        break;

    case Core::ImageInternalFormat::iifRgbaInt16:
        data.textureInternalFormat = Rendering::TextureInternalFormat::tifRgba16;
        data.type = Rendering::DataType::dtShort;
        data.textureFormat = Rendering::TextureFormat::tfRgba;
        break;

    case Core::ImageInternalFormat::iifRgbaF16:
        data.textureInternalFormat = Rendering::TextureInternalFormat::tifRgba16f;
        data.type = Rendering::DataType::dtHalfFloat;
        data.textureFormat = Rendering::TextureFormat::tfRgba;
        break;

    case Core::ImageInternalFormat::iifRgbaUint32:
        data.textureInternalFormat = Rendering::TextureInternalFormat::tifRgba32ui;
        data.type = Rendering::DataType::dtUnsignedInteger;
        data.textureFormat = Rendering::TextureFormat::tfRgba;
        break;

    case Core::ImageInternalFormat::iifRgbaInt32:
        data.textureInternalFormat = Rendering::TextureInternalFormat::tifRgba32i;
        data.type = Rendering::DataType::dtInteger;
        data.textureFormat = Rendering::TextureFormat::tfRgba;
        break;

    case Core::ImageInternalFormat::iifRgbaF32:
        data.textureInternalFormat = Rendering::TextureInternalFormat::tifRgba32f;
        data.type = Rendering::DataType::dtFloat;
        data.textureFormat = Rendering::TextureFormat::tfRgba;
        break;


    case Core::ImageInternalFormat::iifUndefined:
    default:
        break;
    }

    return data;
}

