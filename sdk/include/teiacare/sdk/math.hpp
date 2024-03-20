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

#include <string>
#include <cmath>

namespace tc::sdk
{
/** Returns the larger of two values. */
template <typename T>
constexpr inline T max(T a, T b)
{
    return a < b ? b : a;
}

/** Returns the larger of three values. */
template <typename T>
constexpr inline T max(T a, T b, T c)
{
    return a < b ? (b < c ? c : b) : (a < c ? c : a);
}

/** Returns the larger of four values. */
template <typename T>
constexpr inline T max(T a, T b, T c, T d)
{
    return tc::sdk::max(a, tc::sdk::max(b, c, d));
}

/** Returns the smaller of two values. */
template <typename T>
constexpr inline T min(T a, T b)
{
    return b < a ? b : a;
}

/** Returns the smaller of three values. */
template <typename T>
constexpr inline T min(T a, T b, T c)
{
    return b < a ? (c < b ? c : b) : (c < a ? c : a);
}

/** Returns the smaller of four values. */
template <typename T>
constexpr inline T min(T a, T b, T c, T d)
{
    return tc::sdk::min(a, tc::sdk::min(b, c, d));
}

}