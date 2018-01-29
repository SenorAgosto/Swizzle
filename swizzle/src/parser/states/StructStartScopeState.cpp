#include <swizzle/parser/states/StructStartScopeState.hpp>

#include <swizzle/ast/Matcher.hpp>
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
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/types/NodeStack.hpp>
#include <swizzle/types/TokenStack.hpp>
#include <swizzle/types/IsIntegerType.hpp>
#include <swizzle/types/IsFloatType.hpp>
#include <swizzle/types/utils/AppendNode.hpp>
#include <swizzle/types/utils/NodeStackTopIs.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState StructStartScopeState::consume(const lexer::TokenInfo& token, types::NodeStack& nodeStack, types::NodeStack& attributeStack, types::TokenStack& tokenStack, ParserStateContext& context)
    {
        const auto type = token.token().type();

        if(equalRead_)
        {
            if(type == lexer::TokenType::equal)
            {
                throw SyntaxError("Expected literal value", token);
            }

            if(type == lexer::TokenType::numeric_literal)
            {
                equalRead_ = false;
                types::utils::appendNode<ast::nodes::NumericLiteral>(attributeStack, token);

                return ParserState::StructStartScope;
            }

            if(type == lexer::TokenType::hex_literal)
            {
                equalRead_ = false;
                types::utils::appendNode<ast::nodes::HexLiteral>(attributeStack, token);

                return ParserState::StructStartScope;
            }

            if(type == lexer::TokenType::char_literal)
            {
                equalRead_ = false;
                types::utils::appendNode<ast::nodes::CharLiteral>(attributeStack, token);

                return ParserState::StructStartScope;
            }

            if(type == lexer::TokenType::string_literal)
            {
                equalRead_ = false;
                types::utils::appendNode<ast::nodes::StringLiteral>(attributeStack, token);

                return ParserState::StructStartScope;
            }
        }
        else
        {
            if(type == lexer::TokenType::equal)
            {
                equalRead_ = true;
                return ParserState::StructStartScope;
            }

            if(type == lexer::TokenType::numeric_literal)
            {
                if(types::utils::nodeStackTopIs<ast::nodes::FieldLabel>(nodeStack))
                {
                    throw SyntaxError("Expected field declaration, found duplicitous field label", token);
                }

                context.AllocateFieldLabel(token);
                
                // we want to attach this to the field
                attributeStack.push(new ast::nodes::FieldLabel(token));
                return ParserState::StructFieldLabel;
            }

            if(type == lexer::TokenType::attribute_block)
            {
                if(!attributeStack.empty())
                {
                    types::utils::appendNode<ast::nodes::AttributeBlock>(attributeStack, token);
                    return ParserState::StructStartScope;
                }
            }

            if(type == lexer::TokenType::attribute)
            {
                attributeStack.push(new ast::nodes::Attribute(token));
                return ParserState::StructStartScope;
            }

            if(type == lexer::TokenType::comment)
            {
                types::utils::appendNode<ast::nodes::Comment>(nodeStack, token);
                return ParserState::StructStartScope;
            }

            if(type == lexer::TokenType::multiline_comment)
            {
                types::utils::appendNode<ast::nodes::MultilineComment>(nodeStack, token);
                return ParserState::StructStartScope;
            }

            if(type == lexer::TokenType::type)
            {
                const auto& value = token.token().value();
                if(types::IsIntegerType(value) || types::IsFloatType(value))
                {
                    auto node = types::utils::appendNode<ast::nodes::StructField>(nodeStack);
                    nodeStack.push(node);
                    tokenStack.push(token);

                    auto& top = static_cast<ast::nodes::StructField&>(*node);
                    if(context.MemberIsConst)
                    {
                        top.setConst();
                    }

                    return ParserState::StructFieldNamespaceOrType;
                }

                if(value == "variable_block")
                {
                    if(context.MemberIsConst)
                    {
                        throw SyntaxError("variable_block cannot be const", token);
                    }
                
                    auto node = types::utils::appendNode<ast::nodes::VariableBlock>(nodeStack, token);
                    nodeStack.push(node);

                    return ParserState::StructStartVariableBlock;
                }

                throw SyntaxError("Unsupported type encountered in struct field", token);
            }

            if(type == lexer::TokenType::string)
            {
                if(types::utils::nodeStackTopIs<ast::nodes::StructField>(nodeStack))
                {
                    auto& top = static_cast<ast::nodes::StructField&>(*nodeStack.top());
                    top.name(token);

                    if(context.MemberIsConst)
                    {
                        top.setConst();
                    }
                    
                    return ParserState::StructFieldName;
                }

                auto node = types::utils::appendNode<ast::nodes::StructField>(nodeStack);
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
                if(types::utils::nodeStackTopIs<ast::nodes::Struct>(nodeStack))
                {
                    context.ClearFieldLabels();
                    
                    nodeStack.pop();
                    return ParserState::TranslationUnitMain;
                }

                throw ParserError("Internal parser error, top of node stack is not ast::nodes::Struct");
            }
        }

        throw SyntaxError("Unexpected token encountered", token);
    }
}}}
