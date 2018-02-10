#include <swizzle/parser/states/StructVariableBlockCaseBlockNameReadState.hpp>

#include <swizzle/ast/nodes/Struct.hpp>
#include <swizzle/ast/nodes/VariableBlockCase.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/types/NodeStack.hpp>
#include <swizzle/types/utils/CreateType.hpp>
#include <swizzle/types/utils/ClearTokenStack.hpp>
#include <swizzle/types/utils/NodeStackTopIs.hpp>
#include <swizzle/types/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState StructVariableBlockCaseBlockNameReadState::consume(const lexer::TokenInfo& token, types::NodeStack& nodeStack, types::NodeStack&, types::TokenStack& tokenStack, ParserStateContext& context)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::colon)
        {
            return ParserState::StructVariableBlockNamespaceFirstColonRead;
        }

        if(type == lexer::TokenType::comma)
        {
            const auto structType = types::utils::createType(tokenStack);
            const auto structTypeString = structType.token().to_string();

            if(context.SymbolTable.contains(context.CurrentNamespace, structTypeString))
            {
                auto info = context.SymbolTable.find(context.CurrentNamespace, structTypeString, SyntaxError("Variable block case type must be defined, '" + structTypeString + "' not defined", token));
                if(info.type() != types::SymbolType::Struct)
                {
                    throw SyntaxError("Variable block case type must be a struct, '", structTypeString + "' is not a struct", token);
                }
                
                // set the structType on the variableBlock case node & pop the node
                if(types::utils::nodeStackTopIs<ast::nodes::VariableBlockCase>(nodeStack))
                {
                    auto& blockCase = static_cast<ast::nodes::VariableBlockCase&>(*nodeStack.top());
                    blockCase.type(structType);

                    nodeStack.pop();
                    types::utils::clear(tokenStack);

                    return ParserState::StructVariableBlockBeginCases;
                }

                throw ParserError("Internal parser error, top of node stack was not ast::nodes::VariableBlockCase");
            }

            throw SyntaxError("Variable block case type must be defined, ", structTypeString + " not defined", token);
        }

        throw SyntaxError("Expected ':' or ','", token);
    }
}}}
