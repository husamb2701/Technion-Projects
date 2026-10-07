#pragma once
#include <stdexcept>
#include <cmath>

using namespace std;

template <typename Key, typename Value>
class AVL {
private:
    //private struct node
    struct Node {
        int height;
        Key key;
        Value value;
        Node* left;
        Node* right;
        Node* parent;
        Node(const Key& k, const Value& v, Node* p = nullptr)
            : height(0), key(k), value(v), left(nullptr), right(nullptr), parent(p) {}
        };

    //variable of the tree
    Node* root = nullptr;

    //tree organizing methods
    Node* findParentForInsert(const Key& key) const;
    Node* findNextInOrder(const Node* node) const;
    Node* findNode(const Key& key) const;
    Node* removeNodeWith_0_or_1_son (Node* toRemove);
    Node* removeNodeWith_2_sons (Node* toRemove);
    int BF(Node* node) const;
    int height(Node* node) const;
    Node* balance(Node* node);
    Node* LLroll(Node* node);
    Node* RRroll(Node* node);
    Node* LRroll(Node* node);
    Node* RLroll(Node* node);
    void destroyTree(Node* node);
    Node* copyTree(const Node* sourceNode, Node* newParent);

public:
    AVL() = default;
    AVL(const AVL&);
    AVL& operator=(const AVL& other);
    ~AVL();
    bool isEmpty() const;
    bool insert(const Key& key, const Value& value);
    Value* search(const Key& key);
    bool remove(const Key& key);


};

//private tree organizing methods
template <typename Key, typename Value>
typename AVL<Key, Value>::Node* AVL<Key, Value>::findParentForInsert(const Key& key)const {
    Node* current = this->root;
    Node* parent = nullptr;
    while (current != nullptr) {
        if (current->key == key) return nullptr;
        parent = current;
        if (key < current->key) {
            current = current->left;
        } else if (key > current->key) {
            current = current->right;
        }
    }
    return parent;
}

template <typename Key, typename Value>
typename AVL<Key, Value>::Node* AVL<Key, Value>::findNextInOrder(const Node* node)const {
    if (node == nullptr || node->right == nullptr) return nullptr;
    Node* current = node->right;
    while (current->left != nullptr) {
        current = current->left;
    }
    return current;
}

