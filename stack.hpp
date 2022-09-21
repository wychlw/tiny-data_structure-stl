#pragma once
#include "queue.hpp"

namespace my_lib
{

template <typename ValueT, typename Allocator = deque<ValueT>> class stack
{
  public:
    using container = Allocator;

  private:
    container m_cont;

  public:
    stack()
    {
    }
    stack(const stack &rhs)
    {
        m_cont = rhs.m_cont;
    }
    stack &operator=(const stack &rhs)
    {
        m_cont = rhs.m_cont;
    }
    ~stack()
    {
    }

  public:
    ValueT top()
    {
        return m_cont.back();
    }
    void pop()
    {
        return m_cont.pop_back();
    }
    void push(const ValueT &data)
    {
        m_cont.push_back(data);
    }
    void push(ValueT &&data)
    {
        m_cont.push_back(data);
    }
    template <typename... Args> void emplace(Args... args)
    {
        emplace(std::forward<Args>(args)...);
    }

  private:
    template <typename... Args> void emplace(ValueT &&head, Args... args)
    {
        push(head);
        emplace(args...);
    }
    void emplace()
    {
    }

  public:
    bool empty()
    {
        return m_cont.empty();
    }
    size_t size()
    {
        return m_cont.size();
    }
};

}; // namespace my_lib