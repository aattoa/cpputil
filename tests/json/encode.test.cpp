#include <cpputil/json/encode.hpp>
#include <cppunittest/unittest.hpp>

using Json = cpputil::json::Value;

UNITTEST("json encode")
{
    CHECK_EQUAL(cpputil::json::encode(Json {}), "null");
    CHECK_EQUAL(cpputil::json::encode(Json { Json::Array {} }), "[]");
    CHECK_EQUAL(cpputil::json::encode(Json { Json::Object {} }), "{}");
    CHECK_EQUAL(cpputil::json::encode(Json { true }), "true");
    CHECK_EQUAL(cpputil::json::encode(Json { false }), "false");

    CHECK_EQUAL(
        cpputil::json::encode(Json { Json::Object {
            {
                "a",
                Json { Json::Array {
                    Json { Json::Object {
                        { "b", Json { "c" } },
                    } },
                    Json { 5.0 },
                    Json { Json::Object {
                        { "d", Json { Json::Object {} } },
                    } },
                } },
            },
        } }),
        R"({"a":[{"b":"c"},5,{"d":{}}]})");

    CHECK_EQUAL(
        cpputil::json::encode(Json { Json::Array {
            Json { Json::Object {} },
            Json {},
            Json { 3.3 },
            Json { Json::Array {} },
            Json { Json::Array { Json { Json::Array {} } } },
            Json { 10.0 },
        } }),
        R"([{},null,3.3,[],[[]],10])");

    CHECK_EQUAL(cpputil::json::encode(Json { "hello\t\\tworld" }), R"("hello\t\\tworld")");
}
