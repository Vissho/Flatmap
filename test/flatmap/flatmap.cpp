#include <flatmap.hpp>

#include <gtest/gtest.h>

#include <algorithm>
#include <iostream>
#include <string>

TEST(DList, Init1)
{
    fox::FlatMap<std::string, int> mymap
            = {{"bee", 100}, {"bar", 200}, {"foo", 300}};
    std::cout << "Hello\n";
    std::cout << (*mymap.find("bee")).second << "\n";
    std::cout << (*mymap.find("bar")).second << "\n";
    std::cout << (*mymap.find("foo")).second << "\n";

    std::cout << mymap.empty() << "\n";
    mymap.erase("bee");
    std::cout << mymap.contains("bee") << "\n";

    ASSERT_EQ((*mymap.find("foo")).second, 300);
    ASSERT_EQ((*mymap.find("bar")).second, 200);
}
