#include <iostream>
#include <random>
#include <array>
#include <limits>
#include <optional>

class uuid
{
public:
    constexpr explicit uuid(const std::array<uint8_t, 16>& bytes) noexcept
        : _bytes{bytes}
    {
    }

    [[nodiscard]] constexpr inline std::string to_string() const
    {
        constexpr char uuid_characters[17] = "0123456789abcdef";
        constexpr char null[37] = "00000000-0000-0000-0000-000000000000";
        std::string uuid_str{null};

        for (size_t i = 0, index = 0; i < 36; ++i)
        {
            if (i == 8 || i == 13 || i == 18 || i == 23)
            {
                continue;
            }

            uuid_str[i] = uuid_characters[_bytes[index] >> 4 & 0x0f];
            uuid_str[++i] = uuid_characters[_bytes[index] & 0x0f];
            index++;
        }

        return uuid_str;
    }

    bool operator==(const uuid& other)
    {
        return _bytes == other._bytes;
    }

    bool operator!=(const uuid& other)
    {
        return !operator==(other);
    }

    bool operator<(const uuid& other)
    {
        return _bytes < other._bytes;
    }

    bool operator>(const uuid& other)
    {
        return _bytes > other._bytes;
    }

    bool operator<=(const uuid& other)
    {
        return !operator>(other);
    }

    bool operator>=(const uuid& other)
    {
        return !operator<(other);
    }

    friend std::ostream& operator<<(std::ostream& stream, const uuid& u)
    {
        return stream << u.to_string();
    }

private:
    alignas(uint32_t) std::array<uint8_t, 16> _bytes;
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

    [[nodiscard]] constexpr uuid from_string(std::string_view str) const
    {
        if (str.empty())
            throw std::runtime_error("Invalid uuid format: unable to generate uuid from an empty string");

        size_t has_braces = 0;
        if (str.front() == '{')
            has_braces = 1;

        if (has_braces && str.back() != '}')
            throw std::runtime_error("Invalid uuid format: first character is '{' but there is not a matching '}' as last character");

        size_t byte_index = 0;
        bool is_first_digit = true;
        alignas(uint32_t) std::array<uint8_t, 16> bytes;

        for (size_t i = has_braces; i < str.size() - has_braces; ++i)
        {
            if (str[i] == '-')
                continue;

            if (byte_index >= 16)
            {
                throw std::runtime_error("Invalid uuid format: uuid size is greater than 16 bytes");
            }

            if (!is_hex(str[i]))
            {
                throw std::runtime_error("Invalid uuid format: invalid character found: " + std::to_string(str[i]));
            }

            if (is_first_digit)
            {
                bytes[byte_index] = hex_to_uint8(str[i]) << 4;
                is_first_digit = false;
            }
            else
            {
                bytes[byte_index] = bytes[byte_index] | hex_to_uint8(str[i]);
                byte_index++;
                is_first_digit = true;
            }
        }

        if (byte_index < 16)
        {
            throw std::runtime_error("Invalid uuid format: uuid size is less than 16 bytes");
        }

        return uuid{bytes};
    }

    [[nodiscard]] constexpr bool is_valid(std::string_view str) const noexcept
    {
        if (str.empty())
            return false;

        size_t has_braces = 0;
        if (str.front() == '{')
            has_braces = 1;

        if (has_braces && str.back() != '}')
            return false;

        size_t byte_index = 0;
        bool is_first_digit = true;

        for (size_t i = has_braces; i < str.size() - has_braces; ++i)
        {
            if (str[i] == '-')
                continue;

            if (byte_index >= 16 || !is_hex(str[i]))
            {
                return false;
            }

            if (is_first_digit)
            {
                is_first_digit = false;
            }
            else
            {
                byte_index++;
                is_first_digit = true;
            }
        }

        if (byte_index < 16)
        {
            return false;
        }

        return true;
    }

private:
    std::mt19937 generator;
    std::uniform_int_distribution<uint32_t> distribution;

    [[nodiscard]] constexpr inline bool is_hex(const char c) noexcept
    {
        return (c >= char('0') && c <= char('9')) ||
            (c >= char('a') && c <= char('f')) ||
            (c >= char('A') && c <= char('F'));
    }

    [[nodiscard]] constexpr inline unsigned char hex_to_uchar(const char ch) noexcept
    {
        if (ch >= char('0') && ch <= char('9'))
            return static_cast<unsigned char>(ch - char('0'));

        if (ch >= char('a') && ch <= char('f'))
            return static_cast<unsigned char>(10 + ch - char('a'));

        if (ch >= char('A') && ch <= char('F'))
            return static_cast<unsigned char>(10 + ch - char('A'));

        return 0;
    }

    [[nodiscard]] constexpr inline uint8_t hex_to_uint8(const char ch) noexcept
    {
        if (ch >= char('0') && ch <= char('9'))
            return static_cast<uint8_t>(ch - char('0'));

        if (ch >= char('a') && ch <= char('f'))
            return static_cast<uint8_t>(10 + ch - char('a'));

        if (ch >= char('A') && ch <= char('F'))
            return static_cast<uint8_t>(10 + ch - char('A'));

        return 0;
    }
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
    auto u_str = u.to_string();
    std::cout << u_str << std::endl;

    auto u2 = g.from_string("376c4c64-a180-48ae-9f4a-b645e120d790");
    std::cout << "376c4c64-a180-48ae-9f4a-b645e120d790: " << u2.to_string() << std::endl;

    std::cout << "376c4c64-a180-48ae-9f4a-b645e120d790 is valid: " << g.is_valid("376c4c64-a180-48ae-9f4a-b645e120d790") << std::endl;
    std::cout << "{376c4c64-a180-48ae-9f4a-b645e120d790} is valid: " << g.is_valid("{376c4c64-a180-48ae-9f4a-b645e120d790}") << std::endl;
    std::cout << "376c4c64-xxxx-yyyy-9f4a-b645e120d790 is valid: " << g.is_valid("376c4c64-xxxx-yyyy-9f4a-b645e120d790") << std::endl;

    std::string_view sw = "123abc";

    return 0;
}