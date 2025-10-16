// dsa.cpp
// Implementation of DSA structures

#include "dsa.hpp"
#include "simplic.hpp"

// Stack
void Stack::push(const Value &v) {
    data.push(v);
}

Value Stack::pop() {
    if (data.empty()) return Value::make_nil();
    Value v = data.top();
    data.pop();
    return v;
}

Value Stack::top() const {
    if (data.empty()) return Value::make_nil();
    return data.top();
}

// Queue
void Queue::push(const Value &v) {
    data.push(v);
}

Value Queue::pop() {
    if (data.empty()) return Value::make_nil();
    Value v = data.front();
    data.pop();
    return v;
}

Value Queue::front() const {
    if (data.empty()) return Value::make_nil();
    return data.front();
}

// LinkedList
void LinkedList::insert(double v) {
    LLNode *newNode = new LLNode(v);
    if (!head) {
        head = newNode;
    } else {
        LLNode *curr = head;
        while (curr->next) curr = curr->next;
        curr->next = newNode;
    }
}

std::string LinkedList::to_string() const {
    std::ostringstream ss;
    ss << "[";
    LLNode *curr = head;
    bool first = true;
    while (curr) {
        if (!first) ss << " -> ";
        ss << curr->val;
        curr = curr->next;
        first = false;
    }
    ss << "]";
    return ss.str();
}

// Set
void SimpliSet::add(const Value &v) {
    if (v.type == Value::Type::Number) {
        nums.insert(v.as_number());
    } else if (v.type == Value::Type::Word) {
        words.insert(v.as_word());
    }
}

bool SimpliSet::contains(const Value &v) const {
    if (v.type == Value::Type::Number) {
        return nums.find(v.as_number()) != nums.end();
    } else if (v.type == Value::Type::Word) {
        return words.find(v.as_word()) != words.end();
    }
    return false;
}

std::string SimpliSet::to_string() const {
    std::ostringstream ss;
    ss << "{";
    bool first = true;
    for (auto n : nums) {
        if (!first) ss << ", ";
        ss << n;
        first = false;
    }
    for (auto &w : words) {
        if (!first) ss << ", ";
        ss << w;
        first = false;
    }
    ss << "}";
    return ss.str();
}

// Map
void SimpliMap::set(const std::string &key, double val) {
    data[key] = val;
}

double SimpliMap::get(const std::string &key) const {
    auto it = data.find(key);
    if (it != data.end()) return it->second;
    return 0.0;
}

std::string SimpliMap::to_string() const {
    std::ostringstream ss;
    ss << "{";
    bool first = true;
    for (auto &kv : data) {
        if (!first) ss << ", ";
        ss << kv.first << ": " << kv.second;
        first = false;
    }
    ss << "}";
    return ss.str();
}

// BST
void BST::insert(double val) {
    insert_helper(root, val);
}

void BST::insert_helper(BSTNode *&node, double val) {
    if (!node) {
        node = new BSTNode(val);
        return;
    }
    if (val < node->val) {
        insert_helper(node->left, val);
    } else {
        insert_helper(node->right, val);
    }
}

void BST::inorder(BSTNode *node, std::vector<double> &out) const {
    if (!node) return;
    inorder(node->left, out);
    out.push_back(node->val);
    inorder(node->right, out);
}

std::vector<double> BST::inorder() const {
    std::vector<double> result;
    inorder(root, result);
    return result;
}

std::string BST::to_string() const {
    auto vals = inorder();
    std::ostringstream ss;
    ss << "[";
    for (size_t i = 0; i < vals.size(); ++i) {
        if (i > 0) ss << ", ";
        ss << vals[i];
    }
    ss << "]";
    return ss.str();
}

// Graph
void Graph::addNode(int node) {
    if (adj.find(node) == adj.end()) {
        adj[node] = std::vector<int>();
    }
}

void Graph::addEdge(int u, int v) {
    addNode(u);
    addNode(v);
    adj[u].push_back(v);
}

void Graph::dfs_helper(int node, std::unordered_set<int> &visited, std::vector<int> &result) const {
    visited.insert(node);
    result.push_back(node);
    auto it = adj.find(node);
    if (it != adj.end()) {
        for (int neighbor : it->second) {
            if (visited.find(neighbor) == visited.end()) {
                dfs_helper(neighbor, visited, result);
            }
        }
    }
}

std::vector<int> Graph::dfs(int start) const {
    std::unordered_set<int> visited;
    std::vector<int> result;
    if (adj.find(start) != adj.end()) {
        dfs_helper(start, visited, result);
    }
    return result;
}

std::string Graph::to_string() const {
    std::ostringstream ss;
    ss << "Graph{";
    bool first = true;
    for (auto &kv : adj) {
        if (!first) ss << ", ";
        ss << kv.first << ": [";
        for (size_t i = 0; i < kv.second.size(); ++i) {
            if (i > 0) ss << ", ";
            ss << kv.second[i];
        }
        ss << "]";
        first = false;
    }
    ss << "}";
    return ss.str();
}
