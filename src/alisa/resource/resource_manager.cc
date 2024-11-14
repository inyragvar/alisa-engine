#include "resource_manager.h"

#include "alisa/utils/file.h"

#include "alisa/logger/logger.h"

namespace alisa {
namespace resource {
// helper function
std::string getTextImagePath(const std::string& text, const std::string& font_name) {
    std::string image_path;
    image_path.reserve(font_name.length() + text.length());
    image_path.append(font_name).append(text);
    return image_path;
}

ResourceManager::ResourceManager() {
    
}

ResourceManager::~ResourceManager() {

}

void ResourceManager::init() {
    initEmptyTexture();
}

void clear() {
    // FIXME clear all reasources
}

void ResourceManager::initEmptyTexture() {
    std::string image_path = EmptyTextureName;

    auto it = textures_.find(image_path);
    if (it != textures_.end()) {
        auto& counter = std::get<0>(it->second);
        counter++;

        logger::Logger::info("ResourceManager: texture: %s, counter: %d", std::get<1>(it->second)->getPath().c_str(), counter);
        return;
    }

    auto info = CreateEmptyTexture();
    Texture* texture = new Texture(info);
    int counter = 1;

    auto value = std::make_tuple(counter, texture);
    
    textures_[image_path] = value;
}

Texture* ResourceManager::getTexture(const std::string& image_path, bool is_nearest) {
    auto it = textures_.find(image_path);
    if (it != textures_.end()) {
        auto& counter = std::get<0>(it->second);
        counter++;

        logger::Logger::info("ResourceManager: texture: %s, counter: %d", std::get<1>(it->second)->getPath().c_str(), counter);
        return std::get<1>(it->second);
    }

    Texture* texture = new Texture(image_path, is_nearest);
    int counter = 1;

    auto value = std::make_tuple(counter, texture);
    textures_[image_path] = value;

    bool res = texture->load();
    if (!res) {
        logger::Logger::error("ResourceManager: can't load texture: %s", image_path.c_str());
       
    }
    
    return texture;
}

void ResourceManager::releaseTexture(const std::string& image_path) {
    if (image_path.empty()) {
        logger::Logger::info("image path empty");
    }
    
    auto it = textures_.find(image_path);
    if (it != textures_.end()) {
        auto& counter = std::get<0>(it->second);
        counter--;

        if (counter == 0) {
            auto texture = std::get<1>(it->second);
            textures_.erase(it);

            delete texture;
        }
    }

    if (it == textures_.end()) {
        logger::Logger::info("empty textures_");
    } 
}

}
}