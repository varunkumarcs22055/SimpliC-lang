// SimpliC Language - Core Type System and Environment
#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>
#include <sstream>
#include <iostream>

// Forward declarations
struct Value;
struct Environment;
struct Stack;
struct Queue;
struct LinkedList;
struct SimpliSet;
struct SimpliMap;
struct BST;
struct Graph;

using Number = double;
using Word = std::string;
using YesNo = bool;
using List = std::vector<Value>;
using Map = std::unordered_map<std::string, Value>;

// Value type - can hold any SimpliC data type
struct Value {
    enum class Type {
        Nil, Number, Word, YesNo, List, Map, Function,
        Stack, Queue, LinkedList, Set, BST, Graph
    } type = Type::Nil;
    
    Number number_value = 0.0;
    Word word_value;
    YesNo yesno_value = false;
    std::shared_ptr<List> list_value;
    std::shared_ptr<Map> map_value;
    std::function<Value(std::vector<Value>)> func_value;
    std::shared_ptr<::Stack> stack_value;
    std::shared_ptr<::Queue> queue_value;
    std::shared_ptr<::LinkedList> linklist_value;
    std::shared_ptr<::SimpliSet> set_value;
    std::shared_ptr<::BST> bst_value;
    std::shared_ptr<::Graph> graph_value;

    Value() = default;
    static Value make_nil() { return Value(); }
    static Value from_number(Number n) { Value a; a.type = Type::Number; a.number_value = n; return a; }
    static Value from_word(const Word &s) { Value a; a.type = Type::Word; a.word_value = s; return a; }
    static Value from_yesno(bool b) { Value a; a.type = Type::YesNo; a.yesno_value = b; return a; }
    static Value from_list(const List &l) { Value a; a.type = Type::List; a.list_value = std::make_shared<List>(l); return a; }
    static Value from_map(const Map &m) { Value a; a.type = Type::Map; a.map_value = std::make_shared<Map>(m); return a; }
    static Value from_function(std::function<Value(std::vector<Value>)> f) { Value a; a.type = Type::Function; a.func_value = f; return a; }
    static Value from_stack(std::shared_ptr<::Stack> s) { Value a; a.type = Type::Stack; a.stack_value = s; return a; }
    static Value from_queue(std::shared_ptr<::Queue> q) { Value a; a.type = Type::Queue; a.queue_value = q; return a; }
    static Value from_linklist(std::shared_ptr<::LinkedList> ll) { Value a; a.type = Type::LinkedList; a.linklist_value = ll; return a; }
    static Value from_set(std::shared_ptr<::SimpliSet> st) { Value a; a.type = Type::Set; a.set_value = st; return a; }
    static Value from_bst(std::shared_ptr<::BST> bst) { Value a; a.type = Type::BST; a.bst_value = bst; return a; }
    static Value from_graph(std::shared_ptr<::Graph> g) { Value a; a.type = Type::Graph; a.graph_value = g; return a; }

    Number as_number() const { return number_value; }
    Word as_word() const { return word_value; }
    YesNo as_yesno() const { return yesno_value; }
    List &as_list() { return *list_value; }
    const List &as_list() const { return *list_value; }
    Map &as_map() { return *map_value; }

    std::string to_string() const;
};

// Environment - variable and function scope management
struct Environment {
    std::unordered_map<std::string, Value> vars;
    Environment *parent = nullptr;

    Environment(Environment *p = nullptr) : parent(p) {}

    void set(const std::string &name, const Value &val) {
        vars[name] = val;
    }

    bool exists_in_current(const std::string &name) const {
        return vars.find(name) != vars.end();
    }

    bool get(const std::string &name, Value &out) const {
        auto it = vars.find(name);
        if (it != vars.end()) {
            out = it->second;
            return true;
        }
        if (parent) return parent->get(name, out);
        return false;
    }
};

// Interpreter function declarations
std::vector<std::vector<std::string>> lex_source(const std::string &source);
void run_lines(const std::vector<std::vector<std::string>> &lines, Environment &env, size_t &i, Value *return_value);
