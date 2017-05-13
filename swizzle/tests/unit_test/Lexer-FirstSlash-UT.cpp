#include "./platform/UnitTestSupport.hpp"
#include <swizzle/lexer/states/FirstSlashState.hpp>

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
        states::FirstSlash<CreateTokenCallback> state = states::FirstSlash<CreateTokenCallback>(callback);

        std::size_t position = 0;
    };

    struct CommentFixture : public TokenizerFixture
    {
        // line: // this is a comment
        const std::string s = "/ this is a comment";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(boost::string_view(), TokenType::comment);
    };

    TEST_FIXTURE(CommentFixture, verifyConstruction)
    {
    }

    TEST_FIXTURE(CommentFixture, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::Comment, tokenState);
        CHECK_EQUAL(TokenType::comment, token.type());
        CHECK(token.empty());

        CHECK_EQUAL(0U, tokens.size());
    }

    struct SyntaxErrorFixture : public TokenizerFixture
    {
        // line: /syntax error
        const std::string s = "syntax error";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(boost::string_view(), TokenType::comment);
    };

    TEST_FIXTURE(SyntaxErrorFixture, verifyConsumeThrowsOnSyntaxError)
    {
        CHECK_THROW(state.consume(sv, position++, info, token), swizzle::TokenizerError);
    }
}
