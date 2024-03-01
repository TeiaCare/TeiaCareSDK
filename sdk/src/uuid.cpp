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

#include <teiacare/sdk/endianness.hpp>
#include <teiacare/sdk/uuid.hpp>

#include <emmintrin.h>
#include <immintrin.h>
#include <smmintrin.h>
#include <sstream>

/*
https://github.com/crashoz/uuid_v4/tree/master
*/

namespace tc::sdk
{
namespace
{
/*
  Converts a 128-bits unsigned int to an UUIDv4 string representation.
  Uses SIMD via Intel's AVX2 instruction set.
 */
void inline m128itos(__m128i x, char* mem)
{
    // Expand each byte in x to two bytes in res
    // i.e. 0x12345678 -> 0x0102030405060708
    // Then translate each byte to its hex ascii representation
    // i.e. 0x0102030405060708 -> 0x3132333435363738
    const __m256i mask = _mm256_set1_epi8(0x0F);
    const __m256i add = _mm256_set1_epi8(0x06);
    const __m256i alpha_mask = _mm256_set1_epi8(0x10);
    const __m256i alpha_offset = _mm256_set1_epi8(0x57);

    __m256i a = _mm256_castsi128_si256(x);
    __m256i as = _mm256_srli_epi64(a, 4);
    __m256i lo = _mm256_unpacklo_epi8(as, a);
    __m128i hi = _mm256_castsi256_si128(_mm256_unpackhi_epi8(as, a));
    __m256i c = _mm256_inserti128_si256(lo, hi, 1);
    __m256i d = _mm256_and_si256(c, mask);
    __m256i alpha = _mm256_slli_epi64(_mm256_and_si256(_mm256_add_epi8(d, add), alpha_mask), 3);
    __m256i offset = _mm256_blendv_epi8(_mm256_slli_epi64(add, 3), alpha_offset, alpha);
    __m256i res = _mm256_add_epi8(d, offset);

    // Add dashes between blocks as specified in RFC-4122
    // 8-4-4-4-12
    const __m256i dash_shuffle = _mm256_set_epi32(0x0b0a0908, 0x07060504, 0x80030201, 0x00808080, 0x0d0c800b, 0x0a090880, 0x07060504, 0x03020100);
    const __m256i dash = _mm256_set_epi64x(0x0000000000000000ull, 0x2d000000002d0000ull, 0x00002d000000002d, 0x0000000000000000ull);

    __m256i resd = _mm256_shuffle_epi8(res, dash_shuffle);
    resd = _mm256_or_si256(resd, dash);

    _mm256_storeu_si256((__m256i*)mem, betole256(resd));

#if defined(MSVC)
    #pragma warning(disable : 4244)
#endif
    *(uint16_t*)(mem + 16) = betole16(_mm256_extract_epi16(res, 7));
    *(uint32_t*)(mem + 32) = betole32(_mm256_extract_epi32(res, 7));
#if defined(MSVC)
    #pragma warning(default : 4244)
#endif
}
}

uuid::uuid(const uuid& other)
{
    __m128i x = _mm_load_si128((__m128i*)other.data);
    _mm_store_si128((__m128i*)data, x);
}

uuid::uuid(uint64_t x, uint64_t y)
{
    __m128i z = _mm_set_epi64x(x, y);
    _mm_store_si128((__m128i*)data, z);
}

uuid::uuid(const uint8_t* bytes)
{
    __m128i x = _mm_loadu_si128((__m128i*)bytes);
    _mm_store_si128((__m128i*)data, x);
}

uuid::uuid(const std::string& bytes)
{
    __m128i x = betole128(_mm_loadu_si128((__m128i*)bytes.data()));
    _mm_store_si128((__m128i*)data, x);
}

uuid& uuid::operator=(const uuid& other)
{
    if (&other == this)
    {
        return *this;
    }
    __m128i x = _mm_load_si128((__m128i*)other.data);
    _mm_store_si128((__m128i*)data, x);
    return *this;
}

std::string uuid::bytes() const
{
    std::string uuid_bytes;
    uuid_bytes.resize(sizeof(data));
    bytes((char*)uuid_bytes.data());
    return uuid_bytes;
}

void uuid::bytes(char* bytes) const
{
    __m128i x = betole128(_mm_load_si128((__m128i*)data));
    _mm_storeu_si128((__m128i*)bytes, x);
}

std::string uuid::str() const
{
    std::string uuid_str;
    uuid_str.resize(36);
    str((char*)uuid_str.data());
    return uuid_str;
}

void uuid::str(char* res) const
{
    __m128i x = _mm_load_si128((__m128i*)data);
    m128itos(x, res);
}

size_t uuid::hash() const
{
    const uint64_t a = *((uint64_t*)data);
    const uint64_t b = *((uint64_t*)&data[8]);
    return a ^ (b + 0x9e3779b9 + (a << 6) + (a >> 2));
}

bool operator==(const uuid& lhs, const uuid& rhs)
{
    __m128i x = _mm_load_si128((__m128i*)lhs.data);
    __m128i y = _mm_load_si128((__m128i*)rhs.data);

    __m128i neq = _mm_xor_si128(x, y);
    return _mm_test_all_zeros(neq, neq);
}

bool operator<(const uuid& lhs, const uuid& rhs)
{
    // There are no trivial 128-bits comparisons in SSE/AVX
    // It's faster to compare two uint64_t
    uint64_t* x = (uint64_t*)lhs.data;
    uint64_t* y = (uint64_t*)rhs.data;
    return *x < *y || (*x == *y && *(x + 1) < *(y + 1));
}

bool operator!=(const uuid& lhs, const uuid& rhs)
{
    return !(lhs == rhs);
}

bool operator>(const uuid& lhs, const uuid& rhs)
{
    return rhs < lhs;
}

bool operator<=(const uuid& lhs, const uuid& rhs)
{
    return !(lhs > rhs);
}

bool operator>=(const uuid& lhs, const uuid& rhs)
{
    return !(lhs < rhs);
}

std::ostream& operator<<(std::ostream& stream, const uuid& uuid)
{
    return stream << uuid.str();
}

}
