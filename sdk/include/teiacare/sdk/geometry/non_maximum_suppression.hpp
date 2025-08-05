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

#pragma once

#include <teiacare/sdk/geometry/rectangle.hpp>

#include <algorithm>
#include <stdexcept>
#include <vector>

namespace tc::sdk
{
/**
 * @brief Perform Non-Maximum Suppression on bounding boxes
 * @tparam T The type used for rectangle coordinates and calculations
 * @param boxes Vector of bounding boxes
 * @param scores Vector of confidence scores (must match boxes size)
 * @param iou_threshold IoU threshold for suppression (default: 0.5)
 * @param score_threshold Minimum score threshold (default: 0.0)
 * @return Vector of indices of boxes to keep
 * @throws std::invalid_argument if input vectors have different sizes or are empty
 * @throws std::invalid_argument if IoU threshold is out of [0;1] range
 */
template <typename T>
std::vector<size_t> non_maximum_suppression(
    const std::vector<tc::sdk::rectangle<T>>& boxes,
    const std::vector<T>& scores,
    double iou_threshold = 0.5f,
    double score_threshold = 0.0f)
{
    // Input validation
    if (boxes.empty() || scores.empty())
    {
        return {};
    }

    if (boxes.size() != scores.size())
    {
        throw std::invalid_argument("Boxes and scores vectors must have the same size");
    }

    if (iou_threshold < 0.0 || iou_threshold > 1.0)
    {
        throw std::invalid_argument("IoU threshold must be in range (0;1]");
    }

    std::vector<size_t> indices;
    indices.reserve(boxes.size());

    for (size_t i = 0; i < boxes.size(); ++i)
    {
        if (scores[i] >= score_threshold)
        {
            indices.push_back(i);
        }
    }

    // Sort indices by scores in descending order
    std::sort(indices.begin(), indices.end(), [&scores](size_t idx1, size_t idx2) { return scores[idx1] > scores[idx2]; });

    // Use vector<char> instead of vector<bool> for better performance
    std::vector<char> keep(boxes.size(), 1);
    std::vector<size_t> final_indices;
    final_indices.reserve(indices.size());

    for (size_t i = 0; i < indices.size(); ++i)
    {
        const size_t current_idx = indices[i];

        if (!keep[current_idx])
        {
            continue;
        }

        final_indices.push_back(current_idx);

        // Suppress overlapping boxes
        for (size_t j = i + 1; j < indices.size(); ++j)
        {
            const size_t compare_idx = indices[j];

            if (!keep[compare_idx])
            {
                continue;
            }

            const auto current_box = boxes[current_idx];
            const auto compare_box = boxes[compare_idx];
            const double iou = current_box.calculate_iou(compare_box);
            if (iou > iou_threshold)
            {
                keep[compare_idx] = 0;
            }
        }
    }

    return final_indices;
}

}
