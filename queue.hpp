#pragma once
#include <algorithm>
#include <cstddef>
#include <stdexcept>
#include <utility>

namespace my_lib
{

template <typename ValueT> struct _deque_chunk
{
    const size_t chunk_size = 100;
    ValueT *chunk;
    _deque_chunk *prev, *next;
    size_t front, back;

    _deque_chunk(size_t _front)
    {
        chunk = new ValueT[chunk_size];
        prev = nullptr;
        next = nullptr;
        front = _front;
        back = _front;
    }
    _deque_chunk(size_t _front, _deque_chunk *_prev, _deque_chunk *_next)
    {
        chunk = new ValueT[chunk_size];
        prev = _prev;
        next = _next;
        front = _front;
        back = _front;
    }
    _deque_chunk(const _deque_chunk &rhs)
    {
        chunk = new ValueT[chunk_size];
        prev = rhs.prev;
        next = rhs.next;
        front = rhs.front;
        back = rhs.back;
        for (size_t i = front; i != back; i++)
        {
            chunk[i] = rhs.chunk[i];
        }
    }
    ~_deque_chunk()
    {
        delete chunk;
    }
    bool hit_begin()
    {
        return front == 0;
    }
    bool hit_end()
    {
        return back == chunk_size;
    }
    void push_front(const ValueT &data)
    {
        if (!hit_begin())
        {
            chunk[--front] = data;
        }
    }
    void push_back(const ValueT &data)
    {
        if (!hit_end())
        {
            chunk[back++] = data;
        }
    }
    void pop_front()
    {
        if (front < back)
        {
            front++;
        }
    }
    void pop_back()
    {
        if (front < back)
        {
            back--;
        }
    }
    ValueT &at(size_t n)
    {
        return chunk[n];
    }
    ValueT &operator[](size_t n)
    {
        return at(n);
    }
};

template <typename ValueT> struct _deque_iterator
{
    using chunk_pointer = _deque_chunk<ValueT> *;
    size_t offset;
    chunk_pointer chunk;

    _deque_iterator()
    {
        offset = 0;
        chunk = nullptr;
    }
    _deque_iterator(size_t _offset, chunk_pointer _chunk)
    {
        offset = _offset;
        chunk = _chunk;
    }

    _deque_iterator(const _deque_iterator &rhs)
    {
        offset = rhs.offset;
        chunk = rhs.chunk;
    }

    _deque_iterator &operator=(const _deque_iterator &) = default;

    friend bool operator ==(const _deque_iterator&a,const _deque_iterator &b)
    {
        return a.chunk==b.chunk&&a.offset==b.offset;
    }

    friend bool operator !=(const _deque_iterator&a,const _deque_iterator &b)
    {
        return !(a==b);
    }

    ValueT &operator*()
    {
        return chunk->at(offset);
    }
    ValueT *operator->()
    {
        return &(chunk->at(offset));
    }
    _deque_iterator &operator++()
    {
        if (offset + 1 == chunk->chunk_size)
        {
            offset = 0;
            chunk = chunk->next;
        }
        else
        {
            offset++;
        }
        return *this;
    }
    _deque_iterator operator++(int)
    {
        _deque_iterator res = *this;
        ++*this;
        return res;
    }
    _deque_iterator &operator--()
    {
        if (offset == 0)
        {
            chunk = chunk->prev;
            offset = (chunk->chunk_size) - 1;
        }
        else
        {
            offset--;
        }
        return *this;
    }
    _deque_iterator operator--(int)
    {
        _deque_iterator res = *this;
        --*this;
        return res;
    }
    _deque_iterator &operator+=(size_t n)
    {
        if (n < 0)
        {
            return *this -= -n;
        }
        while (n--)
        {
            ++*this;
        }
        return *this;
    }
    _deque_iterator &operator-=(size_t n)
    {
        if (n < 0)
        {
            return *this += -n;
        }
        while (n--)
        {
            --*this;
        }
        return *this;
    }
    ValueT &operator[](size_t n)
    {
        _deque_iterator tmp = *this;
        tmp += n;
        return *tmp;
    }
};

template <typename ValueT> class deque
{
  public:
    using iterator = _deque_iterator<ValueT>;
    using chunk = _deque_chunk<ValueT>;

  private:
    const size_t chunk_size = 100;
    size_t m_size;
    size_t map_sz;
    chunk **map;
    size_t map_beg, map_end;

  public:
    constexpr deque() noexcept
    {
        map_sz = 5;
        map_beg = 3;
        map_end = 4;
        map = new chunk *[map_sz];
        std::fill(map, map + map_sz, nullptr);
        map[3] = new chunk(50);
        m_size = 0;
    }
    deque(const deque &rhs)
    {
        map_sz = rhs.map_sz;
        map_beg = rhs.map_end;
        map_end = rhs.map_end;
        map = new chunk *[map_sz];
        for (size_t i = map_beg; i != map_end; i++)
        {
            map[i] = new chunk(*(rhs.map[i]));
            if (map[i - 1] != nullptr)
            {
                map[i]->prev = map[i - 1];
                map[i - 1]->next = map[i];
            }
        }
        m_size = rhs.m_size;
    }
    template <typename IT_T> deque(const IT_T &begin, const IT_T &end)
    {
        for (IT_T it = begin; it != end; it++)
        {
            push_back(*it);
        }
    }
    ~deque()
    {
        for (size_t i = map_beg; i != map_end; i++)
        {
            delete map[i];
        }
        delete map;
    }

  private:
    void resize_map()
    {
        chunk **tmp = new chunk *[map_sz * 3];
        std::fill(tmp, tmp + map_sz * 3, nullptr);
        for (size_t i = map_beg; i != map_end; i++)
        {
            tmp[map_sz + i] = map[i];
        }
        map_beg += map_sz;
        map_end += map_sz;
        map_sz *= 3;
        delete map;
        map = tmp;
    }
    void push_front_map()
    {
        if (map_beg == 0)
        {
            resize_map();
        }
        map[--map_beg] = new chunk(chunk_size);
        if (map[map_beg + 1] != nullptr)
        {
            map[map_beg]->next = map[map_beg + 1];
            map[map_beg + 1]->prev = map[map_beg];
        }
    }
    void pop_front_map()
    {
        if (map_beg <= map_end)
        {
            return;
        }
        delete map[map_beg];
        map[map_beg] = nullptr;
        map_beg++;
    }
    void push_back_map()
    {
        if (map_end == map_sz)
        {
            resize_map();
        }
        if (map[map_end - 2] != nullptr)
        {
            map[map_end++] = new chunk(0);
            map[map_end - 1]->prev = map[map_end - 2];
            map[map_end - 2]->next = map[map_end - 1];
        }
    }
    void pop_back_map()
    {
        if (map_beg <= map_end)
        {
            return;
        }
        delete map[map_end - 1];
        map[map_end - 1] = nullptr;
        map_end--;
    }

  public:
    void push_back(const ValueT &data)
    {
        if (map[map_end - 1]->hit_end())
        {
            push_back_map();
        }
        map[map_end - 1]->push_back(data);
        m_size++;
    }
    void pop_back()
    {
        map[map_end - 1]->pop_back();
        if (map[map_end - 1]->back == 0)
        {
            pop_back_map();
        }
        m_size--;
    }
    void push_front(const ValueT &data)
    {
        if (map[map_beg]->hit_front())
        {
            push_front_map();
        }
        map[map_beg]->push_front(data);
        m_size++;
    }
    void pop_front()
    {
        map[map_beg]->pop_front();
        if (map[map_beg]->front == chunk_size)
        {
            pop_front_map();
        }
        m_size--;
    }
    template <typename... Args> void emplace_back(Args &&...args)
    {
        emplace_back(std::forward<Args>(args)...);
    }
    template <typename... Args> void emplace_front(Args &&...args)
    {
        emplace_front(std::forward<Args>(args)...);
    }

  private:
    template <typename... Args> void emplace_back(ValueT &&head, Args &&...args)
    {
        push_back(head);
        emplace_back(args...);
    }
    template <typename... Args> void emplace_front(ValueT &&head, Args &&...args)
    {
        push_front(head);
        emplace_back(args...);
    }
    void emplace_back()
    {
    }
    void emplace_front()
    {
    }

  public:
    iterator begin() noexcept
    {
        iterator ret(map[map_beg]->front, map[map_beg]);
        return ret;
    }
    iterator end() noexcept
    {
        iterator ret(map[map_end - 1]->back, map[map_end - 1]);
        return ret;
    }

  public:
    void clear()
    {
        for (size_t i = map_beg; i != map_end; i++)
        {
            delete map[i];
        }
        delete map;

        map_sz = 5;
        map_beg = 3;
        map_end = 4;
        map = new chunk *[map_sz];
        std::fill(map, map + map_sz, nullptr);
        map[3] = new chunk(50);
        m_size = 0;
    }
    bool empty()
    {
        return m_size == 0;
    }
    ValueT front()
    {
        return *begin();
    }
    ValueT back()
    {
        iterator e = end();
        e--;
        return *e;
    }
    size_t size()
    {
        return m_size;
    }

  public:
    ValueT &at(size_t n)
    {
        if (n > m_size)
        {
            throw std::out_of_range("");
        }
        if (n < map[map_beg]->back - map[map_beg]->front)
        {
            return map[map_beg]->at(n+map[map_beg]->front);
        }
        n -= map[map_beg]->back - map[map_beg]->front;
        size_t chunk_num = n / chunk_size;
        n -= chunk_num * chunk_size;
        return map[chunk_num + 1]->at(n);
    }
    ValueT &operator[](size_t n)
    {
        return at(n);
    }
};

template <typename ValueT, typename Allocator = deque<ValueT>> class queue
{
  public:
    using container = Allocator;

  private:
    container m_cont;

  public:
    queue()
    {
    }
    queue(const queue &rhs)
    {
        m_cont = rhs.m_cont;
    }
    queue &operator=(const queue &rhs)
    {
        m_cont = rhs.m_cont;
    }
    ~queue()
    {
    }

  public:
    ValueT front()
    {
        return m_cont.front();
    }
    ValueT back()
    {
        return m_cont.back();
    }
    void push(const ValueT &data)
    {

        return m_cont.push_back(data);
    }
    void push(ValueT &&data)
    {
        return m_cont.push_back(data);
    }
    void pop()
    {
        return m_cont.pop_front();
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

} // namespace my_lib
