
#include "Rendering/Gpu/OpenGlHeaders.h"
#include <cassert>
#include "Math/CommonMath.h"
#include "Rendering/Gpu/FrameBufferObject.h"


using namespace Realisim;
    using namespace Math;
    using namespace Rendering;

//-----------------------------------------------------------------------------
// FrameBufferAttachement
//-----------------------------------------------------------------------------
FrameBufferAttachement::FrameBufferAttachement(FrameBufferAttachementType ty, Texture2d* tx)
: type(ty)
, texture(tx)
{
}

FrameBufferAttachement::FrameBufferAttachement(FrameBufferAttachement&& o)
    : type(o.type)
    , texture(o.texture.release())
{
    o.texture = nullptr;
}

FrameBufferAttachement& FrameBufferAttachement::operator = (FrameBufferAttachement&& o)
{
    type = o.type;
    texture.reset(o.texture.release());

    o.texture = nullptr;

    return *this;
}

//-----------------------------------------------------------------------------
// FrameBufferObject
//-----------------------------------------------------------------------------
int FrameBufferObject::mDefaultDrawBufferId = -1;
int FrameBufferObject::mDefaultReadBufferId = -1;

Math::Vector2i FrameBufferObject::m_defaultViewportOrigin = Math::Vector2i(0, 0);
Math::Vector2i FrameBufferObject::m_defaultViewportSize = Math::Vector2i(800, 600);
std::vector<FrameBufferObject*> FrameBufferObject::m_previousFbos;
Texture2d FrameBufferObject::mDummyTexture;
int FrameBufferObject::m_maxDrawBuffers = 0;

FrameBufferObject::FrameBufferObject(unsigned int w,unsigned int h,unsigned __int8 aa)
    : m_width(w)
    ,m_height(h)
    ,m_nbSamples(aa)
    ,m_openglId(0)
    ,m_viewportOrigin()
    ,m_viewportSize()
    ,m_dirtySizeFlag(false)
{
}

FrameBufferObject::~FrameBufferObject()
{
    m_attachements.clear();
    m_viewportOrigin = Math::Vector2i();
    m_viewportSize = Math::Vector2i();

    if(m_openglId)
    {
        glDeleteFramebuffers(1,&m_openglId);
        m_openglId = 0;
    }
}

void FrameBufferObject::addError(const std::string& iE) const
{
    m_lastErrors += m_lastErrors.empty() ? iE : "\t\n" + iE;
}

std::string FrameBufferObject::getAndClearLastErrors()
{
    std::string r = m_lastErrors;
    m_lastErrors.clear();
    return r;
}

// returns the FramebufferAttachement identified by iAttachment
//
const Texture2d& FrameBufferObject::getAttachement(FrameBufferAttachementType iAttachment) const
{
    return *getAttachementPtr(iAttachment);
}

const FrameBufferAttachementType FrameBufferObject::getAttachementType(unsigned int i) const
{
	if( i<m_attachements.size())
	{
		return m_attachements[i].type;
	}
	else
	{
		return FrameBufferAttachementType::fbaUnknown;
	}
}

const Texture2d* FrameBufferObject::getAttachementPtr(FrameBufferAttachementType iAttachment) const
{
    Texture2d* r = &mDummyTexture;
    const int index = searchAttachement(iAttachment);
    if (index != -1)
    {
        r = m_attachements[index].texture.get();
    }

    return r;
}

unsigned int FrameBufferObject::getWidth() const
{
    return m_width;
}

unsigned int FrameBufferObject::getHeight() const
{
    return m_height;
}

unsigned int FrameBufferObject::getId() const
{
    return m_openglId;
}

