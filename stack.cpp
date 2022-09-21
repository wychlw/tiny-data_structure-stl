#ifndef _MY_STACK
#define _MY_STACK

#include <functional>
#include <stddef.h>

namespace MY_LIB
{
    template <typename ValueT>
    class stack
    {
    public:
        using value_type = ValueT;
        using reference = ValueT &;
        using const_reference = const ValueT;
        using pointer = ValueT *;

        using size_type = ::size_t;
        using difference_type = ::ptrdiff_t;

    private:
        pointer m_data;
        pointer m_head;
        size_type m_size;
        size_type m_capacity;

    public:
        stack()
        {
            m_size = 0;
            m_capacity = 100;
            m_data = static_cast<pointer>(::operator new(m_capacity * sizeof(ValueT)));
            m_head = m_data;
        }

        stack(const stack &rhs)
        {
            m_size = 0;
            m_capacity = rhs.m_capacity;
            m_data = static_cast<pointer>(::operator new(m_capacity * sizeof(ValueT)));
            m_head = m_data;
            
            try
            {
                for (size_type i = 0; i < rhs.m_size; i++)
                {
                    ::new (&m_data[i]) ValueT(static_cast<ValueT &&>(rhs.m_data[i]));
                    m_size++;
                    m_head++;
                }
            }
            catch(...)
            {
                for (size_type i = 0; i < m_size; i++)
                {
                    m_data[i].~ValueT();
                }
                ::operator delete(m_data);
                m_size=0;
                m_head=m_data;
                throw;
            }
        }

        stack(stack &&rhs) {
            m_size=rhs.m_size;
            m_capacity=rhs.m_capacity;
            m_data=rhs.m_data;
            m_head=rhs.m_head;
            rhs.m_size=0;
            rhs.m_capacity=0;
            rhs.m_data=nullptr;
            rhs.m_head=nullptr;
        }

        ~stack(){
            for (size_type i = 0; i < m_size; i++)
                {
                    m_data[i].~ValueT();
                }
                ::operator delete(m_data);
                m_capacity=0;
                m_size=0;
                m_head=m_data;
        }

    private:
        void resize()
        {
            size_type new_capacity = m_capacity * 1.5;
            pointer new_data = static_cast<pointer>(::operator new(new_capacity * sizeof(value_type)));
            pointer new_head = new_data;
            size_type new_size = 0;

            try
            {
                for (size_type i = 0; i < m_size; i++)
                {
                    ::new (&new_data[i]) ValueT(static_cast<ValueT &&>(m_data[i]));
                    new_size++;
                    new_head++;
                }
            }
            catch(...)
            {
                for (size_type i = 0; i < new_size; i++)
                {
                    new_data[i].~ValueT();
                }
                ::operator delete(new_data);
                throw;
            }
            for (size_type i = 0; i < m_size; i++)
            {
                m_data[i].~ValueT();
            }
            ::operator delete(m_data);
            m_capacity = new_capacity;
            m_data = new_data;
            
        }

    public:
        template <typename... Args>
        void emplace(Args &&...args)
        {
            if (m_size >= m_capacity)
            {
                resize();
            }
            ::new (&m_data[m_size]) ValueT(::std::forward<Args>(args)...);
            m_size++;
            m_head++;
            return;
        }

        void push(const value_type &new_val)
        {
            emplace(new_val);
            return;
        }

        void push(value_type &&new_val) {
            emplace(new_val);
        }

        void pop() {
            (*(m_head-1)).~ValueT();
            m_head--;
            m_size--;
        }

    public:
        bool empty() {
            return m_size==0;
        }

        size_type size(){
            return m_size;
        }

        ValueT top() {
            return *(m_head-1);
        }
    };

} // namespace MY_LIB

#endif