#include <gtest/gtest.h>
#include <Features/LevelLoader/LevelData.h>
#include <nlohmann/json.hpp>

using namespace BlenderLevel;
using json = nlohmann::json;

class LevelDataTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // テスト用のJSONデータを準備
        sampleObjectJson = {
            {"type", "Mesh"},
            {"name", "TestCube"},
            {"transform", {
                {"scales", {1.0, 2.0, 3.0}},
                {"rotation", {0.0, 45.0, 90.0}},
                {"translation", {10.0, 20.0, 30.0}}
            }},
            {"file_name", "cube.obj"},
            {"collider", {
                {"type", "BOX"},
                {"center", {0.0, 0.0, 0.0}},
                {"size", {2.0, 2.0, 2.0}}
            }}
        };

        sampleLevelJson = {
            {"name", "TestLevel"},
            {"objects", {sampleObjectJson}}
        };
    }

    json sampleObjectJson;
    json sampleLevelJson;
};

// Vector3のassign_from_json_vec3関数のテスト
TEST_F(LevelDataTest, AssignFromJsonVec3)
{
    json vecJson = { 1.5f, 2.5f, 3.5f };
    Vector3 vec;

    assign_from_json_vec3(vecJson, vec);

    EXPECT_FLOAT_EQ(vec.x, 1.5f);
    EXPECT_FLOAT_EQ(vec.y, 2.5f);
    EXPECT_FLOAT_EQ(vec.z, 3.5f);
}

// EulerTransformのassign_from_json関数のテスト
TEST_F(LevelDataTest, AssignFromJsonTransform)
{
    json transformJson = {
        {"scales", {2.0, 3.0, 4.0}},
        {"rotation", {45.0, 90.0, 180.0}},
        {"translation", {100.0, 200.0, 300.0}}
    };

    EulerTransform transform;
    assign_from_json(transformJson, transform);

    // Scale
    EXPECT_FLOAT_EQ(transform.scale.x, 2.0f);
    EXPECT_FLOAT_EQ(transform.scale.y, 3.0f);
    EXPECT_FLOAT_EQ(transform.scale.z, 4.0f);

    // Rotation
    EXPECT_FLOAT_EQ(transform.rotate.x, 45.0f);
    EXPECT_FLOAT_EQ(transform.rotate.y, 90.0f);
    EXPECT_FLOAT_EQ(transform.rotate.z, 180.0f);

    // Translation
    EXPECT_FLOAT_EQ(transform.translate.x, 100.0f);
    EXPECT_FLOAT_EQ(transform.translate.y, 200.0f);
    EXPECT_FLOAT_EQ(transform.translate.z, 300.0f);
}

// BoxColliderのfrom_json関数のテスト
TEST_F(LevelDataTest, BoxColliderFromJson)
{
    json boxJson = {
        {"center", {1.0, 2.0, 3.0}},
        {"size", {4.0, 5.0, 6.0}}
    };

    BoxCollider boxCollider;
    from_json(boxJson, boxCollider);

    EXPECT_FLOAT_EQ(boxCollider.center.x, 1.0f);
    EXPECT_FLOAT_EQ(boxCollider.center.y, 2.0f);
    EXPECT_FLOAT_EQ(boxCollider.center.z, 3.0f);

    EXPECT_FLOAT_EQ(boxCollider.size.x, 4.0f);
    EXPECT_FLOAT_EQ(boxCollider.size.y, 5.0f);
    EXPECT_FLOAT_EQ(boxCollider.size.z, 6.0f);
}

// SphereColliderのfrom_json関数のテスト
TEST_F(LevelDataTest, SphereColliderFromJson)
{
    json sphereJson = {
        {"center", {1.0, 2.0, 3.0}},
        {"radius", 5.5}
    };

    SphereCollider sphereCollider;
    from_json(sphereJson, sphereCollider);

    EXPECT_FLOAT_EQ(sphereCollider.center.x, 1.0f);
    EXPECT_FLOAT_EQ(sphereCollider.center.y, 2.0f);
    EXPECT_FLOAT_EQ(sphereCollider.center.z, 3.0f);
    EXPECT_FLOAT_EQ(sphereCollider.radius, 5.5f);
}

// Colliderのassign_from_json関数のテスト（BoxCollider）
TEST_F(LevelDataTest, AssignFromJsonColliderBox)
{
    json colliderJson = {
        {"type", "BOX"},
        {"center", {1.0, 2.0, 3.0}},
        {"size", {4.0, 5.0, 6.0}}
    };

    Collider collider;
    assign_from_json(colliderJson, collider);

    EXPECT_TRUE(std::holds_alternative<BoxCollider>(collider));

    BoxCollider& boxCollider = std::get<BoxCollider>(collider);
    EXPECT_FLOAT_EQ(boxCollider.center.x, 1.0f);
    EXPECT_FLOAT_EQ(boxCollider.size.x, 4.0f);
}

