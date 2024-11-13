#include "surface.h"

#include <algorithm>

#include "alisa/logger/logger.h"

namespace alisa {
namespace graphics {
namespace gl {

void FlipSurfaceVertically(SDL_Surface* surface) {
    int pitch = surface->pitch; // Number of bytes in a row
    int height = surface->h;    // Height of the surface

    // Pointers to the start and end of the pixel data
    uint8_t* start = static_cast<uint8_t*>(surface->pixels);
    uint8_t* end = start + (pitch * (height - 1));

    // Swap rows of pixel data to achieve vertical flip
    while (start < end) {
        std::swap_ranges(start, start + pitch, end);
        start += pitch;
        end -= pitch;
    }
}

SDL_Surface* ConvertSurface(SDL_Surface* bgraSurface) {
    if (bgraSurface == nullptr) {
        logger::Logger::error("Surface: surface is nullptr");
        return nullptr;
    }

    SDL_PixelFormat* pixelFormat = bgraSurface->format;
    if (bgraSurface->format->format == SDL_PIXELFORMAT_ABGR8888) {
        //Log::info("Surface: surface has correct format: SDL_PIXELFORMAT_RGBA8888");
        return bgraSurface;
    } 
    logger::Logger::info("Surface: pixel format: %s - changing to SDL_PIXELFORMAT_RGBA8888", SDL_GetPixelFormatName(pixelFormat->format));

    // Get the current format of the surface
    SDL_Surface* optimizedSurface = SDL_ConvertSurfaceFormat(bgraSurface, SDL_PIXELFORMAT_RGBA8888, 0);
    SDL_FreeSurface(bgraSurface);
    if (optimizedSurface == nullptr) {
        logger::Logger::error("Surface: failed to optimize image surface: %s", SDL_GetError());
        return nullptr;
    }
    bgraSurface = optimizedSurface;

    // Create a new RGBA surface with the same width, height, and depth
    SDL_Surface* rgbaSurface = SDL_CreateRGBSurface(0, bgraSurface->w, bgraSurface->h, 32,
                                            #if SDL_BIG_ENDIAN // OpenGL RGBA masks 
                                                0x000000FF,
                                                0x0000FF00,
                                                0x00FF0000,
                                                0xFF000000
                                            #else
                                                0xFF000000,
                                                0x00FF0000,
                                                0x0000FF00,
                                                0x000000FF
                                            #endif
                                            );

    if (!rgbaSurface) {
        logger::Logger::error("Surface: failed create new surface: %s", SDL_GetError());
        return nullptr;
    }

    // Lock the surfaces to access the pixels
    SDL_LockSurface(bgraSurface);
    SDL_LockSurface(rgbaSurface);

    // Copy each pixel from the RGB surface to the RGBA surface, setting alpha to fully opaque (255)
    //The need to swap the color channels (red, green, and blue) when converting the RGB888 surface to an RGBA8888 surface is related to the difference in the byte ordering between the two formats.
    for (int y = 0; y < bgraSurface->h; y++) {
        for (int x = 0; x < bgraSurface->w; x++) {
            Uint32 pixel = ((Uint32*)bgraSurface->pixels)[y * bgraSurface->w + x];
            Uint8 r, g, b, a;
            SDL_GetRGBA(pixel, bgraSurface->format, &r, &g, &b, &a);

            Uint32 new_pixel = SDL_MapRGBA(rgbaSurface->format, r, g, b, a);
            ((Uint32*)rgbaSurface->pixels)[y * rgbaSurface->w + x] = new_pixel;
        }
    }

    // Unlock the surfaces after pixel manipulation
    SDL_UnlockSurface(bgraSurface);
    SDL_UnlockSurface(rgbaSurface);

    SDL_FreeSurface(bgraSurface);

    return rgbaSurface;
}

}
}
}