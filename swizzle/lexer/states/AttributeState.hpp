#pragma once 
#include <swizzle/lexer/TokenizerStateInterface.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/FileInfo.hpp>
#include <swizzle/lexer/ResetToken.hpp>
#include <swizzle/lexer/Token.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/lexer/TokenProducer.hpp>
#include <swizzle/lexer/TokenizerState.hpp>

#include <cctype>

namespace swizzle { namespace lexer { namespace states {

    template<class CreateTokenCallback>
    class AttributeState
        : public TokenizerStateInterface
        , private TokenProducer<CreateTokenCallback>
    {
    public:
        AttributeState(CreateTokenCallback createToken)
            : TokenProducer<CreateTokenCallback>(createToken)
        {
        }

        TokenizerState consume(const boost::string_view& source, const std::size_t position, FileInfo& fileInfo, Token& token) override
        {
            const char c = source.at(position);

            if(c == '=')
            {
                this->produceToken(token, fileInfo);

                token = ResetToken(source, position, TokenType::equal);
                fileInfo.incrementColumn();

                this->produceToken(token, fileInfo);
                token = ResetToken(source, position + 1, TokenType::attribute_value);

                return TokenizerState::AttributeValue;
            }

            if(c == '{')
            {
                this->produceToken(token, fileInfo);

                token = ResetToken(source, position, TokenType::l_brace);
                fileInfo.incrementColumn();

                this->produceToken(token, fileInfo);

                token = ResetToken(source, position + 1, TokenType::attribute_block);
                return TokenizerState::AttributeBlock;
            }

            if(std::isalnum(c) || (c == '_') || (c == '-'))
            {
                token.expand();
                return TokenizerState::Attribute;
            }

            static const std::string whitespace(" \t\r\n");
            if(whitespace.find_first_of(c) != std::string::npos)
            {
                this->produceToken(token, fileInfo);
                token = ResetToken(source, position, TokenType::string);

                return TokenizerState::Init;
            }

            throw TokenizerError("Unexpected character following attribute name: '" + std::string(1, c) + "'");
        }
    };
}}}
