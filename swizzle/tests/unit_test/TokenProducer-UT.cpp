#include "./platform/UnitTestSupport.hpp"
#include <swizzle/lexer/TokenProducer.hpp>

#include <swizzle/lexer/FileInfo.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/lexer/Token.hpp>

#include <deque>

namespace {

    using namespace swizzle::lexer;

    class Callback
    {
    public:
        Callback(std::deque<TokenInfo>& tokens)
            : tokens_(tokens)
        {
        }

        void operator()(const TokenInfo& info)
        {
            tokens_.push_back(info);
        }

    private:
        std::deque<TokenInfo>& tokens_;
    };

    struct TokenProducerFixture
    {
        std::deque<TokenInfo> tokens;
        Callback callback = Callback(tokens);

        TokenProducer<Callback> producer = TokenProducer<Callback>(callback);
        FileInfo info = FileInfo("test_file", LineInfo(1, 1), LineInfo(1, 5));
    };

    TEST_FIXTURE(TokenProducerFixture, verifyCallback)
    {
        Token token("test", 0, 4, TokenType::string);
        info = producer.produceToken(token, info);

        CHECK_EQUAL(1U, info.start().line());
        CHECK_EQUAL(5U, info.start().column());
        CHECK_EQUAL(1U, info.end().line());
        CHECK_EQUAL(9U, info.end().column());

        REQUIRE CHECK_EQUAL(1U, tokens.size());

        CHECK_EQUAL("test", tokens[0].token().to_string());
        CHECK_EQUAL(TokenType::string, tokens[0].token().type());

        CHECK_EQUAL(1U, tokens[0].fileInfo().start().line());
        CHECK_EQUAL(5U, tokens[0].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[0].fileInfo().end().line());
        CHECK_EQUAL(9U, tokens[0].fileInfo().end().column());
    }

    TEST_FIXTURE(TokenProducerFixture, verifyCallbackOnKeyword)
    {
        Token token("import", 0, 6, TokenType::string);
        producer.produceToken(token, info);

        REQUIRE CHECK_EQUAL(1U, tokens.size());

        CHECK_EQUAL("import", tokens[0].token().to_string());
        CHECK_EQUAL(TokenType::keyword, tokens[0].token().type());

        CHECK_EQUAL(1U, tokens[0].fileInfo().start().line());
        CHECK_EQUAL(5U, tokens[0].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[0].fileInfo().end().line());
        CHECK_EQUAL(11U, tokens[0].fileInfo().end().column());
    }

    TEST_FIXTURE(TokenProducerFixture, verifyCallbackOnEmptyToken)
    {
        Token token("", 0, 0, TokenType::string);
        producer.produceToken(token, info);

        REQUIRE CHECK_EQUAL(0U, tokens.size());
    }
}
