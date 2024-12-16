#pragma once

#include <algorithm>
#include <memory>

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
    NodeBase* m_endNode;

    std::size_t m_size;
};


inline bool IsLeftChild(const NodeBase* pNode) noexcept
{
    return pNode->m_pParent->m_pLeft == pNode;
}

inline bool IsRightChild(const NodeBase* pNode) noexcept
{
    return pNode->m_pParent->m_pRight == pNode;
}

inline void LeftRotate(TreeHeader& header, NodeBase* pRotationNode) noexcept
{
    NodeBase* pSubtree = pRotationNode->m_pRight;
    pRotationNode->m_pRight = pSubtree->m_pLeft; // turn pSubstree's left subtree into pRotationNode's right subtree

    if (pSubtree->m_pLeft)
        pSubtree->m_pLeft->m_pParent = pRotationNode->m_pRight; // if left subtree of pSubtree is not empty, set a parent

    pSubtree->m_pParent = pRotationNode->m_pParent;

    if (pRotationNode->m_pParent->m_pParent == pRotationNode)
        header.m_endNode->m_pParent = pSubtree; // make pSubtree root node if pRotationNode was the root
    else if (IsLeftChild(pRotationNode))
        pRotationNode->m_pParent->m_pLeft = pSubtree;
    else
        pRotationNode->m_pParent->m_pRight = pSubtree;

    pSubtree->m_pLeft = pRotationNode;
    pRotationNode->m_pParent = pSubtree;
}

inline void RightRotate(TreeHeader& header, NodeBase* pRotationNode) noexcept
{
    NodeBase* pSubtree = pRotationNode->m_pLeft;
    pRotationNode->m_pLeft = pSubtree->m_pRight;

    if (pSubtree->m_pRight)
        pSubtree->m_pRight->m_pParent = pRotationNode; // if right subtree of pSubtree is not empty, set parent

    pSubtree->m_pParent = pRotationNode->m_pParent;

    if (pRotationNode->m_pParent->m_pParent == pRotationNode)
        header.m_endNode->m_pParent = pSubtree; // make pSubtree root node if pRotationNode was the root
    else if (IsLeftChild(pRotationNode))
        pRotationNode->m_pParent->m_pLeft = pSubtree;
    else
        pRotationNode->m_pParent->m_pRight = pSubtree;

    pSubtree->m_pRight = pRotationNode;
    pRotationNode->m_pParent = pSubtree;
}

