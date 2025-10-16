// value_impl.cpp
// Implementation of Value::to_string() after DSA types are complete

#include "simplic.hpp"
#include "dsa.hpp"

std::string Value::to_string() const {
    switch (type) {
        case Type::Nil: return "nil";
        case Type::Number: {
            std::ostringstream ss;
            ss << number_value;
            return ss.str();
        }
        case Type::Word: return word_value;
        case Type::YesNo: return yesno_value ? "yes" : "no";
        case Type::List: {
            std::string s = "[";
            bool first = true;
            for (auto &it : *list_value) {
                if (!first) s += ", ";
                s += it.to_string();
                first = false;
            }
            s += "]";
            return s;
        }
        case Type::Map: return "{map}";
        case Type::Function: return "<function>";
        case Type::Stack: return "<stack>";
        case Type::Queue: return "<queue>";
        case Type::LinkedList: return linklist_value ? linklist_value->to_string() : "<linklist>";
        case Type::Set: return set_value ? set_value->to_string() : "<set>";
        case Type::BST: return bst_value ? bst_value->to_string() : "<bst>";
        case Type::Graph: return graph_value ? graph_value->to_string() : "<graph>";
    }
    return "?";
}
