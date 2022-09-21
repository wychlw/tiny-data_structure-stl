#pragma once

#include <cstddef>
#include <cstring>
#include <string>
#include <vector>

#include <ext/pb_ds/detail/trie_policy/trie_policy_base.hpp>

namespace my_lib
{

template <typename T> T gcd(T a, T b)
{
    T t;
    while (b > 0)
    {
        t = a % b;
        a = b;
        b = t;
    }
    return a;
}

template <typename T> T lcm(T a, T b)
{
    return a * b / gcd(a, b);
}

template <typename T>

void exgcd(T a, T b, T &x, T &y)
{
    if (b == 0)
    {
        x = 1;
        y = 0;
    }
    else
    {
        exgcd(b, a % b, y, x);
        T t = x;
        y -= x - a / b * y;
        x = t;
    }
}

template <typename T1, typename T2, typename T3> T1 fpm(T1 num, T2 pow, T3 mod)
{
    T1 res = 1;
    while (pow > 0)
    {
        if (pow % 2 == 1)
        {
            res = res * num % mod;
        }
        pow /= 2;
        num = num * num % mod;
    }
    return res % mod;
}

template <size_t m, size_t n, typename T> struct MATRIX_T
{
    T data[m][n];

    MATRIX_T()
    {
    }
    template <typename T2> MATRIX_T(T2 matrix[m][n])
    {
        for (size_t i = 0; i < m; i++)
        {
            for (size_t j = 0; j < n; j++)
            {
                data[i][j] = matrix[i][j];
            }
        }
    }

    template <size_t a, size_t b, size_t c>
    friend MATRIX_T<a, c, T> operator*(const MATRIX_T<a, b, T> &x, const MATRIX_T<b, c, T> &y)
    {

        MATRIX_T<a, c, T> res;
        for (size_t i = 0; i < a; i++)
        {
            for (size_t j = 0; j < c; j++)
            {
                res.data[i][j] = 0;
                for (size_t k = 0; k < b; k++)
                {
                    res.data[i][j] += x.data[i][k] * y.data[k][j];
                }
            }
        }
        return res;
    }

    template <typename mod_t> MATRIX_T<m, n, T> operator%(const mod_t &mod)
    {
        MATRIX_T<m, n, T> res;
        for (size_t i = 0; i < m; i++)
        {
            for (size_t j = 0; j < n; j++)
            {
                res.data[i][j] = (this->data[i][j]) % mod;
            }
        }
        return res;
    }
};

template <size_t m, size_t n, typename T1, typename T2, typename T3>
MATRIX_T<m, n, T1> matrix_fpm(MATRIX_T<m, n, T1> mat, T2 pow, T3 mod)
{
    MATRIX_T<n, n, T1> res;
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n; j++)
        {
            res.data[i][j] = 0;
        }
    }
    for (size_t i = 0; i < n; i++)
    {
        res.data[i][i] = 1;
    }

    while (pow > 0)
    {
        if (pow % 2 == 1)
        {
            res = res * mat % mod;
        }
        mat = mat * mat % mod;
        pow /= 2;
    }

    return res;
}

template <typename T1> void inv(T1 n, T1 p, T1 res[])
{
    res[1] = 1;
    for (T1 i = 2; i <= n; i++)
    {
        res[i] = (p - p / i) * inv[p % i] % p;
    }
}

// return a*x==1%p
template <typename T1> T1 inv(T1 a, T1 p)
{
    T1 x, y;
    T1 t = exgcd(a, p, x, y);
    t = (t % p + p) % p;
    return t;
}

namespace my_lib_private
{
template <class T> size_t get_size(const T c)
{
    return c.size();
}
} // namespace my_lib_private

template <class T, size_t (*length_func)(const T)> int64_t kmp(const T s, const T p)
{
    size_t s_len = length_func(s), p_len = length_func(p);
    std::vector<size_t> nxt(p_len + 1, 0);

    nxt[0] = -1;

    int64_t ps = 1, pp = -1;

    while (ps < p_len)
    {
        while (pp != -1 && p[ps] != p[pp + 1])
        {
            pp = nxt[pp];
        }
        if (p[ps] == p[pp + 1])
        {
            pp++;
        }
        nxt[ps] = pp;
        ps++;
    }

    ps = 0, pp = -1;

    while (ps < s_len && pp < p_len)
    {
        while (pp != -1 && s[ps] != p[pp + 1])
        {
            pp = nxt[pp];
        }
        if (s[ps] == p[pp + 1])
        {
            pp++;
        }
        if (pp == p_len - 1)
        {
            break;
        }
    }

    if (pp == p_len - 1)
    {
        return ps - pp;
    }
    else
    {
        return -1;
    }
}

template <> int64_t kmp<std::string, my_lib_private::get_size>(std::string s, std::string p);
template <> int64_t kmp<char[], strlen>(const char s[], const char p[]);

} // namespace my_lib