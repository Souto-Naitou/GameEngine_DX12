#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <cmath>

// 基本的な算術演算のテスト
TEST(MathTest, BasicArithmetic)
{
    EXPECT_EQ(2 + 2, 4);
    EXPECT_FLOAT_EQ(3.14f * 2.0f, 6.28f);
}

// より複雑な数学演算のテスト
TEST(MathTest, FloatingPointOperations)
{
    EXPECT_NEAR(sqrt(2.0), 1.4142135, 0.0001);
    EXPECT_NEAR(sin(0.0), 0.0, 0.0001);
}

// 文字列操作のテスト
TEST(StringTest, BasicStringOperations)
{
    std::string str = "Hello";
    str += " World";
    EXPECT_EQ(str, "Hello World");
    EXPECT_EQ(str.length(), 11);
}

// ベクトル操作のテスト
TEST(VectorTest, BasicVectorOperations)
{
    std::vector<int> vec = { 1, 2, 3, 4, 5 };
    EXPECT_EQ(vec.size(), 5);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[4], 5);
}
