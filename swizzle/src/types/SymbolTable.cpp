#include <swizzle/types/SymbolTable.hpp>
#include <swizzle/Exceptions.hpp>

#include <utility>

namespace swizzle { namespace types {

    void SymbolTable::insert(const std::string& symbol, const SymbolInfo& info)
    {
        symbols_.emplace(symbol, info);
    }
    
    bool SymbolTable::contains(const std::string& ns, const std::string& symbol) const
    {
        auto iter = symbols_.find(symbol);
        if(iter != symbols_.cend()) return true;
        
        const auto qualified_symbol = ns + "::" + symbol;
        return symbols_.find(qualified_symbol) != symbols_.cend();
    }

    const SymbolInfo& SymbolTable::find(const std::string& ns, const std::string& symbol, const SyntaxError& error) const
    {
        return find(ns, symbol, static_cast<const std::exception&>(error));
    }
    
    const SymbolInfo& SymbolTable::find(const std::string& ns, const std::string& symbol, const std::exception& error) const
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