inline void RebalanceAfterInsert(TreeHeader& header, NodeBase* pInsertedNode) noexcept
{
    NodeBase* pCurrNode = pInsertedNode;

    while (pCurrNode != header.m_endNode->m_pParent || pCurrNode->m_color == Color::Red)
    {
        if (IsLeftChild(pCurrNode->m_pParent))
        {
            std::cout << "Parent of curr node is left child" << std::endl;

            NodeBase* pUncleNode = pCurrNode->m_pParent->m_pParent->m_pRight;

            if (pUncleNode && pUncleNode->m_color == Color::Red)
            {
                std::cout << "Uncle of curr node is red" << std::endl;

                pCurrNode->m_pParent->m_color = Color::Black;
                pUncleNode->m_color = Color::Black;
                pCurrNode->m_pParent->m_pParent->m_color = Color::Red;
                pCurrNode = pCurrNode->m_pParent->m_pParent;
            }
            else
            {
                std::cout << "Uncle of curr node is black" << std::endl;

                if (IsRightChild(pCurrNode))
                {
                    pCurrNode = pCurrNode->m_pParent;
                    LeftRotate(header, pCurrNode);
                }

                pCurrNode->m_pParent->m_color = Color::Black;
                pCurrNode->m_pParent->m_pParent->m_color = Color::Red;
                RightRotate(header, pCurrNode->m_pParent->m_pParent);
            }
        }
        else
        {
            std::cout << "CurrNode is right child" << std::endl;

            NodeBase* pUncleNode = pCurrNode->m_pParent->m_pParent->m_pLeft;

            if (pUncleNode && pUncleNode->m_color == Color::Red)
            {

                pCurrNode->m_color = Color::Black;
                pUncleNode->m_color = Color::Black;
                pCurrNode->m_pParent->m_pParent->m_color = Color::Red;
                pCurrNode = pCurrNode->m_pParent->m_pParent;
            }
            else
            {
                std::cout << "CurrNode is black" << std::endl;
                if (IsLeftChild(pCurrNode))
                {
                    std::cout << "CurrNode is left child" << std::endl;
                    pCurrNode = pCurrNode->m_pParent;
                    RightRotate(header, pCurrNode);
                    std::cout << "Rotated tree to the right" << std::endl;
                }

                pCurrNode->m_pParent->m_color = Color::Black;
                pCurrNode->m_pParent->m_pParent->m_color = Color::Red;
                LeftRotate(header, pCurrNode->m_pParent->m_pParent);
            }
        }
    }

    header.m_endNode->m_pParent->m_color = Color::Black;
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
        m_endNode = new base_type{};
        m_endNode->m_color = internal::Color::Red;
        m_endNode->m_pParent = nullptr;
        m_endNode->m_pLeft = nullptr;
        m_endNode->m_pRight = nullptr;
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
    node_pointer Insert(const key_value_type& val) noexcept
    {
        return InsertInternal(val);
    }

    // InsertOrUpdate adds new value to the tree or update 
    // already existing one.
    node_pointer InsertOrUpdate(const key_value_type& val) noexcept
    {
        return InsertInternal(val, true);
    }

    // Find returns a node, which holds a `key`.
    node_pointer Find(const key_type& key) const noexcept
    {
        node_pointer pCurrNode = Root();

        while (pCurrNode)
        {
            key_type currKey = internal::Key(pCurrNode);

            if (m_compare(key, currKey))
                pCurrNode = Left(pCurrNode);
            else if (m_compare(currKey, key))
                pCurrNode = Right(pCurrNode);
            else
                return pCurrNode;
        }

        return nullptr;
    }

    // Contains retuns true if value with `key` is presented 
    // in the tree.
    bool Contains(const key_type& key) const noexcept
    {
        return Find(key) == nullptr;
    }

    // Delete removes element with key node and rebalace the
    // tree if needed.
    void Delete(const key_type& key) noexcept
    {

    }

    bool operator==(const RbTree& other) const noexcept
    {
        if (this == std::addressof(other))
            return true;

        return TreesAreEqual(Root(), other.Root());
    }

    bool operator!=(const RbTree& other) const noexcept
    {
        return !(*this == other);
    }

private:
    node_pointer Root() const
    {
        return static_cast<node_pointer>(m_endNode->m_pParent);
    }

    node_pointer InsertInternal(const key_value_type& val, bool updateIfExists = false) noexcept
    {
        node_pointer pCurrNode = Root();
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
                if (!m_compare(keyCurrNode, keyToInsert))
                {
                    if (updateIfExists)
                        pCurrNode->m_value = val;

                    return pCurrNode;
                }

                pCurrNode = Right(pCurrNode);
            }
        }

        node_pointer pNewNode = AllocateNode(val, pParentNode);

        if (pParentNode == nullptr)
        {
            // tree is empty, we need to create a root node
            pNewNode->m_color = internal::Color::Black; // root node should be black
            m_endNode->m_pParent = pNewNode;
            m_endNode->m_pParent->m_pParent = m_endNode;
            m_endNode->m_pLeft = pNewNode; // root node is now the most left
            m_endNode->m_pRight = pNewNode; // and the most right node
            ++m_size;
            return pNewNode;
        }
        else
        {
            if (m_compare(keyToInsert, internal::Key(pParentNode->m_value)))
                pParentNode->m_pLeft = pNewNode;
            else
                pParentNode->m_pRight = pNewNode;
        }

        internal::RebalanceAfterInsert(*this, pNewNode);

        key_type rightMostKey = internal::Key(Right(m_endNode)->m_value);
        key_type leftMostKey = internal::Key(Left(m_endNode)->m_value);

        // change rightmost / leftmost if needed
        if (m_compare(rightMostKey, keyToInsert))
            m_endNode->m_pRight = pNewNode;
        if (m_compare(keyToInsert, leftMostKey))
            m_endNode->m_pLeft = pNewNode;

        ++m_size;

        return pNewNode;
    }

private:
    static node_pointer AllocateNode(const key_value_type& val, base_pointer pParent)
    {
        node_pointer pNode = new node_type{};
        pNode->m_color = internal::Color::Red;
        pNode->m_pParent = pParent;
        pNode->m_value = val;
        return pNode;
    }

    static node_pointer DeallocateNode(node_pointer pNode)
    {
        delete pNode;
    }

    static node_pointer Left(base_pointer pNode) noexcept
    {
        return static_cast<node_pointer>(pNode->m_pLeft);
    }

    static node_pointer Right(base_pointer pNode) noexcept
    {
        return static_cast<node_pointer>(pNode->m_pRight);
    }

    static bool TreesAreEqual(const node_pointer lhs, const node_pointer rhs) noexcept
    {
        if (!lhs && !rhs) return true;
        if ((lhs && !rhs) || (!lhs && rhs)) return false;
        if (lhs->m_value != rhs->m_value || lhs->m_color != rhs->m_color) return false;
        return NodesAreEqual(lhs->m_pLeft, rhs->m_pLeft) && NodesAreEqual(lhs->m_pRight, rhs->m_pRight);
    }

private:
    compare m_compare; // compare function / functor
};

} // namespace ads
