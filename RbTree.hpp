#pragma once

#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

namespace ads {

namespace internal {

enum class Color { Red = false, Black = true };

struct NodeBase {
    Color m_color;  // we need color to make a process of rebalancing easier
    NodeBase* m_pParent;
    NodeBase* m_pLeft;
    NodeBase* m_pRight;
};

template <typename T>
struct Node : public NodeBase {
    T m_value;
};

// TreeHeader contains information about the most left node, the most right node, root node and end
// node. Also it contains current size of container.
struct TreeHeader {
    // `m_endNode` is a special node, which holds pointers to the most left node as its left child,
    // most right node as its right child and a pointer to the root as its parent. Also `m_endNode`
    // is a parent of a root node of tree.
    NodeBase* m_endNode;

    std::size_t m_size;
};

// `IsLeftChild` checks whether node is left child or not.
inline bool IsLeftChild(const NodeBase* pNode) noexcept {
    return pNode->m_pParent->m_pLeft == pNode;
}

// `IsRightChild` checks whether node is right child or not.
inline bool IsRightChild(const NodeBase* pNode) noexcept {
    return pNode->m_pParent->m_pRight == pNode;
}

// `TreeMax` returns the most right node of a tree. Precondition: `pNode` should not be equal
// `nullptr`
inline NodeBase* TreeMax(NodeBase* pNode) {
    while (pNode->m_pRight) {
        pNode = pNode->m_pRight;
    }
    return pNode;
}

// Precondition: `pNode` should not be equal `nullptr` `TreeMin` returns the most left node of a
// tree.
inline NodeBase* TreeMin(NodeBase* pNode) {
    while (pNode->m_pLeft) {
        pNode = pNode->m_pLeft;
    }
    return pNode;
}

// TODO: add description and visualizations.
inline void LeftRotate(TreeHeader& header, NodeBase* pRotationNode) noexcept {
    NodeBase* pSubtree = pRotationNode->m_pRight;
    // turn pSubtrees' left subtree into pRotationNode's right subtree
    pRotationNode->m_pRight = pSubtree->m_pLeft;

    if (pSubtree->m_pLeft) {
        // if left subtree of pSubtree is not empty, set a parent
        pSubtree->m_pLeft->m_pParent = pRotationNode->m_pRight;
    }

    pSubtree->m_pParent = pRotationNode->m_pParent;

    if (pRotationNode->m_pParent->m_pParent == pRotationNode) {
        // make pSubtree root node if pRotationNode was the root
        header.m_endNode->m_pParent = pSubtree;
    } else if (IsLeftChild(pRotationNode)) {
        pRotationNode->m_pParent->m_pLeft = pSubtree;
    } else {
        pRotationNode->m_pParent->m_pRight = pSubtree;
    }

    pSubtree->m_pLeft = pRotationNode;
    pRotationNode->m_pParent = pSubtree;
}

// TODO: add description and visualizations.
inline void RightRotate(TreeHeader& header, NodeBase* pRotationNode) noexcept {
    NodeBase* pSubtree = pRotationNode->m_pLeft;
    pRotationNode->m_pLeft = pSubtree->m_pRight;

    if (pSubtree->m_pRight) {
        // if right subtree of pSubtree is not empty, set parent
        pSubtree->m_pRight->m_pParent = pRotationNode;
    }

    pSubtree->m_pParent = pRotationNode->m_pParent;

    if (pRotationNode->m_pParent->m_pParent == pRotationNode) {
        // make pSubtree root node if pRotationNode was the root
        header.m_endNode->m_pParent = pSubtree;
    } else if (IsLeftChild(pRotationNode)) {
        pRotationNode->m_pParent->m_pLeft = pSubtree;
    } else {
        pRotationNode->m_pParent->m_pRight = pSubtree;
    }

    pSubtree->m_pRight = pRotationNode;
    pRotationNode->m_pParent = pSubtree;
}

inline void RebalanceAfterInsert(TreeHeader& header, NodeBase* pInsertedNode) noexcept {
    NodeBase* pRoot = header.m_endNode->m_pParent;
    NodeBase* pCurrNode = pInsertedNode;

    while (pCurrNode != pRoot && pCurrNode->m_pParent->m_color == Color::Red) {
        if (IsLeftChild(pCurrNode->m_pParent)) {
            NodeBase* pUncleNode = pCurrNode->m_pParent->m_pParent->m_pRight;

            if (pUncleNode && pUncleNode->m_color == Color::Red) {
                pCurrNode->m_pParent->m_color = Color::Black;
                pUncleNode->m_color = Color::Black;
                pCurrNode->m_pParent->m_pParent->m_color = Color::Red;
                pCurrNode = pCurrNode->m_pParent->m_pParent;
            } else {
                if (IsRightChild(pCurrNode)) {
                    pCurrNode = pCurrNode->m_pParent;
                    LeftRotate(header, pCurrNode);
                }

                pCurrNode->m_pParent->m_color = Color::Black;
                pCurrNode->m_pParent->m_pParent->m_color = Color::Red;
                RightRotate(header, pCurrNode->m_pParent->m_pParent);
            }
        } else {
            NodeBase* pUncleNode = pCurrNode->m_pParent->m_pParent->m_pLeft;

            if (pUncleNode && pUncleNode->m_color == Color::Red) {
                pCurrNode->m_pParent->m_color = Color::Black;
                pUncleNode->m_color = Color::Black;
                pCurrNode->m_pParent->m_pParent->m_color = Color::Red;
                pCurrNode = pCurrNode->m_pParent->m_pParent;
            } else {
                if (IsLeftChild(pCurrNode)) {
                    pCurrNode = pCurrNode->m_pParent;
                    RightRotate(header, pCurrNode);
                }

                pCurrNode->m_pParent->m_color = Color::Black;
                pCurrNode->m_pParent->m_pParent->m_color = Color::Red;
                LeftRotate(header, pCurrNode->m_pParent->m_pParent);
            }
        }
    }

    header.m_endNode->m_pParent->m_color = Color::Black;
}

inline void Transplant(TreeHeader& header, NodeBase* pNode, NodeBase* pExchangeNode) {
    if (pNode->m_pParent == header.m_endNode) {
        header.m_endNode->m_pParent = pExchangeNode;
    } else if (IsLeftChild(pNode)) {
        pNode->m_pParent->m_pLeft = pExchangeNode;
    } else {
        pNode->m_pParent->m_pRight = pExchangeNode;
    }

    if (pExchangeNode) {
        pExchangeNode->m_pParent = pNode->m_pParent;
    }
}

inline void RebalanceAfterRemove(TreeHeader& header, NodeBase* pTransplant) {
    if (!pTransplant)
        return;  // Avoid nullptr dereference

    NodeBase*& pRoot = header.m_endNode->m_pParent;
    NodeBase* pCurrNode = pTransplant;

    while (pCurrNode != pRoot && pCurrNode && pCurrNode->m_color == Color::Black) {
        if (!pCurrNode->m_pParent)
            break;  // Prevent nullptr access

        if (IsLeftChild(pCurrNode)) {
            NodeBase* pSibling = pCurrNode->m_pParent->m_pRight;
            if (!pSibling)
                break;

            if (pSibling->m_color == Color::Red) {
                pSibling->m_color = Color::Black;
                pCurrNode->m_pParent->m_color = Color::Red;
                LeftRotate(header, pCurrNode->m_pParent);
                pSibling = pCurrNode->m_pParent->m_pRight;  // Update sibling reference
                if (!pSibling)
                    break;
            }

            if ((!pSibling->m_pLeft || pSibling->m_pLeft->m_color == Color::Black) &&
                (!pSibling->m_pRight || pSibling->m_pRight->m_color == Color::Black)) {
                pSibling->m_color = Color::Red;
                pCurrNode = pCurrNode->m_pParent;
            } else {
                if (!pSibling->m_pRight || pSibling->m_pRight->m_color == Color::Black) {
                    if (pSibling->m_pLeft)
                        pSibling->m_pLeft->m_color = Color::Black;
                    pSibling->m_color = Color::Red;
                    RightRotate(header, pSibling);
                    pSibling = pCurrNode->m_pParent->m_pRight;
                    if (!pSibling)
                        break;
                }
                pSibling->m_color = pCurrNode->m_pParent->m_color;
                pCurrNode->m_pParent->m_color = Color::Black;
                if (pSibling->m_pRight)
                    pSibling->m_pRight->m_color = Color::Black;
                LeftRotate(header, pCurrNode->m_pParent);
                pCurrNode = pRoot;
            }
        } else {
            NodeBase* pSibling = pCurrNode->m_pParent->m_pLeft;
            if (!pSibling)
                break;

            if (pSibling->m_color == Color::Red) {
                pSibling->m_color = Color::Black;
                pCurrNode->m_pParent->m_color = Color::Red;
                RightRotate(header, pCurrNode->m_pParent);
                pSibling = pCurrNode->m_pParent->m_pLeft;
                if (!pSibling)
                    break;
            }

            if ((!pSibling->m_pLeft || pSibling->m_pLeft->m_color == Color::Black) &&
                (!pSibling->m_pRight || pSibling->m_pRight->m_color == Color::Black)) {
                pSibling->m_color = Color::Red;
                pCurrNode = pCurrNode->m_pParent;
            } else {
                if (!pSibling->m_pLeft || pSibling->m_pLeft->m_color == Color::Black) {
                    if (pSibling->m_pRight)
                        pSibling->m_pRight->m_color = Color::Black;
                    pSibling->m_color = Color::Red;
                    LeftRotate(header, pSibling);
                    pSibling = pCurrNode->m_pParent->m_pLeft;
                    if (!pSibling)
                        break;
                }
                pSibling->m_color = pCurrNode->m_pParent->m_color;
                pCurrNode->m_pParent->m_color = Color::Black;
                if (pSibling->m_pLeft)
                    pSibling->m_pLeft->m_color = Color::Black;
                RightRotate(header, pCurrNode->m_pParent);
                pCurrNode = pRoot;
            }
        }
    }

    if (pCurrNode)
        pCurrNode->m_color = Color::Black;
}

/// `KeyValueType` helps to get a `key_type` and a `value_type` from some generic type `T`.
template <typename T>
struct KeyValueType {
    using key_type = T;
    using value_type = T;
};

template <typename T1, typename T2>
struct KeyValueType<std::pair<T1, T2>> {
    using key_type = T1;
    using value_type = T2;
};

template <typename T1, typename T2>
struct KeyValueType<const std::pair<T1, T2>> {
    using key_type = T1;
    using value_type = T2;
};

template <typename T>
inline T Key(const T& val) {
    return val;
}

template <typename T1, typename T2>
inline T1 Key(const std::pair<T1, T2>& val) {
    return val.first;
}

template <typename T>
inline T Value(const T& val) {
    return val;
}

template <typename T1, typename T2>
inline T1 Value(const std::pair<T1, T2>& val) {
    return val.second;
}

};  // namespace internal

template <typename V, typename Cmp = std::less<V>>
class RbTree : private internal::TreeHeader {
public:
    using key_value_type = V;
    using key_type = typename internal::KeyValueType<V>::key_type;
    using value_type = typename internal::KeyValueType<V>::value_type;
    using compare = Cmp;
    using size_type = std::size_t;