bool FrameBufferObject::checkStatus()
{
    GLenum status;
    status = (GLenum)glCheckFramebufferStatus(GL_FRAMEBUFFER);
    bool valid = false;
    switch(status)
    {
    case GL_FRAMEBUFFER_COMPLETE:
        valid = true;
        break;
    case GL_FRAMEBUFFER_UNSUPPORTED:
        addError("Unsupported framebuffer format");
        break;
    case GL_FRAMEBUFFER_UNDEFINED:
        addError("Undefined framebuffer");
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        addError("Framebuffer incomplete, missing attachment");
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
        addError("Framebuffer incomplete, missing draw buffer");
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
        addError("Framebuffer incomplete, missing read buffer");
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        addError("Framebuffer incomplete attachement");
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
        addError("Framebuffer incomplete, multisampling error");
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
        addError("Framebuffer populated attachment is not layered error");
        break;
    default:
        addError("unknown framebuffer error");
        break;
    }

    return valid;
}

void    FrameBufferObject::bake()
{
    if (m_maxDrawBuffers == 0)
    {
        glGetIntegerv(GL_MAX_DRAW_BUFFERS,&m_maxDrawBuffers);
    }

    // create
    if(getId()==0)
    {
        glGenFramebuffers(1,&m_openglId);
    }
    else
    {
        glDeleteFramebuffers(1,&m_openglId);
        glGenFramebuffers(1, &m_openglId);
    }

    push();

    // attach color/depth/stencil
    bool ok = true;
    for(int i=0;i<m_attachements.size();++i)
    {
        if(m_attachements[i].texture)
        {
            auto attach = toGlFrameBufferAttachement(m_attachements[i].type);
            glNamedFramebufferTexture(m_openglId,attach,m_attachements[i].texture->getId(),0);
            ok &= checkStatus();
        }
    }

    pop();

    // do not delete a frame buffer while it is bound.
    if (!ok)
    {
        glDeleteFramebuffers(1, &m_openglId);
        m_openglId = 0;
    }
}
 
// Creates a Texture2d and returns the pointer. If the pointer is passed to addAttachement(),
// FrameBufferObject will take ownership of that pointer.
//
Texture2d*  FrameBufferObject::createAttachementTexture(TextureInternalFormat iInternalFormat,TextureFormat iFormat,DataType dataType,TextureFilter filter,
    TextureWrapMode wrapModeS,
    TextureWrapMode wrapModeT)
{
    Texture2d* texture = new Texture2d();
            
    texture->setMagnificationFilter(filter);
    texture->setMinificationFilter(filter);
    texture->setWrapSMode(wrapModeS);
    texture->setWrapTMode(wrapModeT);

    texture->setFboTexture((m_nbSamples > 1) ? TextureTarget::tt2dMultisampled : TextureTarget::tt2d, iInternalFormat, m_width, m_height, iFormat, dataType, m_nbSamples);

    return texture;
}

// This function assumes the opengl context is active.
// It will activate drawing on the attachment specified by iAttachment
//
void FrameBufferObject::drawTo(FrameBufferAttachementType iAttachment)
{
    auto t = toGlFrameBufferAttachement(iAttachment);
    glDrawBuffer(t);
}

// This function assumes the opengl context is active.
// It will activate drawing on the attachments specified by iAttachment
//
void FrameBufferObject::drawTo(const std::vector<FrameBufferAttachementType>& iAttachments)
{
    const int n = (int)iAttachments.size();
    std::vector<GLenum> t(n, 0);
	int toRender = 0;
    for(int i = 0; i < n; ++i)
    { 
		if( (iAttachments[i] !=FrameBufferAttachementType::fbaDepthOnly)
			&&
			(iAttachments[i] !=FrameBufferAttachementType::fbaStencilOnly)
			&&
			(iAttachments[i] !=FrameBufferAttachementType::fbaDepthStencil) )
		{
			t[toRender] = toGlFrameBufferAttachement(iAttachments[i]); 
			toRender++;
		}
	}

	if(toRender)
		glDrawBuffers(toRender, &t[0]);
}

// you must give the texture pointer to createAttachementTexture or prepare it yourself
// the pointer is owned by the class, do NOT try to delete it
int    FrameBufferObject::addAttachement(FrameBufferAttachementType type,Texture2d* texture)
{
    int index = (int)m_attachements.size();

    m_attachements.push_back(FrameBufferAttachement(type,texture));

    return index;

}

