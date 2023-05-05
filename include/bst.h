#ifndef BST_H
#define BST_H

#include <compare>
#include <functional>
#include <iostream>

class BST {
   public:
    class Node;
    BST();
    BST(const BST& bst);
    BST(BST&& bst);

    BST& operator++();
    BST operator++(int);
    ~BST();
    BST& operator=(const BST& other);
    BST& operator=(BST&& other);

    Node*& get_root();
    void bfs(std::function<void(Node*& node)> func);  // const node or not??
    size_t length() const;
    bool add_node(int value);
    Node** find_node(int value);
    Node** find_parrent(int value);
    Node** find_successor(int value);
    bool delete_node(int value);

    friend std::ostream& operator<<(std::ostream& os, const BST& obj);

   private:
    Node* root;
};

class BST::Node {
   public:
    Node(int value, Node* left = nullptr, Node* right = nullptr);
    Node();
    Node(const Node& node);

    int value;
    Node* left;
    Node* right;
    std::strong_ordering operator<=>(const int& other_val) const;
    friend bool operator==(const Node& lhs, const Node& rhs);
    friend std::ostream& operator<<(std::ostream& os, const Node& obj);
};

#endif  // BST_H