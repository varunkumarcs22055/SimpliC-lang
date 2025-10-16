// SimpliC Interpreter - Lexer, Parser, and Execution Engine
#include "simplic.hpp"
#include "dsa.hpp"
#include <vector>
#include <string>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <cmath>

using Token = std::string;

// Forward declarations
std::vector<std::vector<std::string>> lex_source(const std::string &source);
void run_lines(const std::vector<std::vector<std::string>> &lines, Environment &env, size_t &i, Value *return_value = nullptr);

// Tokenize SimpliC source code into lines of tokens
std::vector<std::vector<std::string>> lex_source(const std::string &source) {
    std::vector<std::vector<std::string>> lines;
    std::istringstream iss(source);
    std::string line;
    
    while (std::getline(iss, line)) {
        std::vector<std::string> tokens;
        std::string token;
        bool in_string = false;
        
        for (size_t i = 0; i < line.size(); ++i) {
            char c = line[i];
            if (in_string) {
                token += c;
                if (c == '"') {
                    in_string = false;
                    tokens.push_back(token);
                    token.clear();
                }
            } else if (c == '"') {
                if (!token.empty()) {
                    tokens.push_back(token);
                    token.clear();
                }
                token += c;
                in_string = true;
            } else if (isspace(c)) {
                if (!token.empty()) {
                    tokens.push_back(token);
                    token.clear();
                }
            } else if (c == '(' || c == ')' || c == '[' || c == ']' || c == ',' || c == ';' || c == '=' || c == '.') {
                if (!token.empty()) {
                    tokens.push_back(token);
                    token.clear();
                }
                tokens.push_back(std::string(1, c));
            } else {
                token += c;
            }
        }
        if (!token.empty()) tokens.push_back(token);
        lines.push_back(tokens);
    }
    return lines;
}

// Helper to check if string is a number
static bool is_num(const std::string &s) {
    if (s.empty()) return false;
    char *end = nullptr;
    strtod(s.c_str(), &end);
    return *end == '\0';
}

// Evaluate a single token
static Value eval_token(const std::string &t, Environment &env) {
    if (t.size() >= 2 && t.front() == '"' && t.back() == '"') {
        return Value::from_word(t.substr(1, t.size()-2));
    }
    if (t == "yes") return Value::from_yesno(true);
    if (t == "no") return Value::from_yesno(false);
    if (is_num(t)) return Value::from_number(std::stod(t));
    
    Value v;
    if (env.get(t, v)) return v;
    return Value::make_nil();
}

// Binary operations
static Value eval_binary(const Value &a, const std::string &op, const Value &b) {
    if (a.type == Value::Type::Number && b.type == Value::Type::Number) {
        double x = a.as_number(), y = b.as_number();
        if (op == "plus") return Value::from_number(x + y);
        if (op == "minus") return Value::from_number(x - y);
        if (op == "times") return Value::from_number(x * y);
        if (op == "divide") return Value::from_number(x / y);
        if (op == "mod") return Value::from_number(std::fmod(x, y));
        if (op == "morethan") return Value::from_yesno(x > y);
        if (op == "lessthan") return Value::from_yesno(x < y);
        if (op == "equals") return Value::from_yesno(x == y);
        if (op == "notequal") return Value::from_yesno(x != y);
    }
    if (op == "equals" && a.type == Value::Type::Word && b.type == Value::Type::Word) {
        return Value::from_yesno(a.as_word() == b.as_word());
    }
    return Value::make_nil();
}