template <typename Key, typename Value>
typename AVL<Key, Value>::Node* AVL<Key, Value>::findNode(const Key& key) const{
    Node* current = this->root;
    while (current != nullptr) {
        if (current->key == key) {
            return current;
        } else if (key < current->key) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return nullptr;
}

template <typename Key, typename Value>
typename AVL<Key, Value>::Node* AVL<Key, Value>::removeNodeWith_0_or_1_son(Node* toRemove) {
    if (toRemove == nullptr) return nullptr;
    Node* child = (toRemove->left != nullptr) ? toRemove->left : toRemove->right;
    Node* parentToReturn = toRemove->parent;
    if (toRemove->parent == nullptr) {
        this->root = child;
    } else if (toRemove->parent->right == toRemove) {
        toRemove->parent->right = child;
    } else {
        toRemove->parent->left = child;
    }
    if (child != nullptr) {
        child->parent = toRemove->parent;
    }
    delete toRemove;
    return parentToReturn;
}

template <typename Key, typename Value>
typename AVL<Key, Value>::Node* AVL<Key, Value>::removeNodeWith_2_sons(Node* toRemove) {
    Node* next = findNextInOrder(toRemove);
    Node* startBalance = next->parent;
    if (next == toRemove->right) {
        startBalance = toRemove;
    }
    toRemove->key = next->key;
    toRemove->value = next->value;
    removeNodeWith_0_or_1_son(next);
    return startBalance;
}

template <typename Key, typename Value>
int  AVL<Key, Value>::BF(Node* node) const {
    if (node == nullptr) {
        return 0;
    }
    return height(node->left) - height(node->right);
}

template <typename Key, typename Value>
int AVL<Key, Value>::height(Node* node) const {
    if (node == nullptr) return -1;
    return node->height;
}

template <typename Key, typename Value>
typename AVL<Key, Value>::Node* AVL<Key, Value>::balance(Node* node) {
    if (BF(node) == 2) {
        if (BF(node->left) >= 0) {
            return LLroll(node);
        } else {
            return LRroll(node);
        }
    }
    if (BF(node) == -2) {
        if (BF(node->right) <= 0) {
            return RRroll(node);
        } else {
            return RLroll(node);
        }
    }
    return node;
}

//roll on the problem root
template <typename Key, typename Value>
typename AVL<Key, Value>::Node* AVL<Key, Value>::LLroll(Node* node) {
    //notations
    Node* B = node;
    Node* A = node->left;
    Node* AR = A->right;
    //doing the roll
    B->left = AR;
    A->right = B;
    //updating parents and root
    if (AR != nullptr) {
        AR->parent = B;
    }
    if (B->parent == nullptr) {
        A->parent = nullptr;
        this->root = A;
    } else if (B->parent->right == B) {
        A->parent = B->parent;
        B->parent->right = A;
    } else {
        A->parent = B->parent;
        B->parent->left = A;
    }
    B->parent = A;
    //updating heights
    B->height = 1 + max(height(B->left), height(B->right));
    A->height = 1 + max(height(A->left), height(A->right));
    return A;
}

template <typename Key, typename Value>
typename AVL<Key, Value>::Node* AVL<Key, Value>::RRroll(Node* node) {
    //notations
    Node* B = node;
    Node* A = node->right;
    Node* AL = A->left;
    //doing the roll
    B->right = AL;
    A->left = B;
    //updating parents and root
    if (AL != nullptr) {
        AL->parent = B;
    }
    if (B->parent == nullptr) {
        A->parent = nullptr;
        this->root = A;
    } else if (B->parent->right == B) {
        A->parent = B->parent;
        B->parent->right = A;
    } else {
        A->parent = B->parent;
        B->parent->left = A;
    }
    B->parent = A;
    //updating heights
    B->height = 1 + max(height(B->left), height(B->right));
    A->height = 1 + max(height(A->left), height(A->right));
    return A;
}

template <typename Key, typename Value>
typename AVL<Key, Value>::Node* AVL<Key, Value>::LRroll(Node* node) {
    node->left = RRroll(node->left);
    return LLroll(node);
}

template <typename Key, typename Value>
typename AVL<Key, Value>::Node* AVL<Key, Value>::RLroll(Node* node) {
    node->right = LLroll(node->right);
    return RRroll(node);
}

template <typename Key, typename Value>
void AVL<Key, Value>:: destroyTree(Node* node) {
    if (node == nullptr) return;
    destroyTree(node->left);
    destroyTree(node->right);
    delete node;
}

template <typename Key, typename Value>
typename AVL<Key, Value>::Node* AVL<Key, Value>::copyTree(const Node* sourceNode, Node* newParent) {
    if (sourceNode == nullptr) return nullptr;
    Node* newNode = new Node(sourceNode->key, sourceNode->value, newParent);
    newNode->height = sourceNode->height;
    newNode->left = copyTree(sourceNode->left, newNode);
    newNode->right = copyTree(sourceNode->right, newNode);
    return newNode;
}

template <typename Key, typename Value>
AVL<Key, Value>::AVL(const AVL& other) {
    this->root = nullptr;
    if (other.root != nullptr) {
        this->root = copyTree(other.root, this->root);
    }
}

template <typename Key, typename Value>
typename AVL<Key, Value>::AVL& AVL<Key, Value>::operator=(const AVL& other) {
    if (this == &other) return *this;
    destroyTree(this->root);
    if (other.root != nullptr) {
        this->root = copyTree(other.root, nullptr);
    } else {
        this->root = nullptr;
    }
    return *this;
}


template <typename Key, typename Value>
AVL<Key, Value>::~AVL(){
    destroyTree(this->root);
    this->root = nullptr;
}

//public user methods
template <typename Key, typename Value>
bool AVL<Key,Value>::isEmpty() const {
    return root == nullptr;
}

template <typename Key, typename Value>
bool AVL<Key, Value>::insert(const Key& key, const Value& value) {
    //trying to insert new node
    if (this->root == nullptr) {
        this->root = new Node(key, value);
        this->root->height = 0;
        return true;
    }

    Node* current = nullptr;
    Node* parentForInsert = findParentForInsert(key);
    if (parentForInsert == nullptr) return false;
    if (parentForInsert->key > key) {
        parentForInsert->left = new Node(key, value, parentForInsert);
        parentForInsert->left->height = 0;
        current = parentForInsert->left;
    } else if (parentForInsert->key < key) {
        parentForInsert->right = new Node(key, value, parentForInsert);
        parentForInsert->right->height = 0;
        current = parentForInsert->right;
    }
    //balancing the tree
    while (current != this->root) {
        Node* parent = current->parent;
        if (parent->height >= current->height + 1) {
            return true;
        }
        parent->height = current->height + 1;
        if (std::abs(BF(parent)) == 2) {
            balance(parent);
            return true;
        }
        current = parent;
    }
    return true;
}

template <typename Key, typename Value>
Value* AVL<Key, Value>::search(const Key& key) {
    Node* result = findNode(key);
    if (result == nullptr) return nullptr;
    return &(result->value);
}

template <typename Key, typename Value>
bool AVL<Key, Value>::remove(const Key& key) {
    Node* toRemove = findNode(key);
    if (toRemove == nullptr) return false;
    Node* current = nullptr;
    if (toRemove->right != nullptr && toRemove->left != nullptr) {
        current = removeNodeWith_2_sons(toRemove);
    } else {
        current = removeNodeWith_0_or_1_son(toRemove);
    }
    while (current != nullptr) {
        int oldHeight = current->height;
        current->height = 1 + max(height(current->left), height(current->right));
        Node* semiTreeRoot = balance(current);
        if (semiTreeRoot->height == oldHeight) return true;
        current = semiTreeRoot->parent;
    }
    return true;
}