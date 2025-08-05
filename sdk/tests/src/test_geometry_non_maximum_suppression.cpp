// Copyright 2024 TeiaCare
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "test_geometry_non_maximum_suppression.hpp"

namespace tc::sdk::tests
{
using types = testing::Types<int, float, double>;
TYPED_TEST_SUITE(test_geometry_non_maximum_suppression_t, types);

TYPED_TEST(test_geometry_non_maximum_suppression_t, empty_input)
{
    using T = TypeParam;

    // Test with empty boxes and scores
    std::vector<tc::sdk::rectangle<T>> empty_boxes;
    std::vector<T> empty_scores;

    auto result = tc::sdk::non_maximum_suppression(empty_boxes, empty_scores);
    EXPECT_TRUE(result.empty());
}

TYPED_TEST(test_geometry_non_maximum_suppression_t, mismatched_sizes)
{
    using T = TypeParam;

    // Test with mismatched sizes
    std::vector<tc::sdk::rectangle<T>> boxes = {
        tc::sdk::rectangle<T>(0, 0, 10, 10)};
    std::vector<T> scores = {T(0.9), T(0.8)}; // Different size

    EXPECT_THROW(tc::sdk::non_maximum_suppression(boxes, scores), std::invalid_argument);
}

TYPED_TEST(test_geometry_non_maximum_suppression_t, invalid_iou_threshold)
{
    using T = TypeParam;

    std::vector<tc::sdk::rectangle<T>> boxes = {
        tc::sdk::rectangle<T>(0, 0, 10, 10)};
    std::vector<T> scores = {T(0.9)};

    if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>)
    {
        // Test with IoU threshold < 0
        EXPECT_THROW(tc::sdk::non_maximum_suppression(boxes, scores, T(-0.1)), std::invalid_argument);

        // Test with IoU threshold > 1
        EXPECT_THROW(tc::sdk::non_maximum_suppression(boxes, scores, T(1.1)), std::invalid_argument);
    }

    // Test with IoU threshold < 0
    EXPECT_THROW(tc::sdk::non_maximum_suppression(boxes, scores, T(-99.123)), std::invalid_argument);

    // Test with IoU threshold > 1
    EXPECT_THROW(tc::sdk::non_maximum_suppression(boxes, scores, T(99.123)), std::invalid_argument);
}

TYPED_TEST(test_geometry_non_maximum_suppression_t, single_box)
{
    using T = TypeParam;

    std::vector<tc::sdk::rectangle<T>> boxes = {
        tc::sdk::rectangle<T>(0, 0, 10, 10)};
    std::vector<T> scores = {T(0.9)};

    auto result = tc::sdk::non_maximum_suppression(boxes, scores);

    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], 0);
}

TYPED_TEST(test_geometry_non_maximum_suppression_t, no_overlap)
{
    using T = TypeParam;

    // Two non-overlapping boxes
    std::vector<tc::sdk::rectangle<T>> boxes = {
        tc::sdk::rectangle<T>(0, 0, 10, 10),  // Box 0
        tc::sdk::rectangle<T>(20, 20, 10, 10) // Box 1 - no overlap
    };
    std::vector<T> scores = {T(0.9), T(0.8)};

    auto result = tc::sdk::non_maximum_suppression(boxes, scores);

    // Both boxes should be kept since they don't overlap
    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0], 0); // Higher score first
    EXPECT_EQ(result[1], 1);
}

TYPED_TEST(test_geometry_non_maximum_suppression_t, high_overlap_suppression)
{
    using T = TypeParam;

    // Two highly overlapping boxes
    std::vector<tc::sdk::rectangle<T>> boxes = {
        tc::sdk::rectangle<T>(0, 0, 10, 10), // Box 0
        tc::sdk::rectangle<T>(2, 2, 10, 10)  // Box 1 - high overlap (IoU ≈ 0.47)
    };
    std::vector<T> scores = {T(0.9), T(0.8)};

    constexpr double iou_threshold = 0.4;
    auto result = tc::sdk::non_maximum_suppression(boxes, scores, iou_threshold);

    // Only the higher scoring box should be kept
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], 0);
}

TYPED_TEST(test_geometry_non_maximum_suppression_t, score_threshold_filtering)
{
    using T = TypeParam;

    std::vector<tc::sdk::rectangle<T>> boxes = {
        tc::sdk::rectangle<T>(0, 0, 10, 10),   // Box 0
        tc::sdk::rectangle<T>(20, 20, 10, 10), // Box 1
        tc::sdk::rectangle<T>(40, 40, 10, 10)  // Box 2
    };
    std::vector<T> scores = {T(0.9), T(0.4), T(0.7)}; // Box 1 below threshold

    constexpr double iou_threshold = 0.5;
    constexpr double score_threshold = 0.5;
    auto result = tc::sdk::non_maximum_suppression(boxes, scores, iou_threshold, score_threshold);

    // Only boxes 0 and 2 should be kept (scores >= 0.5)
    if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>)
    {
        ASSERT_EQ(result.size(), 2);
        EXPECT_EQ(result[0], 0); // Highest score
        EXPECT_EQ(result[1], 2); // Second highest score above threshold
    }
    else
    {
        ASSERT_EQ(result.size(), 0);
    }
}

