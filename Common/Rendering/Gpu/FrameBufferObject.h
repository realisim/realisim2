#pragma once

#include <memory>
#include <vector>
#include <string>
#include "Rendering/Gpu/DataType.h"
#include "Rendering/Gpu/Texture2d.h"

namespace Realisim
{
    namespace Rendering
    {
        //---------------------------------------------------------------------
        // use FrameBufferAttachement if you need to sample the results somewhere
        //
        // VIRER CE TRUC...
        //
        struct FrameBufferAttachement
        {
            FrameBufferAttachementType  type;
            std::unique_ptr<Texture2d>  texture;

            FrameBufferAttachement() = delete;
            FrameBufferAttachement(FrameBufferAttachementType ty, Texture2d* tx);
            FrameBufferAttachement(const FrameBufferAttachement& o) = delete;
            FrameBufferAttachement& operator = (const FrameBufferAttachement& o) = delete;
            FrameBufferAttachement(FrameBufferAttachement&& o);
            FrameBufferAttachement& operator = (FrameBufferAttachement&& o);
            ~FrameBufferAttachement() = default;
        };

        //---------------------------------------------------------------------
        /*
            This class implement's an opengl Frame Buffer Object.

            Usage is quite simple, create a fbo via constructor
                FrameBufferObject(unsigned int iWidth, unsigned int iHeight, unsigned __int8 iNumberOfSamples);

                From there add some attachments... color, depth or stencil.

                Then call bake() to allocate and validate the fbo.

                isValid() will return true allocation was successful.

                call push() to use the fbo.
                call pop() to end the fbo usage.

            The fbo can be resized at any moments and upon next bind,
            all it's attachment will be resized.

            attachements:
                attachements are simply Texture2d, so any Texture2d can
                be attached to the fbo. A convenient function
                FrameBufferObject::createAttachementTexture() can be used
                to create a Texture2d with the correct size for the current
                fbo.

                createAttachmentTextures will return a pointer. Once this
                pointer is passed to addAttachment, FrameBufferObject will
                own the pointer, so no need to delete it.

            Push/Pop mechanism
                A convenient push/pop system is in place to easily stack
                many different frame buffer each with different viewport
                dimension.

                To bind/use the fbo call push() or push(viewportDimension) to
                specify viewport dimensions.

                call pop() to stop using the current fbo and restore the 
                previous fbo with the correct viewport settings.

            static members:
                mDefaultDrawBufferId; The default fbo id. it is usually 0,
                    but in some case (with QT for example) the default fbo
                    can be different than 0. It can be set via 
                    initializeDefaultFrameBuffer
                mDefaultReadBufferId: see mDefaultDrawBufferId
                m_defaultViewportOrigin; The origin of the default fbo.
                m_defaultViewportSize; the size of the default fbo.
                std::vector<FrameBufferObject*> m_previousFbos; This holds
                    the list of pushed fbos, it is used to properly bind the
                    previous fbo when many are used one on top of the others.

            members:
                m_openglId; Opengl id for this ressource. 0 means unallocated
                m_width; width in pixels of the framebuffer
                m_height; height in pixels of the framebuffer
                m_nbSamples; Number of samples for this fbo. 1 is a normal fbo.
                    multisampled fbos will have more than 1, usually 2, 4, 8, 16.
                m_attachements; A vector describing all FrameBufferAttachement for
                    this fbo
                m_lastErrors; Contains all accumulated errors until
                    getAndClearLastErrors is call
                m_viewportOrigin; origin of viewport
                m_viewportSize; size of viewport
                m_dirtySizeFlag; dirty flag to resize the fbo and all of it's
                    attachment.
        */
        class FrameBufferObject
        {        
        public:
            FrameBufferObject() = delete;
            FrameBufferObject(const FrameBufferObject&) = delete;
            FrameBufferObject& operator=(const FrameBufferObject&) = delete;
            explicit FrameBufferObject(unsigned int iWidth, unsigned int iHeight, unsigned __int8 iNumberOfSamples);
            ~FrameBufferObject();

