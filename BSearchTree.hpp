#include <cstddef>
#include <iterator>

#ifndef MY_BSEARCHTREE_HPP
#define MY_BSEARCHTREE_HPP

namespace my_lib
{

template <typename T> class BSearchTree
{
  private:
    struct BSearchTree_node
    {
        T data;
        size_t lcnt, rcnt, cnt;
        BSearchTree_node *l, *r, *p;

        BSearchTree_node()
        {
            this->less = 0;
            this->more = 0;
            this->l = nullptr;
            this->r = nullptr;
            this->p = nullptr;
            this->lcnt = 0;
            this->rcnt = 0;
            this->cnt = 1;
        }
        BSearchTree_node(const T &data)
        {
            this->data = data;
            this->less = 0;
            this->more = 0;
            this->l = nullptr;
            this->r = nullptr;
            this->p = nullptr;
            this->lcnt = 0;
            this->rcnt = 0;
            this->cnt = 1;
        }
        BSearchTree_node(T &&data)
        {
            this->data = data;
            this->less = 0;
            this->more = 0;
            this->l = nullptr;
            this->r = nullptr;
            this->p = nullptr;
            this->lcnt = 0;
            this->rcnt = 0;
            this->cnt = 1;
        }
        BSearchTree_node(const T &data, const BSearchTree_node *parent)
        {
            this->data = data;
            this->less = 0;
            this->more = 0;
            this->l = nullptr;
            this->r = nullptr;
            this->p = parent;
            this->lcnt = 0;
            this->rcnt = 0;
            this->cnt = 1;
        }
        BSearchTree_node(T &&data, const BSearchTree_node *parent)
        {
            this->data = data;
            this->less = 0;
            this->more = 0;
            this->l = nullptr;
            this->r = nullptr;
            this->p = parent;
            this->lcnt = 0;
            this->rcnt = 0;
            this->cnt = 1;
        }

        void destruct_tree()
        {
            if (this->l != nullptr)
            {
                this->l->destruct_tree();
                delete this->l;
            }
            if (this->r != nullptr)
            {
                this->r->destruct_tree();
                delete this->r;
            }
        }
    };

  private:
    BSearchTree_node *root;
    size_t size;
    size_t node_cnt;

  public:
    BSearchTree()
    {
        root = nullptr;
        size = 0;
    }
    ~BSearchTree()
    {
        if (root != nullptr)
        {
            root->destruct_tree();
            delete root;
        }
    }

  protected:
    /**
     * @brief replace two node's cnt and data
     * You shoud NOTICE that: this operation WILL NOT UPDATE the cnt change in the route!!!
     * USE THIS OPERATION CAREFULLY!
     *
     * @param a first node
     * @param b second node
     */
    void swap_node(BSearchTree_node *a, BSearchTree_node *b)
    {
        std::swap(a->data,b->data);
        std::swap(a->cnt,b->cnt);
    }

  private:
    BSearchTree_node *ghost_replacement_delete(BSearchTree_node *node)
    {

        BSearchTree_node *ghost = node->l;
        while (ghost->r != nullptr)
        {
            ghost = ghost->r;
        }

        BSearchTree_node *now = node->l;
        now->cnt -= ghost->cnt;
        while (now != ghost)
        {
            now->rcnt -= ghost->cnt;
            now = now->r;
        }

        now->data = ghost->data;
        now->cnt = ghost->cnt;

        return ghost;
    }

    void erase_node(BSearchTree_node *node)
    {
        node_cnt--;
        BSearchTree_node *now = root;
        while (now != nullptr)
        {
            if (now == node)
            {
                if (node->l == nullptr && node->r == nullptr)
                {
                    if (node == root)
                    {
                        root = nullptr;
                    }
                    else if (node == node->p->l)
                    {
                        node->p->l = nullptr;
                    }
                    else
                    {
                        node->p->r = nullptr;
                    }
                }
                else if (node->r == nullptr)
                {
                    if (node == root)
                    {
                        root = node->l;
                    }
                    else if (node == node->p->l)
                    {
                        node->p->l = node->l;
                        node->l->p = node->p;
                    }
                    else
                    {
                        node->p->r = node->l;
                        node->l->p = node->p;
                    }
                }
                else if (node->l == nullptr)
                {
                    if (node == root)
                    {
                        root = node->r;
                    }
                    else if (node == node->p->l)
                    {
                        node->p->l = node->r;
                        node->r->p = node->p;
                    }
                    else
                    {
                        node->p->r = node->r;
                        node->r->p = node->p;
                    }
                }
                else
                {
                    node = this->ghost_replacement(node);
                }

                delete node;
                return;
            }
            else if (now->data > node->data)
            {
                now->lcnt -= node->cnt;
                now = now->l;
            }
            else
            {
                now->rcnt -= node->cnt;
                now = now->r;
            }
        }

        delete node;
    }

  public:
    void insert(T data)
    {
        size++;
        if (root == nullptr)
        {
            root = new BSearchTree_node(data);
        }

        BSearchTree_node *now = root, *parent;

        while (now != nullptr)
        {
            if (now->data == data)
            {
                now->cnt++;
                return;
            }
            if (now->data > data)
            {
                now->lcnt++;
                parent = now;
                now = now->l;
            }
            else
            {
                now->rcnt++;
                parent = now;
                now = now->r;
            }
        }

        if (parent->data > data)
        {
            parent->l = new BSearchTree_node(data, parent);
        }
        else
        {
            parent->r = new BSearchTree_node(data, parent);
        }

        node_cnt++;

        return;
    }

    void erase(T key)
    {
        if (root == nullptr)
        {
            return;
        }

        BSearchTree_node *now = root;

        while (now != nullptr)
        {
            if (now->data == key)
            {
                erase_node(now);
                return;
            }
            if (now->data > key)
            {
                now = now->l;
            }
            else
            {
                now = now->r;
            }
        }
    }

    void remove(T key)
    {
        if (root == nullptr)
        {
            return;
        }

        BSearchTree_node *now = root;

        while (now != nullptr)
        {
            if (now->data == key)
            {
                if (now->cnt == 1)
                {
                    erase_node(now);
                }
                else
                {
                    now->cnt--;
                }
                return;
            }
            if (now->data > key)
            {
                now->lcnt--;
                now = now->l;
            }
            else
            {
                now->rcnt--;
                now = now->r;
            }
        }
    }

    BSearchTree_node *search(T key)
    {
        if (root == nullptr)
        {
            return nullptr;
        }

        BSearchTree_node *now = root;
        while (now != nullptr)
        {
            if (now->data == key)
            {
                return now;
            }
            if (now->data > key)
            {
                now = now->l;
            }
            else
            {
                now = now->r;
            }
        }

        return nullptr;
    }

    size_t count(T key)
    {
        BSearchTree_node *target = this->search(key);

        if (target == nullptr)
        {
            return 0;
        }
        return target->cnt;
    }

    size_t left_count(T key)
    {
        BSearchTree_node *target = this->search(key);

        if (target == nullptr)
        {
            return 0;
        }
        return target->lcnt;
    }

    size_t right_count(T key)
    {
        BSearchTree_node *target = this->search(key);

        if (target == nullptr)
        {
            return 0;
        }
        return target->lcnt;
    }

    size_t count_less(T key)
    {
        if (root == nullptr)
        {
            return 0;
        }

        BSearchTree_node *now = root;
        size_t less = 0;

        while (now != nullptr)
        {
            if (now->data == key)
            {
                return less + now->lcnt;
            }
            if (now->data > key)
            {
                now = now->l;
            }
            else
            {
                less += now->lcnt + now->cnt;
                now = now->r;
            }
        }

        return less;
    }

    size_t count_more(T key)
    {
        if (root == nullptr)
        {
            return 0;
        }

        BSearchTree_node *now = root;
        size_t more = 0;

        while (now != nullptr)
        {
            if (now->data == key)
            {
                return more + now->rcnt;
            }
            if (now->data > key)
            {
                more += now->rcnt + now->cnt;
                now = now->l;
            }
            else
            {
                now = now->r;
            }
        }

        return more;
    }
};
} // namespace my_lib

#endif