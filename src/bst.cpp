#include "bst.h"

BST::Node::Node(int value, Node* left, Node* right)
    : value(value), left(left), right(right) {}

BST::Node::Node() : value(0), left(nullptr), right(nullptr) {}

BST::Node::Node(const Node& node)
    : value(node.value), left(node.left), right(node.right) {}