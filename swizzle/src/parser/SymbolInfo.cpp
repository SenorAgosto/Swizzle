#include <swizzle/parser/SymbolInfo.hpp>

namespace swizzle { namespace parser {

    SymbolInfo::SymbolInfo(const std::string& symbol, const SymbolType type, ast::Node::smartptr node)
        : symbol_(symbol)
        , type_(type)
        , ast_node_(node)
    {
    }
}}
