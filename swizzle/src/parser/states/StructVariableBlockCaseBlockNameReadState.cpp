#include <swizzle/parser/states/StructVariableBlockCaseBlockNameReadState.hpp>

#include <swizzle/ast/nodes/Struct.hpp>
#include <swizzle/ast/nodes/VariableBlockCase.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/detail/CreateType.hpp>
#include <swizzle/parser/detail/NodeStackTopIs.hpp>
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/TokenStack.hpp>
#include <swizzle/parser/utils/ClearTokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState StructVariableBlockCaseBlockNameReadState::consume(const lexer::TokenInfo& token, NodeStack& nodeStack, NodeStack&, TokenStack& tokenStack, ParserStateContext& context)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::colon)
        {
            return ParserState::StructVariableBlockNamespaceFirstColonRead;
        }

        if(type == lexer::TokenType::comma)
        {
            const auto structType = detail::createType(tokenStack);
            const auto structTypeString = structType.token().to_string();

            auto iter = context.TypeCache.find(structTypeString);
            iter = iter == context.TypeCache.cend()
                ? context.TypeCache.find(context.CurrentNamespace + "::" + structTypeString)
                : iter;

            if(iter == context.TypeCache.end())
            {
                throw SyntaxError("Variable block case type must be defined, ", structTypeString + " not defined", token.fileInfo());
            }

            const auto isStruct = dynamic_cast<ast::nodes::Struct*>(iter->second.get());
            if(!isStruct)
            {
                throw SyntaxError("Variable block case type must be a struct, ", structTypeString + " is not a struct", token.fileInfo());
            }

            // set the structType on the variableBlock case node & pop the node
            if(detail::nodeStackTopIs<ast::nodes::VariableBlockCase>(nodeStack))
            {
                auto& blockCase = static_cast<ast::nodes::VariableBlockCase&>(*nodeStack.top());
                blockCase.type(structType);

                nodeStack.pop();
                utils::clear(tokenStack);

                return ParserState::StructVariableBlockBeginCases;
            }

            throw ParserError("Internal parser error, top of node stack was not ast::nodes::VariableBlockCase");
        }

        throw SyntaxError("Expected ':' or ','", token);
    }
}}}
