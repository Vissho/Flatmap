#include <flatmap.hpp>

#include <gtest/gtest.h>

#include <algorithm>
#include <iostream>
#include <string>

TEST(DList, Init1)
{
    fox::FlatMap<std::string, int> mymap(3);
    mymap.insert("foo", 100);
    // mymap["bar"] = 200;
    // mymap["bee"] = 300;
    // std::cout << "Hello\n";
    std::cout << mymap["foo"] << "\n";
    // std::cout << mymap["bar"] << "\n";
    // std::cout << mymap["bee"] << "\n";

    // std::cout << mymap.empty() << "\n";
    // mymap.erase("bee");
    // std::cout << mymap.contains("bee") << "\n";

    // ASSERT_EQ((*mymap.find("foo")).second, 300);
    // ASSERT_EQ((*mymap.find("bar")).second, 200);
}
