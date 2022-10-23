#ifndef _MY_VECTOR
#define _MY_VECTOR

#include <exception>
#include <functional>
#include <stddef.h>
#include <utility>

namespace my_lib
{

template <typename ValueT>

class vector
{

  public: // 定义
    using value_type = ValueT;
    using reference = ValueT &;
    using const_reference = const ValueT;

    using iterator = ValueT *;
    using const_iterator = const ValueT *;

    using size_type = ::size_t;
    using difference_type = ::ptrdiff_t;

  private:
    ValueT *m_data;
    ::size_t m_size;
    ::size_t m_capacity;

  public: // 构造
    constexpr vector() noexcept
    {
        this->m_capacity = 100;
        this->m_size = 0;
        this->m_data = static_cast<ValueT *>(::operator new(this->m_capacity * sizeof(ValueT)));
    }

    vector(const vector &rhs)
    {
        this->m_data = static_cast<ValueT *>(::operator new(rhs.m_capacity * sizeof(ValueT)));

        this->m_size = 0;
        this->m_capacity = rhs.m_capacity;

        try
        {
            for (::size_t k = 0; k < rhs.m_size; k++)
            {
                ::new (&this->m_data[k]) ValueT(rhs.m_data[k]);
                this->m_size += 1;
            }
        }
        catch (...)
        {
            this->empty_memory();
            throw std::runtime_error("copy failed");
        }
    }

    vector(vector &&rhs) noexcept
    {
        this->m_data = rhs.m_data;
        this->m_size = rhs.m_size;
        this->m_capacity = rhs.m_capacity;

        rhs.m_data = nullptr;
        rhs.m_size = 0;
        rhs.m_capacity = 0;
    }

  public: // 析构
    ~vector()
    {
        this->empty_memory();
    }

  public:
    vector &operator=(const vector &rhs);

    vector &operator=(vector &&rhs) noexcept;

  private:
    void empty_memory() noexcept
    {
        this->clear();
        ::operator delete(this->m_data);
    }

  public: // iterators
    iterator begin() noexcept
    {
        return this->m_data;
    }

    const_iterator begin() const noexcept
    {
        return this->m_data;
    }

    iterator end() noexcept
    {
        return this->m_data + this->m_size;
    }

    const_iterator end() const noexcept
    {
        return this->m_data + this->m_size;
    }

  public: // allocator
    value_type *data() noexcept
    {
        return this->m_data;
    }

    const value_type *data() const noexcept
    {
        return this->m_data;
    }

    size_type size() const noexcept
    {
        return this->m_size;
    }

    size_type capacity() const noexcept
    {
        return this->m_capacity;
    }

    bool empty() const noexcept
    {
        return this->m_size == 0;
    }

  private:
    void resize()
    {
        ::size_t new_capacity = this->m_capacity * 1.5;
        ValueT *new_data = static_cast<ValueT *>(::operator new(new_capacity * sizeof(ValueT)));
        ::size_t new_size = 0;

        try
        {
            for (::size_t k = 0; k < this->m_size; k++)
            {
                ::new (&new_data[k]) ValueT(::std::move(this->m_data[k]));
                new_size += 1;
            }
        }
        catch (...)
        {
            for (::size_t k = 0; k < new_size; k++)
            {
                new_data[k].~ValueT();
            }
            ::operator delete(new_data);
            throw;
        }

        for (::size_t k = 0; k < this->m_size; k++)
        {
            this->m_data[k].~ValueT();
        }

        if (this->m_data)
        {
            delete (this->m_data);
        }

        this->m_data = new_data;
        this->m_size = new_size;
        this->m_capacity = new_capacity;
    }

  public: // modifier
    void clear() noexcept
    {
        for (::size_t k = 0; k < this->m_size; k++)
        {
            this->m_data[k].~ValueT();
        }
        this->m_size = 0;
    }

    void pop_back()
    {
        if (this->empty())
        {
            throw std::out_of_range("Can't pop when vector is empty!");
        }

        this->m_data[this->m_size - 1].~ValueT();
        this->m_size -= 1;
    }

    template <typename... ArgsT> reference emplace_back(ArgsT &&...args)
    {
        if (this->m_size < this->m_capacity)
        {
            ::new (&this->m_data[this->m_size]) ValueT(::std::forward<ArgsT>(args)...);
            this->m_size += 1;
            return this->m_data[this->m_size - 1];
        }

        resize();

        ::new (&this->m_data[this->m_size]) ValueT(::std::forward<ArgsT>(args)...);
        this->m_size += 1;
        return this->m_data[this->m_size - 1];
    }

    void push_back(const ValueT &value)
    {
        this->emplace_back(value);
    }

    void push_back(const ValueT &&value)
    {
        this->emplace_back(::std::move(value));
    }

    ValueT &front()
    {
        return *(begin());
    }

    ValueT &back()
    {
        return *(end() - 1);
    }

  public: // element access
    ValueT &at(size_type n)
    {
        return m_data[n];
    }

    ValueT &operator[](size_type n)
    {
        return at(n);
    }
};

} // namespace my_lib

#endif