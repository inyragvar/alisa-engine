#include "texture.h"

#include <vector>
#include <chrono>

#include "alisa/logger/logger.h"

#include "alisa/graphics/graphics.h"
#include "alisa/graphics/gl/surface.h"
#include "alisa/graphics/gl/function.h"

#include "alisa/utils/file.h"
#include "alisa/utils/math/function.h"

namespace alisa {
namespace resource {

Texture::Texture(const TextureInfo& info) {
    info_.id = info.id;

    info_.width = info.width;
    info_.height = info.height;
    info_.channels = info.channels;

    info_.image_path = info.image_path;
    
    info_.is_nearest = info.is_nearest;
    info_.flip_vertically = info.flip_vertically;

    is_loaded_ = true;
}

Texture::Texture(const std::string& image_path, bool is_nearest, bool flip_vertically) {
    info_.image_path = image_path;
    info_.is_nearest = is_nearest;
    info_.flip_vertically = flip_vertically;

    is_loaded_ = false;
}

Texture::~Texture() {
    graphics::gl::DeleteTextures(1, &info_.id);
    logger::Logger::info("Texture: deleting texture id: %d; and path: %s", info_.id, info_.image_path.c_str());
}

bool Texture::load() {
    auto start = std::chrono::high_resolution_clock::now();

    std::string full_path;
#if !defined(MOBILE_OS)
    full_path.append(SDL_GetBasePath());
    full_path.append(utils::ASSETS_FOLDER);
#endif

    full_path.append(info_.image_path);
    // Load the image using SDL_image
    surface_ = IMG_Load(full_path.c_str());
    if (surface_ == nullptr) {
        logger::Logger::error("Texture: failed to load image: %s\n", IMG_GetError());
        return false;
    } else {
        surface_ = graphics::gl::ConvertSurface(surface_);
        if (info_.flip_vertically) {
            graphics::gl::FlipSurfaceVertically(surface_);
        }
    }

    info_.width = surface_->w;
    info_.height = surface_->h;
    info_.channels = surface_->format->BytesPerPixel;

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    // Log the elapsed time
    logger::Logger::info("Texture: path: '%s'; loaded to RAM: %lld ms", info_.image_path.c_str(), elapsed);
    
    /*
    assync load file for web GL
     // Use Emscripten’s asynchronous file system or XMLHttpRequest for loading the image.
    emscripten_async_wget_data(url.c_str(), nullptr,
        [](void* arg, void* data, int size) {
            // This callback is executed on the main thread after the data has been loaded
            int width, height, channels;
            unsigned char* imageData = stbi_load_from_memory((unsigned char*)data, size, &width, &height, &channels, 0);

            // Upload texture to GPU
            GLuint textureId;
            glGenTextures(1, &textureId);
            glBindTexture(GL_TEXTURE_2D, textureId);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
            
            // Free the image data after uploading
            stbi_image_free(imageData);
        },
        [](void* arg) {
            // Error callback in case loading fails
            printf("Failed to load texture\n");
        }
    );
    */

    return true;
}

unsigned int Texture::bind() {
    auto start = std::chrono::high_resolution_clock::now();

    bool is_power_of_two = utils::math::IsPowerOfTwo(info_.width, info_.height);
    // gen texture id before load texture
    graphics::gl::GenTextures(1, &info_.id);
    graphics::gl::BindTexture2D(info_.id);
    
    unsigned int texture_format = graphics::gl::GetTextureFormat(info_.channels);
    if (texture_format == 0) {
        logger::Logger::warn("Texture: image '%s' is not true color... this will probably break!", info_.image_path.c_str());
        return false;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, texture_format, info_.width, info_.height, 0, texture_format, GL_UNSIGNED_BYTE, surface_->pixels);

    graphics::gl::SetTextureFilters(info_.is_nearest, is_power_of_two);
     
    // in future add param for repeat wrap
    graphics::gl::SetTextureWrapMode(true, is_power_of_two);

    if (!info_.is_nearest && is_power_of_two) {
        // texture must be power of 2 - to use it
        glGenerateMipmap(GL_TEXTURE_2D);
    } 
    
    graphics::gl::BindTexture2D(0);

    auto end = std::chrono::high_resolution_clock::now();
    auto  elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    SDL_FreeSurface(surface_);

    logger::Logger::info("Texture: path: '%s'; loaded to GPU: %lld ms", info_.image_path.c_str(), elapsed);

    is_loaded_ = true;
    return info_.id;
}

unsigned int Texture::getID() const {
    return info_.id;
}

int Texture::getWidth() const {
    return info_.width;
}

int Texture::getHeight() const {
    return info_.height;
}

std::string Texture::getPath() const {
    return info_.image_path;
}

bool Texture::isNearest() const {
    return info_.is_nearest;
}

bool Texture::isLoaded() const {
    return is_loaded_;
}

TextureInfo CreateEmptyTexture() {
    TextureInfo info;
    info.width = 2;
    info.height = 2;
    info.image_path = EmptyTextureName;
    info.channels = 4;
    info.is_nearest = true;
    
    // get the number of channels in the SDL surface       
    unsigned int texture_format = GL_RGBA;

    graphics::gl::GenTextures(1, &info.id);
    graphics::gl::BindTexture2D(info.id);
    
    graphics::gl::SetTextureFilters(info.is_nearest, false);
    // in future add param for repeat wrap
    graphics::gl::SetTextureWrapMode(false, false);

    std::vector<unsigned int> texture_data(info.width * info.height, 0xFFFFFFFF);
    graphics::gl::TexImage2D(0, GL_RGBA, info.width, info.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data.data());
    
    graphics::gl::BindTexture2D(0);
    
    return info;
}

}
}