            int addAttachement(FrameBufferAttachementType type, Texture2d* texture);
            void bake();
            Texture2d* createAttachementTexture(TextureInternalFormat iInternalFormat,TextureFormat iFormat,DataType dataType,
                TextureFilter filter, TextureWrapMode wrapModeS, TextureWrapMode wrapModeT );

			void	clearColor(unsigned int drawBufferIndex, const float* clearValueFloat);
			void	clearDepth(const float* clearValueFloat);
			void	clearStencil(const int* clearValueInt);
			void	clearDepthStencil(const float* clearValueFloat,const int* clearValueInt);

            void drawTo(FrameBufferAttachementType iAttachment);
            void drawTo(const std::vector<FrameBufferAttachementType>& iAttachments);
            std::string getAndClearLastErrors();
            const Texture2d& getAttachement(FrameBufferAttachementType iAttachment) const;
			const Texture2d* getAttachementPtr(FrameBufferAttachementType iAttachment) const;
			const FrameBufferAttachementType getAttachementType(unsigned int i) const;
            int getDefaultDrawBufferId() const { return mDefaultDrawBufferId; }
            int getDefaultReadBufferId() const { return mDefaultReadBufferId; }
            unsigned int getHeight() const;
            unsigned int getId() const;
            FrameBufferAttachementType getNextAvailableColorAttachement() const;
            int getNumberOfAttachements() const;
			unsigned int getNumberOfSamples() const;			
            unsigned int getWidth() const;
            bool hasErrors() const;
            bool hasMultisamples() const;
            bool isValid() const;
            static void initializeDefaultFrameBufferId();
            void push();
            void push(int viewportOriginX, int viewportOriginY, int viewportW, int viewportH);
            void pop();
            void readFrom(FrameBufferAttachementType iAttachment);
            void resolveMultisample(FrameBufferObject* targetFBO, FrameBufferAttachementType targetAttachement, FrameBufferAttachementType sourceAttachement, bool manualStateManagement = false);
            void resize(unsigned int width, unsigned int height);
            static void setDefaultFrameBufferId(int iDefaultId);
            static void setDefaultFrameBufferViewport(const Math::Vector2i& iOrigin, const Math::Vector2i& iSize );
            

            // drawBufferIndex is in the same order as you have put the attachment on the FBO, example :
            //      addAttachementToRender(Simthetiq::Rendering::FrameBufferAttachementType::fbaColor4);    <- attachement 4 will be in draw buffer 0, so drawBufferIndex is 0
            //      addAttachementToRender(Simthetiq::Rendering::FrameBufferAttachementType::fbaColor5);    <- attachement 5 will be in draw buffer 1, so drawBufferIndex is 1
            //      addAttachementToRender(Simthetiq::Rendering::FrameBufferAttachementType::fbaDepthStencil); <- no index for stencil,depth, depthStencil
            static void setAttachementBlending(int drawBufferIndex ,bool enable);
            static void setAttachementBlendingFunction(int drawBufferIndex,BlendFunction source, BlendFunction end);
            static void setAttachementBlendingEquation(int drawBufferIndex,BlendEquation equation);

        protected:
            void addError(const std::string&) const;
            void bind(int viewportOriginX, int viewportOriginY, int viewportW, int viewportH);
            bool isColorAttachment(FrameBufferAttachementType iAttachment);
            bool checkStatus();
            int searchAttachement(FrameBufferAttachementType type) const;
            void update();

            //--- data
            static int mDefaultDrawBufferId;
            static int mDefaultReadBufferId;
            
            static Math::Vector2i m_defaultViewportOrigin;
            static Math::Vector2i m_defaultViewportSize;
            static std::vector<FrameBufferObject*> m_previousFbos;
            static Texture2d mDummyTexture;

            unsigned int m_openglId;
            unsigned int m_width;
            unsigned int m_height;
            unsigned __int8 m_nbSamples;
            std::vector<FrameBufferAttachement> m_attachements;
            mutable std::string m_lastErrors;
            Math::Vector2i m_viewportOrigin;
            Math::Vector2i m_viewportSize;
            bool m_dirtySizeFlag;

            static int m_maxDrawBuffers;
        };
    }
}

