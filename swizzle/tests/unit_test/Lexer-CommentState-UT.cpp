#include "./ut_support/UnitTestSupport.hpp"
#include <swizzle/lexer/states/CommentState.hpp>

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
        states::CommentState<CreateTokenCallback> state = states::CommentState<CreateTokenCallback>(callback);

        std::size_t position = 0;
    };

    struct CommentFixture : public TokenizerFixture
    {
        const std::string s = "bl\n";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::comment);
    };

    TEST_FIXTURE(CommentFixture, verifyConstruction)
    {
    }

    TEST_FIXTURE(CommentFixture, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        CHECK_EQUAL(1U, info.start().line());
        CHECK_EQUAL(1U, info.start().column());
        CHECK_EQUAL(1U, info.end().line());
        CHECK_EQUAL(1U, info.end().column());

        // consume b
        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::Comment, tokenState);
        CHECK_EQUAL(TokenType::comment, token.type());
        CHECK_EQUAL("b", token.to_string());

        // consume l
        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::Comment, tokenState);
        CHECK_EQUAL(TokenType::comment, token.type());
        CHECK_EQUAL("bl", token.to_string());

        // consume \n
        tokenState = state.consume(sv, position++, info, token);

        REQUIRE CHECK_EQUAL(1U, tokens.size());

        CHECK_EQUAL(TokenType::comment, tokens[0].token().type());
        CHECK_EQUAL("bl", tokens[0].token().to_string());

        CHECK_EQUAL(1U, tokens[0].fileInfo().start().line());
        CHECK_EQUAL(1U, tokens[0].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[0].fileInfo().end().line());
        CHECK_EQUAL(3U, tokens[0].fileInfo().end().column());
    }

    struct MultilineCommentFixture : public TokenizerFixture
    {
        const std::string s = "b\\";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::comment);
    };

    TEST_FIXTURE(MultilineCommentFixture, verifyConsumeTransitionsToMultilineComment)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::Comment, tokenState);
        CHECK_EQUAL(TokenType::comment, token.type());
        CHECK_EQUAL("b", token.to_string());

        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::MultilineComment, tokenState);
        CHECK_EQUAL(TokenType::multiline_comment, token.type());
        CHECK_EQUAL("b\\", token.to_string());

        CHECK_EQUAL(0U, tokens.size());
    }
}