    using NodeType = internal::Node<key_value_type>;
    using NodePtr = internal::Node<key_value_type>*;
    using BaseType = internal::NodeBase;
    using BasePtr = internal::NodeBase*;

public:
    // Default constructor.
    RbTree() {
        m_endNode = new BaseType{};
        m_endNode->m_color = internal::Color::Red;
        m_endNode->m_pParent = nullptr;
        m_endNode->m_pLeft = nullptr;
        m_endNode->m_pRight = nullptr;
        m_compare = compare{};
        m_size = 0;
    }

    /// Copy constructor.
    RbTree(const RbTree& other) {}
    /// Move constructor.
    RbTree(RbTree&& other) {}

    /// Copy assignment operator.
    RbTree& operator=(const RbTree& other) {}
    /// Move assignment operator.
    RbTree& operator=(RbTree&& other) {}

    /// Destructor removes all nodes of a tree.
    ~RbTree() {}

public:
    /// Size returns current number of elements in container.
    size_type Size() const noexcept { return m_size; }

    /// Empty returns true if size of container is 0, false otherwise.
    bool Empty() const noexcept { return m_size == 0; }

public:
    /// Insert adds a new value to the container only if it is not presented in the tree.
    NodePtr Insert(const key_value_type& val) noexcept { return InsertInternal(val); }

