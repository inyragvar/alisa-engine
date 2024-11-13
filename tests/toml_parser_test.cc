#include <gtest/gtest.h>

#include "../src/alisa/core/logger.h"

#include "../src/alisa/utils/parser/toml/parser.h"
#include "../src/alisa/utils/parser/toml/value.h"

using namespace alisa::core;
using namespace alisa::utils;
using namespace alisa::utils::parser;

// Test case for parsing a simple key-value pair
TEST(TomlParserTest, ParseBasicKeyValue) {
    std::string file_content = "name = \"test_name\"\nage = 25";
    TomlParser parser;
    parser.parse(file_content);

    EXPECT_EQ(parser.get("name").asString(), "test_name");
    EXPECT_EQ(parser.get("age").asInt(), 25);
}

// Test case for parsing a float value
TEST(TomlParserTest, ParseFloatValue) {
    std::string file_content = "pi = 3.14159";
    TomlParser parser;
    parser.parse(file_content);

    EXPECT_FLOAT_EQ(parser.get("pi").asFloat(), 3.14159);
}

// Test case for parsing a vector3f value
TEST(TomlParserTest, ParseVector3f) {
    std::string file_content = "position = (1.0, 2.0, 3.0)";
    TomlParser parser;
    parser.parse(file_content);

    math::Vector3f position = parser.get("position").asVector3f();

    EXPECT_FLOAT_EQ(position.x, 1.0);
    EXPECT_FLOAT_EQ(position.y, 2.0);
    EXPECT_FLOAT_EQ(position.z, 3.0);
}

// Test case for parsing a table
TEST(TomlParserTest, ParseTable) {
    std::string file_content = "[player]\nname = \"hero\"\nhealth = 100";
    TomlParser parser;
    parser.parse(file_content);

    std::map<std::string, TomlValue> player = parser.get("player").asTable();
    EXPECT_EQ(player["name"].asString(), "hero");
    EXPECT_EQ(player["health"].asInt(), 100);
}
 
// Test case for parsing an array of values
TEST(TomlParserTest, ParseArray) {
    std::string file_content = "colors = [\"red\", \"green\", \"blue\"]";
    TomlParser parser;
    parser.parse(file_content);

    std::vector<TomlValue> colors = parser.get("colors").asArray();
    EXPECT_EQ(colors[0].asString(), "red");
    EXPECT_EQ(colors[1].asString(), "green");
    EXPECT_EQ(colors[2].asString(), "blue");
}

// Test case for parsing an array of tables
TEST(TomlParserTest, ParseArrayOfTables) {
    std::string file_content = R"(
        [[enemies]]
        type = "goblin"
        health = 30

        [[enemies]]
        type = "orc"
        health = 80
    )";
    TomlParser parser;
    parser.parse(file_content);

    std::vector<TomlValue> enemies = parser.get("enemies").asArray();
    
    EXPECT_EQ(enemies[0].asTable()["type"].asString(), "goblin");
    EXPECT_EQ(enemies[0].asTable()["health"].asInt(), 30);
    EXPECT_EQ(enemies[1].asTable()["type"].asString(), "orc");
    EXPECT_EQ(enemies[1].asTable()["health"].asInt(), 80);
}

// Test case for parsing a nested table (subtable)
TEST(TomlParserTest, ParseNestedTable) {
    std::string file_content = "[database]\n[database.settings]\nuser = \"admin\"\npassword = \"secret\"";
    TomlParser parser;
    parser.parse(file_content);

    std::map<std::string, TomlValue> database = parser.get("database").asTable();
    std::map<std::string, TomlValue> settings = database["settings"].asTable();

    EXPECT_EQ(settings["user"].asString(), "admin");
    EXPECT_EQ(settings["password"].asString(), "secret");
}
// Test case for parsing multiple types including a sub-table in a single TOML content
TEST(TomlParserTest, ParseCombinedTypesWithSubTable) {
    std::string file_content = R"(
        name = "test_name"
        age = 30
        pi = 3.14159
        is_active = true

        position = (1.0, 2.0, 3.0)

        colors = ["red", "green", "blue"]

        [player]
        name = "hero"
        health = 100
        strength = 75.5

        [[enemies]]
        type = "goblin"
        health = 30

        [[enemies]]
        type = "orc"
        health = 80

        [player.stats]
        agility = 50
        intelligence = 70
    )";
    
    TomlParser parser;
    parser.parse(file_content);

    // Key-Value pairs
    EXPECT_EQ(parser.get("name").asString(), "test_name");
    EXPECT_EQ(parser.get("age").asInt(), 30);
    EXPECT_FLOAT_EQ(parser.get("pi").asFloat(), 3.14159);
    EXPECT_EQ(parser.get("is_active").asBool(), true);

    // Vector3f value
    math::Vector3f position = parser.get("position").asVector3f();
    EXPECT_FLOAT_EQ(position.x, 1.0);
    EXPECT_FLOAT_EQ(position.y, 2.0);
    EXPECT_FLOAT_EQ(position.z, 3.0);

    // Array of values
    std::vector<TomlValue> colors = parser.get("colors").asArray();
    EXPECT_EQ(colors[0].asString(), "red");
    EXPECT_EQ(colors[1].asString(), "green");
    EXPECT_EQ(colors[2].asString(), "blue");

    // Nested table (player)
    std::map<std::string, TomlValue> player = parser.get("player").asTable();
    EXPECT_EQ(player["name"].asString(), "hero");
    EXPECT_EQ(player["health"].asInt(), 100);
    EXPECT_FLOAT_EQ(player["strength"].asFloat(), 75.5);

    // Sub-table (player.stats)
    std::map<std::string, TomlValue> player_stats = player["stats"].asTable();
    EXPECT_EQ(player_stats["agility"].asInt(), 50);
    EXPECT_EQ(player_stats["intelligence"].asInt(), 70);

    // Array of tables (enemies)
    std::vector<TomlValue> enemies = parser.get("enemies").asArray();
    EXPECT_EQ(enemies[0].asTable()["type"].asString(), "goblin");
    EXPECT_EQ(enemies[0].asTable()["health"].asInt(), 30);
    EXPECT_EQ(enemies[1].asTable()["type"].asString(), "orc");
    EXPECT_EQ(enemies[1].asTable()["health"].asInt(), 80);
}
