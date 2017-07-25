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
}}
