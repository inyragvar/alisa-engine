#include "file.h"

#include <string>

#include "alisa/core/logger.h"
#include "lib/SDL2/SDL.h"

#if defined(__IPHONEOS__)
#include "ios_file_utils.h"
#endif

namespace alisa {
namespace utils {

#if !defined(MOBILE_OS)
const std::string ASSETS_FOLDER = "assets/";
const std::string STORE_FOLDER = "store/";
#endif

std::string GetAssetsFilePath(const char* file_path) {
    std::string full_path;
#if !defined(MOBILE_OS)
    full_path.append(SDL_GetBasePath());
    full_path.append(ASSETS_FOLDER);
#endif
    full_path.append(file_path);
    core::Logger::info("assets file path: %s", full_path.c_str());
    return full_path;
}

std::string GetStoreFilePath(const char* file_path) {
    std::string full_path;
#ifdef __ANDROID__
    full_path.append(SDL_AndroidGetInternalStoragePath());
#elif defined(__IPHONEOS__)
    full_path.append(ios_utils::getDocumentDirectoryPath());
    full_path.append("/");
#else
    full_path.append(SDL_GetBasePath());
    full_path.append(STORE_FOLDER);
#endif

    full_path.append(file_path);
    core::Logger::info("store file path: %s", full_path.c_str());
    return full_path;
}

bool IsFileExists(const char* file_path, bool is_assets) {
    std::string full_path = is_assets ? GetAssetsFilePath(file_path) : GetStoreFilePath(file_path);
    SDL_RWops* file = SDL_RWFromFile(full_path.c_str(), "rb");
    if (file != nullptr) {
        SDL_RWclose(file);
        return true;
    }
    return false;
}

std::string ReadFileStr(const char* file_path, bool is_assets) {
    std::string full_path = is_assets ? GetAssetsFilePath(file_path) : GetStoreFilePath(file_path);
    SDL_RWops* rw = SDL_RWFromFile(full_path.c_str(), "rb");
    if (rw == nullptr) {
        core::Logger::error("File: failed to read file: %s; error: %s", full_path.c_str(),  SDL_GetError());
        return "";
    }

    Sint64 res_size = SDL_RWsize(rw);
    char* res = static_cast<char*>(malloc(res_size + 1));

    Sint64 nb_read_total = 0, nb_read = 1;
    char* buf = res;
    while (nb_read_total < res_size && nb_read != 0) {
        nb_read = SDL_RWread(rw, buf, 1, (res_size - nb_read_total));
        nb_read_total += nb_read;
        buf += nb_read;
    }
    SDL_RWclose(rw);
    if (nb_read_total != res_size) {
        free(res);
        return "";
    }

    res[nb_read_total] = '\0';

    std::string res_str = res;
    free(res);
    return res_str;
}

std::vector<char> ReadFileBinary(const char* file_path, bool is_assets) {
    std::string full_path = is_assets ? GetAssetsFilePath(file_path) : GetStoreFilePath(file_path);
    SDL_RWops* rw = SDL_RWFromFile(full_path.c_str(), "rb");
    if (rw == nullptr) {
        core::Logger::error("File: failed to read file: %s; error: %s", full_path.c_str(), SDL_GetError());
        return std::vector<char>();
    }

    Sint64 res_size = SDL_RWsize(rw);
    std::vector<char> res(res_size);

    Sint64 nb_read_total = 0, nb_read = 1;
    char* buf = res.data();
    while (nb_read_total < res_size && nb_read != 0) {
        nb_read = SDL_RWread(rw, buf, 1, (res_size - nb_read_total));
        nb_read_total += nb_read;
        buf += nb_read;
    }
    SDL_RWclose(rw);

    if (nb_read_total != res_size) {
        core::Logger::error("File: failed to read the complete file: %s", full_path.c_str());
        return std::vector<char>();
    }

    return res;
}


bool SaveFile(const char* file_path, const char* buffer, int bytes_count, bool is_assets) {
    std::string full_path = is_assets ? GetAssetsFilePath(file_path) : GetStoreFilePath(file_path);
#if defined(__IPHONEOS__)
    return ios_utils::saveFileToDirectory(full_path, buffer);
#else
    SDL_RWops* output_file = SDL_RWFromFile(full_path.c_str(), "w");
    if (output_file == nullptr) {
        core::Logger::error("File: failed to open output file: %s; error: %s", full_path.c_str(), SDL_GetError());
        
        return false;
    }

    // Write the modified data to the output file
    Sint64 bytes_written = SDL_RWwrite(output_file, buffer, 1, bytes_count);
    if (bytes_written != bytes_count) {
        core::Logger::error("File: failed to write data to output file: %s", SDL_GetError());;

        return false;
    }

    // Close the output file
    SDL_RWclose(output_file);

    return true;
#endif
}


}  // namespace system
}  // namespace alisa
