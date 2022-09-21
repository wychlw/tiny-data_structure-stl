#include <algorithm>
#include <functional>

#ifndef MY_LINETREE_HPP
#define MY_LINETREE_HPP

namespace my_lib
{

/**
 * line tree is way too... too flexible
 */
template <class T, size_t N> class LineTree
{

  private:
    struct LineTree_node
    {
        size_t l, r;
        T val;

        LineTree_node()
        {
        }
    } tree[N * 4];

    T (*push_up_func)(const T &a,const T &b);

    public:
    LineTree()
    {
        this->push_up_func=std::max;
    }
    LineTree(T (*push_up_func)(const T &a,const T &b))
    {
        this->push_up_func=push_up_func;
    }

    private:
    void push_up(size_t u)
    {
        tree[u].val = push_up_func(tree[u*2].val,tree[u*2+1].val);
    }


    T query(size_t u,size_t l,size_t r)
    {
        if (tree[u].l>=l && tree[u].r<=r)
        {
            return tree[u].val;
        }

        size_t mid=(l+r)/2;
        
        return query(u*2,l,mid);
    }

    void build(size_t u, size_t l, size_t r)
    {
        tree[u] = {.l = l, .r = r};
        if (l == r)
        {
            return;
        }
        size_t mid = (l + r) / 2;

        build(u * 2, l, mid);
        build(u * 2 + 1, mid + 1, r);

        push_up(u);
    }
  public:
    

    T query(size_t l,size_t r)
    {
        return query(1,l,r);
    }

    void build(size_t l, size_t r)
    {
        build(1,l,r);
    }

};

} // namespace my_lib

#endif