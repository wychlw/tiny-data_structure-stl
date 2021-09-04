#define _MY_LIST

#ifndef _STDDEF_H
#include <stddef.h>
#endif

namespace MY_LIB
{

    template <typename ValueT>
    struct list_node
    {
        list_node *prev, *next;
        ValueT *data_ptr;

        list_node() noexcept;
        list_node(ValueT) noexcept;
        list_node(ValueT *) noexcept;
        list_node(const list_node &) noexcept;
        list_node(list_node &&) noexcept;
        list_node(list_node*,list_node*) noexcept;
        list_node(list_node*,list_node*, ValueT) noexcept;
        list_node(list_node*,list_node*, ValueT*) noexcept;
        bool operator==(const list_node x, const list_node y) noexcept;
        bool operator!=(const list_node x, const list_node y) noexcept;
        ~list_node() noexcept;

        list_node() noexcept
        {
            list_node(this,this,nullptr);
        }

        list_node(ValueT new_data) noexcept
        {
            list_node(this,this,new_data);
        }

        list_node(ValueT *new_data_ptr) noexcept
        {
            list_node(this,this,new_data_ptr);
        }

        list_node(const list_node &rhs) noexcept
        {
            this->prev = rhs.prev;
            this->next = rhs.next;
            this->data_ptr = rhs.data_ptr;
        }

        list_node(list_node &&rhs) noexcept
        {
            list_node();
            list_node *tmp = this;
            this = rhs;
            rhs = tmp;
        }

        list_node(list_node *new_prev,list_node *new_next) noexcept{
            list_node(new_prev,new_next,nullptr);
        }

        list_node(list_node *new_prev,list_node *new_next, ValueT new_data) noexcept{
            this->prev=new_prev;
            this->next=new_next;
            this->data_ptr=new ValueT;
            *(this->data_ptr)=new_data;
        }

        list_node(list_node *new_prev,list_node *new_next, ValueT *new_data_ptr) noexcept{
            this->prev=new_prev;
            this->next=new_next;
            this->data_ptr=new_data_ptr;
        }

        bool operator==(const list_node x, const list_node y) noexcept
        {
            if (x.prev == y.prev && x.next == y.next && x.data_ptr == y.data_ptr)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        bool operator!=(const list_node x, const list_node y) noexcept
        {
            return !(x == y);
        }

        ~list_node() noexcept
        {
            delete this->data_ptr;
            data_ptr = nullptr;
            this->prev = this->next = this;
        }
    };

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

        list_iterator() noexcept;
        list_iterator(node *) noexcept;
        list_iterator(const self &) noexcept;
        self m_const_cast() const noexcept;
        reference operator*() const noexcept;
        pointer operator->() const noexcept;
        self &operator++() noexcept;
        self operator++(int) noexcept;
        self &operator--() noexcept;
        self operator--(int) noexcept;
        bool operator==(const self x, const self y) noexcept;
        bool operator!=(const self x, const self y) noexcept;

        list_iterator() noexcept
        {
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

        bool operator==(const self x, const self y) noexcept
        {
            return x.m_node == y.m_node;
        }

        bool operator!=(const self x, const self y) noexcept
        {
            return !(x == y);
        }
    };

    template <typename ValueT>
    struct list_const_iterator
    {
        using self = list_const_iterator<ValueT>;
        using node = const list_node<ValueT>;
        using iterator = list_iterator<ValueT>;

        using difference_type = ::ptrdiff_t;

        using value_type = ValueT;
        using pointer = ValueT *;
        using reference = ValueT &;

        //List node point to
        node *m_node;

        list_const_iterator() noexcept;
        list_const_iterator(node *) noexcept;
        list_const_iterator(const self &) noexcept;
        iterator m_const_cast() const noexcept;
        reference operator*() const noexcept;
        pointer operator->() const noexcept;
        self &operator++() noexcept;
        self operator++(int) noexcept;
        self &operator--() noexcept;
        self operator--(int) noexcept;
        bool operator==(const self x, const self y) noexcept;
        bool operator!=(const self x, const self y) noexcept;

        list_const_iterator() noexcept
        {
            this->m_node = nullptr;
        }

        list_const_iterator(node *new_node) noexcept
        {
            this->m_node = new_node;
        }

        list_const_iterator(const self &rhs) noexcept
        {
            this->m_node = rhs.m_node;
        }

        iterator m_const_cast() const noexcept
        {
            iterator ret(this->m_node);
            return ret;
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

        bool operator==(const self x, const self y) noexcept
        {
            return x.m_node == y.m_node;
        }

        bool operator!=(const self x, const self y) noexcept
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
        using const_iterator = list_const_iterator<ValueT>;

    private:
        pointer base;
        size_type m_size;

    public:
        constexpr list() noexcept {
            base = new value_type;
            m_size=0;
        }

        list (list &rhs) {
            base = new value_type;
            m_size=rhs.m_size;

        }

    public:     //insert and erase
        iterator insert(iterator pos,value_type &&new_data) {
            pointer tmp = new value_type(pos.m_node->prev,pos.m_node,new_data);
            pos.m_node->prev=tmp;
            return pos;
        }

        iterator insert(iterator pos,value_type &&new_data) {
            pointer tmp = new value_type(pos.m_node->prev,pos.m_node,new_data);
            pos.m_node->prev=tmp;
            return pos;
        }

        iterator insert(iterator pos,size_type n,const value_type &default_new_data) {
            for (size_type i=0;i<n;i++) {
                insert(pos,default_new_data);
            }
            return pos;
        }

        template <typename container>
        iterator insert(iterator pos,container::iterator begin,container::iterator end) {
            for (container::iterator it=begin;it!=end;it++) {
                insert(pos,*it);
            }
            return pos;
        }

        iterator erase(iterator pos) {
            if (pos.m_node == base) {
                return pos;
            }
            pos.m_node->prev=pos.m_node->next;
            iterator tmp = pos;
            pos++;
            tmp.m_node->~list_node();
            delete tmp.m_node;
            return pos;
        }

        iterator erase(iterator begin,iterator end) {
            iterator now = begin;
            while (now!=end) {
                now=erase(now);
            }
            return now;
        }
    
    };

} // namespace MY_LIB
