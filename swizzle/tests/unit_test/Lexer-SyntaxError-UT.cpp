#include "./platform/UnitTestSupport.hpp"
#include <swizzle/lexer/states/SyntaxError.hpp>

#include <swizzle/lexer/FileInfo.hpp>
#include <swizzle/lexer/Token.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/lexer/TokenizerState.hpp>

#include <boost/utility/string_view.hpp>
#include <deque>

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

    struct TokenizerFixture
    {
        std::deque<TokenInfo> tokens;
        CreateTokenCallback callback = CreateTokenCallback(tokens);

        FileInfo info = FileInfo("testfile");
        states::SyntaxError<CreateTokenCallback> state = states::SyntaxError<CreateTokenCallback>(callback);

        std::size_t position = 0;
    };

    struct ConstructionFixture : public TokenizerFixture
    {
        const std::string s = "";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, TokenType::string);
    };

    TEST_FIXTURE(ConstructionFixture, verifyConstruction)
    {
    }

    struct AnyInputFixture : public TokenizerFixture
    {
        const std::string s = "blah";
        const boost::string_view sv = boost::string_view(s);

        Token token;
    };

    TEST_FIXTURE(AnyInputFixture, verifyExpandToken)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position++, info, token);
        CHECK_EQUAL(TokenizerState::SyntaxError, tokenState);

        CHECK_EQUAL(TokenType::string, token.type());
        CHECK_EQUAL("", token.to_string());

        tokenState = state.consume(sv, position++, info, token);
        CHECK_EQUAL(TokenizerState::SyntaxError, tokenState);

        CHECK_EQUAL(TokenType::string, token.type());
        CHECK_EQUAL("", token.to_string());

        CHECK_EQUAL(0U, tokens.size());
    }
}
