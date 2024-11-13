#ifndef ALISA_GRAPHICS_GL_SURFACE_H_
#define ALISA_GRAPHICS_GL_SURFACE_H_

#include "lib/SDL2/SDL_image.h"

namespace alisa {
namespace graphics {
namespace gl {

void FlipSurfaceVertically(SDL_Surface* surface);
SDL_Surface* ConvertSurface(SDL_Surface* surface);

}
}
}

#endif