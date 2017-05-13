#include "./platform/UnitTestSupport.hpp"
#include <swizzle/lexer/states/StringLiteralState.hpp>

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

    struct StringLiteralFixture
    {
        std::deque<TokenInfo> tokens;
        CreateTokenCallback callback = CreateTokenCallback(tokens);

        Token token = Token(boost::string_view(""), TokenType::string_literal);
        FileInfo info = FileInfo("testfile");

        states::StringLiteralState<CreateTokenCallback> state = states::StringLiteralState<CreateTokenCallback>(callback);

        std::size_t position = 0;
    };

    TEST_FIXTURE(StringLiteralFixture, verifyConstruction)
    {
    }

    struct WhenNextCharIsAlpha : public StringLiteralFixture
    {
        const std::string s = "a";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsAlpha, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::StringLiteral, tokenState);
        CHECK_EQUAL(TokenType::string_literal, token.type());
        CHECK_EQUAL("a", token.to_string());

        CHECK_EQUAL(0U, tokens.size());
    }

    struct WhenNextCharIsNumber : public StringLiteralFixture
    {
        const std::string s = "4";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsNumber, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::StringLiteral, tokenState);
        CHECK_EQUAL(TokenType::string_literal, token.type());
        CHECK_EQUAL("4", token.to_string());

        CHECK_EQUAL(0U, tokens.size());
    }

    struct WhenNextCharIsBackslash : public StringLiteralFixture
    {
        const std::string s = "\\";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsBackslash, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::EscapedCharInStringLiteral, tokenState);
        CHECK_EQUAL(TokenType::string_literal, token.type());
        CHECK_EQUAL("\\", token.to_string());

        CHECK_EQUAL(0U, tokens.size());
    }

    struct WhenNextCharIsDoubleQuote : public StringLiteralFixture
    {
        const std::string s = "\"";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsDoubleQuote, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::Init, tokenState);
        REQUIRE CHECK_EQUAL(1U, tokens.size());
    }
}
