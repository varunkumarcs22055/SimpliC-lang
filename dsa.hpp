// SimpliC Language - Data Structure Library
#pragma once

#include <vector>
#include <stack>
#include <queue>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <sstream>

struct Value;

// Stack - LIFO container
struct Stack {
    std::stack<Value> data;
    
    void push(const Value &v);
    Value pop();
    Value top() const;
    size_t size() const { return data.size(); }
    bool empty() const { return data.empty(); }
};

// Queue - FIFO container
struct Queue {
    std::queue<Value> data;
    
    void push(const Value &v);
    Value pop();
    Value front() const;
    size_t size() const { return data.size(); }
    bool empty() const { return data.empty(); }
};

// Linked List node
struct LLNode {
    double val;
    LLNode *next = nullptr;
    LLNode(double v) : val(v), next(nullptr) {}
};

struct LinkedList {
    LLNode *head = nullptr;
    
    ~LinkedList() {
        while (head) {
            LLNode *tmp = head;
            head = head->next;
            delete tmp;
        }
    }
    
    void insert(double v);
    std::string to_string() const;
};

// Set wrapper
struct SimpliSet {
    std::set<double> nums; // simplified: only numbers
    std::set<std::string> words;
    
    void add(const Value &v);
    bool contains(const Value &v) const;
    size_t size() const { return nums.size() + words.size(); }
    std::string to_string() const;
};

// Map wrapper (string keys, store double for simplicity)
struct SimpliMap {
    std::unordered_map<std::string, double> data;
    
    void set(const std::string &key, double val);
    double get(const std::string &key) const;
    size_t size() const { return data.size(); }
    std::string to_string() const;
};

// BST Node
struct BSTNode {
    double val;
    BSTNode *left = nullptr;
    BSTNode *right = nullptr;
    BSTNode(double v) : val(v) {}
};

struct BST {
    BSTNode *root = nullptr;
    
    ~BST() { clear(root); }
    
    void clear(BSTNode *node) {
        if (!node) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }
    
    void insert(double val);
    void insert_helper(BSTNode *&node, double val);
    void inorder(BSTNode *node, std::vector<double> &out) const;
    std::vector<double> inorder() const;
    std::string to_string() const;
};

// Graph (adjacency list, unweighted)
struct Graph {
    std::unordered_map<int, std::vector<int>> adj;
    
    void addNode(int node);
    void addEdge(int u, int v);
    std::vector<int> dfs(int start) const;
    void dfs_helper(int node, std::unordered_set<int> &visited, std::vector<int> &result) const;
    std::string to_string() const;
};
