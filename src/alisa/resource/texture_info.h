#ifndef ALISA_RESOURCE_TEXTURE_INFO_H_
#define ALISA_RESOURCE_TEXTURE_INFO_H_

#include <string>

namespace alisa{
namespace resource {
    
struct TextureInfo {
    unsigned int id;

    int width;
    int height;
    int channels;

    std::string image_path;

    bool is_nearest;
    bool flip_vertically;

    TextureInfo();
};
}
}

#endif