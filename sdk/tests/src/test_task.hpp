#pragma once

#include <gtest/gtest.h>
#include <sdk/task.hpp>

namespace tc::sdk::tests
{
bool global_function_invoked = false;
void global_function()
{
    global_function_invoked = true;
};

bool global_function_template_invoked = false;
template<class T>
void global_function_template()
{
    global_function_template_invoked = true;
};

struct Foo
{
    bool member_function_invoked = false;
    void member_function()
    {
        member_function_invoked = true;
    };

    bool member_function_template_invoked = false;    
    template<class T>
    void member_function_template()
    {
        member_function_template_invoked = true;
    };
};

template <class T>
struct FooT
{
public:
    bool member_function_template_class_invoked = false;
    void member_function_template_class()
    {
        member_function_template_class_invoked = true;
    };
};

struct Functor
{
    static bool functor_invoked;
    void operator()()
    {
        functor_invoked = true;
    }
};

}
