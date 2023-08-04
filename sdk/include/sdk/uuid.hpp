#pragma once

#include <string>

namespace tc::sdk
{
class uuid_generator;

class uuid
{
    friend class tc::sdk::uuid_generator;

public:
    explicit uuid() = default;
    uuid(const uuid& other);
    uuid& operator=(const uuid& other);
    
    std::string bytes() const;
    void bytes(std::string& out) const;
    
    std::string str() const;
    void str(std::string& str) const;

    size_t hash() const;

    friend bool operator==(const uuid& lhs, const uuid& rhs);
    friend bool operator<(const uuid& lhs, const uuid& rhs);
    friend bool operator!=(const uuid& lhs, const uuid& rhs);
    friend bool operator>(const uuid& lhs, const uuid& rhs);
    friend bool operator<=(const uuid& lhs, const uuid& rhs);
    friend bool operator>=(const uuid& lhs, const uuid& rhs);

    friend std::ostream& operator<<(std::ostream& stream, const uuid& uuid);

private:
    uuid(uint64_t x, uint64_t y);
    uuid(const uint8_t* bytes);
    uuid(const std::string& bytes);

    void bytes(char* bytes) const;
    void str(char* res) const;

    alignas(128) uint8_t data[16];
};

}

namespace std
{
template <>
struct hash<tc::sdk::uuid>
{
    size_t operator()(const tc::sdk::uuid& uuid) const
    {
        return uuid.hash();
    }
};

}
