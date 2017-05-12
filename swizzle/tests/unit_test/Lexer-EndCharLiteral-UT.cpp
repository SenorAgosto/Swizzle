#include "./platform/UnitTestSupport.hpp"
#include <swizzle/lexer/states/EndCharLiteral.hpp>

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

    struct EndCharLiteralFixture
    {
        std::deque<TokenInfo> tokens;
        CreateTokenCallback callback = CreateTokenCallback(tokens);

        Token token = Token(boost::string_view("a"), TokenType::char_literal);
        FileInfo info = FileInfo("testfile");

        states::EndCharLiteral<CreateTokenCallback> state = states::EndCharLiteral<CreateTokenCallback>(callback);

        std::size_t position = 0;
    };

    TEST_FIXTURE(EndCharLiteralFixture, verifyConstruction)
    {
    }

    struct WhenNextCharIsSingleQuote : public EndCharLiteralFixture
    {
        const std::string s = "'";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsSingleQuote, verifyConsume)
    {
        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::Init, tokenState);
        CHECK_EQUAL(TokenType::string, token.type());
        CHECK_EQUAL("'", token.to_string());

        REQUIRE CHECK_EQUAL(1U, tokens.size());
        CHECK_EQUAL(TokenType::char_literal, tokens[0].token().type());
        CHECK_EQUAL("a", tokens[0].token().to_string());
    }

    struct WhenNextCharIsInvalid : public EndCharLiteralFixture
    {
        const std::string s = " ";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsInvalid, verifyConsume)
    {
        CHECK_THROW(state.consume(sv, position++, info, token), swizzle::TokenizerSyntaxError);
    }
}
