#pragma once
#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iosfwd>
#include <memory>
#include <stdexcept>
#include <stdexcept>
#include <string>
#include <utility>
#include <type_traits>
#include <algorithm>
#include <iterator>
#include <new>
#include <utility>

#include "algorithm.hpp"
#include "vector.hpp"
#include "ext/char_traits.hpp"

namespace my_lib
{

template<class T,class traits=char_traits<T>>

class basic_string
{
    public:
        using traits_type=traits;
        using size_type=size_t;
        using value=T;
        using reference=T&;
        using const_reference=const T&;
        using pointer=T*;
        using const_pointer=const T*;
}

};