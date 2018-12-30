#ifndef AISDI_MAPS_TREEMAP_H
#define AISDI_MAPS_TREEMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>

namespace aisdi {

template <typename KeyType, typename ValueType>
class TreeMap {
public:
    using key_type = KeyType;
    using mapped_type = ValueType;
    using value_type = std::pair<const key_type, mapped_type>;
    using size_type = std::size_t;
    using reference = value_type&;
    using const_reference = const value_type&;

    class ConstIterator;
    class Iterator;
    using iterator = Iterator;
    using const_iterator = ConstIterator;
private:
    struct Node {
        Node *up;
        Node *left, *right;

        value_type *value;
        int bf;
    };

    size_type SIZE;
    Node *root;

    ///metody prywatne
    void Clear(Node *node) {
        if(node) {
            Clear(node->left);
            Clear(node->right);
            value_type *val = node->value;
            delete val;
            delete node;
        }
    }
     //ROTACJE
    void RR(Node *A) {
        Node *B = A->right;
        Node *parent = A->up;

        A->right = B->left;

        if(A->right) {
            A->right->up = A;
        }

        B->left = A;
        B->up = parent;
        A->up = B;

        if(parent) {
            if(parent->left == A) {
                parent->left = B;
            } else {
                parent->right = B;
            }
        } else {
            root = B;
        }

        if(B->bf == -1) {
            A->bf = B->bf = 0;
        } else {
            A->bf = -1;
            B->bf = 1;
        }
    }
    void LL(Node *A) {
        Node *B = A->left;
        Node *parent = A->up;

        A->left = B->right;
        if(A->left) {
            A->left->up = A;
        }

        B->right = A;
        B->up = parent;
        A->up = B;

        if(parent) {
            if(parent->left == A) {
                parent->left = B;
            } else {
                parent->right = B;
            }
        } else {
            root = B;
        }

        if(B->bf == 1) {
            A->bf = B->bf = 0;
        } else {
            A->bf = 1;
            B->bf = -1;
        }
    }
    void RL(Node *A) {
        Node *B = A->right;
        Node *C = B->left;
        Node *parent = A->up;

        B->left = C->right;
        if(B->left) {
            B->left->up = B;
        }

        A->right = C->left;
        if(A->right) {
            A->right->up = A;
        }

        C->left = A;
        C->right = B;
        A->up = B->up = C;
        C->up = parent;

        if(parent) {
            if(parent->left == A) {
                parent->left = C;
            } else {
                parent->right = C;
            }
        } else root = C;

        if(C->bf == -1) {
            A->bf =  1;
        } else {
            A->bf = 0;
        }

        if(C->bf ==  1) {
            B->bf = -1;
        } else {
            B->bf = 0;
        }

        C->bf = 0;
    }
    void LR(Node *A) {
        Node *B = A->left;
        Node *C = B->right;
        Node *parent = A->up;

        B->right = C->left;
        if(B->right) {
            B->right->up = B;
        }

        A->left = C->right;
        if(A->left) {
            A->left->up = A;
        }

        C->right = A;
        C->left = B;
        A->up = B->up = C;
        C->up = parent;

        if(parent) {
            if(parent->left == A) {
                parent->left = C;
            } else {
                parent->right = C;
            }
        } else {
            root = C;
        }

        if(C->bf ==  1) {
            A->bf = -1;
        } else {
            A->bf = 0;
        }

        if(C->bf == -1) {
            B->bf =  1;
        } else {
            B->bf = 0;
        }

        C->bf = 0;
    }

