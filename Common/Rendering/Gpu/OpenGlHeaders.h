#pragma once

#ifdef _WIN32
        //glew.h doit etre avant gl.h
        #include "GL/glew.h"
        #include "GL/wglew.h"


        #include <GL/gl.h>
        #include <GL/glu.h>
#endif

#ifndef BUFFER_OFFSET
#define BUFFER_OFFSET(i) ((char*)NULL + (i))
#endif