// Parse and evaluate expression (handles: token | left op right | func(arg) | complex)
static Value eval_expr(const std::vector<std::string> &tokens, size_t start, size_t end, Environment &env) {
    if (start >= end) return Value::make_nil();
    if (end - start == 1) return eval_token(tokens[start], env);
    
    // Check for function call at start: fname(...)
    if (end > start+2 && tokens[start+1] == "(") {
        std::string fname = tokens[start];
        // Find matching )
        size_t j = start+2, depth = 1;
        while (j < end && depth > 0) {
            if (tokens[j] == "(") ++depth;
            if (tokens[j] == ")") --depth;
            ++j;
        }
        Value arg = eval_expr(tokens, start+2, j-1, env);
        Value fn;
        if (env.get(fname, fn) && fn.type == Value::Type::Function) {
            Value result = fn.func_value({arg});
            // If there's more after the function call, continue evaluating
            if (j < end) {
                // We have: result <op> <rest>
                if (j+1 < end) {
                    std::string op = tokens[j];
                    Value right = eval_expr(tokens, j+1, end, env);
                    return eval_binary(result, op, right);
                }
            }
            return result;
        }
    }
    
    // Simple binary: left op right
    if (end - start == 3) {
        Value l = eval_token(tokens[start], env);
        std::string op = tokens[start+1];
        Value r = eval_token(tokens[start+2], env);
        return eval_binary(l, op, r);
    }
    
    // Complex expression: evaluate left-to-right
    // Find first operator and split
    for (size_t i = start; i < end; ++i) {
        if (tokens[i] == "plus" || tokens[i] == "minus" || tokens[i] == "times" || 
            tokens[i] == "divide" || tokens[i] == "mod") {
            Value left = eval_expr(tokens, start, i, env);
            Value right = eval_expr(tokens, i+1, end, env);
            return eval_binary(left, tokens[i], right);
        }
    }
    
    return eval_token(tokens[start], env);
}

// Handle method calls like nums.sort(), s.top, tree.insert(10), tree.inorder(), g.dfs(1)
static Value handle_method_call(Value &obj, const std::string &method, const std::vector<std::string> &args, Environment &env) {
    if (obj.type == Value::Type::List && method == "sort") {
        std::sort(obj.as_list().begin(), obj.as_list().end(), [](const Value &a, const Value &b) {
            if (a.type == Value::Type::Number && b.type == Value::Type::Number) return a.as_number() < b.as_number();
            return false;
        });
        return Value::make_nil();
    }
    if (obj.type == Value::Type::Stack && method == "top") {
        return obj.stack_value->top();
    }
    if (obj.type == Value::Type::Queue && method == "front") {
        return obj.queue_value->front();
    }
    if (obj.type == Value::Type::LinkedList && method == "insert" && args.size() == 1) {
        Value arg = eval_token(args[0], env);
        if (arg.type == Value::Type::Number) obj.linklist_value->insert(arg.as_number());
        return Value::make_nil();
    }
    if (obj.type == Value::Type::Set && method == "add" && args.size() == 1) {
        Value arg = eval_token(args[0], env);
        obj.set_value->add(arg);
        return Value::make_nil();
    }
    if (obj.type == Value::Type::BST && method == "insert" && args.size() == 1) {
        Value arg = eval_token(args[0], env);
        if (arg.type == Value::Type::Number) obj.bst_value->insert(arg.as_number());
        return Value::make_nil();
    }
    if (obj.type == Value::Type::BST && method == "inorder") {
        auto vals = obj.bst_value->inorder();
        List result;
        for (auto v : vals) result.push_back(Value::from_number(v));
        return Value::from_list(result);
    }
    if (obj.type == Value::Type::Graph && method == "addNode" && args.size() == 1) {
        Value arg = eval_token(args[0], env);
        if (arg.type == Value::Type::Number) obj.graph_value->addNode((int)arg.as_number());
        return Value::make_nil();
    }
    if (obj.type == Value::Type::Graph && method == "addEdge" && args.size() == 2) {
        Value arg1 = eval_token(args[0], env);
        Value arg2 = eval_token(args[1], env);
        if (arg1.type == Value::Type::Number && arg2.type == Value::Type::Number) {
            obj.graph_value->addEdge((int)arg1.as_number(), (int)arg2.as_number());
        }
        return Value::make_nil();
    }
    if (obj.type == Value::Type::Graph && method == "dfs" && args.size() == 1) {
        Value arg = eval_token(args[0], env);
        if (arg.type == Value::Type::Number) {
            auto path = obj.graph_value->dfs((int)arg.as_number());
            List result;
            for (auto n : path) result.push_back(Value::from_number(n));
            return Value::from_list(result);
        }
    }
    return Value::make_nil();
}