TYPED_TEST(test_geometry_non_maximum_suppression_t, score_ordering)
{
    using T = TypeParam;

    std::vector<tc::sdk::rectangle<T>> boxes = {
        tc::sdk::rectangle<T>(0, 0, 10, 10),   // Box 0
        tc::sdk::rectangle<T>(20, 20, 10, 10), // Box 1
        tc::sdk::rectangle<T>(40, 40, 10, 10)  // Box 2
    };
    std::vector<T> scores = {T(0.7), T(0.9), T(0.8)}; // Box 1 has highest score

    auto result = tc::sdk::non_maximum_suppression(boxes, scores);
    if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>)
    {
        // Results should be ordered by score (descending)
        ASSERT_EQ(result.size(), 3);
        EXPECT_EQ(result[0], 1); // Highest score (0.9)
        EXPECT_EQ(result[1], 2); // Second highest score (0.8)
        EXPECT_EQ(result[2], 0); // Lowest score (0.7)
    }
}

TYPED_TEST(test_geometry_non_maximum_suppression_t, complex_scenario)
{
    using T = TypeParam;

    // Complex scenario with multiple overlapping and non-overlapping boxes
    std::vector<tc::sdk::rectangle<T>> boxes = {
        tc::sdk::rectangle<T>(0, 0, 10, 10),   // Box 0
        tc::sdk::rectangle<T>(5, 5, 10, 10),   // Box 1 - overlaps with 0 (IoU ≈ 0.143)
        tc::sdk::rectangle<T>(20, 20, 10, 10), // Box 2 - no overlap
        tc::sdk::rectangle<T>(25, 25, 10, 10), // Box 3 - overlaps with 2 (IoU ≈ 0.143)
        tc::sdk::rectangle<T>(40, 40, 10, 10)  // Box 4 - no overlap
    };
    std::vector<T> scores = {T(0.9), T(0.8), T(0.7), T(0.6), T(0.5)};

    constexpr double iou_threshold = 0.1;
    auto result = tc::sdk::non_maximum_suppression(boxes, scores, iou_threshold); // Lower threshold to suppress overlapping boxes

    // Expected: Box 0 (suppresses 1), Box 2 (suppresses 3), Box 4
    ASSERT_EQ(result.size(), 3);
    EXPECT_EQ(result[0], 0);
    EXPECT_EQ(result[1], 2);
    EXPECT_EQ(result[2], 4);
}

TYPED_TEST(test_geometry_non_maximum_suppression_t, edge_case_iou_threshold)
{
    using T = TypeParam;

    // Two boxes with known IoU for testing threshold edge cases
    std::vector<tc::sdk::rectangle<T>> boxes = {
        tc::sdk::rectangle<T>(0, 0, 10, 10), // Box 0
        tc::sdk::rectangle<T>(5, 0, 10, 10)  // Box 1 - 50% overlap (IoU = 0.33)
    };
    std::vector<T> scores = {T(0.9), T(0.8)};

    // With IoU threshold 0.4, both should be kept
    {
        constexpr double iou_threshold = 0.4;
        auto result1 = tc::sdk::non_maximum_suppression(boxes, scores, iou_threshold);
        ASSERT_EQ(result1.size(), 2);
    }

    // With IoU threshold 0.3, only the higher scoring one should be kept
    {
        constexpr double iou_threshold = 0.3;
        auto result = tc::sdk::non_maximum_suppression(boxes, scores, iou_threshold);
        ASSERT_EQ(result.size(), 1);
        EXPECT_EQ(result[0], 0);
    }
}

TYPED_TEST(test_geometry_non_maximum_suppression_t, boundary_values)
{
    using T = TypeParam;

    std::vector<tc::sdk::rectangle<T>> boxes = {
        tc::sdk::rectangle<T>(0, 0, 10, 10)};
    std::vector<T> scores = {T(0.5)};

    // Test with IoU threshold at minimum
    {
        constexpr double iou_threshold = 0.0;
        auto result1 = tc::sdk::non_maximum_suppression(boxes, scores, iou_threshold);
        ASSERT_EQ(result1.size(), 1);
    }

    // Test with IoU threshold at maximum
    {
        constexpr double iou_threshold = 1.0;
        auto result2 = tc::sdk::non_maximum_suppression(boxes, scores, iou_threshold);
        ASSERT_EQ(result2.size(), 1);
    }

    // Test with score threshold at boundary
    {
        // With score threshold 0.5, the box should be kept
        constexpr double iou_threshold = 0.5;
        constexpr double score_threshold = 0.5;
        auto result3 = tc::sdk::non_maximum_suppression(boxes, scores, iou_threshold, score_threshold);
        if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>)
        {
            ASSERT_EQ(result3.size(), 1);
        }
        else
        {
            ASSERT_EQ(result3.size(), 0);
        }
    }

    // Test with score threshold at boundary
    {
        // With score threshold 0.6, the box should be discarded
        constexpr double iou_threshold = 0.5;
        constexpr double score_threshold = 0.6;
        auto result4 = tc::sdk::non_maximum_suppression(boxes, scores, iou_threshold, score_threshold);
        ASSERT_EQ(result4.size(), 0);
    }
}

TYPED_TEST(test_geometry_non_maximum_suppression_t, identical_scores)
{
    using T = TypeParam;

    // Test with identical scores to verify stable sorting behavior
    std::vector<tc::sdk::rectangle<T>> boxes = {
        tc::sdk::rectangle<T>(0, 0, 10, 10),   // Box 0
        tc::sdk::rectangle<T>(20, 20, 10, 10), // Box 1
        tc::sdk::rectangle<T>(40, 40, 10, 10)  // Box 2
    };
    std::vector<T> scores = {T(0.8), T(0.8), T(0.8)}; // All same score

    auto result = tc::sdk::non_maximum_suppression(boxes, scores);

    // All boxes should be kept since they don't overlap
    ASSERT_EQ(result.size(), 3);

    // The order should be stable (original order when scores are equal)
    EXPECT_EQ(result[0], 0);
    EXPECT_EQ(result[1], 1);
    EXPECT_EQ(result[2], 2);
}

}
