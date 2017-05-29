#pragma once 
#include <swizzle/ast/Node.hpp>
#include <swizzle/lexer/TokenInfo.hpp>

#include <boost/utility/string_view.hpp>

namespace swizzle { namespace ast { namespace nodes {

    class Enum : public Node
    {
    public:
        Enum(const lexer::TokenInfo& info, const std::string& containingNamespace);

        const lexer::TokenInfo& info() const;
        const lexer::TokenInfo& underlyingTypeInfo() const;

        std::string name() const;

        void underlying(const boost::string_view& value);
        const boost::string_view& underlying() const;

    private:
        const lexer::TokenInfo info_;
        lexer::TokenInfo underlyingInfo_;

        const std::string name_;
        boost::string_view underlyingType_;
    };
}}}