    Node* treeSearch(Node *node, key_type key) const {
        while(node != nullptr && key != node->value->first) {
            if(key < node->value->first) {
                node = node->left;
            } else {
                node = node->right;
            }
        }

        return node;
    }
    Node* removeNode(Node *A) { //tylko wyjmuje wezel z drzewa, usunac (zwolnic pamiec) nalezy recznie po wyjeciu wezla z drzewa
        --SIZE;
        Node *tmp;
        Node *B;
        Node *C;

        bool nest;

        if(A->left && A->right) {
            B = removeNode(treePredecessor(A));
            nest = false;
        } else {
            if(A->left) {
                B = A->left;
                A->left = nullptr;
            } else {
                B = A->right;
                A->right = nullptr;
            }
            A->bf = 0;
            nest = true;
        }

        if(B) {
            B->up = A->up;
            B->left = A->left;

            if(B->left) {
                B->left->up = B;
            }

            B->right = A->right;

            if(B->right) {
                B->right->up = B;
            }

            B->bf = A->bf;
        }

        if(A->up) {
            if(A->up->left == A) {
                A->up->left = B;
            } else {
                A->up->right = B;
            }
        } else root = B;

        if(nest) {
            C = B;
            C = A->up;
            while(B) {
                if(!B->bf) {
                    if(B->left == C) {
                        B->bf = -1;
                    } else {
                        B->bf = 1;
                    }
                    break;
                } else {
                    if(((B->bf == 1) && (B->left == C)) || ((B->bf == -1) && (B->right == C))) {
                        B->bf = 0;
                        C = B;
                        B = B->up;
                    } else {
                        if(B->left == C) {
                            tmp = B->right;
                        } else {
                            tmp = B->left;
                        }
                        if(!tmp->bf) {
                            if(B->bf == 1) {
                                LL(B);
                            } else {
                                RR(B);
                            }
                            break;
                        } else if(B->bf == tmp->bf) {
                            if(B->bf == 1) {
                                LL(B);
                            } else {
                                RR(B);
                            }

                            C = tmp;
                            B = tmp->up;
                        } else {
                            if(B->bf == 1) {
                                LR(B);
                            } else {
                                RL(B);
                            }

                            C = B->up;
                            B = C->up;
                        }
                    }
                }
            }
        }
        return A;
    }
    Node* treeMinimum(Node *node) const {
        if(node == nullptr) {
            return node;
        }

        while(node->left != nullptr) {
            node = node->left;
        }

        return node;
    }
    Node* treeMaximum(Node *node) const {
        if(node == nullptr) {
            return node;
        }

        while(node->right != nullptr) {
            node = node->right;
        }

        return node;
    }
    Node* treeSuccessor(Node *node) const {
        if(node == nullptr) {
            return node;
        }

        if(node->right != nullptr) {
            return treeMinimum(node->right);
        }

        Node *B = node->up;

        while(B != nullptr && node == B->right) {
            node = B;
            B = B->up;
        }

        return B;
    }
    Node* treePredecessor(Node *node) const {
        if(node == nullptr) {
            return node;
        }

        if(node->left != nullptr) {
            return treeMaximum(node->left);
        }

        Node *B = node->up;

        while(B != nullptr && node == B->left) {
            node = B;
            B = B->up;
        }

        return B;
    }

public:
    TreeMap() {
        root = nullptr;
        SIZE = 0;
    }

    ~TreeMap() {
        Clear(root);
        SIZE = 0;
    }

    TreeMap(std::initializer_list<value_type> list):TreeMap() {
        for(auto it = list.begin(); it < list.end(); it++) {
            this->operator[](it->first) = it->second;
        }
    }

    TreeMap(const TreeMap& other):TreeMap() {
        for(auto it = other.begin(); it != other.end(); it++) {
            this->operator[](it->first) = it->second;
        }
    }

    TreeMap(TreeMap&& other):TreeMap() {
        *this = std::move(other);
    }

    TreeMap& operator=(const TreeMap& other) {
        if(other == *this) {
            return *this;
        }

        Clear(root);
        SIZE = 0;
        root = nullptr;

        for(auto it = other.begin(); it != other.end(); it++) {
            this->operator[](it->first) = it->second;
        }

        return *this;
    }

    TreeMap& operator=(TreeMap&& other) {
        Clear(root);
        root = nullptr;
        SIZE = 0;

        std::swap(root, other.root);
        std::swap(SIZE, other.SIZE);
        return *this;
    }

