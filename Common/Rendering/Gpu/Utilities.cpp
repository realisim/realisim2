
#include "Rendering/Gpu/OpenGlHeaders.h" // this must be the first header

#include <cassert>
#include "Utilities.h"

namespace Realisim
{
    namespace Rendering
    {
        //------------------------------------------------------------------------------
        std::string glErrorToString(GLenum iError)
        {
            std::string r = "n/a";

            switch (iError)
            {
            case GL_INVALID_ENUM: r = "GL_INVALID_ENUM."; break;
            case GL_INVALID_VALUE: r = "GL_INVALID_VALUE."; break;
            case GL_INVALID_OPERATION: r = "GL_INVALID_OPERATION."; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: r = "GL_INVALID_FRAMEBUFFER_OPERATION."; break;
            case GL_OUT_OF_MEMORY: r = "GL_OUT_OF_MEMORY."; break;
            case GL_STACK_UNDERFLOW: r = "GL_STACK_UNDERFLOW."; break;
            case GL_STACK_OVERFLOW: r = "GL_STACK_OVERFLOW."; break;
                //case GL_CONTEXT_LOST: r="GL_CONTEXT_LOST."; break; //opengl 4.5
            case GL_TABLE_TOO_LARGE: r = "GL_TABLE_TOO_LARGE."; break; //ARB_IMAGING
            default: break;
            }
            return r;
        }

        //------------------------------------------------------------------------------
        std::string getAndClearLastOpenGlErrors()
        {
            std::string r;

            GLenum err = GL_NO_ERROR;
            while ((err = glGetError()) != GL_NO_ERROR)
            {
                r += r.empty() ? glErrorToString(err) : " | " + glErrorToString(err);
            }

            return r;
        }

        //-----------------------------------------------------------------------------
        void waitOnGlFenceSync(void* p)
        {
            GLsync sync = (GLsync)p;

            //std::string m;
            const int timeoutInMs = 50; // x ms
            GLenum result = GL_WAIT_FAILED;
            bool processed = false;

            while (!processed)
            {
                result = glClientWaitSync(sync, GL_SYNC_FLUSH_COMMANDS_BIT, timeoutInMs);
                switch (result)
                {
                case GL_ALREADY_SIGNALED: processed = true; break;
                case GL_TIMEOUT_EXPIRED: /*assert(0); processed = true;*/ break;
                case GL_CONDITION_SATISFIED: processed = true; break;
                case GL_WAIT_FAILED: assert(0); processed = true; break;
                default: assert(0); break;
                }

                //printf("waitOnFenceSync: %s\n", m.c_str());
            }

            glDeleteSync(sync);
            sync = 0;
        }

        std::vector<float> toFloatArray(const std::vector<Math::Vector2>& in)
        {
            const int numberOfElements = (int)in.size() * 2;
            std::vector<float> out(numberOfElements);

            for (int i = 0; i < in.size(); ++i)
            {
                out[i * 2 + 0] = (float)in[i].x();
                out[i * 2 + 1] = (float)in[i].y();
            }

            return out;
        }

        std::vector<float> toFloatArray(const std::vector<Math::Vector3>& in)
        {
            const int numberOfElements = (int)in.size() * 3;
            std::vector<float> out(numberOfElements);

            for (int i = 0; i < in.size(); ++i)
            {
                out[i * 3 + 0] = (float)in[i].x();
                out[i * 3 + 1] = (float)in[i].y();
                out[i * 3 + 2] = (float)in[i].z();
            }

            return out;
        }

        std::vector<float> toFloatArray(const std::vector<Math::Vector4>& in)
        {
            const int numberOfElements = (int)in.size() * 4;
            std::vector<float> out(numberOfElements);

            for (int i = 0; i < in.size(); ++i)
            {
                out[i * 4 + 0] = (float)in[i].x();
                out[i * 4 + 1] = (float)in[i].y();
                out[i * 4 + 2] = (float)in[i].z();
                out[i * 4 + 3] = (float)in[i].w();
            }

            return out;
        }
    }
}