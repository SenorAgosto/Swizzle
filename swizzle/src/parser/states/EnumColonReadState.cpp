#include <swizzle/parser/states/EnumColonReadState.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/ast/nodes/Enum.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/detail/NodeStackTopIs.hpp>
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/TokenStack.hpp>
#include <swizzle/types/IsIntegerType.hpp>

#include <cstdint>

namespace swizzle { namespace parser { namespace states {

    ParserState EnumColonReadState::consume(const lexer::TokenInfo& token, NodeStack& nodeStack, TokenStack&, ParserStateContext& context)
    {
        const auto type = token.token().type();
        const auto& value = token.token().value();

        if(type == lexer::TokenType::type)
        {
            if(!types::IsIntegerType(token.token().value()))
            {
                throw SyntaxError("Underlying type must be an integer type", token);
            }

            if(detail::nodeStackTopIs<ast::nodes::Enum>(nodeStack))
            {
                auto& top = static_cast<ast::nodes::Enum&>(*nodeStack.top());
                top.underlying(token);

                if(value == "u8")
                {
                    context.CurrentEnumValue = types::EnumValue(std::uint8_t(0));
                    return ParserState::EnumUnderlyingType;
                }

                if(value == "u16")
                {
                    context.CurrentEnumValue = types::EnumValue(std::uint16_t(0));
                    return ParserState::EnumUnderlyingType;
                }

                if(value == "u32")
                {
                    context.CurrentEnumValue = types::EnumValue(std::uint32_t(0));
                    return ParserState::EnumUnderlyingType;
                }

                if(value == "u64")
                {
                    context.CurrentEnumValue = types::EnumValue(std::uint64_t(0));
                    return ParserState::EnumUnderlyingType;
                }

                if(value == "i8")
                {
                    context.CurrentEnumValue = types::EnumValue(std::int8_t(0));
                    return ParserState::EnumUnderlyingType;
                }

                if(value == "i16")
                {
                    context.CurrentEnumValue = types::EnumValue(std::int16_t(0));
                    return ParserState::EnumUnderlyingType;
                }

                if(value == "i32")
                {
                    context.CurrentEnumValue = types::EnumValue(std::int32_t(0));
                    return ParserState::EnumUnderlyingType;
                }

                if(value == "i64")
                {
                    context.CurrentEnumValue = types::EnumValue(std::int64_t(0));
                    return ParserState::EnumUnderlyingType;
                }

                throw SyntaxError("Expecting integer enum underyling type", token);
            }

            throw ParserError("Internal parser error: expected top of node stack to be ast::nodes::Enum");
        }

        throw SyntaxError("Expecting enum underlying type", token);
    }
}}}
