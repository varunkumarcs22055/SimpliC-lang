// SimpliC Interpreter - Main Entry Point
#include "simplic.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

void run_source(const std::string &source);

int main(int argc, char **argv) {
    if (argc >= 2) {
        std::ifstream f(argv[1]);
        if (!f) {
            std::cerr << "Could not open file: " << argv[1] << std::endl;
            return 1;
        }
        
        std::ostringstream ss;
        ss << f.rdbuf();
        
        Environment root(nullptr);
        auto lines = lex_source(ss.str());
        size_t i = 0;
        Value ret = Value::make_nil();
        
        bool has_giveback = false;
        for (const auto& line : lines) {
            if (!line.empty() && line[0] == "giveback") {
                has_giveback = true;
                break;
            }
        }
        
        run_lines(lines, root, i, &ret);
        
        if (has_giveback && ret.type != Value::Type::Nil) {
            std::cout << ret.to_string() << std::endl;
        }
        return 0;
    }

    std::cout << "SimpliC REPL. Type 'exit' or 'quit' to leave." << std::endl;
    std::string line, source;
    
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) break;
        if (line == "exit" || line == "quit") break;
        
        source += line + "\n";
        
        if (line.empty()) {
            Environment root(nullptr);
            auto lines = lex_source(source);
            size_t i = 0;
            Value ret = Value::make_nil();
            
            run_lines(lines, root, i, &ret);
            
            if (ret.type != Value::Type::Nil) {
                std::cout << ret.to_string() << std::endl;
            }
            source.clear();
        }
    }
    return 0;
}
