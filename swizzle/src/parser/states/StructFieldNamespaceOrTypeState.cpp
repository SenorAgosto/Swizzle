#include <swizzle/parser/states/StructFieldNamespaceOrTypeState.hpp>

#include <swizzle/ast/nodes/StructField.hpp>
#include <swizzle/Exceptions.hpp>
#include <swizzle/IsIntegerType.hpp>
#include <swizzle/IsFloatType.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/detail/CreateType.hpp>
#include <swizzle/parser/detail/NodeStackTopIs.hpp>
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState StructFieldNamespaceOrTypeState::consume(const lexer::TokenInfo& token, NodeStack& nodeStack, TokenStack& tokenStack, ParserStateContext& context)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::colon)
        {
            return ParserState::StructFieldNamespaceFirstColon;
        }

        if(type == lexer::TokenType::l_bracket)
        {
            return ParserState::StructStartArray;
        }

        if(type == lexer::TokenType::string)
        {
            if(detail::nodeStackTopIs<ast::nodes::StructField>(nodeStack))
            {
                auto& top = static_cast<ast::nodes::StructField&>(*nodeStack.top());
                const auto t = detail::createType(tokenStack);
                const auto& value = t.token().value();

                if(IsIntegerType(value) || IsFloatType(value))
                {
                    top.type(value.to_string());
                    top.name(token);

                    nodeStack.pop();
                    return ParserState::StructFieldName;
                }

                if(context.TypeCache.find(value.to_string()) != context.TypeCache.cend())
                {
                    top.type(value.to_string());
                    top.name(token);

                    nodeStack.pop();
                    return ParserState::StructFieldName;
                }

                const auto typeWithNamespace = context.CurrentNamespace + "::" + value.to_string();
                if(context.TypeCache.find(typeWithNamespace) != context.TypeCache.cend())
                {
                    top.type(typeWithNamespace);
                    top.name(token);

                    nodeStack.pop();
                    return ParserState::StructFieldName;
                }

                throw SyntaxError("Struct member using undeclared type", token);
            }

            throw ParserError("Internal parser error, top of node stack was not ast::nodes::StructField");
        }

        throw SyntaxError("Expected member name or ':'", token);
    }
}}}
