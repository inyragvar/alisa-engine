#include "function.h"

#include "alisa/graphics/graphics.h"
#include "alisa/logger/logger.h"

namespace alisa {
namespace graphics {
namespace gl {    

const char* getGLErrorString(GLenum error) {
    switch (error) {
        case GL_NO_ERROR:
            return "No error";
        case GL_INVALID_ENUM:
            return "Invalid enum";
        case GL_INVALID_VALUE:
            return "Invalid value";
        case GL_INVALID_OPERATION:
            return "Invalid operation";
        case GL_OUT_OF_MEMORY:
            return "Out of memory";
        // Add more cases for other possible error codes
        default:
            return "Unknown error";
    }
}

void CheckGLError() {
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        logger::Logger::error("OpenGL error: %s", getGLErrorString(error));
    }
} 

unsigned int GetTextureFormat(int channels) {
    unsigned int texture_format = 0;
    switch (channels) {
        case 1: 
        #if defined(MOBILE_OS) || defined(WEB_GL)
            texture_format = GL_LUMINANCE;
        #else
            texture_format = GL_RED;
        #endif
            break;
        
        case 3:
            texture_format = GL_RGB;
            break; 

        case 4:
            texture_format = GL_RGBA;
    }

    return texture_format;
}

void SetTextureFilters(bool is_nearest, bool is_power_of_two) {
    if (is_nearest) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    } else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, is_power_of_two ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
}

void SetTextureWrapMode(bool is_repeat, bool is_power_of_two) { 
    if (is_repeat) { // && is_power_of_two) {
         // texture must be power of 2 - to use it
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    } else {
        if (is_repeat) {
            logger::Logger::warn("GL: can't set texture wrap mode to GL_REPEAT - because texture size isn't power of 2; - will set: GL_CLAMP_TO_EDGE");
        }
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
}

void GenVertexArrays(unsigned int n, unsigned int* arrays) {
#ifndef MOBILE_OS
    glGenVertexArrays(n, arrays);
#else
    glGenVertexArraysOES(n, arrays);
#endif
}

void BindVertexArray(unsigned int array) {
#ifndef MOBILE_OS
    glBindVertexArray(array);
#else
    glBindVertexArrayOES(array);
#endif
}

void DeleteBuffers(unsigned int n, unsigned int* buffers) {
    glDeleteBuffers(n, buffers);
}

void ClearColor(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

void PolygonMode(unsigned int type) {
#ifndef MOBILE_OS
    glPolygonMode(GL_FRONT_AND_BACK, type);
#else
#endif
}

void GenTextures(unsigned int n, unsigned int* ids) {
    glGenTextures(n, ids);
}

void DeleteTextures(unsigned int n, const unsigned int* ids) {
    glDeleteTextures(n, ids);
}

void BindTexture2D(unsigned int id) {
    glBindTexture(GL_TEXTURE_2D, id);
}

void TexImage2D(int level, int internal_format, int width, int height, int border, unsigned int format, unsigned int type, const void* pixels) {
    glTexImage2D(GL_TEXTURE_2D, level, internal_format, width, height, border, format, type, pixels);
}

}
}
}
