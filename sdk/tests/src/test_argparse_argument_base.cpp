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

#include "test_argparse_argument_base.hpp"

namespace tc::sdk::tests
{
void set_env(const std::string& name, const std::string& value)
{
#ifdef _WIN32
    _putenv_s(name.c_str(), value.c_str());
#else
    setenv(name.c_str(), value.c_str(), 1);
#endif
}

void clear_env(const std::string& name)
{
#ifdef _WIN32
    _putenv_s(name.c_str(), "");
#else
    unsetenv(name.c_str());
#endif
}

TEST_F(test_argparse_argument_base, name_long)
{
    argument<std::string> arg("name_long");

    EXPECT_EQ(arg.get_name_long(), "name_long");
    EXPECT_EQ(arg.get_name_short(), "");
    EXPECT_EQ(arg.get_description(), "");
    EXPECT_EQ(arg.get_env(), "");
    EXPECT_FALSE(arg.is_parsed());
    EXPECT_FALSE(arg.is_required());
}

TEST_F(test_argparse_argument_base, name_short)
{
    argument<std::string> arg("name_long", "name_short");

    EXPECT_EQ(arg.get_name_long(), "name_long");
    EXPECT_EQ(arg.get_name_short(), "name_short");
    EXPECT_EQ(arg.get_description(), "");
    EXPECT_EQ(arg.get_env(), "");
    EXPECT_FALSE(arg.is_parsed());
    EXPECT_FALSE(arg.is_required());
}

TEST_F(test_argparse_argument_base, description)
{
    argument<std::string> arg("name_long", "name_short", "description");

    EXPECT_EQ(arg.get_name_long(), "name_long");
    EXPECT_EQ(arg.get_name_short(), "name_short");
    EXPECT_EQ(arg.get_description(), "description");
    EXPECT_EQ(arg.get_env(), "");
    EXPECT_FALSE(arg.is_parsed());
    EXPECT_FALSE(arg.is_required());
}

TEST_F(test_argparse_argument_base, required)
{
    argument<std::string> arg("name_long", "name_short", "description", true);

    EXPECT_EQ(arg.get_name_long(), "name_long");
    EXPECT_EQ(arg.get_name_short(), "name_short");
    EXPECT_EQ(arg.get_description(), "description");
    EXPECT_EQ(arg.get_env(), "");
    EXPECT_FALSE(arg.is_parsed());
    EXPECT_TRUE(arg.is_required());
}

TEST_F(test_argparse_argument_base, env_var)
{
    argument<std::string> arg("name_long", "name_short", "description", true, "env_var");

    EXPECT_EQ(arg.get_name_long(), "name_long");
    EXPECT_EQ(arg.get_name_short(), "name_short");
    EXPECT_EQ(arg.get_description(), "description");
    EXPECT_EQ(arg.get_env(), "env_var");
    EXPECT_FALSE(arg.is_parsed());
    EXPECT_TRUE(arg.is_required());
}

TEST_F(test_argparse_argument_base, parse)
{
    argument<std::string> arg("name_long");

    EXPECT_FALSE(arg.is_parsed());
    EXPECT_NO_THROW(arg.parse("dummy_string"));
    EXPECT_TRUE(arg.is_parsed());
}

TEST_F(test_argparse_argument_base, convert_string)
{
    using Type = std::string;
    argument<Type> arg("name_long");

    EXPECT_EQ(std::string("123"), arg.convert_wrapper("123"));
    EXPECT_EQ(std::string("123.456"), arg.convert_wrapper("123.456"));
    EXPECT_EQ(std::string("dummy_string"), arg.convert_wrapper("dummy_string"));
    EXPECT_EQ(std::string("123.456789"), arg.convert_wrapper("123.456789"));
    EXPECT_EQ(std::string("1,2,3,4,5"), arg.convert_wrapper("1,2,3,4,5"));
    EXPECT_EQ(std::string("a,b,c,d,e"), arg.convert_wrapper("a,b,c,d,e"));
}

TEST_F(test_argparse_argument_base, convert_const_char_ptr)
{
    using Type = const char*;
    argument<Type> arg("name_long");

    EXPECT_EQ(std::string("123"), std::string(arg.convert_wrapper("123")));
    EXPECT_EQ(std::string("123.456"), std::string(arg.convert_wrapper("123.456")));
    EXPECT_EQ(std::string("dummy_string"), std::string(arg.convert_wrapper("dummy_string")));
    EXPECT_EQ(std::string("123.456789"), std::string(arg.convert_wrapper("123.456789")));
    EXPECT_EQ(std::string("1,2,3,4,5"), std::string(arg.convert_wrapper("1,2,3,4,5")));
    EXPECT_EQ(std::string("a,b,c,d,e"), std::string(arg.convert_wrapper("a,b,c,d,e")));
}

TEST_F(test_argparse_argument_base, convert_int)
{
    using Type = int;
    argument<Type> arg("name_long");

    EXPECT_EQ(123, arg.convert_wrapper("123"));
    EXPECT_EQ(123, arg.convert_wrapper("123.456"));
    EXPECT_EQ(123, arg.convert_wrapper("123.456789"));

    EXPECT_EQ(1, arg.convert_wrapper("1,2,3,4,5"));
    EXPECT_THROW(arg.convert_wrapper("a,b,c,d,e"), std::runtime_error);
    EXPECT_THROW(arg.convert_wrapper("a,1,c,3,e"), std::runtime_error);
    EXPECT_THROW(arg.convert_wrapper("a1b2c3d4e5"), std::runtime_error);
}

TEST_F(test_argparse_argument_base, convert_float)
{
    using Type = float;
    argument<Type> arg("name_long");

    EXPECT_FLOAT_EQ(123, arg.convert_wrapper("123"));
    EXPECT_FLOAT_EQ(123.456f, arg.convert_wrapper("123.456"));
    EXPECT_FLOAT_EQ(123.456789f, arg.convert_wrapper("123.456789"));

    EXPECT_EQ(1, arg.convert_wrapper("1,2,3,4,5"));
    EXPECT_THROW(arg.convert_wrapper("a,b,c,d,e"), std::runtime_error);
    EXPECT_THROW(arg.convert_wrapper("a,1,c,3,e"), std::runtime_error);
    EXPECT_THROW(arg.convert_wrapper("a1b2c3d4e5"), std::runtime_error);
}

TEST_F(test_argparse_argument_base, convert_double)
{
    using Type = double;
    argument<Type> arg("name_long");

    EXPECT_DOUBLE_EQ(123, arg.convert_wrapper("123"));
    EXPECT_DOUBLE_EQ(123.456, arg.convert_wrapper("123.456"));
    EXPECT_DOUBLE_EQ(123.456789, arg.convert_wrapper("123.456789"));

    EXPECT_EQ(1, arg.convert_wrapper("1,2,3,4,5"));
    EXPECT_THROW(arg.convert_wrapper("a,b,c,d,e"), std::runtime_error);
    EXPECT_THROW(arg.convert_wrapper("a,1,c,3,e"), std::runtime_error);
    EXPECT_THROW(arg.convert_wrapper("a1b2c3d4e5"), std::runtime_error);
}

TEST_F(test_argparse_argument_base, convert_unsigned_long_long)
{
    using Type = unsigned long long;
    argument<Type> arg("name_long");

    EXPECT_EQ(123ull, arg.convert_wrapper("123"));

    EXPECT_THROW(arg.convert_wrapper("123.456789"), std::runtime_error);
    EXPECT_THROW(arg.convert_wrapper("1,2,3,4,5"), std::runtime_error);
    EXPECT_THROW(arg.convert_wrapper("a,b,c,d,e"), std::runtime_error);
    EXPECT_THROW(arg.convert_wrapper("a,1,c,3,e"), std::runtime_error);
    EXPECT_THROW(arg.convert_wrapper("a1b2c3d4e5"), std::runtime_error);
}

TEST_F(test_argparse_argument_base, convert_vector_string)
{
    using Type = std::vector<std::string>;
    argument<Type> arg("name_long");

    EXPECT_EQ((Type{}), arg.convert_wrapper(""));
    EXPECT_EQ((Type{""}), arg.convert_wrapper(","));
    EXPECT_EQ((Type{"a"}), arg.convert_wrapper("a"));
    EXPECT_EQ((Type{"a"}), arg.convert_wrapper("a,"));
    EXPECT_EQ((Type{"12345"}), arg.convert_wrapper("12345"));
    EXPECT_EQ((Type{"1", "2", "3", "4", "5"}), arg.convert_wrapper("1,2,3,4,5"));
    EXPECT_EQ((Type{"a", "b", "c", "d", "e"}), arg.convert_wrapper("a,b,c,d,e"));
    EXPECT_EQ((Type{" a", " b", " c", " d", " e"}), arg.convert_wrapper(" a, b, c, d, e")); // Note the spaces before each literal!
}

TEST_F(test_argparse_argument_base, convert_vector_int)
{
    using Type = std::vector<int>;
    argument<Type> arg("name_long");

    EXPECT_EQ(Type{}, arg.convert_wrapper(""));
    EXPECT_THROW(arg.convert_wrapper(","), std::runtime_error);
    EXPECT_THROW(arg.convert_wrapper("a"), std::runtime_error);
    EXPECT_THROW(arg.convert_wrapper("a,"), std::runtime_error);
    EXPECT_EQ((Type{12345}), arg.convert_wrapper("12345"));
    EXPECT_EQ((Type{1, 2, 3, 4, 5}), arg.convert_wrapper("1,2,3,4,5"));
    EXPECT_EQ((Type{1, 2, 3, 4, 5}), arg.convert_wrapper(" 1,  2,   3,    4,     5"));
    EXPECT_EQ((Type{1, 2}), arg.convert_wrapper("1.234,2.678"));
    EXPECT_THROW(arg.convert_wrapper("a,b,c,d,e"), std::runtime_error);
    EXPECT_THROW(arg.convert_wrapper("1,a,b,c,d,e"), std::runtime_error);
}

TEST_F(test_argparse_argument_base, convert_vector_float)
{
    using Type = std::vector<float>;
    argument<Type> arg("name_long");

    EXPECT_EQ(Type{}, arg.convert_wrapper(""));
    EXPECT_THROW(arg.convert_wrapper(","), std::runtime_error);
    EXPECT_THROW(arg.convert_wrapper("a"), std::runtime_error);
    EXPECT_THROW(arg.convert_wrapper("a,"), std::runtime_error);
    EXPECT_EQ((Type{1.2345f}), arg.convert_wrapper("1.2345"));
    EXPECT_EQ((Type{1.f, 2.f, 3.f, 4.f, 5.f}), arg.convert_wrapper("1,2,3,4,5"));
    EXPECT_EQ((Type{1.1f, 2.2f, 3.3f, 4.4f, 5.5f}), arg.convert_wrapper(" 1.1,  2.2,   3.3,    4.4,     5.5"));
    EXPECT_EQ((Type{1.234f, 2.456f}), arg.convert_wrapper("1.234,2.456"));
    EXPECT_THROW(arg.convert_wrapper("a,b,c,d,e"), std::runtime_error);
    EXPECT_THROW(arg.convert_wrapper("1,a,b,c,d,e"), std::runtime_error);
}

TEST_F(test_argparse_argument_base, convert_vector_double)
{
    using Type = std::vector<double>;
    argument<Type> arg("name_long");

    EXPECT_EQ(Type{}, arg.convert_wrapper(""));
    EXPECT_THROW(arg.convert_wrapper(","), std::runtime_error);
    EXPECT_THROW(arg.convert_wrapper("a"), std::runtime_error);
    EXPECT_THROW(arg.convert_wrapper("a,"), std::runtime_error);
    EXPECT_EQ((Type{1.2345}), arg.convert_wrapper("1.2345"));
    EXPECT_EQ((Type{1.f, 2.f, 3.f, 4.f, 5.f}), arg.convert_wrapper("1,2,3,4,5"));
    EXPECT_EQ((Type{1.1, 2.2, 3.3, 4.4, 5.5}), arg.convert_wrapper(" 1.1,  2.2,   3.3,    4.4,     5.5"));
    EXPECT_EQ((Type{1.234, 2.456}), arg.convert_wrapper("1.234,2.456"));
    EXPECT_THROW(arg.convert_wrapper("a,b,c,d,e"), std::runtime_error);
    EXPECT_THROW(arg.convert_wrapper("1,a,b,c,d,e"), std::runtime_error);
}

#if defined(WIN32)
TEST_F(test_argparse_argument_base, convert_filesystem_path)
{
    using Type = std::filesystem::path;
    argument<Type> arg("name_long");

    EXPECT_EQ(Type{}, arg.convert_wrapper(""));

    const auto non_spaced_path = std::filesystem::path("C:\\TeiaCare\\DummyDirectory\\log.txt");
    EXPECT_EQ(arg.convert_wrapper(R"(C:\\TeiaCare\\DummyDirectory\\log.txt)"), non_spaced_path);
    EXPECT_EQ(arg.convert_wrapper(R"(C:\TeiaCare\DummyDirectory\log.txt)"), non_spaced_path);
    EXPECT_EQ(arg.convert_wrapper(R"(C:/TeiaCare/DummyDirectory/log.txt)"), non_spaced_path);
    
    const auto spaced_path = std::filesystem::path("C:\\TeiaCare\\Dummy Directory\\log.txt");
    EXPECT_EQ(arg.convert_wrapper(R"(C:\\TeiaCare\\Dummy Directory\\log.txt)"), spaced_path);
    EXPECT_EQ(arg.convert_wrapper(R"(C:\TeiaCare\Dummy Directory\log.txt)"), spaced_path);
    EXPECT_EQ(arg.convert_wrapper(R"(C:/TeiaCare/Dummy Directory/log.txt)"), spaced_path);
}
#else
TEST_F(test_argparse_argument_base, convert_filesystem_path)
{
    using Type = std::filesystem::path;
    argument<Type> arg("name_long");

    EXPECT_EQ(Type{}, arg.convert_wrapper(""));

    const auto non_spaced_path = std::filesystem::path("/home/TeiaCare/DummyDirectory/log.txt");
    EXPECT_EQ(arg.convert_wrapper("/home/TeiaCare/DummyDirectory/log.txt"), non_spaced_path);
    
    const auto spaced_path = std::filesystem::path("/home/TeiaCare/Dummy Directory/log.txt");
    EXPECT_EQ(arg.convert_wrapper("/home/TeiaCare/Dummy Directory/log.txt"), spaced_path);
}
#endif

}
