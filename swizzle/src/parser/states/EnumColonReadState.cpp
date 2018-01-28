#include <swizzle/parser/states/EnumColonReadState.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/ast/nodes/Enum.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/types/EnumValue.hpp>
#include <swizzle/types/IsIntegerType.hpp>
#include <swizzle/types/NodeStack.hpp>
#include <swizzle/types/utils/NodeStackTopIs.hpp>
#include <swizzle/types/TokenStack.hpp>

#include <cstdint>
#include <memory>

namespace swizzle { namespace parser { namespace states {

    ParserState EnumColonReadState::consume(const lexer::TokenInfo& token, types::NodeStack& nodeStack, types::NodeStack&, types::TokenStack&, ParserStateContext& context)
    {
        const auto type = token.token().type();
        const auto& value = token.token().value();

        if(type == lexer::TokenType::type)
        {
            if(!types::IsIntegerType(token.token().value()))
            {
                throw SyntaxError("Underlying type must be an integer type", token);
            }

            if(types::utils::nodeStackTopIs<ast::nodes::Enum>(nodeStack))
            {
                auto& top = static_cast<ast::nodes::Enum&>(*nodeStack.top());
                top.underlying(token);

                if(value == "u8")
                {
                    context.CurrentEnumValue = std::unique_ptr<types::EnumValueInterface>(new types::EnumValue<std::uint8_t>());
                    return ParserState::EnumUnderlyingType;
                }

                if(value == "u16")
                {
                    context.CurrentEnumValue = std::unique_ptr<types::EnumValueInterface>(new types::EnumValue<std::uint16_t>());
                    return ParserState::EnumUnderlyingType;
                }

                if(value == "u32")
                {
                    context.CurrentEnumValue = std::unique_ptr<types::EnumValueInterface>(new types::EnumValue<std::uint32_t>());
                    return ParserState::EnumUnderlyingType;
                }

                if(value == "u64")
                {
                    context.CurrentEnumValue = std::unique_ptr<types::EnumValueInterface>(new types::EnumValue<std::uint64_t>());
                    return ParserState::EnumUnderlyingType;
                }

                if(value == "i8")
                {
                    context.CurrentEnumValue = std::unique_ptr<types::EnumValueInterface>(new types::EnumValue<std::int8_t>());
                    return ParserState::EnumUnderlyingType;
                }

                if(value == "i16")
                {
                    context.CurrentEnumValue = std::unique_ptr<types::EnumValueInterface>(new types::EnumValue<std::int16_t>());
                    return ParserState::EnumUnderlyingType;
                }

                if(value == "i32")
                {
                    context.CurrentEnumValue = std::unique_ptr<types::EnumValueInterface>(new types::EnumValue<std::int32_t>());
                    return ParserState::EnumUnderlyingType;
                }

                if(value == "i64")
                {
                    context.CurrentEnumValue = std::unique_ptr<types::EnumValueInterface>(new types::EnumValue<std::int64_t>());
                    return ParserState::EnumUnderlyingType;
                }

                throw SyntaxError("Expecting integer enum underyling type", token);
            }

            throw ParserError("Internal parser error: expected top of node stack to be ast::nodes::Enum");
        }

        throw SyntaxError("Expecting enum underlying type", token);
    }
}}}
