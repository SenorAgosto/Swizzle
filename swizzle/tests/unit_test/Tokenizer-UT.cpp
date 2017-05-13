#include "./platform/UnitTestSupport.hpp"
#include <swizzle/lexer/Tokenizer.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/FileInfo.hpp>
#include <swizzle/lexer/Token.hpp>
#include <swizzle/lexer/TokenInfo.hpp>

#include <cstddef>
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

    struct TokenizerFixture
    {
        std::deque<TokenInfo> tokens;
        CreateTokenCallback callback = CreateTokenCallback(tokens);

        Token token = Token(boost::string_view(""), TokenType::numeric_literal);
        FileInfo info = FileInfo("testfile");

        Tokenizer<CreateTokenCallback> tokenizer = Tokenizer<CreateTokenCallback>("messages.swizzle", callback);
    };

    TEST_FIXTURE(TokenizerFixture, verifyConstruction)
    {
    }

    struct InputIsAComment : public TokenizerFixture
    {
        const std::string s = "// this is a comment";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(InputIsAComment, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        for(std::size_t position = 0, end = sv.length(); position < end; ++position)
        {
            tokenizer.consume(sv, position);
        }

        tokenizer.flush();

        REQUIRE CHECK_EQUAL(1U, tokens.size());
        CHECK_EQUAL(TokenType::comment, tokens[0].token().type());
        CHECK_EQUAL(" this is a comment", tokens[0].token().to_string());

        // TODO: check file info is correct
        CHECK_EQUAL(1U, tokens[0].fileInfo().start().line());
        CHECK_EQUAL(1U, tokens[0].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[0].fileInfo().end().line());
        CHECK_EQUAL(21U, tokens[0].fileInfo().end().column());
    }
}
