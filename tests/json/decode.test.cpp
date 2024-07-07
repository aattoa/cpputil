#include <cpputil/json/decode.hpp>
#include <cpputil/json/format.hpp>
#include <cppunittest/unittest.hpp>

using Json = cpputil::json::Value;

UNITTEST("json decode")
{
    CHECK_EQUAL(cpputil::json::decode("null").value(), Json {});
    CHECK_EQUAL(cpputil::json::decode("[]").value(), Json { Json::Array {} });
    CHECK_EQUAL(cpputil::json::decode("{}").value(), Json { Json::Object {} });
    CHECK_EQUAL(cpputil::json::decode("true").value(), Json { true });
    CHECK_EQUAL(cpputil::json::decode("false").value(), Json { false });

    CHECK_EQUAL(
        cpputil::json::decode(R"({"a":[{"b":"c"},5,{"d":{}}]})").value(),
        Json { Json::Object {
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
        } });

    CHECK_EQUAL(
        cpputil::json::decode(R"([{},null,3.3,[],[[]],10])").value(),
        Json { Json::Array {
            Json { Json::Object {} },
            Json {},
            Json { 3.3 },
            Json { Json::Array {} },
            Json { Json::Array { Json { Json::Array {} } } },
            Json { 10.0 },
        } });
}
