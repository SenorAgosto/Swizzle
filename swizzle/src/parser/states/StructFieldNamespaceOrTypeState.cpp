#include <swizzle/parser/states/StructFieldNamespaceOrTypeState.hpp>

#include <swizzle/ast/nodes/FieldLabel.hpp>
#include <swizzle/ast/nodes/StructField.hpp>
#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/detail/CreateType.hpp>
#include <swizzle/parser/detail/NodeStackTopIs.hpp>
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/TokenStack.hpp>
#include <swizzle/parser/utils/ClearTokenStack.hpp>
#include <swizzle/types/IsIntegerType.hpp>
#include <swizzle/types/IsFloatType.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState StructFieldNamespaceOrTypeState::consume(const lexer::TokenInfo& token, NodeStack& nodeStack, NodeStack&, TokenStack& tokenStack, ParserStateContext& context)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::colon)
        {
            // NOTE:
            // We come here from StructArray and StructVector after a ],
            // the token stack shouldn't be empty here, but there's no
            // good way to validate it here. We'll let createType() validate
            // the type exists. If we're here with a bad token stack, the type
            // would be like "::something::bar::baz" which should fail to be found
            // in the TypeCache.

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
                auto sf = nodeStack.top();
                auto& top = static_cast<ast::nodes::StructField&>(*sf);

                tokenStack.push(token);

                const auto t = detail::createType(tokenStack);
                utils::clear(tokenStack);

                const auto& value = t.token().value();

                if(types::IsIntegerType(value) || types::IsFloatType(value))
                {
                    top.type(value.to_string());
                    top.name(token);

                    nodeStack.pop();

                    if(detail::nodeStackTopIs<ast::nodes::FieldLabel>(nodeStack))
                    {
                        sf->append(nodeStack.top());
                        nodeStack.pop();
                    }

                    nodeStack.push(sf);
                    return ParserState::StructFieldName;
                }

                if(context.TypeCache.find(value.to_string()) != context.TypeCache.cend())
                {
                    top.type(value.to_string());
                    top.name(token);

                    nodeStack.pop();

                    if(detail::nodeStackTopIs<ast::nodes::FieldLabel>(nodeStack))
                    {
                        sf->append(nodeStack.top());
                        nodeStack.pop();
                    }

                    nodeStack.push(sf);
                    return ParserState::StructFieldName;
                }

                const auto typeWithNamespace = context.CurrentNamespace + "::" + value.to_string();
                if(context.TypeCache.find(typeWithNamespace) != context.TypeCache.cend())
                {
                    top.type(typeWithNamespace);
                    top.name(token);

                    nodeStack.pop();

                    if(detail::nodeStackTopIs<ast::nodes::FieldLabel>(nodeStack))
                    {
                        sf->append(nodeStack.top());
                        nodeStack.pop();
                    }

                    nodeStack.push(sf);
                    return ParserState::StructFieldName;
                }

                throw SyntaxError("Struct member using undeclared type", token);
            }

            throw ParserError("Internal parser error, top of node stack was not ast::nodes::StructField");
        }

        throw SyntaxError("Expected member name or ':'", token);
    }
}}}
