#pragma once
#include <swizzle/ast/Node.hpp>
#include <swizzle/parser/SymbolType.hpp>

#include <string>

namespace swizzle { namespace parser {

    // Information to be stored in the SymbolTable
    class SymbolInfo
    {
    public:
        // @symbol is fully qualified symbol, e.g. foo::bar::EnumType::EnumValue
        SymbolInfo(const std::string& symbol, const SymbolType type, ast::Node::smartptr node);
        
        const std::string& symbol() const { return symbol_; }
        SymbolType type() const { return type_; }
        
        ast::Node::smartptr node() const { return ast_node_; }
        
    private:
        std::string symbol_;
        SymbolType type_;
        
        ast::Node::smartptr ast_node_;
    };
}}
