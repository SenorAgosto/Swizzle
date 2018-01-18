#include <swizzle/parser/ParserStateContext.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>

#include <cstddef>
#include <cstdint>

namespace swizzle { namespace parser {

    namespace {

        struct EnumValueVisitor
        {
            EnumValueVisitor(std::intmax_t& valueRef)
                : value(valueRef)
            {
            }

            template<typename T>
            void operator()(const T i)  { value = i; }

        private:
            std::intmax_t& value;
        };
    }

    void ParserStateContext::AllocateEnumValue(const lexer::TokenInfo& token, const types::EnumValue& enumValue)
    {
        std::intmax_t value = 0;
        EnumValueVisitor visitor(value);

        const auto valueVariant = enumValue.value();
        boost::apply_visitor(visitor, valueVariant);

        auto iter = UsedEnumValues.find(value);
        if(iter != UsedEnumValues.cend())
        {
            throw SyntaxError("Enum field value previously assigned.", token);
        }

        UsedEnumValues.insert(value);
    }

    void ParserStateContext::ClearEnumValueAllocations()
    {
        UsedEnumValues.clear();
    }
    
    void ParserStateContext::AllocateFieldLabel(const lexer::TokenInfo& token)
    {
        const auto label = token.token().value().to_string();
        
        const auto iter = UsedFieldLabels.find(label);
        if(iter != UsedFieldLabels.cend())
        {
            throw SyntaxError("Duplicate field label value assigned.", token);
        }
        
        UsedFieldLabels.insert(label);
    }
    
    void ParserStateContext::ClearFieldLabels()
    {
        UsedFieldLabels.clear();
    }

}}
