#ifndef ALISA_GRAPHICS_GRAPHICS_H_
#define ALISA_GRAPHICS_GRAPHICS_H_

#if defined(MOBILE_OS) || defined(WEB_GL)
#define GL_GLEXT_PROTOTYPES 1
#include "lib/SDL2/SDL_opengles2.h"
#else
#include "lib/GL/glew.h"

#include "lib/SDL2/SDL_opengl.h"
#endif

#endif