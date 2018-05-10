#pragma once
#include <swizzle/types/SymbolInfo.hpp>

#include <exception>
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
        const SymbolInfo& find(const std::string& ns, const std::string& symbol, const std::exception& error) const;
        
        template<typename Exception>
        SymbolInfo& find(const std::string& ns, const std::string& symbol, const Exception& error);
        
    private:
        using SymbolContainer = std::unordered_map<std::string, SymbolInfo>;
        SymbolContainer symbols_;
    };

    template<typename Exception>
    SymbolInfo& SymbolTable::find(const std::string& ns, const std::string& symbol, const Exception& error)
    {
        auto iter = symbols_.find(symbol);
        if(iter != symbols_.cend())
        {
            return iter->second;
        }
        
        const auto qualified_symbol = ns + "::" + symbol;

        iter = symbols_.find(qualified_symbol);
        if(iter != symbols_.cend())
        {
            return iter->second;
        }
        
        throw error;
    }
}}