    /// InsertOrUpdate adds new value to the tree or update already existing one.
    NodePtr InsertOrUpdate(const key_value_type& val) noexcept { return InsertInternal(val, true); }

    /// Find returns a node, which holds a `key`.
    NodePtr Find(const key_type& key) const noexcept {
        NodePtr pCurrNode = Root();

        while (pCurrNode) {
            key_type currKey = internal::Key(pCurrNode->m_value);

            if (m_compare(key, currKey)) {
                pCurrNode = Left(pCurrNode);
            } else if (m_compare(currKey, key)) {
                pCurrNode = Right(pCurrNode);
            } else {
                return pCurrNode;
            }
        }

        return nullptr;
    }

    /// Contains retuns true if value with `key` is presented in the tree.
    bool Contains(const key_type& key) const noexcept { return Find(key) != nullptr; }

    /// `Remove` removes element with key node and re-balance the tree if needed.
    void Remove(const key_type& key) {
        NodePtr pNodeToRemove = Find(key);

        if (!pNodeToRemove) {
            return;
        }

        // TODO: this is bad, situation with tree of size 1 should be handled more generic
        if (m_size == 1) {
            DeallocateNode(static_cast<NodePtr>(m_endNode->m_pParent));
            m_endNode->m_pParent = nullptr;
            --m_size;
            return;
        }

        // Try to handle a situation then node to remove has no children
        // //////////////////////////////////
        if (!pNodeToRemove->m_pLeft && !pNodeToRemove->m_pRight) {
            if (internal::IsLeftChild(pNodeToRemove)) {
                pNodeToRemove->m_pParent->m_pLeft = nullptr;
            } else {
                pNodeToRemove->m_pParent->m_pRight = nullptr;
            }
            DeallocateNode(pNodeToRemove);
            --m_size;
            return;
        }
        ///////////////////////////////////////////////////////////////////////////////////////////////////

        NodePtr pNode = pNodeToRemove;
        auto nodeOriginalColor = pNodeToRemove->m_color;

        NodePtr pTransplant = nullptr;

        if (pNode->m_pLeft == nullptr) {
            pTransplant = Right(pNode);
            internal::Transplant(*this, pNode, pNode->m_pRight);
        } else if (pNode->m_pRight == nullptr) {
            pTransplant = Left(pNode);
            internal::Transplant(*this, pNode, pNode->m_pLeft);
        } else {
            pNode = static_cast<NodePtr>(internal::TreeMin(pNode->m_pRight));
            nodeOriginalColor = pNode->m_color;
            // NOTE: changed it to conditional stmt, because if pNode has no children, pTransplant
            // will be nullptr
            pTransplant = pNode->m_pRight ? Right(pNode) : pNode;

            if (pNode != Right(pNodeToRemove)) {
                internal::Transplant(*this, pNode, pNode->m_pRight);
                pNode->m_pRight = pNodeToRemove->m_pRight;
                pNode->m_pRight->m_pParent = pNode;
            } else {
                pTransplant->m_pParent = pNode;
            }

            internal::Transplant(*this, pNodeToRemove, pNode);
            pNode->m_pLeft = pNodeToRemove->m_pLeft;
            pNode->m_pLeft->m_pParent = pNode;
            pNode->m_color = pNodeToRemove->m_color;
        }

        DeallocateNode(pNodeToRemove);

        --m_size;

        if (nodeOriginalColor == internal::Color::Black) {
            internal::RebalanceAfterRemove(*this, pTransplant);
        }
    }

