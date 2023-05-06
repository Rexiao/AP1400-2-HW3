#include "bst.h"

#include <compare>
#include <functional>
#include <iostream>
#include <memory>
#include <queue>
#include <stack>
#include <utility>
#include <vector>

BST::Node* copy_bst(const BST::Node* other_root);

BST::Node::Node(int value, Node* left, Node* right)
    : value{value}, left(left), right(right) {}

BST::Node::Node() : value(0), left(nullptr), right(nullptr) {}

BST::Node::Node(const Node& node)
    : value(node.value), left(node.left), right(node.right) {}

std::strong_ordering BST::Node::operator<=>(const int& other_val) const {
    return value <=> other_val;
}

bool operator==(const BST::Node& lhs, const BST::Node& rhs) {
    return lhs.value == rhs.value;
}

std::ostream& operator<<(std::ostream& os, const BST::Node& obj) {
    std::cout << &obj << ' ' << obj.value << ' ' << obj.left << ' ' << obj.right << std::endl;
    return os;
}

BST::Node* copy_bst(const BST::Node* other_node) {
    if (!other_node) {
        return nullptr;
    }
    // std::cout << other_node->value << std::endl;
    BST::Node* node = new BST::Node(other_node->value, copy_bst(other_node->left), copy_bst(other_node->right));
    return node;
}
BST::BST() : root(nullptr) {}

BST::BST(const BST& bst) {
    root = copy_bst(bst.root);
}
BST::BST(BST&& bst) : root{std::exchange(bst.root, nullptr)} {
}

BST::~BST() {
    std::vector<Node*> nodes;
    bfs([&nodes](BST::Node*& node) { nodes.push_back(node); });
    for (auto& node : nodes)
        delete node;
}

void BST::bfs(std::function<void(Node*& node)> func) const {
    if (!root) {
        return;
    }
    std::queue<Node*> q({root});
    while (!q.empty()) {
        Node*& node = q.front();
        if (node->left) {
            q.push(node->left);
        }
        if (node->right) {
            q.push(node->right);
        }
        func(node);
        q.pop();
    }
}

BST::Node*& BST::get_root() {
    return root;
}

std::pair<BST::Node*, BST::Node*> find_parent_and_node(const int value, BST::Node* root) {
    BST::Node* node = root;
    BST::Node* parent = nullptr;
    while (node) {
        if (*node == value) {
            break;
        } else if (*node > value) {
            parent = node;
            node = node->left;
        } else {
            parent = node;
            node = node->right;
        }
    }
    if (!node) {
        return {nullptr, nullptr};
    }
    return {parent, node};
}

bool BST::add_node(int value) {
    Node* node = root;
    if (node == nullptr) {
        root = new Node(value);
        return true;
    }
    Node* parent = nullptr;
    while (node) {
        if (node->value == value) {
            return false;
        } else if (node->value > value) {
            parent = node;
            node = node->left;
        } else {
            parent = node;
            node = node->right;
        }
    }
    if (parent->value > value) {
        parent->left = new Node(value);
    } else {
        parent->right = new Node(value);
    }
    return true;
}

BST::Node** BST::find_parrent(int value) {
    auto [parent, _] = find_parent_and_node(value, root);
    if (!parent)
        return nullptr;
    return new BST::Node*(parent);
}

BST::Node** BST::find_node(int value) {
    auto [_, node] = find_parent_and_node(value, root);
    if (!node) {
        return nullptr;
    }
    return new BST::Node*(node);
}

size_t BST::length() const {
    size_t len = 0;
    bfs([&len](Node*& node) { len += 1; });
    return len;
}

std::ostream& operator<<(std::ostream& os, const BST& obj) {
    std::cout << "********************************************************************************" << std::endl;
    obj.bfs([](BST::Node*& node) { std::cout << *node; });
    std::cout << "********************************************************************************" << std::endl;
    return os;
}

BST::Node** BST::find_successor(int value) {
    if (!root) {
        return nullptr;
    }
    // push all left node
    std::stack<const Node*> st;
    const auto* node = root;
    while (node) {
        st.push(node);
        node = node->left;
    }
    const Node* prev = nullptr;
    node = nullptr;
    while (!st.empty()) {
        prev = node;
        node = st.top();
        st.pop();
        if (*node == value) {
            break;
        }
        if (node->right) {
            const auto* tmp = node->right;
            while (tmp) {
                st.push(tmp);
                tmp = tmp->left;
            }
        }
    }
    if (!prev) {
        return nullptr;
    }
    return new Node*(const_cast<Node*>(prev));
}

bool BST::delete_node(int value) {
    auto [parent, node] = find_parent_and_node(value, root);
    if (!node) {
        return false;
    }
    Node* new_node = nullptr;
    if (!node->left) {
        new_node = node->right;
    } else if (!node->right) {
        new_node = node->left;
    } else {
        auto prev_ptr = std::unique_ptr<Node*>(find_successor(value));
        auto [prev_parent, _] = find_parent_and_node((*prev_ptr)->value, node);
        if (prev_parent->left == *prev_ptr) {
            prev_parent->left = nullptr;
        } else {
            prev_parent->right = nullptr;
        }
        new_node = *prev_ptr;
        if (node->left != new_node) {
            new_node->left = node->left;
            new_node->right = node->right;
        }
    }
    if (!parent) {
        root = new_node;
    } else if (parent->left == node) {
        parent->left = new_node;
    } else {
        parent->right = new_node;
    }
    delete node;
    return true;
}

BST& BST::operator++() {
    bfs([](Node*& node) { node->value += 1; });
    return *this;
}

BST BST::operator++(int) {
    BST old = *this;
    operator++();
    return old;
}

BST& BST::operator=(const BST& other) {
    // Guard self assignment
    if (this == &other) {
        return *this;
    }
    bfs([](Node*& node) { delete node; });
    root = copy_bst(other.root);
    return *this;
}
BST& BST::operator=(BST&& other) {
    if (this == &other) {
        return *this;
    }
    bfs([](Node*& node) { delete node; });
    root = std::exchange(other.root, nullptr);
    return *this;
}

BST::BST(std::initializer_list<int> numbers) : BST() {
    for (const auto& value : numbers) {
        add_node(value);
    }
}