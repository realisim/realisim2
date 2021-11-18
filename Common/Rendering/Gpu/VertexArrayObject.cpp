
#include "Rendering/Gpu/DataType.h"
#include "Rendering/Gpu/OpenGlHeaders.h"
#include "Rendering/Gpu/VertexArrayObject.h"

using namespace Realisim;
using namespace Math;
using namespace Rendering;

namespace
{
    enum layoutPosition { lpVertex = 0,lpNormal,lpColor,lp2dTextureCoord };
}

//-----------------------------------------------------------------------------
VertexArrayObject::VertexArrayObject() :
    mVaoId(0),
    mDrawMode(dmUndefined),
    mElementArrayDataType(dtUndefined),
    mElementArrayNumberOfElements(0)
{}

//-----------------------------------------------------------------------------
VertexArrayObject::~VertexArrayObject()
{
    clear();
}

//-----------------------------------------------------------------------------
void VertexArrayObject::add(BufferObject* iVbo)
{
    mVbosToBake.push_back(std::make_pair(false,iVbo));
}

void VertexArrayObject::addAndTakeOwnership(BufferObject* iVbo)
{
    mVbosToBake.push_back(std::make_pair(true,iVbo));
}

//-----------------------------------------------------------------------------
void VertexArrayObject::bake(DrawMode iDm)
{
    if (getVaoId() != 0)
    { clear(); }

    mDrawMode = iDm;

    glGenVertexArrays(1, &mVaoId);
    glBindVertexArray(mVaoId);

    for (auto vboPair : mVbosToBake)
    {
        BufferObject* pVbo = vboPair.second;

        GLenum target = (GLenum)toGlBufferBindingTarget(pVbo->getBufferBindingTarget());
        pVbo->bind();        
        mBindedVboIds.push_back(pVbo->getId());

        if (target == GL_ARRAY_BUFFER)
        {
            for (int i = 0; i < pVbo->getNumberOfAttributes(); ++i)
            {
                const GLenum glDataType = toGlDataType(pVbo->getDataType(i));
                const int attribArrayIndex = pVbo->getLayoutLocationIndex(i);
                glEnableVertexAttribArray(attribArrayIndex);

                if (pVbo->getDataType(i) == DataType::dtDouble)
                {
                    glVertexAttribLPointer(attribArrayIndex,
                        pVbo->getInterleaveNumberOfComponents(i),
                        glDataType,
                        pVbo->getBufferObjectDataSizeInBytes(),
                        BUFFER_OFFSET(pVbo->getInterleavedOffsetInBytes(i)));
                }
                else if (pVbo->getDataType(i) == DataType::dtFloat)
                {
                    glVertexAttribPointer(attribArrayIndex,
                        pVbo->getInterleaveNumberOfComponents(i),
                        glDataType, GL_FALSE,
                        pVbo->getBufferObjectDataSizeInBytes(),
                        BUFFER_OFFSET(pVbo->getInterleavedOffsetInBytes(i)));
                }
                else
                {
                    glVertexAttribIPointer(attribArrayIndex,
                        pVbo->getInterleaveNumberOfComponents(i),
                        glDataType,
                        pVbo->getBufferObjectDataSizeInBytes(),
                        BUFFER_OFFSET(pVbo->getInterleavedOffsetInBytes(i)));
                }
            }
        }
        
        // We need to keep some data regarding the element array to issue the draw call.
        // We store the datatype and the number of elements.
        //
        if (target == GL_ELEMENT_ARRAY_BUFFER)
        {
            mElementArrayDataType = pVbo->getDataType(0);
            mElementArrayNumberOfElements = (int)pVbo->getNumberOfElements();
        }
    }

    glBindVertexArray(0);
}

//-----------------------------------------------------------------------------
void VertexArrayObject::clear()
{
    if(getVaoId() != 0)
    {
        glDeleteVertexArrays(1,&mVaoId);
        mVaoId = 0;
    }

    // delete VBOs
    for(auto& vbo : mVbosToBake)
    {
        if(vbo.first && vbo.second)
            delete vbo.second;
        vbo.second = nullptr;
    }

    mDrawMode = dmUndefined;
    mVbosToBake.clear();
    mBindedVboIds.clear();
    mElementArrayDataType = dtUndefined;
    mElementArrayNumberOfElements = 0;
}

//-----------------------------------------------------------------------------
void VertexArrayObject::draw() const
{
	draw(0,0,(unsigned int)mElementArrayNumberOfElements);
}

void VertexArrayObject::draw(unsigned int offset, unsigned int startIndex, unsigned int count) const
{
	if (getVaoId() != 0)
	{
		const GLenum glDrawMode = toGlDrawMode(getDrawMode());
		const GLenum glDataType = toGlDataType(mElementArrayDataType);

		glBindVertexArray(getVaoId());
		glDrawElementsBaseVertex(glDrawMode, count, glDataType, BUFFER_OFFSET(offset*getSizeOf(mElementArrayDataType)), startIndex);
		glBindVertexArray(0);
	}
}

//-----------------------------------------------------------------------------
DrawMode VertexArrayObject::getDrawMode() const
{
    return mDrawMode;
}

//-----------------------------------------------------------------------------
int VertexArrayObject::getVaoId() const
{
    return (int)mVaoId;
}

//-----------------------------------------------------------------------------
bool VertexArrayObject::isBaked() const
{
    return getVaoId() != 0;
}

