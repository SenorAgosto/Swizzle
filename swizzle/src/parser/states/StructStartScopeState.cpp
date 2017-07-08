#include <swizzle/parser/states/StructStartScopeState.hpp>

#include <swizzle/ast/nodes/Attribute.hpp>
#include <swizzle/ast/nodes/AttributeBlock.hpp>
#include <swizzle/ast/nodes/CharLiteral.hpp>
#include <swizzle/ast/nodes/Comment.hpp>
#include <swizzle/ast/nodes/FieldLabel.hpp>
#include <swizzle/ast/nodes/HexLiteral.hpp>
#include <swizzle/ast/nodes/MultilineComment.hpp>
#include <swizzle/ast/nodes/NumericLiteral.hpp>
#include <swizzle/ast/nodes/StringLiteral.hpp>
#include <swizzle/ast/nodes/Struct.hpp>
#include <swizzle/ast/nodes/StructField.hpp>
#include <swizzle/ast/nodes/VariableBlock.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/IsIntegerType.hpp>
#include <swizzle/IsFloatType.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/detail/AppendNode.hpp>
#include <swizzle/parser/detail/NodeStackTopIs.hpp>
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState StructStartScopeState::consume(const lexer::TokenInfo& token, NodeStack& nodeStack, TokenStack& tokenStack, ParserStateContext& context)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::numeric_literal)
        {
            if(detail::nodeStackTopIs<ast::nodes::Attribute>(nodeStack))
            {
                detail::appendNode<ast::nodes::NumericLiteral>(nodeStack, token);
                nodeStack.pop();

                return ParserState::StructStartScope;
            }

            if(detail::nodeStackTopIs<ast::nodes::FieldLabel>(nodeStack))
            {
                throw SyntaxError("Expected field declaration, found duplicitous field label", token);
            }

            nodeStack.push(new ast::nodes::FieldLabel(token));
            return ParserState::StructFieldLabel;
        }

        if(type == lexer::TokenType::hex_literal)
        {
            if(detail::nodeStackTopIs<ast::nodes::Attribute>(nodeStack))
            {
                detail::appendNode<ast::nodes::HexLiteral>(nodeStack, token);
                nodeStack.pop();

                return ParserState::StructStartScope;
            }

            throw ParserError("Internal parser error, top of node stack was not ast::nodes::Attribute");
        }

        if(type == lexer::TokenType::char_literal)
        {
            if(detail::nodeStackTopIs<ast::nodes::Attribute>(nodeStack))
            {
                detail::appendNode<ast::nodes::CharLiteral>(nodeStack, token);
                nodeStack.pop();

                return ParserState::StructStartScope;
            }

            throw ParserError("Internal parser error, top of node stack was not ast::nodes::Attribute");
        }

        if(type == lexer::TokenType::string_literal)
        {
            if(detail::nodeStackTopIs<ast::nodes::Attribute>(nodeStack))
            {
                detail::appendNode<ast::nodes::StringLiteral>(nodeStack, token);
                nodeStack.pop();

                return ParserState::StructStartScope;
            }

            throw ParserError("Internal parser error, top of node stack was not ast::nodes::Attribute");
        }

        if(type == lexer::TokenType::attribute_block)
        {
            if(detail::nodeStackTopIs<ast::nodes::Attribute>(nodeStack))
            {
                detail::appendNode<ast::nodes::AttributeBlock>(nodeStack, token);
                nodeStack.pop();

                return ParserState::StructStartScope;
            }

            throw ParserError("Internal parser error, top of node stack was not ast::nodes::Attribute");
        }

        // cleanup node stack if the last value was an attribute
        if(detail::nodeStackTopIs<ast::nodes::Attribute>(nodeStack))
        {
            nodeStack.pop();
        }

        if(type == lexer::TokenType::attribute)
        {
            const auto node = detail::appendNode<ast::nodes::Attribute>(nodeStack, token);
            nodeStack.push(node);

            return ParserState::StructStartScope;
        }

        if(type == lexer::TokenType::comment)
        {
            detail::appendNode<ast::nodes::Comment>(nodeStack, token);
            return ParserState::StructStartScope;
        }

        if(type == lexer::TokenType::multiline_comment)
        {
            detail::appendNode<ast::nodes::MultilineComment>(nodeStack, token);
            return ParserState::StructStartScope;
        }

        if(type == lexer::TokenType::type)
        {
            const auto& value = token.token().value();
            if(IsIntegerType(value) || IsFloatType(value))
            {
                auto node = detail::appendNode<ast::nodes::StructField>(nodeStack);
                nodeStack.push(node);
                tokenStack.push(token);

                return ParserState::StructFieldNamespaceOrType;
            }

            if(value == "variable_block")
            {
                auto node = detail::appendNode<ast::nodes::VariableBlock>(nodeStack, token);
                nodeStack.push(node);

                return ParserState::StructStartVariableBlock;
            }

            throw SyntaxError("Unsupported type encountered in struct field", token);
        }

        if(type == lexer::TokenType::string)
        {
            if(detail::nodeStackTopIs<ast::nodes::StructField>(nodeStack))
            {
                auto& top = static_cast<ast::nodes::StructField&>(*nodeStack.top());
                top.name(token);

                return ParserState::StructFieldName;
            }

            auto node = detail::appendNode<ast::nodes::StructField>(nodeStack);
            nodeStack.push(node);
            tokenStack.push(token);

            return ParserState::StructFieldNamespaceOrType;
        }

        if(type == lexer::TokenType::keyword)
        {
            const auto& value = token.token().value();
            if(value == "const")
            {
                context.MemberIsConst = true;
                return ParserState::StructStartScope;
            }
        }

        if(type == lexer::TokenType::r_brace)
        {
            if(detail::nodeStackTopIs<ast::nodes::Struct>(nodeStack))
            {
                const auto& top = static_cast<ast::nodes::Struct&>(*nodeStack.top());
                if(top.empty())
                {
                    throw SyntaxError("Enum must have fields, no fields declared in '" + top.name() + "'", token);
                }
            }

            throw ParserError("Internal parser error, top of node stack is not ast::nodes::Struct");
        }

        throw SyntaxError("Unexpected token encountered", token);
    }
}}}
