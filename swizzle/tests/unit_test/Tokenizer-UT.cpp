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

        Token token = Token(sv, 0, 0, TokenType::string);
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
        CHECK_EQUAL("// this is a comment", tokens[0].token().to_string());

        CHECK_EQUAL(1U, tokens[0].fileInfo().start().line());
        CHECK_EQUAL(1U, tokens[0].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[0].fileInfo().end().line());
        CHECK_EQUAL(21U, tokens[0].fileInfo().end().column());
    }

    struct InputIsAMultiLineComment : public TokenizerFixture
    {
        const std::string s = "// this is a multi-line \\\ncomment";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::string);
    };

    TEST_FIXTURE(InputIsAMultiLineComment, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        for(std::size_t position = 0, end = sv.length(); position < end; ++position)
        {
            tokenizer.consume(sv, position);
        }

        tokenizer.flush();

        REQUIRE CHECK_EQUAL(1U, tokens.size());
        CHECK_EQUAL(TokenType::multiline_comment, tokens[0].token().type());
        CHECK_EQUAL("// this is a multi-line \\\ncomment", tokens[0].token().to_string());

        CHECK_EQUAL(1U, tokens[0].fileInfo().start().line());
        CHECK_EQUAL(1U, tokens[0].fileInfo().start().column());
        CHECK_EQUAL(2U, tokens[0].fileInfo().end().line());
        CHECK_EQUAL(8U, tokens[0].fileInfo().end().column());
    }

    struct InputIsImportStatement : public TokenizerFixture
    {
        const std::string s = "import Type;";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::string);
    };

    TEST_FIXTURE(InputIsImportStatement, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        for(std::size_t position = 0, end = sv.length(); position < end; ++position)
        {
            tokenizer.consume(sv, position);
        }

        tokenizer.flush();

        REQUIRE CHECK_EQUAL(3U, tokens.size());

        CHECK_EQUAL(TokenType::keyword, tokens[0].token().type());
        CHECK_EQUAL("import", tokens[0].token().to_string());

        CHECK_EQUAL(1U, tokens[0].fileInfo().start().line());
        CHECK_EQUAL(1U, tokens[0].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[0].fileInfo().end().line());
        CHECK_EQUAL(7U, tokens[0].fileInfo().end().column());

        CHECK_EQUAL(TokenType::string, tokens[1].token().type());
        CHECK_EQUAL("Type", tokens[1].token().to_string());

        CHECK_EQUAL(1U, tokens[1].fileInfo().start().line());
        CHECK_EQUAL(8U, tokens[1].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[1].fileInfo().end().line());
        CHECK_EQUAL(12U, tokens[1].fileInfo().end().column());

        CHECK_EQUAL(TokenType::end_statement, tokens[2].token().type());
        CHECK_EQUAL(";", tokens[2].token().to_string());

        CHECK_EQUAL(1U, tokens[2].fileInfo().start().line());
        CHECK_EQUAL(12U, tokens[2].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[2].fileInfo().end().line());
        CHECK_EQUAL(13U, tokens[2].fileInfo().end().column());
    }

    struct InputIsImportStatementWithNamespaces : public TokenizerFixture
    {
        const std::string s = "import foo::bar::Type;\n";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::string);
    };

    TEST_FIXTURE(InputIsImportStatementWithNamespaces, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        for(std::size_t position = 0, end = sv.length(); position < end; ++position)
        {
            tokenizer.consume(sv, position);
        }

        tokenizer.flush();

        REQUIRE CHECK_EQUAL(9U, tokens.size());

        CHECK_EQUAL(TokenType::keyword, tokens[0].token().type());
        CHECK_EQUAL("import", tokens[0].token().to_string());

        CHECK_EQUAL(1U, tokens[0].fileInfo().start().line());
        CHECK_EQUAL(1U, tokens[0].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[0].fileInfo().end().line());
        CHECK_EQUAL(7U, tokens[0].fileInfo().end().column());

        CHECK_EQUAL(TokenType::string, tokens[1].token().type());
        CHECK_EQUAL("foo", tokens[1].token().to_string());

        CHECK_EQUAL(1U, tokens[1].fileInfo().start().line());
        CHECK_EQUAL(8U, tokens[1].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[1].fileInfo().end().line());
        CHECK_EQUAL(11U, tokens[1].fileInfo().end().column());

        CHECK_EQUAL(TokenType::colon, tokens[2].token().type());
        CHECK_EQUAL(":", tokens[2].token().to_string());

        CHECK_EQUAL(1U, tokens[2].fileInfo().start().line());
        CHECK_EQUAL(11U, tokens[2].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[2].fileInfo().end().line());
        CHECK_EQUAL(12U, tokens[2].fileInfo().end().column());

        CHECK_EQUAL(TokenType::colon, tokens[3].token().type());
        CHECK_EQUAL(":", tokens[3].token().to_string());

        CHECK_EQUAL(1U, tokens[3].fileInfo().start().line());
        CHECK_EQUAL(12U, tokens[3].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[3].fileInfo().end().line());
        CHECK_EQUAL(13U, tokens[3].fileInfo().end().column());

        CHECK_EQUAL(TokenType::string, tokens[4].token().type());
        CHECK_EQUAL("bar", tokens[4].token().to_string());

        CHECK_EQUAL(1U, tokens[4].fileInfo().start().line());
        CHECK_EQUAL(13U, tokens[4].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[4].fileInfo().end().line());
        CHECK_EQUAL(16U, tokens[4].fileInfo().end().column());

        CHECK_EQUAL(TokenType::colon, tokens[5].token().type());
        CHECK_EQUAL(":", tokens[5].token().to_string());

        CHECK_EQUAL(1U, tokens[5].fileInfo().start().line());
        CHECK_EQUAL(16U, tokens[5].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[5].fileInfo().end().line());
        CHECK_EQUAL(17U, tokens[5].fileInfo().end().column());

        CHECK_EQUAL(TokenType::colon, tokens[6].token().type());
        CHECK_EQUAL(":", tokens[6].token().to_string());

        CHECK_EQUAL(1U, tokens[6].fileInfo().start().line());
        CHECK_EQUAL(17U, tokens[6].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[6].fileInfo().end().line());
        CHECK_EQUAL(18U, tokens[6].fileInfo().end().column());

        CHECK_EQUAL(TokenType::string, tokens[7].token().type());
        CHECK_EQUAL("Type", tokens[7].token().to_string());

        CHECK_EQUAL(1U, tokens[7].fileInfo().start().line());
        CHECK_EQUAL(18U, tokens[7].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[7].fileInfo().end().line());
        CHECK_EQUAL(22U, tokens[7].fileInfo().end().column());

        CHECK_EQUAL(TokenType::end_statement, tokens[8].token().type());
        CHECK_EQUAL(";", tokens[8].token().to_string());

        CHECK_EQUAL(1U, tokens[8].fileInfo().start().line());
        CHECK_EQUAL(22U, tokens[8].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[8].fileInfo().end().line());
        CHECK_EQUAL(23U, tokens[8].fileInfo().end().column());
    }

    struct InputIsEnum : public TokenizerFixture
    {
        const std::string s = "enum Test {\n\tfield1,\n\tfield2,\n\tfield3,\n}";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::string);
    };

    TEST_FIXTURE(InputIsEnum, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        for(std::size_t position = 0, end = sv.length(); position < end; ++position)
        {
            tokenizer.consume(sv, position);
        }

        tokenizer.flush();

        REQUIRE CHECK_EQUAL(10U, tokens.size());

        CHECK_EQUAL(TokenType::keyword, tokens[0].token().type());
        CHECK_EQUAL("enum", tokens[0].token().to_string());

        CHECK_EQUAL(TokenType::string, tokens[1].token().type());
        CHECK_EQUAL("Test", tokens[1].token().to_string());

        CHECK_EQUAL(TokenType::l_brace, tokens[2].token().type());
        CHECK_EQUAL("{", tokens[2].token().to_string());

        CHECK_EQUAL(TokenType::string, tokens[3].token().type());
        CHECK_EQUAL("field1", tokens[3].token().to_string());

        CHECK_EQUAL(TokenType::comma, tokens[4].token().type());
        CHECK_EQUAL(",", tokens[4].token().to_string());

        CHECK_EQUAL(TokenType::string, tokens[5].token().type());
        CHECK_EQUAL("field2", tokens[5].token().to_string());

        CHECK_EQUAL(TokenType::comma, tokens[6].token().type());
        CHECK_EQUAL(",", tokens[6].token().to_string());

        CHECK_EQUAL(TokenType::string, tokens[7].token().type());
        CHECK_EQUAL("field3", tokens[7].token().to_string());

        CHECK_EQUAL(TokenType::comma, tokens[8].token().type());
        CHECK_EQUAL(",", tokens[8].token().to_string());

        CHECK_EQUAL(TokenType::r_brace, tokens[9].token().type());
        CHECK_EQUAL("}", tokens[9].token().to_string());
    }
}
