#include <cstddef>
#include <exception>
#include <iostream>

#ifndef MY_RBTREE_HPP
#define MY_RBTREE_HPP

namespace my_lib
{

template <typename K, typename V, typename _Cmp = std::less<K>> class RBTree
{
  private:
    static const bool black = false, red = true;
    _Cmp Cmp = _Cmp();
    struct RBTree_node
    {
        K key;
        V data;
        bool color;
        RBTree_node *l, *r, *p;

        RBTree_node() : l(nullptr), r(nullptr), p(nullptr), color(red)
        {
        }

        RBTree_node(const K &key, const V &data) : l(nullptr), r(nullptr), p(nullptr), color(red), key(key), data(data)
        {
        }

        RBTree_node(const K &key, const V &data, RBTree_node *p)
            : l(nullptr), r(nullptr), p(p), color(red), key(key), data(data)
        {
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

        bool is_red()
        {
            return color == red;
        }

        bool is_black()
        {
            return color == black;
        }

        bool has_uncle()
        {
            if (p == nullptr || p->p == nullptr || p->p->l == nullptr || p->p->r == nullptr)
            {
                return false;
            }

            return true;
        }

        RBTree_node *get_uncle()
        {
            if (!has_uncle())
            {
                return nullptr;
            }

            if (p == p->p->l)
            {
                return p->p->r;
            }
            else
            {
                return p->p->l;
            }
        }

        bool has_brother()
        {
            if (p == nullptr || p->l == nullptr || p->r == nullptr)
            {
                return false;
            }
            return true;
        }

        RBTree_node *get_brother()
        {
            if (!has_brother())
            {
                return nullptr;
            }

            if (this == p->l)
            {
                return p->r;
            }
            else
            {
                return p->l;
            }
        }

        bool has_grandparent()
        {
            return p != nullptr && p->p != nullptr;
        }

        RBTree_node *get_grandparent()
        {
            if (!has_grandparent())
            {
                return nullptr;
            }

            return p->p;
        }

        V &get_val()
        {
            return data;
        }

        K &get_key()
        {
            return key;
        }

        void set_val(const V &val)
        {
            data = val;
        }

        void set_key(const K &key)
        {
            key = key;
        }

        void ptr()
        {
            std::cout << "[ ";
            if (l != nullptr)
            {
                l->ptr();
            }

            std::cout << " " << key << " ";
            if (r != nullptr)
            {
                r->ptr();
            }

            std::cout << " ] ";
        }
    };

  private:
    RBTree_node *root;
    size_t m_size;

  public:
    RBTree()
    {
        root = nullptr;
        m_size = 0;
    }
    ~RBTree()
    {
        if (root != nullptr)
        {
            root->destruct_tree();
            delete root;
        }
    }

  private:
    void maintain_color(RBTree_node *node)
    {
        if (node == nullptr || root == nullptr)
        {
            return;
        }

        if (root == node)
        {
            if (node->is_red())
            {
                node->color = black;
            }
            return;
        }

        if (node->p->is_black())
        {
            return;
        }

        if (node->p == root)
        {
            if (node->p->is_red())
            {
                node->p->color = black;
            }
            return;
        }

        if (node->has_uncle() && node->get_uncle()->is_red())
        {
            node->p->color = black;
            node->get_uncle()->color = black;
            node->get_grandparent()->color = red;
            maintain_color(node->get_grandparent());
            return;
        }

        if (!node->has_uncle() || node->get_uncle()->is_black())
        {
            if ((node == node->p->l && node->p == node->get_grandparent()->r) ||
                (node == node->p->r && node->p == node->get_grandparent()->l))
            {
                RBTree_node *tmp = node->p;

                if (node == node->p->l)
                {
                    RR(node->get_grandparent());
                }
                else
                {
                    LR(node->get_grandparent());
                }

                node = tmp;
            }

            if (node->p == node->get_grandparent()->l)
            {
                RR(node->get_grandparent());
            }
            else
            {
                LR(node->get_grandparent());
            }

            node->p->color = black;
            if (node->has_brother())
            {
                node->get_brother()->color = red;
            }
            return;
        }
    }

    void maintain_remove(RBTree_node *node)
    {
        if (node == root)
        {
            return;
        }

        if (!(node->is_black() && node->has_brother()))
        {
            throw std::logic_error("Shouldn't happen this situation. Just in case.");
        }

        RBTree_node *bro = node->get_brother();
        bool dir = (node->p->l == node);

        if (bro->is_red())
        {

            RBTree_node *par = node->p;

            if (par == nullptr || par->is_red() || bro->l == nullptr || bro->is_red() || bro->r == nullptr ||
                bro->is_red())
            {
                throw std::logic_error("Shouldn't happen this situation too. Just in case.");
            }

            if (dir)
            {
                LR(node->p);
            }
            else
            {
                RR(node->p);
            }

            bro->color = black;
            node->p->color = red;

            bro = node->get_brother();
        }

        RBTree_node *close_bro_son, *far_bro_son;

        if (dir)
        {
            close_bro_son = bro->l;
            far_bro_son = bro->r;
        }
        else
        {
            close_bro_son = bro->r;
            far_bro_son = bro->l;
        }

        if (bro->is_red())
        {
            throw std::logic_error("Shouldn't happen this situation three. Just in case.");
        }

        if ((close_bro_son == nullptr || close_bro_son->is_black()) &&
            (far_bro_son == nullptr || far_bro_son->is_black()))
        {
            if (node->p->is_red())
            {
                bro->color = red;
                node->p->color = black;
                return;
            }
            else
            {

                bro->color = red;
                maintain_remove(node->p);
                return;
            }
        }
        else
        {
            if (close_bro_son != nullptr && close_bro_son->is_red())
            {
                if (dir)
                {
                    RR(bro);
                }
                else
                {
                    LR(bro);
                }

                close_bro_son->color = black;
                bro->color = red;

                bro = node->get_brother();

                if (dir)
                {
                    close_bro_son = bro->l;
                    far_bro_son = bro->r;
                }
                else
                {
                    close_bro_son = bro->r;
                    far_bro_son = bro->l;
                }
            }

            if ((close_bro_son != nullptr && close_bro_son->is_red()) || (far_bro_son->is_black()))
            {
                throw std::logic_error("Fine...");
            }

            if (dir)
            {
                LR(node->p);
            }
            else
            {
                RR(node->p);
            }

            bro->color = node->p->color;
            node->p->color = black;

            if (far_bro_son != nullptr)
            {
                far_bro_son->color = black;
            }

            return;
        }
    }

    void maintian_leaf(RBTree_node *node)
    {
        if (node == nullptr)
        {
            return;
        }

        if (node->l != nullptr)
        {
            node->l->p = node;
        }
        if (node->r != nullptr)
        {
            node->r->p = node;
        }
    }

    /**
     * @brief Left rotate
     *   A              C
     *  / \            / \
     * B   C          A   E
     *    / \        / \
     *   D   E      B   D
     */
    void LR(RBTree_node *node)
    {
        if (node == nullptr || node->r == nullptr)
        {
            return;
        }

        RBTree_node *p = node->p, *r = node->r;

        node->r = r->l;
        r->l = node;

        maintian_leaf(node);
        maintian_leaf(r);

        if (root == node)
        {
            root = r;
        }
        else if (p->l == node)
        {
            p->l = r;
        }
        else if (p->r == node)
        {
            p->r = r;
        }

        r->p = p;
    }

    /**
     * @brief Right rotate
     * The reverse version of Left Rotate
     *
     */
    void RR(RBTree_node *node)
    {
        if (node == nullptr || node->l == nullptr)
        {
            return;
        }

        RBTree_node *p = node->p, *l = node->l;

        node->l = l->r;
        l->r = node;

        maintian_leaf(node);
        maintian_leaf(l);

        if (root == node)
        {
            root = l;
        }
        else if (p->l == node)
        {
            p->l = l;
        }
        else if (p->r == node)
        {
            p->r = l;
        }

        l->p = p;
    }

    void swap_node(RBTree_node *&a, RBTree_node *&b)
    {
        std::swap(a->key, b->key);
        std::swap(a->data, b->data);
        std::swap(a, b);
    }

  private:
    void insert(RBTree_node *node, const K &key, const V &val)
    {
        if (node == nullptr)
        {
            return;
        }

        if (!Cmp(key, node->key)&&!Cmp(node->key,key))
        {
            node->set_val(val);
        }

        if (Cmp(key, node->key))
        {
            if (node->l == nullptr)
            {
                RBTree_node *new_node = new RBTree_node(key, val, node);
                node->l = new_node;
                maintain_color(new_node);
                m_size++;
            }
            else
            {
                insert(node->l, key, val);
            }
        }
        else
        {
            if (node->r == nullptr)
            {
                RBTree_node *new_node = new RBTree_node(key, val, node);
                node->r = new_node;
                maintain_color(new_node);
                m_size++;
            }
            else
            {
                insert(node->r, key, val);
            }
        }
    }

    void remove(RBTree_node *node, const K &key)
    {
        if (node == nullptr)
        {
            return;
        }

        if (Cmp(key, node->key)||Cmp(node->key,key))
        {
            if (Cmp(key, node->key))
            {
                if (node->l != nullptr)
                {
                    remove(node->l, key);
                    maintian_leaf(node);
                }
            }
            else
            {
                if (node->r != nullptr)
                {
                    remove(node->r, key);
                    maintian_leaf(node);
                }
            }
            return;
        }

        if (m_size == 1)
        {
            clear();
            return;
        }

        RBTree_node *ori = node;

        if (node->l != nullptr && node->r != nullptr)
        {
            RBTree_node *rep = node->r;
            RBTree_node *par = node;
            while (rep->l != nullptr)
            {
                par = rep;
                rep = par->l;
            }

            swap_node(rep, node);
            maintian_leaf(par);
        }

        if (node->l == nullptr && node->r == nullptr)
        {
            if (node->p == nullptr)
            {
                return;
            }

            if (node->is_black())
            {
                maintain_remove(node);
            }

            if (node->p->l == node)
            {
                node->p->l = nullptr;
            }
            else
            {
                node->p->r = nullptr;
            }
        }
        else
        {
            if (node->l != nullptr && node->r != nullptr)
            {
                throw std::logic_error("WTF? Why does it have two son after replacement???");
            }

            RBTree_node *par = node->p;
            RBTree_node *rep;
            if (node->l != nullptr)
            {
                rep = node->l;
            }
            else
            {
                rep = node->r;
            }

            if (par == nullptr)
            {
                root = rep;
            }
            else if (par->l == node)
            {
                par->l = rep;
            }
            else
            {
                par->r = rep;
            }

            rep->p = par;

            if (node->is_black())
            {
                if (rep->is_red())
                {
                    rep->color = black;
                }
                else
                {
                    maintain_remove(rep);
                }
            }
        }

        m_size--;
        delete ori;
        return;
    }

  private:
    bool has_key(RBTree_node *node, const K &key)
    {
        if (node == nullptr)
        {
            return false;
        }
        if (!Cmp(key, node->key)&&!Cmp(node->key,key))
        {
            return true;
        }
        if (Cmp(key, node->key))
        {
            return has_key(node->l, key);
        }
        else
        {
            return has_key(node->r, key);
        }
    }

    K &get_key(RBTree_node *node,const K &key)
    {
        if (!Cmp(key, node->key)&&!Cmp(node->key,key))
        {
            return node->key;
        }
        if (Cmp(key, node->key))
        {
            return get_key(node->l, key);
        }
        else
        {
            return get_key(node->r, key);
        }
    }

    V &get(RBTree_node *node, const K &key)
    {
        if (!Cmp(key, node->key)&&!Cmp(node->key,key))
        {
            return node->data;
        }
        if (Cmp(key, node->key))
        {
            return get(node->l, key);
        }
        else
        {
            return get(node->r, key);
        }
    }

  public:
    void clear()
    {
        if (root != nullptr)
        {
            root->destruct_tree();
        }
        root = nullptr;
        m_size = 0;
    }

    size_t size()
    {
        return m_size;
    }

    bool is_empty()
    {
        return m_size == 0;
    }

    bool has_key(const K &key)
    {
        return has_key(root, key);
    }

    /**
     * @brief Return the value corresponding to the key.
     *        **IF NOT EXIST, THEN CREATE.**
     *
     * @param key
     * @return V&
     */
    V &get(const K &key)
    {
        if (!has_key(key))
        {
            insert(key, V());
        }

        return get(root, key);
    }

    K &get_key(const K &key)
    {
        if (!has_key(key))
        {
            insert(key, V());
        }

        return get_key(root, key);
    }

    /**
     * @brief Return the value corresponding to the key.
     *        **IF NOT EXIST, THEN CREATE.**
     *
     * @param key
     * @return V&
     */
    V &operator[](const K &key)
    {
        return get(key);
    }

    /**
     * @brief Insert a <key,value> pair.
     *        **IF THE KEY EXIST, THEN REPLACE!**
     *
     * @param key
     * @param val
     */
    void insert(const K &key, const V &val)
    {
        if (root == nullptr)
        {
            root = new RBTree_node(key, val);
            root->color = black;
            m_size++;
        }
        else
        {
            insert(root, key, val);
        }
    }

    void remove(const K &key)
    {
        if (!has_key(key))
        {
            return;
        }

        remove(root, key);
    }

    /**
     * @brief This func is for debug use only. Use it at your own risk!
     *
     * @param res
     */
    void prt_tree()
    {
        if (root != nullptr)
        {
            root->ptr();
            std::cout << std::endl;
        }
    }
};

} // namespace my_lib

#endif