// Colliderのassign_from_json関数のテスト（SphereCollider）
TEST_F(LevelDataTest, AssignFromJsonColliderSphere)
{
    json colliderJson = {
        {"type", "SPHERE"},
        {"center", {1.0, 2.0, 3.0}},
        {"radius", 7.5}
    };

    Collider collider;
    assign_from_json(colliderJson, collider);

    EXPECT_TRUE(std::holds_alternative<SphereCollider>(collider));

    SphereCollider& sphereCollider = std::get<SphereCollider>(collider);
    EXPECT_FLOAT_EQ(sphereCollider.center.x, 1.0f);
    EXPECT_FLOAT_EQ(sphereCollider.radius, 7.5f);
}

// 不正なColliderタイプのテスト
TEST_F(LevelDataTest, AssignFromJsonColliderInvalidType)
{
    json colliderJson = {
        {"type", "INVALID_TYPE"},
        {"center", {1.0, 2.0, 3.0}}
    };

    Collider collider;
    EXPECT_THROW(assign_from_json(colliderJson, collider), std::runtime_error);
}

// Objectのfrom_json関数のテスト
TEST_F(LevelDataTest, ObjectFromJson)
{
    Object object;
    from_json(sampleObjectJson, object);

    EXPECT_EQ(object.type, "Mesh");
    EXPECT_EQ(object.name, "TestCube");
    EXPECT_EQ(object.filename, "cube.obj");

    // Transform確認
    EXPECT_FLOAT_EQ(object.transform.scale.x, 1.0f);
    EXPECT_FLOAT_EQ(object.transform.scale.y, 2.0f);
    EXPECT_FLOAT_EQ(object.transform.scale.z, 3.0f);

    EXPECT_FLOAT_EQ(object.transform.rotate.x, 0.0f);
    EXPECT_FLOAT_EQ(object.transform.rotate.y, 45.0f);
    EXPECT_FLOAT_EQ(object.transform.rotate.z, 90.0f);

    EXPECT_FLOAT_EQ(object.transform.translate.x, 10.0f);
    EXPECT_FLOAT_EQ(object.transform.translate.y, 20.0f);
    EXPECT_FLOAT_EQ(object.transform.translate.z, 30.0f);

    // Collider確認
    EXPECT_TRUE(object.collider.has_value());
    EXPECT_TRUE(std::holds_alternative<BoxCollider>(object.collider.value()));
}

// Colliderがないオブジェクトのテスト
TEST_F(LevelDataTest, ObjectFromJsonWithoutCollider)
{
    json objectJsonNoCollider = {
        {"type", "Light"},
        {"name", "TestLight"},
        {"transform", {
            {"scales", {1.0, 1.0, 1.0}},
            {"rotation", {0.0, 0.0, 0.0}},
            {"translation", {0.0, 0.0, 0.0}}
        }}
    };

    Object object;
    from_json(objectJsonNoCollider, object);

    EXPECT_EQ(object.type, "Light");
    EXPECT_EQ(object.name, "TestLight");
    EXPECT_FALSE(object.collider.has_value());
}

// LevelDataのfrom_json関数のテスト
TEST_F(LevelDataTest, LevelDataFromJson)
{
    LevelData levelData;
    from_json(sampleLevelJson, levelData);

    EXPECT_EQ(levelData.name, "TestLevel");
    EXPECT_EQ(levelData.objects.size(), 1);

    const Object& firstObject = levelData.objects.front();
    EXPECT_EQ(firstObject.type, "Mesh");
    EXPECT_EQ(firstObject.name, "TestCube");
}

// 複数オブジェクトを含むLevelDataのテスト
TEST_F(LevelDataTest, LevelDataFromJsonMultipleObjects)
{
    json lightObject = {
        {"type", "Light"},
        {"name", "TestLight"},
        {"transform", {
            {"scales", {1.0, 1.0, 1.0}},
            {"rotation", {0.0, 0.0, 0.0}},
            {"translation", {5.0, 10.0, 15.0}}
        }}
    };

    json levelJson = {
        {"name", "MultiObjectLevel"},
        {"objects", {sampleObjectJson, lightObject}}
    };

    LevelData levelData;
    from_json(levelJson, levelData);

    EXPECT_EQ(levelData.name, "MultiObjectLevel");
    EXPECT_EQ(levelData.objects.size(), 2);

    auto it = levelData.objects.begin();
    EXPECT_EQ(it->type, "Mesh");
    ++it;
    EXPECT_EQ(it->type, "Light");
}
