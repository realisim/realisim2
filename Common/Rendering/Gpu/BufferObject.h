
#pragma once

#include "Math/Vector.h"
#include "Rendering/Gpu/BufferObjectData.h"
#include "Rendering/Gpu/DataType.h"
#include "Rendering/Gpu/Shader.h"

#include <vector>
#include <memory>

namespace Realisim
{
namespace Rendering
{
    //****************************************************
    //  BufferObject handles a GPU buffer data.
    //  You must provide the data structure of your buffer via a pointer to an instance of BufferObjectData.
    //  The class will take the ownership of the provided BufferObjectData pointer, do not use the pointer after.
    //  For a uniform buffer, you must provide a pointer to BufferObjectDataInterleaved, since the class will add padding if needed.
    //  
    //  https://www.khronos.org/opengl/wiki/Buffer_Object#Alignment
    //
    //  Normal buffer:
    //  - BufferBindingTarget::bbtArrayBuffer
    //  - BufferBindingTarget::bbtElementArrayBuffer
    //  - BufferBindingTarget::bbtQueryBuffer (NOT TESTED YET 2018/02/07)
    //  - BufferBindingTarget::bbtTextureBuffer (NOT TESTED YET 2018/02/07)
    //  - BufferBindingTarget::bbtDrawIndirectBuffer (NOT TESTED YET 2018/02/07)
    //  - BufferBindingTarget::bbtDispatchIndirectBuffer (NOT TESTED YET 2018/02/07)
    //  
    //  Indexed buffer:
    //  -BufferBindingTarget::bbtUniformBuffer
    //  -BufferBindingTarget::bbtShaderStorageBuffer (padding is not mandatory)
    //  -BufferBindingTarget::bbtTransformFeedbackBuffer (NOT TESTED YET 2018/02/07)
    //  -BufferBindingTarget::bbtAtomicCounterBuffer (NOT TESTED YET 2018/02/07)
    //
    //******************************************************
    
    enum BufferObjectMappingMode : int // it is a binary mask !
    {
        read = 1,
        write = 2,
        read_write = 3,
        flush_entirebuffer = 4
    };

    class BufferObject
    {
    public:

        enum LayoutLocationIndex { lliUndefined=-1, lliVertex=0, lliNormal, lliTexture, lliColor };

        BufferObject();
        ~BufferObject();
        BufferObject(const BufferObject&) = delete;
        BufferObject& operator=(const BufferObject&) = delete;
                
        void clear();
        
        BufferDataUsage getBufferDataUsage() const;                
        BufferBindingTarget getBufferBindingTarget() const;
        int getId() const;
       
        void* mapBuffer(unsigned int offset, unsigned int sizeInBytes, BufferObjectMappingMode mode);
        void unmapBuffer(void*);

        void setDataStructure(Rendering::BufferObjectData* dataStructure);
        void assignData(BufferBindingTarget iType, BufferDataUsage iUsage, uint64_t iNumberOfElements, const void *ipData);

		void bind() const;
        void bindBufferBase();
        void bindBufferTargetRange(uint64_t offset, uint64_t size);
		void unbind() const;
        void unbindBufferBase();
        
        int getNumberOfAttributes() const;
        uint64_t getNumberOfElements() const;
        uint64_t getSizeInBytes() const;
        int getInterleavedOffsetInBytes(int i) const;
        int getInterleaveNumberOfComponents(int i) const;
        int getLayoutLocationIndex(int i) const;
        DataType getDataType(int i) const;

        int getBufferObjectDataSizeInBytes() const;

        static int getAlignement(BufferBindingTarget iType);

        int getElementSizeInBytes() const;

    protected:
        void allocateGlResources(const void *ipData);

        //--- data
        unsigned int mId;
        BufferBindingTarget mBufferBindingTarget;
        BufferDataUsage mUsage;
        
        std::unique_ptr<Rendering::BufferObjectData>   mDataStructure;

        uint64_t mNumberOfElements;
        uint64_t mTotalSizeInBytes;
    };

}
}