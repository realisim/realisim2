
#include <algorithm> //min and max
#include <cassert>

#include "Rendering/Gpu/OpenGlHeaders.h"
#include "Rendering/Gpu/BufferObject.h"

using namespace Realisim;
using namespace Math;
using namespace Rendering;
using namespace std;


BufferObject::BufferObject() :
    mId(0),
    mBufferBindingTarget(bbtUndefined),
    mUsage(bduUndefined),
    mDataStructure(nullptr),
    mNumberOfElements(0),
    mTotalSizeInBytes(0)
{
}

//----------------------------------------------------------------------------
BufferObject::~BufferObject()
{
    clear();
}

//----------------------------------------------------------------------------
void BufferObject::allocateGlResources(const void *ipData)
{
    if (getId() != 0)
    {
        clear();
    }

    const void *dataPtr = ipData;

    //--- create opengl resource.
    GLenum target = (GLenum)toGlBufferBindingTarget( getBufferBindingTarget() );
    GLenum usage = toGlBufferDataUsage(getBufferDataUsage());

    glGenBuffers(1, &mId);
    glBindBuffer(target, mId);

    //--- assign data using map buffer
		
	// From Vertex buffer Object Songho tutorials...
	//
	// To avoid waiting(idle), you can call first glBufferDataARB() with 
	// NULL pointer, then call glMapBufferARB().In this case, the previous
	// data will be discarded and glMapBufferARB() returns a new allocated
	// pointer immediately even if GPU is still working with the previous data
	//
    int size = (int)getSizeInBytes();
	glBufferData(target,size, 0, usage);
	
	if(dataPtr)	// if we have datas to put in the buffer
	{
		void *ptr = (void*)glMapBuffer(target, GL_WRITE_ONLY);
		if (ptr)
		{
			memcpy(ptr, dataPtr, size);
			glUnmapBuffer(target);
		}
	}

    //--------------------------------------------------------------
    // Assign data without using map buffer... it has a tendency to
    // hang the GPU... leaving here for debugging purposes.
    //
    //glBufferData(target, mTotalSizeInBytes, dataPtr, usage);
    //--------------------------------------------------------------

    glBindBuffer(target, 0);
}

//----------------------------------------------------------------------------
void BufferObject::clear()
{
    if (getId() != 0)
    {
        glDeleteBuffers(1, (GLuint*)&mId);
        mId = 0;
    }
    
    mBufferBindingTarget = bbtUndefined;
    mUsage = bduUndefined;
    
    mNumberOfElements = 0;
    mTotalSizeInBytes = 0;
}



//----------------------------------------------------------------------------
BufferDataUsage BufferObject::getBufferDataUsage() const
{
    return mUsage;
}

//-----------------------------------------------------------------------------
DataType BufferObject::getDataType(int iIndex) const
{
    if(mDataStructure)
    {
        return mDataStructure->getDataType(iIndex);
    }
    else
    {
        return DataType::dtUndefined;
    }
}


//-----------------------------------------------------------------------------
int BufferObject::getLayoutLocationIndex(int iIndex) const
{
    if(mDataStructure)
    {
        return mDataStructure->getLayoutLocationIndex(iIndex);
    }
    else
    {
        return -1;
    }
}

//-----------------------------------------------------------------------------
int BufferObject::getInterleaveNumberOfComponents(int iIndex) const
{
    if(mDataStructure)
    {
        return mDataStructure->getNumberOfComponents(iIndex);
    }
    else
    {
        return 0;
    }
}