// bind the FBO in read/write in the specified viewport
void    FrameBufferObject::bind(int   viewportOriginX,int   viewportOriginY, int   viewportW,int   viewportH)
{
    if(isValid())
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER,this->m_openglId);
        glBindFramebuffer(GL_READ_FRAMEBUFFER,0);

        // update attachments, mostly applying openGL states that
        // could have been invalidated outside the OpenGL context...
        update();

        // handle multisamping
        if(hasMultisamples())
            glEnable(GL_MULTISAMPLE);
        else
            glDisable(GL_MULTISAMPLE);
                
        // set viewport.
        glViewport(viewportOriginX, viewportOriginY, viewportW, viewportH);
    }
}

//---------------------------------------------------------------------------------------------------------------------
// At fist glance, thus function seems superfluous but it has a very particular use case. It is highly possible
// the the frameBuffer 0 is already taken by something else in the pipeline (QT for example) thus it is mandatory
// to set the default framebuffer id, so we can revert back to non-offscreen rendering.
// This function should be called only once during the resizeGL phase
//
void FrameBufferObject::initializeDefaultFrameBufferId()
{
    if (mDefaultDrawBufferId == -1 || mDefaultReadBufferId == -1)
    {
        GLint defaultDraw;
        GLint defaultRead;
        glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &defaultDraw);
        glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &defaultRead);
        mDefaultDrawBufferId = defaultDraw;
        mDefaultReadBufferId = defaultRead;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void FrameBufferObject::push()
{
    push(0, 0, getWidth(), getHeight());
}

//---------------------------------------------------------------------------------------------------------------------
void FrameBufferObject::push(int viewportOriginX, int viewportOriginY, int viewportW, int viewportH)
{
    if(!isValid()) {return;} //early out

    m_viewportOrigin.set(viewportOriginX, viewportOriginY);
    m_viewportSize.set(viewportW, viewportH);
    m_previousFbos.push_back(this);

    bind(m_viewportOrigin.x(), m_viewportOrigin.y(), m_viewportSize.x(), m_viewportSize.y());
}

