#include <swizzle/parser/SymbolTable.hpp>
#include <swizzle/Exceptions.hpp>

#include <utility>

namespace swizzle { namespace parser {

    void SymbolTable::insert(const std::string& symbol, const SymbolInfo& info)
    {
        symbols_.emplace(symbol, info);
    }
    
    void SymbolTable::insert(std::string&& symbol, SymbolInfo&& info)
    {
        symbols_.emplace(std::move(symbol), std::move(info));
    }
    
    bool SymbolTable::contains(const std::string& symbol) const
    {
        return symbols_.find(symbol) == symbols_.cend();
    }
    
    SymbolType SymbolTable::type(const std::string& symbol) const
    {
        const auto iter = symbols_.find(symbol);
        if(iter != symbols_.cend())
        {
            const auto& info = iter->second;
            return info.type();
        }
        
        throw SymbolNotFound(symbol);
    }
    
    ast::Node::smartptr SymbolTable::node(const std::string& symbol) const
    {
        const auto iter = symbols_.find(symbol);
        if(iter != symbols_.cend())
        {
            const auto& info = iter->second;
            return info.node();
        }
        
        throw SymbolNotFound(symbol);
    }
}}
