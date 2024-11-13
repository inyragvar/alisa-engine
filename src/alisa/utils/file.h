#ifndef ALISA_UTILS_FILE_H_
#define ALISA_UTILS_FILE_H_

#include <string>
#include <vector>

#include <tuple>

namespace alisa {
namespace utils {

#if !defined(MOBILE_OS)
extern const std::string ASSETS_FOLDER;
extern const std::string STORE_FOLDER;
#endif

std::string GetAssetsFilePath(const char* file_path);

std::string GetStoreFilePath(const char* file_path);

bool IsFileExists(const char* file_path, bool is_assets = true);

std::string ReadFileStr(const char* file_path, bool is_assets = true);
std::vector<char> ReadFileBinary(const char* file_path, bool is_assets = true);

bool SaveFile(const char* file_path, const char* buffer, int bytes_count, bool is_assets = false);

}
}  // namespace alisa

#endif