    bool operator==(const RbTree& other) const noexcept {
        if (this == std::addressof(other)) {
            return true;
        }
        return TreesAreEqual(Root(), other.Root());
    }

    bool operator!=(const RbTree& other) const noexcept { return !(*this == other); }

    void Dump() const { Print(std::cout, Root(), 0, false); }

private:
    void Print(std::ostream& out, NodePtr pNode, size_type level, bool isLeftChild) const {
        if (!pNode) {
            out << "{ }\n";
            return;
        }
        if (level > 0) {
            for (size_type i = 0; i < level - 1; ++i) {
                out << "|   ";
            }
            out << "|---";
        }

        out << "{ " << pNode->m_value;
        if (level > 0) {
            out << ", " << (isLeftChild ? "Left, " : "Right, ");
        } else {
            out << ", Root, ";
        }
        out << (pNode->m_color == internal::Color::Black ? "Black }\n" : "Red }\n");

        if (pNode->m_pLeft) {
            Print(out, Left(pNode), level + 1, true);
        }
        if (pNode->m_pRight) {
            Print(out, Right(pNode), level + 1, false);
        }
    }

    NodePtr Root() const noexcept { return static_cast<NodePtr>(m_endNode->m_pParent); }

    NodePtr InsertInternal(const key_value_type& val, bool updateIfExists = false) noexcept {
        NodePtr pCurrNode = Root();
        NodePtr pParentNode = nullptr;  // this node will be a parent of a new node

        key_type keyToInsert = internal::Key(val);
        bool insertLeft = false;

        while (pCurrNode != nullptr) {
            pParentNode = pCurrNode;
            key_type keyCurrNode = internal::Key(pCurrNode->m_value);

            if (m_compare(keyToInsert, keyCurrNode)) {
                insertLeft = true;
                pCurrNode = Left(pCurrNode);
            } else {
                insertLeft = false;
                if (!m_compare(keyCurrNode, keyToInsert)) {
                    if (updateIfExists) {
                        pCurrNode->m_value = val;
                    }
                    return pCurrNode;
                }

                pCurrNode = Right(pCurrNode);
            }
        }

        NodePtr pNewNode = nullptr;

        if (pParentNode == nullptr) {
            // tree is empty, we need to create a root node
            pNewNode = AllocateNode(val, pParentNode, internal::Color::Black);
            m_endNode->m_pParent = pNewNode;
            m_endNode->m_pParent->m_pParent = m_endNode;
            m_endNode->m_pLeft = pNewNode;   // root node is now the most left
            m_endNode->m_pRight = pNewNode;  // and the most right node
        } else {
            pNewNode = AllocateNode(val, pParentNode);

            if (insertLeft) {
                pParentNode->m_pLeft = pNewNode;
                if (pParentNode == m_endNode->m_pLeft) {
                    m_endNode->m_pLeft = pNewNode;
                }
            } else {
                pParentNode->m_pRight = pNewNode;
                if (pParentNode == m_endNode->m_pRight) {
                    m_endNode->m_pRight = pNewNode;
                }
            }
        }

        internal::RebalanceAfterInsert(*this, pNewNode);

        ++m_size;

        return pNewNode;
    }

private:
    NodePtr AllocateNode(const key_value_type& val,
                         BasePtr pParent,
                         internal::Color color = internal::Color::Red) {
        NodePtr pNode = new NodeType{};
        pNode->m_color = color;
        pNode->m_pParent = pParent;
        pNode->m_value = val;
        return pNode;
    }

    static void DeallocateNode(NodePtr pNode) { delete pNode; }

    static NodePtr Left(BasePtr pNode) { return static_cast<NodePtr>(pNode->m_pLeft); }

    static NodePtr Right(BasePtr pNode) { return static_cast<NodePtr>(pNode->m_pRight); }

    static bool TreesAreEqual(const NodePtr lhs, const NodePtr rhs) {
        if (!lhs && !rhs) {
            return true;
        }
        if ((lhs && !rhs) || (!lhs && rhs)) {
            return false;
        }
        if (lhs->m_value != rhs->m_value || lhs->m_color != rhs->m_color) {
            return false;
        }
        return NodesAreEqual(lhs->m_pLeft, rhs->m_pLeft) &&
               NodesAreEqual(lhs->m_pRight, rhs->m_pRight);
    }

private:
    compare m_compare;  // compare function / functor
};

}  // namespace ads
