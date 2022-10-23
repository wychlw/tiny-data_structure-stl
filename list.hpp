#pragma once

#define _MY_LIST

#include <functional>
#include <cstddef>

namespace my_lib
{
    template <typename ValueT>
    struct list_node
    {
        list_node *prev, *next;
        ValueT *data_ptr;

        constexpr list_node() noexcept
        {
            list_node_init(this, this, nullptr);
        }

        list_node(ValueT new_data) noexcept
        {
            list_node_init(this, this, new_data);
        }

        list_node(ValueT *new_data_ptr) noexcept
        {
            list_node_init(this, this, new_data_ptr);
        }

        list_node(const list_node &rhs) noexcept
        {
            this->prev = rhs.prev;
            this->next = rhs.next;
            this->data_ptr = rhs.data_ptr;
        }

        list_node(list_node &&rhs) noexcept
        {
            list_node_init(this, this, nullptr);
            list_node *tmp = this;
            this = rhs;
            rhs = tmp;
        }

        list_node(list_node *new_prev, list_node *new_next) noexcept
        {
            list_node_init(new_prev, new_next, nullptr);
        }

        list_node(list_node *new_prev, list_node *new_next, ValueT new_data) noexcept
        {
            list_node_init(new_prev, new_next, new_data);
        }

        list_node(list_node *new_prev, list_node *new_next, ValueT *new_data_ptr) noexcept
        {
            list_node_init(new_prev, new_next, new_data_ptr);
        }

        void list_node_init(list_node *new_prev, list_node *new_next, ValueT *new_data_ptr) noexcept
        {
            this->prev = new_prev;
            this->next = new_next;
            this->data_ptr = new_data_ptr;
        }

        void list_node_init(list_node *new_prev, list_node *new_next, ValueT new_data) noexcept
        {
            this->prev = new_prev;
            this->next = new_next;
            this->data_ptr = new ValueT;
            *(this->data_ptr) = new_data;
        }

        bool operator==(const list_node target) noexcept
        {
            if (prev == target.prev && next == target.next && data_ptr == target.data_ptr)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        bool operator!=(const list_node target) noexcept
        {
            return !(*this == target);
        }

        ~list_node() noexcept
        {
            delete this->data_ptr;
            data_ptr = nullptr;
            this->prev = this->next = this;
        }
    };

    template<typename T>
    void swap(list_node<T> &a,list_node<T> &b)
    {
        std::swap(a.data_ptr,b.data_ptr);
    }

    template <typename ValueT>
    struct list_iterator
    {
        
        using self = list_iterator<ValueT>;
        using node = list_node<ValueT>;

        using difference_type = ::ptrdiff_t;

        using value_type = ValueT;
        using pointer = ValueT *;
        using reference = ValueT &;

        //List node point to
        node *m_node;

        list_iterator() noexcept
        {
            int a,b;
            this->m_node = nullptr;
        }

        list_iterator(node *new_node) noexcept
        {
            this->m_node = new_node;
        }

        list_iterator(const self &rhs) noexcept
        {
            this->m_node = rhs.m_node;
        }

        self m_const_cast() const noexcept
        {
            return *this;
        }

        reference operator*() const noexcept
        {
            return *(this->m_node->data_ptr);
        }

        pointer operator->() const noexcept
        {
            return this->m_node->data_ptr;
        }

        self &operator++() noexcept
        {
            m_node = m_node->next;
            return *this;
        }

        self operator++(int) noexcept
        {
            self tmp = *this;
            m_node = m_node->next;
            return tmp;
        }

        self &operator--() noexcept
        {
            m_node = m_node->prev;
            return *this;
        }

        self operator--(int) noexcept
        {
            self tmp = *this;
            m_node = m_node->prev;
            return tmp;
        }

        friend bool operator==(const self x, const self y) noexcept
        {
            return x.m_node == y.m_node;
        }

        friend bool operator!=(const self x, const self y) noexcept
        {
            return !(x == y);
        }
    };

