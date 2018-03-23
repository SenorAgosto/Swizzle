#include <swizzle/parser/states/StructFieldNamespaceOrTypeState.hpp>

#include <swizzle/ast/nodes/FieldLabel.hpp>
#include <swizzle/ast/nodes/StructField.hpp>
#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/types/IsIntegerType.hpp>
#include <swizzle/types/IsFloatType.hpp>
#include <swizzle/types/NodeStack.hpp>
#include <swizzle/types/TokenStack.hpp>
#include <swizzle/types/utils/AttachAttributes.hpp>
#include <swizzle/types/utils/ClearTokenStack.hpp>
#include <swizzle/types/utils/CreateType.hpp>
#include <swizzle/types/utils/NodeStackTopIs.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState StructFieldNamespaceOrTypeState::consume(const lexer::TokenInfo& token, types::NodeStack& nodeStack, types::NodeStack& attributeStack, types::TokenStack& tokenStack, ParserStateContext& context)
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
            if(types::utils::nodeStackTopIs<ast::nodes::StructField>(nodeStack))
            {
                auto sf = nodeStack.top();
                types::utils::attachAttributes(attributeStack, sf);

                const auto t = types::utils::createType(tokenStack);
                types::utils::clear(tokenStack);

                auto& top = static_cast<ast::nodes::StructField&>(*sf);
                const auto& value = t.token().value();

                if(types::IsIntegerType(value) || types::IsFloatType(value))
                {
                    top.type(value.to_string());
                    return ParserState::StructStartArray;
                }

                const auto valueType = value.to_string();
                const auto info = context.SymbolTable.find(context.CurrentNamespace, valueType, SyntaxError("Struct array/vector member using undeclared type", token));
                
                top.type(info.symbol());
                return ParserState::StructStartArray;
            }

            throw ParserError("Internal parser error, expect top of node stack to be ast::nodes::StructField");
        }

        if(type == lexer::TokenType::string)
        {
            if(types::utils::nodeStackTopIs<ast::nodes::StructField>(nodeStack))
            {
                auto sf = nodeStack.top();
                types::utils::attachAttributes(attributeStack, sf);

                auto& top = static_cast<ast::nodes::StructField&>(*sf);
                top.name(token);
                
                if(top.isArray() || top.isVector())
                {
                    return ParserState::StructFieldName;
                }

                const auto t = types::utils::createType(tokenStack);
                types::utils::clear(tokenStack);
                
                top.typeDecl(t);
                const auto& value = t.token().value();

                if(types::IsIntegerType(value) || types::IsFloatType(value))
                {
                    top.type(value.to_string());
                    return ParserState::StructFieldName;
                }

                const auto valueType = value.to_string();
                const auto info = context.SymbolTable.find(context.CurrentNamespace, valueType, SyntaxError("Struct member using undeclared type", token));

                top.type(info.symbol());
                return ParserState::StructFieldName;
            }

            throw ParserError("Internal parser error, top of node stack was not ast::nodes::StructField");
        }

        // supply a better error if we can
        if(types::utils::nodeStackTopIs<ast::nodes::StructField>(nodeStack) && !tokenStack.empty())
        {
            throw SyntaxError("Struct member name missing in type declaration", tokenStack.top());
        }

        throw SyntaxError("Expected member name or ':'", token);
    }
}}}
