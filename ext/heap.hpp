#pragma once

#include "../queue.hpp"
#include <cstddef>
#include <functional>
#include <utility>

namespace my_lib
{

template <typename ValueT, typename Container = deque<ValueT>, class Comp = std::less<ValueT>> class heap
{

  public:
  private:
    Container m_data;
    size_t m_size;
    Comp cmp;

  public:
    heap()
    {
        m_data.push_back(0);
    }

    heap(std::initializer_list<ValueT> init)
    {
        m_data.push_back(0);
        for (auto i : init)
        {
            push(i);
        }
    }

    template <class InitCont> heap(InitCont init)
    {
        m_data.push_back(0);
        for (auto i : init)
        {
            push(i);
        }
    }

    heap(const heap &rhs)
    {
        m_data = rhs.m_data;
        m_size = rhs.m_size;
    }

    heap(heap &&rhs)
    {
        m_data = rhs.m_data;
        m_size = rhs.m_size;
    }

    heap &operator=(const heap &rhs)
    {
        m_data = rhs.m_data;
        m_size = rhs.m_size;
        return *this;
    }

    heap &operator=(const heap &&rhs)
    {
        m_data = rhs.m_data;
        m_size = rhs.m_size;
        return *this;
    }

  public:
    size_t size()
    {
        return m_size;
    }

    ValueT top()
    {
        return m_data[1];
    }

    void push(ValueT data)
    {
        m_data.push_back(data);
        m_size++;
        size_t now = m_size;
        size_t fa = now / 2;
        while (fa != 0 && !cmp(m_data[now], m_data[fa]))
        {
            std::swap(m_data[now], m_data[fa]);
            now = fa;
            fa = now / 2;
        }
    }

    void pop()
    {
        std::swap(m_data[1], m_data[m_size]);
        m_data.pop_back();
        m_size--;

        size_t now = 1;
        while (now * 2 <= m_size)
        {
            size_t p = now * 2;

            if (now * 2 + 1 <= m_size && cmp(m_data[now * 2], m_data[now * 2 + 1]))
            {
                p = now * 2 + 1;
            }
            if (!cmp(m_data[now], m_data[p]))
            {
                break;
            }

            std::swap(m_data[now], m_data[p]);
            now = p;
        }
    }
};

}; // namespace my_lib