#include <iostream>
#include <random>
#include <array>
#include <limits>
#include <optional>

template <typename TChar>
[[nodiscard]] constexpr inline unsigned char hex2char(TChar const ch) noexcept
{
    if (ch >= static_cast<TChar>('0') && ch <= static_cast<TChar>('9'))
        return static_cast<unsigned char>(ch - static_cast<TChar>('0'));
    if (ch >= static_cast<TChar>('a') && ch <= static_cast<TChar>('f'))
        return static_cast<unsigned char>(10 + ch - static_cast<TChar>('a'));
    if (ch >= static_cast<TChar>('A') && ch <= static_cast<TChar>('F'))
        return static_cast<unsigned char>(10 + ch - static_cast<TChar>('A'));
    return 0;
}

template <typename TChar>
[[nodiscard]] constexpr inline bool is_hex(TChar const ch) noexcept
{
    return (ch >= static_cast<TChar>('0') && ch <= static_cast<TChar>('9')) ||
           (ch >= static_cast<TChar>('a') && ch <= static_cast<TChar>('f')) ||
           (ch >= static_cast<TChar>('A') && ch <= static_cast<TChar>('F'));
}

template <typename TChar>
[[nodiscard]] constexpr std::basic_string_view<TChar> to_string_view(TChar const* str) noexcept
{
    if (str)
        return str;
    return {};
}

class uuid
{
public:
    static inline constexpr char null[37] = "00000000-0000-0000-0000-000000000000";

    constexpr explicit uuid(const std::array<uint8_t, 16>& bytes) noexcept
        : data{bytes}
    {
    }

    [[nodiscard]] inline std::string to_string() const
    {
        static constexpr char guid_encoder[17] = "0123456789abcdef";
        std::string uuid_str{uuid::null};

        for (size_t i = 0, index = 0; i < 36; ++i)
        {
            if (i == 8 || i == 13 || i == 18 || i == 23)
            {
                continue;
            }

            uuid_str[i] = guid_encoder[data[index] >> 4 & 0x0f];
            uuid_str[++i] = guid_encoder[data[index] & 0x0f];
            index++;
        }

        return uuid_str;
    }

    template <typename StringType>
    [[nodiscard]] constexpr static bool is_valid(StringType const& in_str) noexcept
    {
        auto str = to_string_view(in_str);
        bool firstDigit = true;
        size_t hasBraces = 0;
        size_t index = 0;

        if (str.empty())
            return false;

        if (str.front() == '{')
            hasBraces = 1;
        if (hasBraces && str.back() != '}')
            return false;

        for (size_t i = hasBraces; i < str.size() - hasBraces; ++i)
        {
            if (str[i] == '-')
                continue;

            if (index >= 16 || !is_hex(str[i]))
            {
                return false;
            }

            if (firstDigit)
            {
                firstDigit = false;
            }
            else
            {
                index++;
                firstDigit = true;
            }
        }

        if (index < 16)
        {
            return false;
        }

        return true;
    }

    template <typename StringType>
    [[nodiscard]] constexpr static std::optional<uuid> from_string(StringType const& in_str) noexcept
    {
        auto str = to_string_view(in_str);
        bool is_first_digit = true;
        size_t has_braces = 0;
        size_t index = 0;

        std::array<uint8_t, 16> data{{0}};

        if (str.empty())
            return {};

        if (str.front() == '{')
            has_braces = 1;
        
        if (has_braces && str.back() != '}')
            return {};

        for (size_t i = has_braces; i < str.size() - has_braces; ++i)
        {
            if (str[i] == '-')
                continue;

            if (index >= 16 || !is_hex(str[i]))
            {
                return {};
            }

            if (is_first_digit)
            {
                data[index] = static_cast<uint8_t>(hex2char(str[i]) << 4);
                is_first_digit = false;
            }
            else
            {
                data[index] = static_cast<uint8_t>(data[index] | hex2char(str[i]));
                index++;
                is_first_digit = true;
            }
        }

        if (index < 16)
        {
            return {};
        }

        return uuid{data};
    }

    bool operator==(const uuid& other)
    {
        return data == other.data;
    }

    bool operator!=(const uuid& other)
    {
        return !operator==(other);
    }

    friend std::ostream& operator<<(std::ostream& stream, const uuid& u)
    {
        return stream << u.to_string();
    }

private:
    alignas(uint32_t) std::array<uint8_t, 16> data;
};

class uuid_random_generator
{
public:
    explicit uuid_random_generator()
        : generator(std::mt19937(std::random_device()()))
        , distribution(std::numeric_limits<uint32_t>::min(), std::numeric_limits<uint32_t>::max())
    {
    }

    [[nodiscard]] uuid create()
    {
        alignas(uint32_t) std::array<uint8_t, 16> bytes;

        for (int i = 0; i < 16; i += 4)
            *reinterpret_cast<uint32_t*>(bytes.data() + i) = distribution(generator);

        // variant must be 10xxxxxx
        bytes[8] &= 0xBF;
        bytes[8] |= 0x80;

        // version must be 0100xxxx
        bytes[6] &= 0x4F;
        bytes[6] |= 0x40;

        return uuid{bytes};
    }

private:
    std::mt19937 generator;
    std::uniform_int_distribution<uint32_t> distribution;
};

int main()
{
    // std::random_device rd;
    // auto seed_data = std::array<int, std::mt19937::state_size>{};
    // std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
    // std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
    // std::mt19937 generator(seq);
    // auto g = uuid_random_generator{generator};

    auto g = uuid_random_generator{};

    auto u = g.create();
    auto str = u.to_string();
    std::cout << str << std::endl;

    // std::cout << CppCommon::UUID::Nil() << std::endl;

    return 0;
}