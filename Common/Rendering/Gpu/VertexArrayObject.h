
#pragma once

#include "Math/Vector.h"
#include "Rendering/Gpu/BufferObject.h"
#include <vector>

namespace Realisim
{
    namespace Rendering
    {
    /*
    This class handles OpenGl ressources needed by a Vertex Array Object.

    Most of the configuration is handled by VertexBufferObject. VAO is mostly
    a container to which VBOs are added.

    Opengl ressources will be assigned when bake() is called. Method isBaked()
    will indicate if the OpenGl ressources have been properly initialized.

    Clear() will release all gl resources.

    Example:
        pVboData = new VertexBufferObject();
        pVboData->setAsInterleaved( bbtArrayBuffer, ...);

        pVboIndices = new VertexBufferObject();
        pVboIndices->set(bbtElementArrayBuffer, ... );

        VerexArrayObject cube;
        cube.add(*pVboData);
        cube.add(*pVboIndices);
        cube.bake(dmTriangles);

        ...

        cube.draw();

    Implementation details:
        mVaoId: the GL id of the Vao
        mDrawMode: the draw mode, when invoking glDrawElement
        mVbosToBake: a temporary buffer to hold all vbo until bake is called.
            The buffer is clear after bake is called.
        mElementArrayDataType; The datatype of the element array used in the vao. This
            is necessary to issue the draw call.
        mElementArrayNumberOfElements; Number of elements in the element array used
            in the vao. It will be 0 if no elementArray is attached to the vao.
            this is necessary to issue the draw call.
        mBindedVboIds: We keep the indices to all VBO used by this VAO. Just
            for convenience...
    */
    class VertexArrayObject
    {
    public:
        VertexArrayObject();
        ~VertexArrayObject();
        VertexArrayObject(const VertexArrayObject&) = delete;
        VertexArrayObject& operator=(const VertexArrayObject&) = delete;

        void add(BufferObject* iVbo);
        void addAndTakeOwnership(BufferObject* iVbo);
        void bake(DrawMode iDrawMode);
        void clear();
		void draw() const;

		// draw the array by starting at index "startIndex" (ie : 0,1,2,3,4,5,6,7,8,9 with startIndex=3 will draw 2,3,4,5,6,7,8,9)
		// draw the array by applying an "offset" to the index (ie : 0,1,2,3 with offset=10 becomes drawing indexes 10,11,12,13)
		void draw(unsigned int offset, unsigned int startIndex, unsigned int count) const;		
        DrawMode getDrawMode() const;
        int getVaoId() const;
        bool isBaked() const;
    
	protected:

        //--- data
        unsigned int mVaoId;
        DrawMode mDrawMode;
        std::vector<std::pair<bool,BufferObject*>> mVbosToBake; //own VBOs
        DataType mElementArrayDataType;
        int mElementArrayNumberOfElements;
        std::vector<int> mBindedVboIds;
    };

}
}