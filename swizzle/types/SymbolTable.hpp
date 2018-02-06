#pragma once
#include <swizzle/parser/SymbolInfo.hpp>

#include <string>
#include <unordered_map>

namespace swizzle { namespace parser {

    class SymbolTable
    {
    public:
        void insert(const std::string& symbol, const SymbolInfo& info);
        void insert(std::string&& symbol, SymbolInfo&& info);
        
        bool contains(const std::string& symbol) const;
        
        SymbolType type(const std::string& symbol) const;
        ast::Node::smartptr node(const std::string& symbol) const;
    
    private:
        using SymbolContainer = std::unordered_map<std::string, SymbolInfo>;
        SymbolContainer symbols_;
    };
}}