// Main line interpreter with return support
void run_lines(const std::vector<std::vector<std::string>> &lines, Environment &env, size_t &i, Value *return_value) {
    size_t n = lines.size();
    while (i < n) {
        if (return_value && return_value->type != Value::Type::Nil) return; // early return
        
        auto &toks = lines[i];
        if (toks.empty()) { ++i; continue; }

        // skip "start program" and "end program"
        if (toks[0] == "start" || toks[0] == "end") { ++i; continue; }

        // make <type> <name> [= value]
        if (toks[0] == "make") {
            if (toks.size() >= 2) {
                std::string dtype = toks[1];
                std::string name = (toks.size() >= 3) ? toks[2] : "";
                
                // Check for shorthand syntax: make x is 10
                auto ispos = std::find(toks.begin(), toks.end(), std::string("is"));
                if (ispos != toks.end() && ispos == toks.begin() + 2) {
                    // Shorthand: make varname is value
                    name = toks[1];
                    size_t start = 3; // After "is"
                    auto val = eval_expr(toks, start, toks.size(), env);
                    env.set(name, val);
                    ++i; continue;
                }
                
                if (dtype == "number" || dtype == "word" || dtype == "yesno") {
                    auto eqpos = std::find(toks.begin(), toks.end(), std::string("="));
                    if (eqpos != toks.end()) {
                        size_t start = (eqpos - toks.begin()) + 1;
                        auto val = eval_expr(toks, start, toks.size(), env);
                        env.set(name, val);
                    } else {
                        env.set(name, Value::make_nil());
                    }
                } else if (dtype == "list") {
                    auto itl = std::find(toks.begin(), toks.end(), std::string("["));
                    auto itr = std::find(toks.begin(), toks.end(), std::string("]"));
                    List L;
                    if (itl != toks.end() && itr != toks.end() && itr > itl) {
                        for (auto it = itl+1; it != itr; ++it) {
                            if (*it == ",") continue;
                            L.push_back(eval_token(*it, env));
                        }
                    }
                    env.set(name, Value::from_list(L));
                } else if (dtype == "stack") {
                    env.set(name, Value::from_stack(std::make_shared<Stack>()));
                } else if (dtype == "queue") {
                    env.set(name, Value::from_queue(std::make_shared<Queue>()));
                } else if (dtype == "linkedlist") {
                    env.set(name, Value::from_linklist(std::make_shared<LinkedList>()));
                } else if (dtype == "map") {
                    env.set(name, Value::from_map(Map()));
                } else if (dtype == "set") {
                    env.set(name, Value::from_set(std::make_shared<SimpliSet>()));
                } else if (dtype == "bst") {
                    env.set(name, Value::from_bst(std::make_shared<BST>()));
                } else if (dtype == "graph") {
                    env.set(name, Value::from_graph(std::make_shared<Graph>()));
                }
            }
            ++i; continue;
        }

        // show <values...>
        if (toks[0] == "show") {
            for (size_t j=1; j<toks.size();) {
                // Skip semicolons and commas
                if (toks[j] == ";" || toks[j] == ",") { 
                    if (toks[j] == ",") std::cout << " "; 
                    ++j; 
                    continue; 
                }
                
                // Check for map[key] access
                if (j+3 < toks.size() && toks[j+1] == "[" && toks[j+3] == "]") {
                    std::string mapname = toks[j];
                    std::string key = toks[j+2];
                    // Remove quotes from key if present
                    if (key.size() >= 2 && key.front() == '"' && key.back() == '"') {
                        key = key.substr(1, key.size()-2);
                    }
                    Value mapobj;
                    if (env.get(mapname, mapobj) && mapobj.type == Value::Type::Map) {
                        auto &m = mapobj.as_map();
                        if (m.find(key) != m.end()) {
                            std::cout << m[key].to_string();
                        } else {
                            std::cout << "nil";
                        }
                    } else {
                        std::cout << "nil";
                    }
                    j += 4;  // Skip past map[key]
                    if (j < toks.size() && toks[j] != ";") std::cout << " ";
                    continue;
                }
                
                // Check for obj.method() or obj.property
                if (j+2 < toks.size() && toks[j+1] == ".") {
                    std::string objname = toks[j];
                    std::string method = toks[j+2];
                    Value obj;
                    if (env.get(objname, obj)) {
                        std::vector<std::string> args;
                        size_t k = j+3;
                        if (k < toks.size() && toks[k] == "(") {
                            ++k;
                            while (k < toks.size() && toks[k] != ")") {
                                if (toks[k] != ",") args.push_back(toks[k]);
                                ++k;
                            }
                            ++k; // skip )
                        }
                        Value result = handle_method_call(obj, method, args, env);
                        env.set(objname, obj);  // Save modified object back to environment
                        std::cout << result.to_string();
                        j = k;
                    } else {
                        ++j;
                    }
                } else {
                    Value v = eval_token(toks[j], env);
                    std::cout << v.to_string();
                    ++j;
                }
                // Only add space if next token is not semicolon or end
                if (j < toks.size() && toks[j] != ";") std::cout << " ";
            }
            std::cout << std::endl;
            ++i; continue;
        }

        // put <value> into <container>
        if (toks[0] == "put" && toks.size() >= 4 && toks[2] == "into") {
            Value val = eval_token(toks[1], env);
            std::string cname = toks[3];
            Value container;
            if (env.get(cname, container)) {
                if (container.type == Value::Type::Stack) {
                    container.stack_value->push(val);
                    env.set(cname, container);  // Save back to environment
                } else if (container.type == Value::Type::Queue) {
                    container.queue_value->push(val);
                    env.set(cname, container);  // Save back to environment
                }
            }
            ++i; continue;
        }

        // takeout <container>
        if (toks[0] == "takeout" && toks.size() >= 2) {
            std::string cname = toks[1];
            Value container;
            if (env.get(cname, container)) {
                if (container.type == Value::Type::Stack) {
                    container.stack_value->pop();
                    env.set(cname, container);  // Save back to environment
                } else if (container.type == Value::Type::Queue) {
                    container.queue_value->pop();
                    env.set(cname, container);  // Save back to environment
                }
            }
            ++i; continue;
        }

        // when <condition> ... otherwise ... done
        if (toks[0] == "when") {
            Value cond = eval_expr(toks, 1, toks.size(), env);
            bool take = (cond.type == Value::Type::YesNo && cond.as_yesno());
            
            size_t j = i+1;
            std::vector<std::vector<std::string>> trueblock, elseblock;
            bool in_else = false;
            while (j < n) {
                if (!lines[j].empty() && lines[j][0] == "done") break;
                if (!lines[j].empty() && lines[j][0] == "otherwise") { in_else = true; ++j; continue; }
                if (in_else) elseblock.push_back(lines[j]);
                else trueblock.push_back(lines[j]);
                ++j;
            }
            
            size_t sub_i = 0;
            if (take) run_lines(trueblock, env, sub_i, return_value);
            else run_lines(elseblock, env, sub_i, return_value);
            
            i = j + 1;
            continue;
        }

        // for <var> in <container> ... done  OR  for each <var> in <container> ... done
        if (toks[0] == "for" && toks.size() >= 4) {
            std::string varname, cname;
            
            // Check for "for each item in list" syntax
            if (toks.size() >= 5 && toks[1] == "each" && toks[3] == "in") {
                varname = toks[2];
                cname = toks[4];
            }
            // Check for "for item in list" syntax
            else if (toks.size() >= 4 && toks[2] == "in") {
                varname = toks[1];
                cname = toks[3];
            } else {
                ++i; continue;
            }
            
            Value container;
            if (env.get(cname, container)) {
                size_t j = i+1;
                std::vector<std::vector<std::string>> body;
                int depth = 1;
                while (j < n && depth > 0) {
                    const auto &line = lines[j];
                    if (!line.empty() && (line[0] == "when" || line[0] == "for" || line[0] == "define")) {
                        depth++;
                    } else if (line.size() == 1 && line[0] == "done") {
                        depth--;
                        if (depth == 0) break;
                    }
                    body.push_back(line);
                    ++j;
                }
                
                if (container.type == Value::Type::List) {
                    for (auto &elem : container.as_list()) {
                        env.set(varname, elem);
                        size_t sub_i = 0;
                        run_lines(body, env, sub_i, return_value);
                        if (return_value && return_value->type != Value::Type::Nil) break;
                    }
                }
                i = j + 1;
            } else {
                ++i;
            }
            continue;
        }

        // define <name> with <arg> ... done
        if (toks[0] == "define" && toks.size() >= 2) {
            std::string fname = toks[1];
            std::string argname = "";
            // Check for "with <argname>" pattern
            if (toks.size() >= 4 && toks[2] == "with") {
                argname = toks[3];
            }
            
            size_t j = i+1;
            std::vector<std::vector<std::string>> body;
            int depth = 1;  // Track nesting depth
            while (j < n && depth > 0) {
                const auto &line = lines[j];
                // Check if this line opens a new block (when, for, define)
                if (!line.empty() && (line[0] == "when" || line[0] == "for" || line[0] == "define")) {
                    depth++;
                } else if (line.size() == 1 && line[0] == "done") {
                    depth--;
                    if (depth == 0) break;  // This is the matching done
                }
                body.push_back(line);
                ++j;
            }
            
            auto func = [body, argname, &env](std::vector<Value> args) -> Value {
                Environment local_env(&env);  // Set parent to current env
                if (!argname.empty() && !args.empty()) {
                    local_env.set(argname, args[0]);
                }
                
                Value ret_val = Value::make_nil();
                size_t sub_i = 0;
                run_lines(body, local_env, sub_i, &ret_val);
                return ret_val;
            };
            
            env.set(fname, Value::from_function(func));
            i = j + 1;
            continue;
        }

        // giveback <expr>
        if (toks[0] == "giveback" && return_value) {
            *return_value = eval_expr(toks, 1, toks.size(), env);
            return;
        }

        // Handle method calls: obj.method() or obj.method(args)
        if (toks.size() >= 3 && toks[1] == ".") {
            std::string objname = toks[0];
            std::string method = toks[2];
            Value obj;
            if (env.get(objname, obj)) {
                // Parse args if present: method(arg1, arg2)
                std::vector<std::string> args;
                if (toks.size() > 3 && toks[3] == "(") {
                    for (size_t k = 4; k < toks.size(); ++k) {
                        if (toks[k] == ")" || toks[k] == ",") continue;
                        args.push_back(toks[k]);
                    }
                }
                handle_method_call(obj, method, args, env);
                env.set(objname, obj);  // Save modified object back to environment
            }
            ++i; continue;
        }

        // Handle map subscript assignment: mp["key"] = value
        if (toks.size() >= 5 && toks[1] == "[" && toks[3] == "]" && toks[4] == "=") {
            std::string mapname = toks[0];
            std::string key = toks[2];
            if (key.size() >= 2 && key.front() == '"' && key.back() == '"') {
                key = key.substr(1, key.size()-2);
            }
            Value mapobj;
            if (env.get(mapname, mapobj) && mapobj.type == Value::Type::Map) {
                Value val = eval_expr(toks, 5, toks.size(), env);
                mapobj.as_map()[key] = val;
                env.set(mapname, mapobj);  // Save modified map back
            }
            ++i; continue;
        }

        ++i;
    }
}

// Public entry point
void run_source(const std::string &source) {
    Environment root(nullptr);
    auto lines = lex_source(source);
    size_t i = 0;
    Value ret = Value::make_nil();
    run_lines(lines, root, i, &ret);
    // Do not print anything here; output is handled in main.cpp
}