    bool isEmpty() const {
        if(SIZE == 0) return true;
        else return false;
    }

    mapped_type& operator[](const key_type& key) { //wstawianie
        Node *temp = new Node;
        temp->up = temp->left = temp->right = nullptr;
        temp->bf = 0;
        temp->value = new value_type(key, mapped_type{});

        Node *parent = root;


        if(parent == nullptr) {
            root = temp;
            SIZE++;
            return temp->value->second;
        }

        while(true) {
            if(key == parent->value->first) {
                //mapped_type test;
                //if(test == mapped_type{}) {
                    delete temp->value;
                    delete temp;
                    return parent->value->second;
                //}

                delete parent->value;
                parent->value = temp->value;
                delete temp;
                return parent->value->second;
            }

            if(key < parent->value->first) {
                if(parent->left == nullptr) {
                    parent->left = temp;
                    break;
                }

                parent = parent->left;
            } else {
                if(parent->right == nullptr) {
                    parent->right = temp;
                    break;
                }

                parent = parent->right;
            }
        }

        temp->up = parent;

        if(parent->bf) {
            parent->bf = 0;
            SIZE++;
            return temp->value->second;
        }

        if(parent->left == temp) {
            parent->bf = 1;
        } else {
            parent->bf = -1;
        }

        Node *pre_parent = parent->up;
        bool unbalanced = false;

        while(pre_parent != nullptr) {
            if(pre_parent->bf) {
                unbalanced = true;
                break;
            }

            if(pre_parent->left == parent) {
                pre_parent->bf = 1;
            } else {
                pre_parent->bf = -1;
            }

            parent = pre_parent;
            pre_parent = pre_parent->up;
        }

        if(unbalanced == true) {
            if(pre_parent->bf == 1) {
                if(pre_parent->right == parent) {
                    pre_parent->bf = 0;
                } else if(parent->bf == -1) {
                    LR(pre_parent);
                } else {
                    LL(pre_parent);
                }
            } else {
                if(pre_parent->left == parent) {
                    pre_parent->bf = 0;
                } else if(parent->bf == 1) {
                    RL(pre_parent);
                } else {
                    RR(pre_parent);
                }
            }
        }
        ++SIZE;
        return temp->value->second;
    }

    const mapped_type& valueOf(const key_type& key) const {

        Node *tmp = root;
        key_type k = key;
        tmp = treeSearch(tmp, k);
        if(tmp == nullptr) {
            throw std::out_of_range("Tree valueOf");
        }
        return tmp->value->second;
    }

    mapped_type& valueOf(const key_type& key) {
        Node *tmp;

        tmp = treeSearch(root, key);
        if(tmp == nullptr) {
            throw std::out_of_range("Tree valueOf");
        }
        return tmp->value->second;
    }

    const_iterator find(const key_type& key) const {
        ConstIterator it;
        it.tree = this;
        it.node = treeSearch(root, key);

        return it;
    }

    iterator find(const key_type& key) {
        Iterator it;
        it.tree = this;
        it.node = treeSearch(root, key);

        return it;
    }

    void remove(const key_type& key) {
        Node *tmp = treeSearch(root, key);
        if(tmp == nullptr) {
            throw std::out_of_range("Tree remove");
        }
        removeNode(tmp);
        delete tmp->value;
        delete tmp;
    }

    void remove(const const_iterator& it) {
        Node *tmp = it.node;
        if(tmp == nullptr) {
            throw std::out_of_range("Tree remove");
        }
        removeNode(tmp);
        delete tmp->value;
        delete tmp;
    }

    size_type getSize() const {
        return SIZE;
    }

    bool operator==(const TreeMap& other) const {
        if(SIZE != other.getSize()) {
            return false;
        }

        Iterator act;
        act.node = treeMinimum(root);
        act.tree = this;

        for(auto it = other.begin(); it != other.end(); ++it, ++act) {
            if(*act != *it) {
                return false;
            }
        }

        return true;
    }

    bool operator!=(const TreeMap& other) const {
        return !(*this == other);
    }

