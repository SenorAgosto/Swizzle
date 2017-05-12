#include "./platform/UnitTestSupport.hpp"
#include <swizzle/lexer/states/CharLiteral.hpp>

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

    struct CharLiteralFixture
    {
        std::deque<TokenInfo> tokens;
        CreateTokenCallback callback = CreateTokenCallback(tokens);

        Token token = Token(boost::string_view(), TokenType::char_literal);
        FileInfo info = FileInfo("testfile");

        states::CharLiteral<CreateTokenCallback> state = states::CharLiteral<CreateTokenCallback>(callback);

        std::size_t position = 0;
    };

    TEST_FIXTURE(CharLiteralFixture, verifyConstruction)
    {
    }

    struct WhenNextCharIsBackslash : public CharLiteralFixture
    {
        const std::string s = "\\";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsBackslash, verifyConsume)
    {
        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::EscapedCharInCharLiteral, tokenState);
        CHECK_EQUAL(TokenType::char_literal, token.type());
        CHECK(token.empty());
    }

    struct WhenNextCharIsNotBackslash : public CharLiteralFixture
    {
        const std::string s = "a";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsNotBackslash, verifyConsume)
    {
        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::EndCharLiteral, tokenState);
        CHECK_EQUAL(TokenType::char_literal, token.type());
        CHECK_EQUAL("a", token.to_string());
    }
}
