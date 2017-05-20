#pragma once 

#include <swizzle/lexer/states/AttributeState.hpp>
#include <swizzle/lexer/states/AttributeBlockState.hpp>
#include <swizzle/lexer/states/BeginHexLiteralState.hpp>
#include <swizzle/lexer/states/BeginStringState.hpp>
#include <swizzle/lexer/states/CharLiteralState.hpp>
#include <swizzle/lexer/states/CommentState.hpp>
#include <swizzle/lexer/states/EndCharLiteralState.hpp>
#include <swizzle/lexer/states/EscapedCharInCharLiteralState.hpp>
#include <swizzle/lexer/states/EscapedCharInStringLiteralState.hpp>
#include <swizzle/lexer/states/FirstSlashState.hpp>
#include <swizzle/lexer/states/HexLiteralState.hpp>
#include <swizzle/lexer/states/InitState.hpp>
#include <swizzle/lexer/states/MultilineCommentState.hpp>
#include <swizzle/lexer/states/NumericLiteralState.hpp>
#include <swizzle/lexer/states/StringLiteralState.hpp>

#include <swizzle/lexer/FileInfo.hpp>
#include <swizzle/lexer/Token.hpp>
#include <swizzle/lexer/TokenizerState.hpp>


#include <boost/utility/string_view.hpp>
#include <cstddef>

namespace swizzle { namespace lexer {

    template<class CreateTokenCallback>
    class TokenizerStatesPack
    {
    public:
        TokenizerStatesPack(CreateTokenCallback callback)
            : init_(callback)
            , firstSlash_(callback)
            , comment_(callback)
            , multilineComment_(callback)
            , stringLiteral_(callback)
            , charLiteral_(callback)
            , numericLiteral_(callback)
            , beginHexLiteral_(callback)
            , hexLiteral_(callback)
            , escapedCharInCharLiteral_(callback)
            , endCharLiteral_(callback)
            , escapedCharInString_(callback)
            , beginString_(callback)
            , attribute_(callback)
            , attributeBlock_(callback)
        {
        }

        TokenizerState consume(const TokenizerState state, const boost::string_view& source, const std::size_t position, FileInfo& fileInfo, Token& token)
        {
            switch(state)
            {
            case TokenizerState::Init:                          return init_.consume(source, position, fileInfo, token);
            case TokenizerState::FirstSlash:                    return firstSlash_.consume(source, position, fileInfo, token);
            case TokenizerState::Comment:                       return comment_.consume(source, position, fileInfo, token);
            case TokenizerState::MultilineComment:              return multilineComment_.consume(source, position, fileInfo, token);
            case TokenizerState::StringLiteral:                 return stringLiteral_.consume(source, position, fileInfo, token);
            case TokenizerState::CharLiteral:                   return charLiteral_.consume(source, position, fileInfo, token);
            case TokenizerState::NumericLiteral:                return numericLiteral_.consume(source, position, fileInfo, token);
            case TokenizerState::BeginHexLiteral:               return beginHexLiteral_.consume(source, position, fileInfo, token);
            case TokenizerState::HexLiteral:                    return hexLiteral_.consume(source, position, fileInfo, token);
            case TokenizerState::EscapedCharInCharLiteral:      return escapedCharInCharLiteral_.consume(source, position, fileInfo, token);
            case TokenizerState::EndCharLiteral:                return endCharLiteral_.consume(source, position, fileInfo, token);
            case TokenizerState::EscapedCharInStringLiteral:    return escapedCharInString_.consume(source, position, fileInfo, token);
            case TokenizerState::BeginString:                   return beginString_.consume(source, position, fileInfo, token);
            case TokenizerState::Attribute:                     return attribute_.consume(source, position, fileInfo, token);
            case TokenizerState::AttributeBlock:                return attributeBlock_.consume(source, position, fileInfo, token);

            default:    break;
            };

            throw UnknownTokenizerState(state);
        }

    private:
        states::InitState<CreateTokenCallback> init_;
        states::FirstSlashState<CreateTokenCallback> firstSlash_;
        states::CommentState<CreateTokenCallback> comment_;
        states::MultilineCommentState<CreateTokenCallback> multilineComment_;
        states::StringLiteralState<CreateTokenCallback> stringLiteral_;
        states::CharLiteralState<CreateTokenCallback> charLiteral_;
        states::NumericLiteralState<CreateTokenCallback> numericLiteral_;
        states::BeginHexLiteralState<CreateTokenCallback> beginHexLiteral_;
        states::HexLiteralState<CreateTokenCallback> hexLiteral_;
        states::EscapedCharInCharLiteralState<CreateTokenCallback> escapedCharInCharLiteral_;
        states::EndCharLiteralState<CreateTokenCallback> endCharLiteral_;
        states::EscapedCharInStringLiteralState<CreateTokenCallback> escapedCharInString_;
        states::BeginStringState<CreateTokenCallback> beginString_;
        states::AttributeState<CreateTokenCallback> attribute_;
        states::AttributeBlockState<CreateTokenCallback> attributeBlock_;
    };
}}
