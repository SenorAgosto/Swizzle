#include "./platform/UnitTestSupport.hpp"
#include <swizzle/lexer/states/EscapedCharInCharLiteralState.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/Token.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/lexer/TokenizerState.hpp>
#include <swizzle/lexer/FileInfo.hpp>

#include <boost/utility/string_view.hpp>
#include <deque>
#include <string>

namespace {
    using namespace swizzle::lexer;

    class CreateTokenCallback
    {
    public:
        CreateTokenCallback(std::deque<TokenInfo>& tokens)
            : tokens_(tokens)
        {
        }

        void operator()(const TokenInfo& info)
        {
            tokens_.push_back(info);
        }

        void operator()(TokenInfo&& info)
        {
            tokens_.emplace_back(info);
        }

    private:
        std::deque<TokenInfo>& tokens_;
    };

    struct EscapedCharInCharLiteralFixture
    {
        std::deque<TokenInfo> tokens;
        CreateTokenCallback callback = CreateTokenCallback(tokens);

        Token token = Token(boost::string_view(""), TokenType::char_literal);
        FileInfo info = FileInfo("testfile");

        states::EscapedCharInCharLiteral<CreateTokenCallback> state = states::EscapedCharInCharLiteral<CreateTokenCallback>(callback);

        std::size_t position = 0;
    };

    TEST_FIXTURE(EscapedCharInCharLiteralFixture, verifyConstruction)
    {
    }

    struct WhenNextCharIsSingleQuote : public EscapedCharInCharLiteralFixture
    {
        const std::string s = "'";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsSingleQuote, verifyConsume)
    {
        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::EndCharLiteral, tokenState);
        CHECK_EQUAL(TokenType::char_literal, token.type());
        CHECK_EQUAL("'", token.to_string());
    }

    struct WhenNextCharIsZero : public EscapedCharInCharLiteralFixture
    {
        const std::string s = "0";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsZero, verifyConsume)
    {
        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::EndCharLiteral, tokenState);
        CHECK_EQUAL(TokenType::char_literal, token.type());
        CHECK_EQUAL("0", token.to_string());
    }

    struct WhenNextCharIsA : public EscapedCharInCharLiteralFixture
    {
        const std::string s = "a";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsA, verifyConsume)
    {
        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::EndCharLiteral, tokenState);
        CHECK_EQUAL(TokenType::char_literal, token.type());
        CHECK_EQUAL("a", token.to_string());
    }

    struct WhenNextCharIsNewLine : public EscapedCharInCharLiteralFixture
    {
        const std::string s = "n";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsNewLine, verifyConsume)
    {
        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::EndCharLiteral, tokenState);
        CHECK_EQUAL(TokenType::char_literal, token.type());
        CHECK_EQUAL("n", token.to_string());
    }

    struct WhenNextCharIsReturn : public EscapedCharInCharLiteralFixture
    {
        const std::string s = "r";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsReturn, verifyConsume)
    {
        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::EndCharLiteral, tokenState);
        CHECK_EQUAL(TokenType::char_literal, token.type());
        CHECK_EQUAL("r", token.to_string());
    }

    struct WhenNextCharIsInvalid : public EscapedCharInCharLiteralFixture
    {
        const std::string s = " ";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsInvalid, verifyConsume)
    {
        CHECK_THROW(state.consume(sv, position++, info, token), swizzle::TokenizerSyntaxError);
    }
}
