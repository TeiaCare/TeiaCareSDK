#include <sdk/uuid_generator.hpp>
#include <sdk/endianness.hpp>

#include <emmintrin.h>
#include <smmintrin.h>
#include <immintrin.h>

#include <random>
#include <limits>

/*
https://github.com/crashoz/uuid_v4/tree/master
*/

namespace tc::sdk
{
namespace
{
/*
  Converts an uuid string representation to a 128-bits unsigned int.
  Uses SIMD via Intel's AVX2 instruction set.
 */
__m128i inline stom128i(const char* mem)
{
    // Remove dashes and pack hex ascii bytes in a 256-bits int
    const __m256i dash_shuffle = _mm256_set_epi32(0x80808080, 0x0f0e0d0c, 0x0b0a0908, 0x06050403, 0x80800f0e, 0x0c0b0a09, 0x07060504, 0x03020100);

    __m256i x = betole256(_mm256_loadu_si256((__m256i*)mem));
    x = _mm256_shuffle_epi8(x, dash_shuffle);
    x = _mm256_insert_epi16(x, betole16(*(uint16_t*)(mem + 16)), 7);
    x = _mm256_insert_epi32(x, betole32(*(uint32_t*)(mem + 32)), 7);

    // Build a mask to apply a different offset to alphas and digits
    const __m256i sub = _mm256_set1_epi8(0x2F);
    const __m256i mask = _mm256_set1_epi8(0x20);
    const __m256i alpha_offset = _mm256_set1_epi8(0x28);
    const __m256i digits_offset = _mm256_set1_epi8(0x01);
    const __m256i unweave = _mm256_set_epi32(0x0f0d0b09, 0x0e0c0a08, 0x07050301, 0x06040200, 0x0f0d0b09, 0x0e0c0a08, 0x07050301, 0x06040200);
    const __m256i shift = _mm256_set_epi32(0x00000000, 0x00000004, 0x00000000, 0x00000004, 0x00000000, 0x00000004, 0x00000000, 0x00000004);

    // Translate ascii bytes to their value
    // i.e. 0x3132333435363738 -> 0x0102030405060708
    // Shift hi-digits
    // i.e. 0x0102030405060708 -> 0x1002300450067008
    // Horizontal add
    // i.e. 0x1002300450067008 -> 0x12345678
    __m256i a = _mm256_sub_epi8(x, sub);
    __m256i alpha = _mm256_slli_epi64(_mm256_and_si256(a, mask), 2);
    __m256i sub_mask = _mm256_blendv_epi8(digits_offset, alpha_offset, alpha);
    a = _mm256_sub_epi8(a, sub_mask);
    a = _mm256_shuffle_epi8(a, unweave);
    a = _mm256_sllv_epi32(a, shift);
    a = _mm256_hadd_epi32(a, _mm256_setzero_si256());
    a = _mm256_permute4x64_epi64(a, 0b00001000);

    return _mm256_castsi256_si128(a);
}
}

uuid_generator& uuid_generator::instance()
{
    static uuid_generator instance;
    return instance;
}

uuid uuid_generator::create()
{
    const __m128i and_mask = _mm_set_epi64x(0xFFFFFFFFFFFFFF3Full, 0xFF0FFFFFFFFFFFFFull);
    const __m128i or_mask = _mm_set_epi64x(0x0000000000000080ull, 0x0040000000000000ull);
    __m128i n = _mm_set_epi64x(_distribution(_generator), _distribution(_generator));
    __m128i uuid_bytes = _mm_or_si128(_mm_and_si128(n, and_mask), or_mask);

    tc::sdk::uuid uuid;
    _mm_store_si128((__m128i*)uuid.data, uuid_bytes);
    return uuid;
}

uuid uuid_generator::from_string(const std::string& s)
{
    return from_string(s.c_str());
}

uuid uuid_generator::from_string(const char* raw)
{
    __m128i uuid_bytes = stom128i(raw);

    tc::sdk::uuid uuid;
    _mm_store_si128((__m128i*)uuid.data, uuid_bytes);
    return uuid;
}

uuid_generator::uuid_generator()
    : _generator(std::mt19937_64(std::random_device()()))
    , _distribution(std::numeric_limits<uint64_t>::min(), std::numeric_limits<uint64_t>::max())
{
}

}
