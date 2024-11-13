#ifndef ALISA_RESOURCE_TEXTURE_H_
#define ALISA_RESOURCE_TEXTURE_H_

#include <string>
#include <memory>

#include "texture_info.h"

#include "lib/SDL2/SDL_image.h"

namespace alisa {
namespace resource {

const std::string EmptyTextureName = "empty_texture";

class Texture {
public:
    Texture(const TextureInfo& info);
    Texture(const std::string& image_path, bool is_nearest = false, bool flip_vertically = true);
    ~Texture();

    bool load();

    unsigned int bind();

    int getWidth() const;
    int getHeight() const;

    unsigned int getID() const;

    std::string getPath() const;

    bool isNearest() const;
    bool isLoaded() const;

private:
    TextureInfo info_;

    SDL_Surface* surface_;

    bool is_loaded_;
};

typedef std::shared_ptr<Texture> TexturePtr;

TextureInfo CreateEmptyTexture();

}
}

#endif