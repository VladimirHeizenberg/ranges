#include <processing.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(FlowConvertionsTest, AsDataFlow) {
    std::vector<int> input = {1, 2, 3, 4, 5};
    auto flow = AsDataFlow(input);
    ASSERT_THAT(flow, testing::ElementsAre(1, 2, 3, 4, 5));
}

TEST(FlowConvertionsTest, AsVector) {
    std::vector<int> input = {1, 2, 3, 4, 5};
    auto result = AsDataFlow(input) | AsVector();
    ASSERT_THAT(result, testing::ElementsAreArray(std::vector<int>{1, 2, 3, 4, 5}));
}

TEST(FlowConvertionsTest, ConstDataFlow) {
    const std::vector<int> input = {1, 2, 3, 4, 5};
    auto flow = AsDataFlow(input);
    ASSERT_THAT(flow, testing::ElementsAre(1, 2, 3, 4, 5));
}

TEST(FlowConvertionsTest, RValue) {
    auto result = AsDataFlow(std::vector({1, 2, 3, 4, 5})) | AsVector();
    ASSERT_THAT(result, testing::ElementsAreArray(std::vector<int>{1, 2, 3, 4, 5}));
}

TEST(FilterTest, NotConstRangeChange) {
    std::vector<int> input = {1, 2, 3, 4, 5};
    auto result = AsDataFlow(input);
    for (auto& res : result) res *= 2;
    ASSERT_THAT(result, testing::ElementsAre(2, 4, 6, 8, 10));
}