#include <swizzle/parser/states/StructFieldEqualReadState.hpp>

#include <swizzle/ast/nodes/DefaultFloatValue.hpp>
#include <swizzle/ast/nodes/DefaultStringValue.hpp>
#include <swizzle/ast/nodes/DefaultValue.hpp>
#include <swizzle/ast/nodes/StructField.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/Token.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/lexer/TokenType.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/types/NodeStack.hpp>
#include <swizzle/types/utils/AppendNode.hpp>
#include <swizzle/types/utils/CheckFloatValue.hpp>
#include <swizzle/types/utils/NodeStackTopIs.hpp>
#include <swizzle/types/utils/SetValue.hpp>
#include <swizzle/types/utils/ValidateEnumValue.hpp>
#include <swizzle/types/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    namespace {
        ParserState consume_impl(const lexer::TokenInfo& token, types::NodeStack& nodeStack, types::NodeStack&, types::TokenStack& tokenStack, ParserStateContext&)
        {
            const auto type = token.token().type();

            if(type == lexer::TokenType::l_brace)
            {
                // [ARG]: TODO: add state for handling initialization list.
            }

            if(type == lexer::TokenType::float_literal)
            {
                if(types::utils::nodeStackTopIs<ast::nodes::StructField>(nodeStack))
                {
                    const auto& structField = static_cast<ast::nodes::StructField&>(*nodeStack.top());

                    if(structField.isVector())
                    {
                        throw SyntaxError("Default values not permitted for vector types.", token);
                    }

                    if(structField.isArray())
                    {
                        throw SyntaxError("Float literal cannot be assigned to array type, use initialization list instead.", token);
                    }

                    types::utils::appendNode<ast::nodes::DefaultFloatValue>(nodeStack, token, structField.type());
                    types::utils::checkFloatValue(structField.type(), token, "Attempting to assign float literal to a non-float type");

                    return ParserState::StructFieldValueRead;
                }

                throw ParserError("Internal parser error, expected top of node stack to be ast::nodes::StructField");
            }
            
            if(type == lexer::TokenType::numeric_literal)
            {
                if(types::utils::nodeStackTopIs<ast::nodes::StructField>(nodeStack))
                {
                    const auto& structField = static_cast<ast::nodes::StructField&>(*nodeStack.top());

                    if(structField.isVector())
                    {
                        throw SyntaxError("Default values not permitted for vector types.", token);
                    }

                    if(structField.isArray())
                    {
                        throw SyntaxError("Numeric literal cannot be assigned to array type, use initialization list instead.", token);
                    }

                    types::utils::appendNode<ast::nodes::DefaultValue>(nodeStack, token, structField.type());
                    types::utils::setValue(structField.type(), token, "Attempting to assign numeric literal to unsupported type");

                    return ParserState::StructFieldValueRead;
                }

                throw ParserError("Internal parser error, expected top of node stack to be ast::nodes::StructField");
            }

            if(type == lexer::TokenType::hex_literal)
            {
                if(types::utils::nodeStackTopIs<ast::nodes::StructField>(nodeStack))
                {
                    const auto& structField = static_cast<ast::nodes::StructField&>(*nodeStack.top());

                    if(structField.isVector())
                    {
                        throw SyntaxError("Default values not permitted for vector types.", token);
                    }

                    if(structField.isArray())
                    {
                        throw SyntaxError("Numeric literal cannot be assigned to array type, use initialization list instead.", token);
                    }

                    types::utils::appendNode<ast::nodes::DefaultValue>(nodeStack, token, structField.type());
                    types::utils::setValue(structField.type(), token, types::utils::isHex, "Attempting to assign hex literal to unsupported type");

                    return ParserState::StructFieldValueRead;
                }

                throw ParserError("Internal parser error, expected top of node stack to be ast::nodes::StructField");
            }

            if(type == lexer::TokenType::char_literal)
            {
                if(types::utils::nodeStackTopIs<ast::nodes::StructField>(nodeStack))
                {
                    const auto& structField = static_cast<ast::nodes::StructField&>(*nodeStack.top());

                    if(structField.isVector())
                    {
                        throw SyntaxError("Default values not permitted for vector types.", token);
                    }

                    if(structField.isArray())
                    {
                        throw SyntaxError("Numeric literal cannot be assigned to array type, use initialization list instead.", token);
                    }

                    types::utils::appendNode<ast::nodes::DefaultValue>(nodeStack, token, structField.type());
                    return ParserState::StructFieldValueRead;
                }

                throw ParserError("Internal parser error, expected top of node stack to be ast::nodes::StructField");
            }

            if(type == lexer::TokenType::string_literal)
            {
                if(types::utils::nodeStackTopIs<ast::nodes::StructField>(nodeStack))
                {
                    const auto& structField = static_cast<ast::nodes::StructField&>(*nodeStack.top());

                    if(structField.isVector())
                    {
                        throw SyntaxError("Default values not permitted for vector types.", token);
                    }

                    if(!structField.isArray())
                    {
                        throw SyntaxError("String literal default values can only be assigned to array types.", token);
                    }

                    const auto storageLength = structField.arraySize();
                    types::utils::appendNode<ast::nodes::DefaultStringValue>(nodeStack, token, structField.type(), storageLength);

                    if((storageLength < 0) || (static_cast<std::size_t>(storageLength) < token.token().value().length() - 2))
                    {
                        throw SyntaxError("Default string value would be truncated.", token);
                    }

                    return ParserState::StructFieldValueRead;
                }

                throw ParserError("Internal parser error, expected top of node stack to be ast::nodes::StructField");
            }
            
            if(type == lexer::TokenType::string)    // this is a enum value
            {
                tokenStack.push(token);
                return ParserState::StructFieldEnumOrNamespaceRead;
            }

            throw SyntaxError("Expected numeric_literal, hex_literal, char_literal, string_literal, or enum value", token);
        }
    }
    
    ParserState StructFieldEqualReadState::consume(const lexer::TokenInfo& token, types::NodeStack& nodeStack, types::NodeStack& attributeStack, types::TokenStack& tokenStack, ParserStateContext& context)
    {
        try
        {
            return consume_impl(token, nodeStack, attributeStack, tokenStack, context);
        }
        catch(const StreamInputCausesOverflow&)
        {
            throw SyntaxError("Enum field value overflows underlying type", token);
        }
        catch(const StreamInputCausesUnderflow& valueError)
        {
            throw SyntaxError("Enum field value underflows undelying type", token);
        }
        catch(const InvalidStreamInput&)
        {
            throw SyntaxError("Enum field value contais an invalid character", token);
        }
    }
}}}
