#include "./platform/UnitTestSupport.hpp"
#include <swizzle/lexer/states/EndCharLiteralState.hpp>

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

        FileInfo info = FileInfo("testfile");
        states::EndCharLiteralState<CreateTokenCallback> state = states::EndCharLiteralState<CreateTokenCallback>(callback);

        std::size_t position = 0;
    };

    TEST_FIXTURE(EndCharLiteralFixture, verifyConstruction)
    {
    }

    struct WhenNextCharIsSingleQuote : public EndCharLiteralFixture
    {
        const std::string s = "a'";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 1, TokenType::char_literal);
    };

    TEST_FIXTURE(WhenNextCharIsSingleQuote, verifyConsume)
    {
        // consume '
        auto tokenState = state.consume(sv, ++position, info, token);

        REQUIRE CHECK_EQUAL(1U, tokens.size());

        CHECK_EQUAL(TokenType::char_literal, tokens[0].token().type());
        CHECK_EQUAL("a", tokens[0].token().to_string());
    }

    struct WhenNextCharIsInvalid : public EndCharLiteralFixture
    {
        const std::string s = " ";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::char_literal);
    };

    TEST_FIXTURE(WhenNextCharIsInvalid, verifyConsume)
    {
        CHECK_THROW(state.consume(sv, position++, info, token), swizzle::TokenizerSyntaxError);
    }
}
