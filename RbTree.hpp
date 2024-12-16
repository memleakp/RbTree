#pragma once

#include <algorithm>

namespace ads
{

namespace internal
{

enum class Color { Red = false, Black = true };

struct NodeBase
{
    Color     m_color; // we need color to make a process of rebalancing easier
    NodeBase* m_pParent;
    NodeBase* m_pLeft;
    NodeBase* m_pRight;
};

template<typename T>
struct Node : public NodeBase
{
    T m_value;
};

// TreeHeader contains information about the most left node,
// the most right node, root node and end node. Also it contains
// current size of container.
struct TreeHeader
{
    // `m_endNode` is a special node, wich holds pointers to the
    // most left node as its left child, most right node as its
    // right child and a pointer to the root as its parent. Also
    // `m_endNode` is a parent of a root node of tree.
    NodeBase m_endNode;

    std::size_t m_size;
};

inline void RotateLeft() noexcept
{

}

inline void RotateRight() noexcept
{

}

inline void RebalanceAfterInsert() noexcept
{

}

inline void RebalanceAfterRemove() noexcept
{

}

// `KeyValueType` helps to get a `key_type` and a `value_type`
// from some generic type `T`.
template<typename T>
struct KeyValueType
{
    using key_type = T;
    using value_type = T;
};

template<typename T1, typename T2>
struct KeyValueType<std::pair<T1, T2>>
{
    using key_type = T1;
    using value_type = T2;
};

template<typename T1, typename T2>
struct KeyValueType<const std::pair<T1, T2>>
{
    using key_type = T1;
    using value_type = T2;
};

template<typename T>
inline T Key(const T& val) noexcept
{
    return val;
}

template<typename T1, typename T2>
inline T1 Key(const std::pair<T1, T2>& val) noexcept
{
    return val.first;
}

template<typename T>
inline T Value(const T& val) noexcept
{
    return val;
}

template<typename T1, typename T2>
inline T1 Value(const std::pair<T1, T2>& val) noexcept
{
    return val.second;
}

}; // namespace internal

template<typename V, typename Cmp = std::less<V>>
class RbTree : private internal::TreeHeader
{
public:
    using key_value_type = V;
    using key_type = typename internal::KeyValueType<V>::key_type;
    using value_type = typename internal::KeyValueType<V>::value_type;
    using compare = Cmp;

    using size_type = std::size_t;

    using node_type = internal::Node<key_value_type>;
    using node_pointer = internal::Node<key_value_type>*;
    using base_type = internal::NodeBase;
    using base_pointer = internal::NodeBase*;

public:
    // Default constructor.
    RbTree()
    {
        m_endNode = base_type{};
        m_endNode.m_color = internal::Color::Red;
        m_endNode.m_pParent = nullptr;
        m_endNode.m_pLeft = nullptr;
        m_endNode.m_pRight = nullptr;
        m_compare = compare{};
    }

    // Copy constructor.
    RbTree(const RbTree& other) { }
    // Move constructor.
    RbTree(RbTree&& other) { }

    // Copy assignment operator.
    RbTree& operator=(const RbTree& other) { }
    // Move assignment operator.
    RbTree& operator=(RbTree&& other) { }

    // Destructor removes all nodes of a tree.
    ~RbTree() { }

public:
    // Size returns current number of elements in container.
    size_type Size() const noexcept { return m_size; }

    // Empty returns true if size of container is 0, false overweise.
    bool Empty() const noexcept { return m_size == 0; }

public:
    // Insert adds a new value to the container only if it is 
    // not presented in the tree.
    void Insert(const key_value_type& val) noexcept
    {
        node_pointer pCurrNode = static_cast<node_pointer>(m_endNode.m_pParent); // first node is root node
        node_pointer pParentNode = nullptr; // this node will be a parent of a new node

        key_type keyToInsert = internal::Key(val);

        while (pCurrNode != nullptr)
        {
            pParentNode = pCurrNode;
            key_type keyCurrNode = internal::Key(pCurrNode->m_value);

            if (m_compare(keyToInsert, keyCurrNode))
            {
                pCurrNode = Left(pCurrNode);
            }
            else
            {
                if (!m_compare(keyCurrNode, keyToInsert)) return; // do nothing if key is already presented

                pCurrNode = Right(pCurrNode);
            }
        }

        node_pointer pNewNode = AllocateNode(val, pParentNode);

        if (pParentNode == nullptr)
        {
            // tree is empty, we need to create a root node
            m_endNode.m_pParent = pNewNode;
            m_endNode.m_pLeft = pNewNode; // root node is now the most left
            m_endNode.m_pRight = pNewNode; // and the most right node
        }
        else
        {
            if (m_compare(keyToInsert, internal::Key(pParentNode->m_value)))
                pParentNode->m_pLeft = pNewNode;
            else
                pParentNode->m_pRight = pNewNode;
        }

        internal::RebalanceAfterInsert();

        ++m_size;
    }

    // InsertOrUpdate adds new value to the tree or update 
    // already existing one.
    void InsertOrUpdate(const key_value_type& val) noexcept
    {

    }

    // Find returns a value of `value_type` stored under the `key`.
    value_type Find(const key_type& key) const noexcept
    {

    }

    // Contains retuns true if value with `key` is presented 
    // in the tree.
    bool Contains(const key_type& key) const noexcept
    {

    }

    // Delete removes element with key node and rebalace the
    // tree if needed.
    void Delete(const key_type& key) noexcept
    {

    }

private: // some utils functuions
    static node_pointer AllocateNode(const key_value_type& val, base_pointer pParent)
    {
        node_pointer pNode = new node_type{};
        pNode->m_color = internal::Color::Red;
        pNode->m_pParent = pParent;
        pNode->m_value = val;
        return pNode;
    }

    static node_pointer Left(base_pointer pNode) noexcept
    {
        return static_cast<node_pointer>(pNode->m_pLeft);
    }

    static node_pointer Right(base_pointer pNode) noexcept
    {
        return static_cast<node_pointer>(pNode->m_pRight);
    }

    static bool IsLeftChild(base_pointer pNode) noexcept
    {
        return pNode->m_pParent->m_pLeft == pNode;
    }

    static bool IsRightChild(base_pointer pNode) noexcept
    {
        return pNode->m_pParent->m_pRight == pNode;
    }

private:
    compare m_compare; // compare function
};

} // namespace ads