int BufferObject::getBufferObjectDataSizeInBytes() const
{
    if(mDataStructure)
    {
        return mDataStructure->getSizeInBytes();
    }
    else
    {
        return 0;
    }
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
int BufferObject::getInterleavedOffsetInBytes(int iIndex) const
{
    if(mDataStructure)
    {
        return mDataStructure->getOffsetInBytes(iIndex);
    }
    else
    {
        return -1;
    }
}


//----------------------------------------------------------------------------
uint64_t BufferObject::getNumberOfElements() const
{
    return mNumberOfElements;
}

//----------------------------------------------------------------------------
int BufferObject::getNumberOfAttributes() const
{
    if(mDataStructure)
    {
        return mDataStructure->getNumberOfAttributes();
    }
    else
    {
        return 0;
    }
}

//----------------------------------------------------------------------------
uint64_t BufferObject::getSizeInBytes() const
{
    return mTotalSizeInBytes;
}

//----------------------------------------------------------------------------
BufferBindingTarget BufferObject::getBufferBindingTarget() const
{
    return mBufferBindingTarget;
}

//----------------------------------------------------------------------------
int BufferObject::getId() const
{ return (int)mId; }

void BufferObject::setDataStructure(BufferObjectData* dataStructure)
{
    mDataStructure.reset(dataStructure);    // take ownership
}

void BufferObject::assignData(BufferBindingTarget iType, BufferDataUsage iUsage, uint64_t iNumberOfElements, const void *ipData)
{
    if(mDataStructure)
    {
        //--- assign member variable
        mBufferBindingTarget = iType;
        mUsage = iUsage;        
        mNumberOfElements = iNumberOfElements;        

        int inputElementSize = mDataStructure->getSizeInBytes();

        // take care of padding if needed
        int alignement = this->getAlignement(mBufferBindingTarget);

        // OpenGL 4.5, 7.6.2.2 Standard Uniform Block Layout, rule 4
        if( (mBufferBindingTarget == BufferBindingTarget::bbtShaderStorageBuffer)
            &&
            (mDataStructure->getNumberOfAttributes() == 1) )
        {
            int comp_size = mDataStructure->getSizeInBytes()/mDataStructure->getNumberOfComponents(0);

            // if you have a SSBO of vec3, it must be converted to vec4 (the shader will access it as vec4)
            if(mDataStructure->getNumberOfComponents(0) == 3)
            {
                alignement = 4*comp_size;
            }
            else
            {
                alignement = mDataStructure->getNumberOfComponents(0)*comp_size;
            }
            
            // if you provide a structure, the alignement is the size of the structure (to be tested)
        }
        int padding = mDataStructure->pad(alignement);

        // final size
        mTotalSizeInBytes = getNumberOfElements()*mDataStructure->getSizeInBytes();

        // do the padding
        unsigned char*  padded = (unsigned char*)ipData;
        if(padding && padded)
        {
            // we do a copy with padding of the data
            padded = new unsigned char[mTotalSizeInBytes];
            memset(padded,0,mTotalSizeInBytes);

            const unsigned char* in = (const unsigned char*)ipData;            
            // now do the padding
            for(int i=0;i<mNumberOfElements;++i)
            {
                int offsetOut = i*mDataStructure->getSizeInBytes();
                int offsetIn = i*inputElementSize;

                memcpy(&padded[offsetOut], &in[offsetIn], inputElementSize);
            }
        }
        
        // go in the GPU
        allocateGlResources(padded);

        // release memory if needs
        if(padding && padded)
        {
            // we release the padded copy
            delete[] padded;
            padded = nullptr;
        }
    }
}

int BufferObject::getElementSizeInBytes() const
{
    if(mDataStructure)
    {
        return mDataStructure->getSizeInBytes();
    }
    else
    {
        return 0;
    }
}

//----------------------------------------------------------------------------
void* BufferObject::mapBuffer( unsigned int offset,
    unsigned int size,
    BufferObjectMappingMode mode)
{
	void *map = nullptr;
	if(mId && size)
	{	
		GLenum target = (GLenum)toGlBufferBindingTarget(getBufferBindingTarget());
		glBindBuffer(target,mId);
        
		// https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glMapBufferRange.xhtml
		GLbitfield access = 0;

        if(mode & BufferObjectMappingMode::read)
            access |= GL_MAP_READ_BIT;
        if(mode & BufferObjectMappingMode::write)
            access |= GL_MAP_WRITE_BIT;

        if(mode & BufferObjectMappingMode::flush_entirebuffer)
            access |= GL_MAP_INVALIDATE_RANGE_BIT;  //  we make sure the region we update is flushed

        map = (void*)glMapBufferRange(target,offset,size,access);
    }

    if(size == 0)    
    {
        std::cout<<"Trying to map a buffer of size 0B"<<std::endl;
    }

	return map;
}

//----------------------------------------------------------------------------
void BufferObject::unmapBuffer(void* map)
{
    if(map)
    {
        GLenum target = (GLenum)toGlBufferBindingTarget(getBufferBindingTarget());
        glUnmapBuffer(target);
        glBindBuffer(target,0);
    }
}

void BufferObject::bind() const
{
    GLenum target = (GLenum)toGlBufferBindingTarget( getBufferBindingTarget() );
    glBindBuffer(target,mId);
}

void BufferObject::unbind() const
{
    GLenum target = (GLenum)toGlBufferBindingTarget( getBufferBindingTarget() );
    glBindBuffer(target,0);
}

void BufferObject::bindBufferTargetRange(uint64_t offset, uint64_t size)
{
    if( mDataStructure )
    {
        GLenum target = (GLenum)toGlBufferBindingTarget(getBufferBindingTarget());

		int bindingPoint = mDataStructure->getLayoutLocationIndex(0);
		glBindBufferRange(target,bindingPoint,this->getId(),offset,size);
    }
}

void BufferObject::bindBufferBase()
{
    if( mDataStructure )
    {
        GLenum target = (GLenum)toGlBufferBindingTarget(getBufferBindingTarget());

		int bindingPoint = mDataStructure->getLayoutLocationIndex(0);
        glBindBufferBase(target,bindingPoint,this->getId());
    }
}

void BufferObject::unbindBufferBase()
{
    if( mDataStructure )
    {
        GLenum target = (GLenum)toGlBufferBindingTarget(getBufferBindingTarget());

		int bindingPoint = mDataStructure->getLayoutLocationIndex(0);
        glBindBufferBase(target,bindingPoint,0);
    }
}

int BufferObject::getAlignement(BufferBindingTarget iType)
{
    GLenum target = (GLenum)toGlBufferBindingTarget(iType);

    int alignement = 1;
    if(target == GL_UNIFORM_BUFFER)
        glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT,&alignement);
    if(target == GL_SHADER_STORAGE_BUFFER)
        glGetIntegerv(GL_SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT,&alignement);    

    return alignement;
}
