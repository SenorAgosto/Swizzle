#include <swizzle/ast/nodes/Enum.hpp>

#include <swizzle/ast/AncestorInfo.hpp>
#include <swizzle/ast/VisitorInterface.hpp>
#include <swizzle/types/utils/SetValueFromChar.hpp>

namespace swizzle { namespace ast { namespace nodes {

    Enum::Enum(const lexer::TokenInfo& keyword, const lexer::TokenInfo& name, const std::string& containingNamespace)
        : keyword_(keyword)
        , nameDecl_(name)
        , name_(containingNamespace + "::" + nameDecl_.token().to_string())
    {
    }

    const lexer::TokenInfo& Enum::keyword() const
    {
        return keyword_;
    }

    const lexer::TokenInfo& Enum::nameDecl() const
    {
        return nameDecl_;
    }

    std::string Enum::name() const
    {
        return name_;
    }

    void Enum::underlying(const lexer::TokenInfo& value)
    {
        underlyingType_ = value;
        const auto underlying = underlyingType_.token().value();
        
        if(underlying == "u8")
        {
            currentEnumValue_ = std::unique_ptr<types::EnumValueInterface>(new types::EnumValue<std::uint8_t>());
            return;
        }

        if(underlying == "u16")
        {
            currentEnumValue_ = std::unique_ptr<types::EnumValueInterface>(new types::EnumValue<std::uint16_t>());
            return;
        }

        if(underlying == "u32")
        {
            currentEnumValue_ = std::unique_ptr<types::EnumValueInterface>(new types::EnumValue<std::uint32_t>());
            return;
        }

        if(underlying == "u64")
        {
            currentEnumValue_ = std::unique_ptr<types::EnumValueInterface>(new types::EnumValue<std::uint64_t>());
            return;
        }

        if(underlying == "i8")
        {
            currentEnumValue_ = std::unique_ptr<types::EnumValueInterface>(new types::EnumValue<std::int8_t>());
            return;
        }

        if(underlying == "i16")
        {
            currentEnumValue_ = std::unique_ptr<types::EnumValueInterface>(new types::EnumValue<std::int16_t>());
            return;
        }

        if(underlying == "i32")
        {
            currentEnumValue_ = std::unique_ptr<types::EnumValueInterface>(new types::EnumValue<std::int32_t>());
            return;
        }

        if(underlying == "i64")
        {
            currentEnumValue_ = std::unique_ptr<types::EnumValueInterface>(new types::EnumValue<std::int64_t>());
            return;
        }
        
        throw SyntaxError("Expecting integer enum underyling type", value);
    }

    const lexer::TokenInfo& Enum::underlying() const
    {
        return underlyingType_;
    }

    void Enum::assign_enum_field_value(ast::nodes::EnumField& field, const lexer::TokenInfo& token)
    {
        field.value(currentEnumValue_->assign_field_value(token));
        currentEnumValue_->increment();
    }

    void Enum::set_value(const lexer::TokenInfo& token)
    {
        currentEnumValue_->value(types::utils::setValue(underlyingType_.token().value(), token, "Encountered unknown enum type"));
    }
    
    void Enum::set_value(const types::utils::isHexTag&, const lexer::TokenInfo& token)
    {
        currentEnumValue_->value(types::utils::setValue(underlyingType_.token().value(), token, types::utils::isHex, "Encountered unknown enum type"));
    }

    void Enum::set_value_from_char(const lexer::TokenInfo& token)
    {
        currentEnumValue_->value(types::utils::setValueFromChar(underlyingType_.token().value(), token));
    }
    
    // virtual
    void Enum::accept(VisitorInterface& visitor, AncestorInfo& ancestors, const Node::Depth depth)
    {
        visitor(ancestors, *this);
        
        if(depth == Depth::One) return;
        ancestors.push(*this);
        
        for(auto& child : children())
        {
            child->accept(visitor, ancestors, depth);
        }
        
        ancestors.pop();
    }
}}}
