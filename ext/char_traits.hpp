#pragma once

#include <cstddef>
#include <cstring>
#include <type_traits>

#if !defined(_LIBCPP_HAS_NO_WIDE_CHARACTERS)
#include <cwchar>
#endif

namespace my_lib
{

template <class T> struct char_traits
{
    using char_type = T;
    static inline constexpr void assign(char_type &dst, const char_type &src);
    static inline constexpr bool eq(const char_type &a, const char_type &b);
    static inline constexpr bool lt(const char_type &a, const char_type &b);

    static inline constexpr int compare(const char_type &a, const char_type &b);

    static inline int compare(const char_type *a, const char_type *b, size_t n);

    static inline size_t length(const char_type *s);
    static inline char_type *find(const char_type *s, size_t n, const char_type &ch);
    static inline char_type *move(char_type *dst, const char_type *src, size_t n);
    static inline char_type *copy(char_type *dst, const char_type *src, size_t n);
    static inline char_type *assign(char_type *dst, size_t n, const char_type &ch);
};

template <class T> inline constexpr void char_traits<T>::assign(char_type &dst, const char_type &src)
{
    dst = src;
}

template <class T> inline constexpr bool char_traits<T>::eq(const char_type &a, const char_type &b)
{
    {
        return a == b;
    }
}

template <class T> inline constexpr bool char_traits<T>::lt(const char_type &a, const char_type &b)
{
    {
        return a < b;
    }
}

template <class T> inline constexpr int char_traits<T>::compare(const char_type &a, const char_type &b)
{
    if (lt(a, b))
    {
        return -1;
    }
    if (lt(b, a))
    {
        return 1;
    }
    return 0;
}

template <class T> inline int char_traits<T>::compare(const char_type *a, const char_type *b, size_t n)
{
    while (n--)
    {
        if (lt(*a, *b))
        {
            return -1;
        }
        if (lt(*b, *a))
        {
            return 1;
        }
        ++a;
        ++b;
    }
    return 0;
}

template <class T> inline size_t char_traits<T>::length(const char_type *s)
{
    size_t res = 0;
    while (!eq(s, char_type(0)))
    {
        ++s;
        ++res;
    }
    return res;
}

template <class T>
inline typename char_traits<T>::char_type *char_traits<T>::find(const char_type *s, size_t n, const char_type &ch)
{
    while (n--)
    {
        if (eq(*s, ch))
        {
            return s;
        }
        s++;
    }
    return nullptr;
}

template <class T>
inline typename char_traits<T>::char_type *char_traits<T>::move(char_type *dst, const char_type *src, size_t n)
{
    if (n == 0)
    {
        return dst;
    }
    char_type *res = dst;

    if (dst < src)
    {
        while (n--)
        {
            assign(*dst, *src);
            dst++;
            src++;
        }
    }
    else if (dst > src)
    {
        dst += n;
        src += n;
        while (n--)
        {
            assign(*--dst, *--src);
        }
    }
    return res;
}

template <class T>
inline typename char_traits<T>::char_type *char_traits<T>::copy(char_type *dst, const char_type *src, size_t n)
{
    // static_assert(src < dst || src >= dst + n, "copy overlapped range");
    char_type *res = dst;
    while (n--)
    {
        assign(*dst, *src);
        dst++;
        src++;
    }
    return res;
}

template <class T>
inline typename char_traits<T>::char_type *char_traits<T>::assign(char_type *dst, size_t n, const char_type &ch)
{

    char_type *res = dst;
    while (n--)
    {
        assign(*dst, ch);
        dst++;
    }
    return res;
}

template <> struct char_traits<char>
{
    using char_type = char;

    static inline constexpr void assign(char_type &dst, const char_type &src);
    static inline constexpr bool eq(const char_type &a, const char_type &b);
    static inline constexpr bool lt(const char_type &a, const char_type &b);

    static inline constexpr int compare(const char_type &a, const char_type &b);

    static inline int compare(const char_type *a, const char_type *b, size_t n);

    static inline size_t length(const char_type *s);
    static inline char_type *find(const char_type *s, size_t n, const char_type &ch);
    static inline char_type *move(char_type *dst, const char_type *src, size_t n);
    static inline char_type *copy(char_type *dst, const char_type *src, size_t n);
    static inline char_type *assign(char_type *dst, size_t n, const char_type &ch);
};

inline constexpr void char_traits<char>::assign(char_type &dst, const char_type &src)
{
    dst = src;
}

inline constexpr bool char_traits<char>::eq(const char_type &a, const char_type &b)
{

    return a == b;
}

inline constexpr bool char_traits<char>::lt(const char_type &a, const char_type &b)
{

    return a < b;
}

inline constexpr int char_traits<char>::compare(const char_type &a, const char_type &b)
{
    if (lt(a, b))
    {
        return -1;
    }
    if (lt(b, a))
    {
        return 1;
    }
    return 0;
}

inline int char_traits<char>::compare(const char_type *a, const char_type *b, size_t n)
{
    if (n == 0)
    {
        return 0;
    }
    return memcmp(a, b, n);
}

inline size_t char_traits<char>::length(const char_type *s)
{
    return strlen(s);
}

inline typename char_traits<char>::char_type *char_traits<char>::find(const char_type *s, size_t n, const char_type &ch)
{
    if (n == 0)
    {
        return nullptr;
    }
    return static_cast<char_type *>(memchr(s, ch, n));
}

inline typename char_traits<char>::char_type *char_traits<char>::move(char_type *dst, const char_type *src, size_t n)
{
    return strncpy(dst, src, n);
}

inline typename char_traits<char>::char_type *char_traits<char>::copy(char_type *dst, const char_type *src, size_t n)
{
    // static_assert(src < dst || src >= dst + n, "copy overlapped range");
    return strncpy(dst, src, n);
}

inline typename char_traits<char>::char_type *char_traits<char>::assign(char_type *dst, size_t n, const char_type &ch)
{
    return static_cast<char_type *>(memset(dst, ch, n));
}

template <> struct char_traits<wchar_t>
{
    using char_type = wchar_t;

    static inline constexpr void assign(char_type &dst, const char_type &src);
    static inline constexpr bool eq(const char_type &a, const char_type &b);
    static inline constexpr bool lt(const char_type &a, const char_type &b);

    static inline constexpr int compare(const char_type &a, const char_type &b);

    static inline int compare(const char_type *a, const char_type *b, size_t n);

    static inline size_t length(const char_type *s);
    static inline char_type *find(const char_type *s, size_t n, const char_type &ch);
    static inline char_type *move(char_type *dst, const char_type *src, size_t n);
    static inline char_type *copy(char_type *dst, const char_type *src, size_t n);
    static inline char_type *assign(char_type *dst, size_t n, const char_type &ch);
};

inline constexpr void char_traits<wchar_t>::assign(char_type &dst, const char_type &src)
{
    dst = src;
}

inline constexpr bool char_traits<wchar_t>::eq(const char_type &a, const char_type &b)
{

    return a == b;
}

inline constexpr bool char_traits<wchar_t>::lt(const char_type &a, const char_type &b)
{

    return a < b;
}

inline constexpr int char_traits<wchar_t>::compare(const char_type &a, const char_type &b)
{
    if (lt(a, b))
    {
        return -1;
    }
    if (lt(b, a))
    {
        return 1;
    }
    return 0;
}

inline int char_traits<wchar_t>::compare(const char_type *a, const char_type *b, size_t n)
{
    if (n == 0)
    {
        return 0;
    }
    return wmemcmp(a, b, n);
}

inline size_t char_traits<wchar_t>::length(const char_type *s)
{
    return wcslen(s);
}

inline typename char_traits<wchar_t>::char_type *char_traits<wchar_t>::find(const char_type *s, size_t n, const char_type &ch)
{
    if (n == 0)
    {
        return nullptr;
    }
    return static_cast<char_type *>(wmemchr(s, ch, n));
}

inline typename char_traits<wchar_t>::char_type *char_traits<wchar_t>::move(char_type *dst, const char_type *src, size_t n)
{
    return wcsncpy(dst, src, n);
}

inline typename char_traits<wchar_t>::char_type *char_traits<wchar_t>::copy(char_type *dst, const char_type *src, size_t n)
{
    // static_assert(src < dst || src >= dst + n, "copy overlapped range");
    return wcsncpy(dst, src, n);
}

inline typename char_traits<wchar_t>::char_type *char_traits<wchar_t>::assign(char_type *dst, size_t n, const char_type &ch)
{
    return static_cast<char_type *>(wmemset(dst, ch, n));
}



} // namespace my_lib