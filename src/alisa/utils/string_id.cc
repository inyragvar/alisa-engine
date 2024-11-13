#include "string_id.h"

namespace alisa {
namespace utils {

// Function to generate CRC32 table
uint32_t crc32_table_entry(uint32_t index) {
    uint32_t crc = index;
    for (uint32_t j = 0; j < 8; ++j) {
        crc = (crc & 1) ? (crc >> 1) ^ 0xEDB88320 : crc >> 1;
    }
    return crc;
}

// Function to generate the entire table
std::array<uint32_t, 256> generate_crc32_table() {
    std::array<uint32_t, 256> table = {};
    for (uint32_t i = 0; i < 256; ++i) {
        table[i] = crc32_table_entry(i);
    }
    return table;
}

// Initialize the CRC32 table
const std::array<uint32_t, 256> crc_table = generate_crc32_table();

// CRC32 calculation function
uint32_t crc32(const std::string& data) {
    uint32_t crc = 0xFFFFFFFF;
    for (unsigned char byte : data) {
        crc = (crc >> 8) ^ crc_table[(crc ^ byte) & 0xFF];
    }
    return crc ^ 0xFFFFFFFF;
}

StringID GetStringID(const std::string& data) {
    return (StringID)crc32(data);
}


}
}