void FrameBufferObject::pop()
{
    // an invalid fbo was not bound anyway... so we wont unbind it
    // as it would pop a fbo from the queue while it should not.
    //
    // NOTE: deleting/clear/makingTheFboInvalid before unbinding it
    // will result in undefined behavior.
    //
    if (isValid())
    {
        assert(!m_previousFbos.empty());
        // remove the the frame buffer from the list...
        if (!m_previousFbos.empty())
        { m_previousFbos.pop_back(); }
    
        Math::Vector2i o = m_defaultViewportOrigin;
        Math::Vector2i s = m_defaultViewportSize;
        if (!m_previousFbos.empty())
        {
            FrameBufferObject *pFbo = m_previousFbos.back();
            o = pFbo->m_viewportOrigin;
            s = pFbo->m_viewportSize;
            pFbo->bind(o.x(), o.y(), s.x(), s.y());
        }
        else
        {
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mDefaultDrawBufferId);
            glBindFramebuffer(GL_READ_FRAMEBUFFER, mDefaultReadBufferId);
            glDisable(GL_MULTISAMPLE);
            glViewport(o.x(), o.y(), s.x(), s.y());
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
FrameBufferAttachementType FrameBufferObject::getNextAvailableColorAttachement() const
{
    FrameBufferAttachementType r = fbaUnknown;

    for (int i = fbaColor0; i < fbaColor15; ++i)
    {
        if (!getAttachement((FrameBufferAttachementType)i).isValid())
        {
            r = (FrameBufferAttachementType)i;
            break;
        }
    }

    return r;
}

unsigned int FrameBufferObject::getNumberOfSamples() const
{
    return m_nbSamples;
}

int FrameBufferObject::getNumberOfAttachements() const
{
	return (int)m_attachements.size();
}

bool FrameBufferObject::hasErrors() const
{
    return !m_lastErrors.empty();
}

bool FrameBufferObject::hasMultisamples() const
{
    return getNumberOfSamples() > 1;
}

void FrameBufferObject::setAttachementBlending(int drawBufferIndex,bool enable)
{
    if(drawBufferIndex <= m_maxDrawBuffers)
    {
        if(enable)
        {
            glEnablei(GL_BLEND, drawBufferIndex);
        }
        else
        {
            glDisablei(GL_BLEND, drawBufferIndex);
        }
    }
}

void FrameBufferObject::setAttachementBlendingFunction(int drawBufferIndex,BlendFunction source,BlendFunction end)
{
    if (drawBufferIndex <= m_maxDrawBuffers)
    {
        GLenum src = toGlBlendFunction(source);
        GLenum dst = toGlBlendFunction(end);

        glBlendFunci(drawBufferIndex,src, dst);
    }
}

void FrameBufferObject::setAttachementBlendingEquation(int drawBufferIndex,BlendEquation equation)
{
    if (drawBufferIndex <= m_maxDrawBuffers)
    {
        GLenum eq = toGlBlendEquation(equation);

        glBlendEquationi(drawBufferIndex,eq);
    }
}

void    FrameBufferObject::resize(unsigned int width,unsigned int height)
{
    m_width = width;
    m_height = height;

    if (!m_attachements.empty())
    {
        if (m_attachements[0].texture->hasMipmaps())
        {
            m_width = nextPowerOf2(width);
            m_height = nextPowerOf2(height);
        }
    }

    m_dirtySizeFlag = true;
}

bool FrameBufferObject::isValid() const
{
    return (m_openglId != 0);
}

bool FrameBufferObject::isColorAttachment(FrameBufferAttachementType iAttachment)
{
    return !(iAttachment == fbaDepthOnly ||
        iAttachment == fbaStencilOnly ||
        iAttachment == fbaDepthStencil );
}

// return the index of the attachment or -1 if not found.
//
int FrameBufferObject::searchAttachement(FrameBufferAttachementType type) const
{
    int res = -1;

    for(int i=0;i<m_attachements.size();++i)
    {
        if( m_attachements[i].type == type )
        {
            res = i;
            break;
        }
    }

    return res;
}

//--------------------------------------------------------------------
// see method initializeDefaultFrameBufferId for an explanation
//
void FrameBufferObject::setDefaultFrameBufferId(int iDefaultId)
{
    if (mDefaultDrawBufferId == -1 || mDefaultReadBufferId == -1)
    {
        mDefaultDrawBufferId = iDefaultId;
        mDefaultReadBufferId = iDefaultId;
    }
}

//--------------------------------------------------------------------
void FrameBufferObject::setDefaultFrameBufferViewport(const Math::Vector2i& iOrigin,
    const Math::Vector2i& iSize)
{
    m_defaultViewportOrigin = iOrigin;
    m_defaultViewportSize = iSize;
}

//--------------------------------------------------------------------
// This function will resolve (copy) the color attachment sourceAttachement to the 
// output FBO's targetAttachement.
//
// When the current buffer is multisampled and the output buffer is not, the multisampled
// buffer will be resolved. It is important to note that buffer size must be equal in order to
// resolve a multisampled buffer. (see openGL doc about glBlitFramebuffer)
//
// If both buffer are multisampled or not, a copy is performed.
//
// If buffers are not multisampled, they can have different size and the blit will be performed.
//
void    FrameBufferObject::resolveMultisample(FrameBufferObject* targetFBO, FrameBufferAttachementType targetAttachement,FrameBufferAttachementType sourceAttachement, bool manualStateManagement)
{
    if(targetFBO)
    {
        if(manualStateManagement == false)
            targetFBO->push();//save fbo state.

        // search attachement index
        auto targetAttach = toGlFrameBufferAttachement(targetAttachement);
        auto sourceAttach = toGlFrameBufferAttachement(sourceAttachement);
               
        bool targetIsDepth = (targetAttachement == FrameBufferAttachementType::fbaDepthOnly) || (targetAttachement == FrameBufferAttachementType::fbaDepthStencil) ||(targetAttachement == FrameBufferAttachementType::fbaStencilOnly);
        bool sourceIsDepth = (sourceAttachement == FrameBufferAttachementType::fbaDepthOnly) || (sourceAttachement == FrameBufferAttachementType::fbaDepthStencil) ||(sourceAttachement == FrameBufferAttachementType::fbaStencilOnly);
                
        GLenum iBufferBitMask = GL_COLOR_BUFFER_BIT;
        if(targetAttachement == FrameBufferAttachementType::fbaDepthOnly)
        {
            iBufferBitMask = GL_DEPTH_BUFFER_BIT;
        }
        if(targetAttachement == FrameBufferAttachementType::fbaStencilOnly)
        {
            iBufferBitMask = GL_STENCIL_BUFFER_BIT;
        }
        if(targetAttachement == FrameBufferAttachementType::fbaDepthStencil)
        {
            iBufferBitMask = GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
        }

        glBindFramebuffer(GL_READ_FRAMEBUFFER,this->m_openglId); // Make sure your multisampled FBO is the read framebuffer
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER,targetFBO->m_openglId); // Make sure your multisampled FBO is the read framebuffer
        
        if(!sourceIsDepth)
            glReadBuffer(sourceAttach);
        if(!targetIsDepth)
            glDrawBuffer(targetAttach);

        // now do the good blit
        if( (this->m_width == targetFBO->m_width) && (this->m_height == targetFBO->m_height) )
        {
            glBlitFramebuffer(0, 0, this->m_width, this->m_height, 0, 0, targetFBO->m_width, targetFBO->m_height, iBufferBitMask, GL_NEAREST);
        }
        else
        {
            if( (this->m_nbSamples>1) || (targetFBO->m_nbSamples>1))
            {
                // error
            }
            else
            {
                if(targetIsDepth)
                {
                    glBlitFramebuffer(0, 0, this->m_width, this->m_height, 0, 0, targetFBO->m_width, targetFBO->m_height, iBufferBitMask, GL_NEAREST);
                }
                else
                {
                    // interpolate
                    glBlitFramebuffer(0, 0, this->m_width, this->m_height, 0, 0, targetFBO->m_width, targetFBO->m_height, iBufferBitMask, GL_LINEAR);
                }
            }
        }

        if (manualStateManagement == false)
            targetFBO->pop();// reset
    }
}

// This function is used to update dirty members of fbo. It is mostly
// used to sync the opengl data on card with the member of the class.
//
// The typical case is to call resize on the fbo outside the validity
// of the OpenGl context (between makeCurrent/doneCurrent).
//
void FrameBufferObject::update()
{
    // resize all attachments if necessary.
    if (m_dirtySizeFlag)
    {
        for (int i = 0; i < m_attachements.size(); ++i)
        {
            if (m_attachements[i].texture)
            {
                m_attachements[i].texture->resizeFbo(m_width, m_height);
            }
        }
        m_dirtySizeFlag = false;
    }
}

void	FrameBufferObject::clearColor(unsigned int drawBufferIndex,const float* clearValueFloat)
{
	assert(drawBufferIndex<=FrameBufferAttachementType::fbaColor15);

	if(clearValueFloat && (drawBufferIndex <= FrameBufferAttachementType::fbaColor15))
	{
		glClearNamedFramebufferfv(getId(),GL_COLOR,drawBufferIndex,clearValueFloat);
	}
}

void	FrameBufferObject::clearDepth(const float* clearValueFloat)
{
	glClearNamedFramebufferfv(getId(), GL_DEPTH, 0, clearValueFloat);
}

void	FrameBufferObject::clearStencil(const int* clearValueInt)
{
	glClearNamedFramebufferiv(getId(), GL_STENCIL, 0, clearValueInt);

}

void	FrameBufferObject::clearDepthStencil(const float* clearValueFloat,const int* clearValueInt)
{
    glClearNamedFramebufferfv(getId(), GL_DEPTH, 0, clearValueFloat);
    glClearNamedFramebufferiv(getId(), GL_STENCIL, 0, clearValueInt);

    //the correct function is as follow...
    // it seems our GLEW is not up to date and we do not have access to this function...
    //
    //glClearNamedFramebufferfi(getId(),GL_DEPTH_STENCIL,0,clearValueFloat[0],clearValueInt[0]);
}