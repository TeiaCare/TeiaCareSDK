#pragma once

// NOLINTBEGIN
#define TC_NO_COPY(Class)                           \
public:                                             \
    Class(const Class&) = delete;                   \
    auto operator=(const Class&)->Class& = delete;
// NOLINTEND

// NOLINTBEGIN
#define TC_NO_MOVE(Class)                       \
public:                                         \
    Class(Class&&) = delete;                    \
    auto operator=(Class&&)->Class& = delete;
// NOLINTEND

// NOLINTBEGIN
#define TC_SEALED(Class) \
    TC_NO_COPY(Class)    \
    TC_NO_MOVE(Class)
// NOLINTEND

//NOLINTNEXTLINE
#define TC_UNUSED(x) ((void)x);