    template <typename ValueT>
    class list
    {
    public:
        using value_type = list_node<ValueT>;
        using reference = list_node<ValueT> &;
        using pointer = list_node<ValueT> *;
        using const_reference = const list_node<ValueT>;

        using size_type = ::size_t;
        using defference_type = ::ptrdiff_t;

        using iterator = list_iterator<ValueT>;

    private:
        pointer base;
        size_type m_size;

    public:
        constexpr list() noexcept
        {
            base = new value_type;
            m_size = 0;
        }

        list(const list &rhs)
        {
            base = new value_type;
            m_size = 0;
            try
            {
                for (iterator it = rhs.begin(); it != rhs.end(); it++)
                {
                    push_back(*it);
                    m_size++;
                }
            }
            catch (...)
            {
                clear();
                throw;
            }
        }

        list(list &&rhs)
        {
            base = rhs.base;
            m_size = rhs.m_size;
            rhs.m_size = 0;
            rhs.base = new value_type;
        }

        ~list()
        {
            clear();
            delete base;
        }

    public: //insert and erase
        iterator insert(iterator pos, const ValueT &new_data)
        {
            pointer tmp = new value_type(pos.m_node->prev, pos.m_node, new_data);
            pos.m_node->prev->next = tmp;
            pos.m_node->prev = tmp;
            m_size++;
            return pos;
        }

        iterator insert(iterator pos, ValueT &&new_data)
        {
            pointer tmp = new value_type(pos.m_node->prev, pos.m_node, new_data);
            pos.m_node->prev->next = tmp;
            pos.m_node->prev = tmp;
            m_size++;
            return pos;
        }

        iterator insert(iterator pos, size_type n, const ValueT &default_new_data)
        {
            for (size_type i = 0; i < n; i++)
            {
                insert(pos, default_new_data);
            }
            return pos;
        }

        template <typename container_iterator>
        iterator insert(iterator pos, container_iterator it_begin, container_iterator it_end)
        {
            for (container_iterator it = it_begin; it != it_end; it++)
            {
                insert(pos, *it);
            }
            return pos;
        }

        iterator erase(iterator pos)
        {
            if (pos.m_node == base)
            {
                return pos;
            }
            pos.m_node->prev->next = pos.m_node->next;
            pos.m_node->next->prev = pos.m_node->prev;
            iterator tmp = pos;
            pos++;
            tmp.m_node->~list_node();
            delete tmp.m_node;
            m_size--;
            return pos;
        }

        iterator erase(iterator begin, iterator end)
        {
            iterator now = begin;
            while (now != end)
            {
                now = erase(now);
            }
            return now;
        }

    public: //push and pop
        void push_back(const ValueT &new_data)
        {
            insert(end(), new_data);
        }

        void push_back(ValueT &&new_data)
        {
            insert(end(), new_data);
        }

        void pop_back()
        {
            erase(iterator(end().m_node->prev));
        }

        void push_front(const ValueT &new_data)
        {
            insert(begin(), new_data);
        }

        void push_front(ValueT &&new_data)
        {
            insert(begin(), new_data);
        }

        void pop_front()
        {
            erase(iterator(begin()));
        }

        template <typename... Args>
        void emplace_back(Args &&...args)
        {
            insert(end(), std::forward<Args>(args)...);
        }

        template <typename... Args>
        void emplace_front(Args &&...args)
        {
            insert(begin(), std::forward<Args>(args)...);
        }

        template <typename... Args>
        iterator emplace(iterator pos, Args &&...args)
        {
            insert(pos, std::forward<Args>(args)...);
            return pos;
        }

    public: //iterator
        iterator begin() noexcept
        {
            iterator ret(this->base->next);
            return ret;
        }

        iterator end() noexcept
        {
            iterator ret(this->base);
            return ret;
        }

    public:
        void clear()
        {
            erase(begin(), end());
        }

        ValueT front()
        {
            return *iterator(this->base->next);
        }

        ValueT back()
        {
            return *iterator(this->base->prev);
        }

        size_type size()
        {
            return m_size;
        }

        bool empty()
        {
            return m_size==0;
        }
    };
} // namespace my_lib
