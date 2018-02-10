#pragma once
#include <swizzle/types/SymbolInfo.hpp>

#include <string>
#include <unordered_map>

namespace swizzle {
    class SyntaxError;
}

namespace swizzle { namespace types {

    class SymbolTable
    {
    public:
        void insert(const std::string& symbol, const SymbolInfo& info);
        bool contains(const std::string& ns, const std::string& symbol) const;
        
        const SymbolInfo& find(const std::string& ns, const std::string& symbol, const SyntaxError& error) const;
        SymbolInfo& find(const std::string& ns, const std::string& symbol, const SyntaxError& error);
        
    private:
        using SymbolContainer = std::unordered_map<std::string, SymbolInfo>;
        SymbolContainer symbols_;
    };
}}
