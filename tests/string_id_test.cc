#include <gtest/gtest.h>
#include "../src/alisa/utils/string_id.h"

using namespace alisa::utils;

// Test case for empty string
TEST(StringIDTest, EmptyString) {
    std::string emptyString = "";
    StringID id = GetStringID(emptyString);
    
    // The expected CRC32 for an empty string is a well-known value
    EXPECT_EQ(id, 0x00000000);
}

// Test case for a basic string
TEST(StringIDTest, BasicString) {
    std::string testString = "test";
    StringID id = GetStringID(testString);
    
    // This is the expected CRC32 result for the string "test"
    EXPECT_EQ(id, 0xD87F7E0C);
}

// Test case for different strings generating different IDs
TEST(StringIDTest, DifferentStrings) {
    std::string string1 = "test1";
    std::string string2 = "test2";

    StringID id1 = GetStringID(string1);
    StringID id2 = GetStringID(string2);

    // Ensure different strings generate different IDs
    EXPECT_NE(id1, id2);
}

// Test case for consistency (same input string generates same ID)
TEST(StringIDTest, ConsistentStringID) {
    std::string testString = "consistent_string";
    StringID id1 = GetStringID(testString);
    StringID id2 = GetStringID(testString);

    // Ensure that the same string generates the same StringID
    EXPECT_EQ(id1, id2);
}

// Test case for long strings
TEST(StringIDTest, LongString) {
    std::string longString = std::string(1000, 'a'); // 1000 'a' characters
    StringID id = GetStringID(longString);

    // The CRC32 result for this long string is expected to be deterministic
    // Here we only test that it works (compare to a known value if desired)
    EXPECT_NE(id, 0);  // Ensure that the result is not zero
}
