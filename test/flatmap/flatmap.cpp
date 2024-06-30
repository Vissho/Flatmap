#include <flatmap.hpp>

#include <gtest/gtest.h>

#include <algorithm>
#include <iostream>
#include <string>

#include <map>

TEST(FlatMap, SimpleCheck)
{
    fox::FlatMap<std::string, int> mymap;
    mymap.insert("foo", 100);
    mymap["bar"] = 200;
    mymap["bee"] = 300;

    ASSERT_EQ(mymap.at("foo"), 100);
    ASSERT_EQ(mymap.at("bar"), 200);
    ASSERT_EQ(mymap.at("bee"), 300);
}

TEST(FlatMap, Initial1)
{
    const fox::FlatMap<std::string, int> mymap;
    SUCCEED();
}

TEST(FlatMap, Initial2)
{
    const fox::FlatMap<std::string, int> mymap(3);
    SUCCEED();
}

TEST(FlatMap, Initial3)
{
    const std::map<std::string, int> map
            = {{"foo", 100}, {"bar", 200}, {"bee", 300}};
    const fox::FlatMap<std::string, int> mymap(map.begin(), map.end());
    SUCCEED();
}

TEST(FlatMap, Initial4)
{
    const fox::FlatMap<std::string, int> mymap
            = {{"foo", 100}, {"bar", 200}, {"bee", 300}};
    SUCCEED();
}

TEST(FlatMap, CopyConstructor)
{
    fox::FlatMap<std::string, int> mymap1;
    mymap1.insert("foo", 100);
    mymap1.insert("bar", 200);
    mymap1.insert("bee", 300);

    const fox::FlatMap<std::string, int> mymap2(mymap1);

    ASSERT_EQ(mymap1.at("foo"), 100);
    ASSERT_EQ(mymap2.at("bar"), 200);
    ASSERT_EQ(mymap2.at("bee"), 300);
}

TEST(FlatMap, CopyAssignmentOperator)
{
    fox::FlatMap<std::string, int> mymap1;
    mymap1.insert("foo", 100);
    mymap1.insert("bar", 200);
    mymap1.insert("bee", 300);

    const fox::FlatMap<std::string, int> mymap2 = mymap1;

    ASSERT_EQ(mymap1.at("foo"), 100);
    ASSERT_EQ(mymap2.at("bar"), 200);
    ASSERT_EQ(mymap2.at("bee"), 300);
}

TEST(FlatMap, MoveConstructor)
{
    fox::FlatMap<std::string, int> mymap1;
    mymap1.insert("foo", 100);
    mymap1.insert("bar", 200);
    mymap1.insert("bee", 300);

    const fox::FlatMap<std::string, int> mymap2(std::move(mymap1));

    ASSERT_EQ(mymap2.at("foo"), 100);
    ASSERT_EQ(mymap2.at("bar"), 200);
    ASSERT_EQ(mymap2.at("bee"), 300);
}

TEST(FlatMap, MoveAssignmentOperator)
{
    fox::FlatMap<std::string, int> mymap1;
    mymap1.insert("foo", 100);
    mymap1.insert("bar", 200);
    mymap1.insert("bee", 300);

    fox::FlatMap<std::string, int> mymap2;
    mymap2 = std::move(mymap1);

    ASSERT_EQ(mymap2.at("foo"), 100);
    ASSERT_EQ(mymap2.at("bar"), 200);
    ASSERT_EQ(mymap2.at("bee"), 300);
}

TEST(FlatMap, SquareBracketsOperator1)
{
    fox::FlatMap<std::string, int> mymap;
    mymap.insert("foo", 100);

    ASSERT_EQ(mymap["foo"], 100);
}

TEST(FlatMap, SquareBracketsOperator2)
{
    fox::FlatMap<std::string, int> mymap;
    mymap["foo"] = 100;

    ASSERT_EQ(mymap["foo"], 100);
}

TEST(FlatMap, SquareBracketsOperator3)
{
    fox::FlatMap<std::string, int> mymap;
    mymap.insert("foo", 100);
    mymap["foo"] = 200;

    ASSERT_EQ(mymap["foo"], 200);
}

TEST(FlatMap, At)
{
    fox::FlatMap<std::string, int> mymap;
    mymap.insert("foo", 100);
    const int value = mymap.at("foo");

    ASSERT_EQ(value, 100);
}

TEST(FlatMap, Empty)
{
    const fox::FlatMap<std::string, int> mymap;

    ASSERT_TRUE(mymap.empty());
}

TEST(FlatMap, Insert1)
{
    fox::FlatMap<std::string, int> mymap;
    mymap.insert("foo", 100);

    ASSERT_EQ(mymap.at("foo"), 100);
}

TEST(FlatMap, Insert2)
{
    fox::FlatMap<std::string, int> mymap;
    const std::pair<std::string, int> pair("foo", 100);
    mymap.insert(pair);

    ASSERT_EQ(mymap.at("foo"), 100);
}

TEST(FlatMap, InsertOrAssign1)
{
    fox::FlatMap<std::string, int> mymap;
    mymap.insert_or_assign("foo", 100);

    ASSERT_EQ(mymap.at("foo"), 100);
}

TEST(FlatMap, InsertOrAssign2)
{
    fox::FlatMap<std::string, int> mymap;
    mymap.insert_or_assign("foo", 100);
    mymap.insert_or_assign("foo", 200);

    ASSERT_EQ(mymap.at("foo"), 200);
}

TEST(FlatMap, Erase)
{
    fox::FlatMap<std::string, int> mymap;
    mymap.insert("foo", 100);
    mymap.erase("foo");

    ASSERT_TRUE(mymap.empty());
}

TEST(FlatMap, Find)
{
    fox::FlatMap<std::string, int> mymap;
    mymap.insert("foo", 100);

    ASSERT_NE(mymap.find("foo"), mymap.end());
}

TEST(FlatMap, Contains)
{
    fox::FlatMap<std::string, int> mymap;
    mymap.insert("foo", 100);

    ASSERT_TRUE(mymap.contains("foo"));
}

TEST(FlatMap, Size)
{
    fox::FlatMap<std::string, int> mymap;
    mymap.insert("foo", 100);
    mymap.insert("bar", 200);
    mymap.insert("bee", 300);

    ASSERT_EQ(mymap.size(), 3);
}

TEST(FlatMap, Algoritm1)
{
    fox::FlatMap<int, int> mymap;
    mymap.insert(1, 100);
    mymap.insert(2, 200);
    mymap.insert(3, 300);

    ASSERT_TRUE(std::any_of(mymap.begin(), mymap.end(), [](auto& pair) {
        return pair.first == 1;
    }));
}

TEST(FlatMap, Algoritm2)
{
    fox::FlatMap<int, int> mymap;
    mymap.insert(1, 100);
    mymap.insert(2, 200);
    mymap.insert(3, 300);

    std::exchange(mymap, fox::FlatMap<int, int>{});
    ASSERT_TRUE(mymap.empty());
}

TEST(FlatMap, Algoritm3)
{
    fox::FlatMap<int, int> mymap;
    mymap.insert(1, 100);
    mymap.insert(2, 200);
    mymap.insert(3, 300);

    auto iter = std::lower_bound(
            mymap.begin(), mymap.end(), 3, [](auto& pair, int key) {
                return pair.first < key;
            });

    ASSERT_EQ(iter->first, 3);
}
