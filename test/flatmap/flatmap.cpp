#include <flatmap.hpp>

#include <gtest/gtest.h>

#include <algorithm>
#include <iostream>
#include <string>

#include <map>

TEST(DList, SimpleCheck)
{
    fox::FlatMap<std::string, int> mymap;
    mymap.insert("foo", 100);
    mymap["bar"] = 200;
    mymap["bee"] = 300;

    ASSERT_EQ(mymap.at("foo"), 100);
    ASSERT_EQ(mymap.at("bar"), 200);
    ASSERT_EQ(mymap.at("bee"), 200);
}

TEST(DList, Initial1)
{
    const fox::FlatMap<std::string, int> mymap;
    SUCCEED();
}

TEST(DList, Initial2)
{
    const fox::FlatMap<std::string, int> mymap(3);
    SUCCEED();
}

TEST(DList, Initial3)
{
    const std::map<std::string, int> map = {{"foo", 100}, {"bar", 200}, {"bee", 300}};
    const fox::FlatMap<std::string, int> mymap(map.begin(), map.end());
    SUCCEED();
}

TEST(DList, CopyConstructor)
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

TEST(DList, CopyAssignmentOperator)
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

TEST(DList, MoveConstructor)
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

TEST(DList, MoveAssignmentOperator)
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

TEST(DList, SquareBracketsOperator1)
{
    fox::FlatMap<std::string, int> mymap;
    mymap.insert("foo", 100);

    ASSERT_EQ(mymap["foo"], 100);
}

TEST(DList, SquareBracketsOperator2)
{
    fox::FlatMap<std::string, int> mymap;
    mymap["foo"] = 100;

    ASSERT_EQ(mymap["foo"], 100);
}

TEST(DList, SquareBracketsOperator3)
{
    fox::FlatMap<std::string, int> mymap;
    mymap.insert("foo", 100);
    mymap["foo"] = 200;

    ASSERT_EQ(mymap["foo"], 200);
}

TEST(DList, At)
{
    fox::FlatMap<std::string, int> mymap;
    mymap.insert("foo", 100);
    const int value = mymap.at("foo");

    ASSERT_EQ(value, 100);
}

TEST(DList, Empty)
{
    const fox::FlatMap<std::string, int> mymap;

    ASSERT_TRUE(mymap.empty());
}

TEST(DList, Insert1)
{
    fox::FlatMap<std::string, int> mymap;
    mymap.insert("foo", 100);

    ASSERT_EQ(mymap.at("foo"), 100);
}

TEST(DList, Insert2)
{
    fox::FlatMap<std::string, int> mymap;
    const std::pair<std::string, int> pair("foo", 100);
    mymap.insert(pair);

    ASSERT_EQ(mymap.at("foo"), 100);
}

TEST(DList, InsertOrAssign1)
{
    fox::FlatMap<std::string, int> mymap;
    mymap.insert_or_assign("foo", 100);

    ASSERT_EQ(mymap.at("foo"), 100);
}

TEST(DList, InsertOrAssign2)
{
    fox::FlatMap<std::string, int> mymap;
    mymap.insert_or_assign("foo", 100);
    mymap.insert_or_assign("foo", 200);

    ASSERT_EQ(mymap.at("foo"), 200);
}

// TEST(DList, Erase1)
// {
//     fox::FlatMap<std::string, int> mymap;
//     mymap.insert("foo", 100);
//     mymap.erase("foo");

//     ASSERT_TRUE(mymap.empty());
// }

// TEST(DList, Find)
// {
//     fox::FlatMap<std::string, int> mymap;
//     mymap.insert("foo", 100);

//     ASSERT_NE(mymap.find("foo"), mymap.end());
// }
