#include <processing.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <cctype>

TEST(FilterTest, FilterEven) {
    std::vector<int> input = {1, 2, 3, 4, 5};
    auto result = AsDataFlow(input) | Filter([](int x) { return x % 2 == 0; }) | AsVector();
    ASSERT_THAT(result, testing::ElementsAre(2, 4));
}

TEST(FilterTest, FilterUpperCase) {
    std::vector<std::string> input = {"hello", "world", "HELLO", "WORLD"};
    auto result =
        AsDataFlow(input)
            | Filter([](const std::string& x) { return std::all_of(x.begin(), x.end(), [](char c) { return std::isupper(c); }); })
            | AsVector();
    ASSERT_THAT(result, testing::ElementsAre("HELLO", "WORLD"));
}

TEST(FilterTest, ConstFilter1) {
    const std::vector<int> input = {1, 2, 3, 4, 5};
    auto result = AsDataFlow(input);
    auto result2 = result | Filter([](int x) { return x % 2 == 0; }) | AsVector();
    ASSERT_THAT(result2, testing::ElementsAre(2, 4));
}

TEST(FilterTest, ConstFilter2) {
    std::vector<int> input = {1, 2, 3, 4, 5};
    const auto result = AsDataFlow(input);
    auto result2 = result | Filter([](const int& x) { return x % 2 == 0; }) | AsVector();
    ASSERT_THAT(result2, testing::ElementsAre(2, 4));
}

// TEST(FilterTest, NotConstFilter) {
//     std::vector<int> input = {1, 2, 3, 4, 5};
//     auto result = AsDataFlow(input);
//     auto result2 = result | Filter([](int& x) {
//          bool result = x % 2 == 0; 
//          x += 2;
//          return result;
//         }) | AsVector();
//     auto result3 = result | AsVector();
//     ASSERT_THAT(result2, testing::ElementsAre(2, 4));
//     ASSERT_THAT(result3, testing::ElementsAre(3, 4, 5, 6, 7));
// }

