#pragma once 
#include <swizzle/ast/Node.hpp>
#include <swizzle/ast/nodes/EnumField.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/types/EnumValue.hpp>
#include <swizzle/types/utils/SetValue.hpp>

#include <memory>
#include <string>

namespace swizzle { namespace ast { namespace nodes {

    class Enum : public Node
    {
    public:
        Enum(const lexer::TokenInfo& keyword, const lexer::TokenInfo& name, const std::string& containingNamespace);

        const lexer::TokenInfo& keyword() const;
        const lexer::TokenInfo& nameDecl() const;

        std::string name() const;

        void underlying(const lexer::TokenInfo& value);
        const lexer::TokenInfo& underlying() const;

        // assign field value to @field, @token references where we're at in the parse
        void assign_enum_field_value(ast::nodes::EnumField& field, const lexer::TokenInfo& token);
        
        // set explicitly assigned values
        void set_value(const lexer::TokenInfo& token);
        void set_value(const types::utils::isHexTag&, const lexer::TokenInfo& token);
        void set_value_from_char(const lexer::TokenInfo& token);
        
    public:
        void accept(VisitorInterface& visitor, AncestorInfo& ancestors, const Depth depth = Depth::All) override;

    private:
        std::unique_ptr<types::EnumValueInterface> currentEnumValue_ = std::unique_ptr<types::EnumValueInterface>(new types::EnumValue<std::uint64_t>());

        const lexer::TokenInfo keyword_;
        const lexer::TokenInfo nameDecl_;

        lexer::TokenInfo underlyingType_;

        const std::string name_;
    };
}}}