    iterator begin() {
        Iterator it;
        it.tree = this;
        it.node = treeMinimum(root);

        return it;
    }

    iterator end() {
        Iterator it;
        it.tree = this;
        it.node = nullptr;

        return it;
    }

    const_iterator cbegin() const {
        ConstIterator it;
        it.tree = this;
        it.node = treeMinimum(root);

        return it;
    }

    const_iterator cend() const {
        ConstIterator it;
        it.tree = this;
        it.node = nullptr;

        return it;
    }

    const_iterator begin() const {
        return cbegin();
    }

    const_iterator end() const {
        return cend();
    }
};

template <typename KeyType, typename ValueType>
class TreeMap<KeyType, ValueType>::ConstIterator {
public:
    using reference = typename TreeMap::const_reference;
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = typename TreeMap::value_type;
    using pointer = const typename TreeMap::value_type*;

    Node *node;
    const TreeMap *tree;

    explicit ConstIterator() {
    }

    ConstIterator(const ConstIterator& other): node(other.node), tree(other.tree) {
    }

    ConstIterator& operator++() { //PRE
        if(node == nullptr) {
            throw std::out_of_range("Tree operator++ PRE");
        }

        node = tree->treeSuccessor(node);
        return *this;
    }

    ConstIterator operator++(int) { //POST
        if(node == nullptr) {
            throw std::out_of_range("Tree operator++ POST");
        }

        ConstIterator tmp;
        tmp.node = node;
        tmp.tree = tree;

        node = tree->treeSuccessor(node);
        return tmp;
    }

    ConstIterator& operator--() { //PRE
        if(*this == tree->begin()) {
            throw std::out_of_range("Tree operator-- PRE");
        }

        if(node == nullptr) {
            node = tree->treeMaximum(tree->root);
            return *this;
        }

        node = tree->treePredecessor(node);
        return *this;
    }

    ConstIterator operator--(int) {
        if(*this == tree->begin()) {
            throw std::out_of_range("Tree operator-- POST");
        }

        ConstIterator tmp;
        tmp.node = node;
        tmp.tree = tree;

        if(node == nullptr) {
            node = tree->treeMaximum(tree->root);
            return tmp;
        }

        node = tree->treePredecessor(node);
        return tmp;
    }

    value_type& getValueType() const {
        value_type *value;
        value = node->value;
        return *value;
    }

    reference operator*() const {
        if(node == nullptr) {
            throw std::out_of_range("");
        }

        return getValueType();
    }

    pointer operator->() const {
        return &this->operator*();
    }

    bool operator==(const ConstIterator& other) const {
        if(node == nullptr && other.node == nullptr) {
            return true;
        } else if((node == nullptr  && other.node != nullptr) || (node != nullptr && other.node == nullptr)) {
            return false;
        }

        if(node->value == other.node->value && tree == other.tree) {
            return true;
        } else {
            return false;
        }
    }

    bool operator!=(const ConstIterator& other) const {
        return !(*this == other);
    }
};

template <typename KeyType, typename ValueType>
class TreeMap<KeyType, ValueType>::Iterator : public TreeMap<KeyType, ValueType>::ConstIterator {
public:
    using reference = typename TreeMap::reference;
    using pointer = typename TreeMap::value_type*;

    explicit Iterator() {
    }

    Iterator(const ConstIterator& other)
        : ConstIterator(other) {
    }

    Iterator& operator++() {
        ConstIterator::operator++();
        return *this;
    }

    Iterator operator++(int) {
        auto result = *this;
        ConstIterator::operator++();
        return result;
    }

    Iterator& operator--() {
        ConstIterator::operator--();
        return *this;
    }

    Iterator operator--(int) {
        auto result = *this;
        ConstIterator::operator--();
        return result;
    }

    pointer operator->() const {
        return &this->operator*();
    }

    reference operator*() const {
        // ugly cast, yet reduces code duplication.
        return const_cast<reference>(ConstIterator::operator*());
    }
};

}

#endif /* AISDI_MAPS_MAP_H */