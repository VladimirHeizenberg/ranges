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

TEST(FilterTest, FewFilters) {
    std::vector<int> input = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto result = AsDataFlow(input) | Filter([](int x) { return x % 2 == 0; }) | AsVector();
    auto result2 = AsDataFlow(input) | Filter([](int x) { return x % 2 == 1; }) | AsVector();
    ASSERT_THAT(result, testing::ElementsAre(2, 4, 6, 8, 10));
    ASSERT_THAT(result2, testing::ElementsAre(1, 3, 5, 7, 9));
}

TEST(FilterTest, FewFilters2) {
    std::vector<int> input = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto flow = AsDataFlow(input) | Transform([](int x) {return x * x;});
    auto result = flow | Filter([](int x) { return x % 2 == 0; }) | AsVector();
    auto result2 = flow | Filter([](int x) { return x % 2 == 1; }) | AsVector();
    ASSERT_THAT(result, testing::ElementsAre(4, 16, 36, 64, 100));
    ASSERT_THAT(result2, testing::ElementsAre(1, 9, 25, 49, 81));
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

TEST(FilterTest, NotConstFilter) {
    std::vector<int> input = {1, 2, 3, 4, 5};
    auto result = AsDataFlow(input);
    auto result2 = result | Filter([](int& x) {
         bool result = x % 2 == 0; 
         x += 2;
         return result;
        }) | AsVector();
    auto result3 = result | AsVector();
    ASSERT_THAT(result2, testing::ElementsAre(4, 6));
    ASSERT_THAT(result3, testing::ElementsAre(3, 4, 5, 6, 7));
}

