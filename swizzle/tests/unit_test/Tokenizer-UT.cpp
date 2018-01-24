#include "./ut_support/UnitTestSupport.hpp"
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

    struct InputIsNamespaceDeclaration : public TokenizerFixture
    {
        const std::string s = "namespace foo ;\n";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::string);
    };

    TEST_FIXTURE(InputIsNamespaceDeclaration, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        for(std::size_t position = 0, end = sv.length(); position < end; ++position)
        {
            tokenizer.consume(sv, position);
        }

        tokenizer.flush();

        REQUIRE CHECK_EQUAL(3U, tokens.size());

        CHECK_EQUAL(TokenType::keyword, tokens[0].token().type());
        CHECK_EQUAL("namespace", tokens[0].token().to_string());

        CHECK_EQUAL(TokenType::string, tokens[1].token().type());
        CHECK_EQUAL("foo", tokens[1].token().to_string());

        CHECK_EQUAL(TokenType::end_statement, tokens[2].token().type());
        CHECK_EQUAL(";", tokens[2].token().to_string());
    }

    struct InputIsChainedNamespaceDeclaration : public TokenizerFixture
    {
        const std::string s = "namespace foo::bar    ;\n";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::string);
    };

    TEST_FIXTURE(InputIsChainedNamespaceDeclaration, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        for(std::size_t position = 0, end = sv.length(); position < end; ++position)
        {
            tokenizer.consume(sv, position);
        }

        tokenizer.flush();

        REQUIRE CHECK_EQUAL(6U, tokens.size());

        CHECK_EQUAL(TokenType::keyword, tokens[0].token().type());
        CHECK_EQUAL("namespace", tokens[0].token().to_string());

        CHECK_EQUAL(TokenType::string, tokens[1].token().type());
        CHECK_EQUAL("foo", tokens[1].token().to_string());

        CHECK_EQUAL(TokenType::colon, tokens[2].token().type());
        CHECK_EQUAL(":", tokens[2].token().to_string());

        CHECK_EQUAL(TokenType::colon, tokens[3].token().type());
        CHECK_EQUAL(":", tokens[3].token().to_string());

        CHECK_EQUAL(TokenType::string, tokens[4].token().type());
        CHECK_EQUAL("bar", tokens[4].token().to_string());

        CHECK_EQUAL(TokenType::end_statement, tokens[5].token().type());
        CHECK_EQUAL(";", tokens[5].token().to_string());

    }

    struct InputIsFloatingPoint : public TokenizerFixture
    {
        const std::string s = "42.24";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::string);
    };

    TEST_FIXTURE(InputIsFloatingPoint, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        for(std::size_t position = 0, end = sv.length(); position < end; ++position)
        {
            tokenizer.consume(sv, position);
        }

        tokenizer.flush();

        REQUIRE CHECK_EQUAL(1U, tokens.size());
        CHECK_EQUAL(TokenType::float_literal, tokens[0].token().type());
        CHECK_EQUAL("42.24", tokens[0].token().to_string());
    }

    struct InputIsRange : public TokenizerFixture
    {
        const std::string s = "42..24";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::string);
    };

    TEST_FIXTURE(InputIsRange, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        for(std::size_t position = 0, end = sv.length(); position < end; ++position)
        {
            tokenizer.consume(sv, position);
        }

        tokenizer.flush();

        REQUIRE CHECK_EQUAL(4U, tokens.size());

        CHECK_EQUAL(TokenType::numeric_literal, tokens[0].token().type());
        CHECK_EQUAL("42", tokens[0].token().to_string());

        CHECK_EQUAL(1U, tokens[0].fileInfo().start().line());
        CHECK_EQUAL(1U, tokens[0].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[0].fileInfo().end().line());
        CHECK_EQUAL(3U, tokens[0].fileInfo().end().column());

        CHECK_EQUAL(TokenType::dot, tokens[1].token().type());
        CHECK_EQUAL(".", tokens[1].token().to_string());

        CHECK_EQUAL(1U, tokens[1].fileInfo().start().line());
        CHECK_EQUAL(3U, tokens[1].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[1].fileInfo().end().line());
        CHECK_EQUAL(4U, tokens[1].fileInfo().end().column());

        CHECK_EQUAL(TokenType::dot, tokens[2].token().type());
        CHECK_EQUAL(".", tokens[2].token().to_string());

        CHECK_EQUAL(1U, tokens[2].fileInfo().start().line());
        CHECK_EQUAL(4U, tokens[2].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[2].fileInfo().end().line());
        CHECK_EQUAL(5U, tokens[2].fileInfo().end().column());

        CHECK_EQUAL(TokenType::numeric_literal, tokens[3].token().type());
        CHECK_EQUAL("24", tokens[3].token().to_string());

        CHECK_EQUAL(1U, tokens[3].fileInfo().start().line());
        CHECK_EQUAL(5U, tokens[3].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[3].fileInfo().end().line());
        CHECK_EQUAL(7U, tokens[3].fileInfo().end().column());
    }

    struct InputIsEnum : public TokenizerFixture
    {
        // NOTE: this will be a syntax error on parse as underlying type ": <type>" is required
        // after an enum declaration.
        const std::string s = "enum Test {\n    field1,\n    field2,\n    field3,\n}";
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

        CHECK_EQUAL(1U, tokens[0].fileInfo().start().line());
        CHECK_EQUAL(1U, tokens[0].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[0].fileInfo().end().line());
        CHECK_EQUAL(5U, tokens[0].fileInfo().end().column());

        CHECK_EQUAL(TokenType::string, tokens[1].token().type());
        CHECK_EQUAL("Test", tokens[1].token().to_string());

        CHECK_EQUAL(1U, tokens[1].fileInfo().start().line());
        CHECK_EQUAL(6U, tokens[1].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[1].fileInfo().end().line());
        CHECK_EQUAL(10U, tokens[1].fileInfo().end().column());

        CHECK_EQUAL(TokenType::l_brace, tokens[2].token().type());
        CHECK_EQUAL("{", tokens[2].token().to_string());

        CHECK_EQUAL(1U, tokens[2].fileInfo().start().line());
        CHECK_EQUAL(11U, tokens[2].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[2].fileInfo().end().line());
        CHECK_EQUAL(12U, tokens[2].fileInfo().end().column());

        CHECK_EQUAL(TokenType::string, tokens[3].token().type());
        CHECK_EQUAL("field1", tokens[3].token().to_string());

        CHECK_EQUAL(2U, tokens[3].fileInfo().start().line());
        CHECK_EQUAL(5U, tokens[3].fileInfo().start().column());
        CHECK_EQUAL(2U, tokens[3].fileInfo().end().line());
        CHECK_EQUAL(11U, tokens[3].fileInfo().end().column());

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

    struct InputIsEnumWithUnderlyingTypeAndDefaultValues : public TokenizerFixture
    {
        const std::string s = "enum Test : u8 {\n  field1 = 1,\n  field2 = 0x02,\n  field3,\n}";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::string);
    };

    TEST_FIXTURE(InputIsEnumWithUnderlyingTypeAndDefaultValues, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        for(std::size_t position = 0, end = sv.length(); position < end; ++position)
        {
            tokenizer.consume(sv, position);
        }

        tokenizer.flush();

        REQUIRE CHECK_EQUAL(16U, tokens.size());

        CHECK_EQUAL(TokenType::keyword, tokens[0].token().type());
        CHECK_EQUAL("enum", tokens[0].token().to_string());

        CHECK_EQUAL(TokenType::string, tokens[1].token().type());
        CHECK_EQUAL("Test", tokens[1].token().to_string());

        CHECK_EQUAL(TokenType::colon, tokens[2].token().type());
        CHECK_EQUAL(":", tokens[2].token().to_string());

        CHECK_EQUAL(TokenType::type, tokens[3].token().type());
        CHECK_EQUAL("u8", tokens[3].token().to_string());

        CHECK_EQUAL(TokenType::l_brace, tokens[4].token().type());
        CHECK_EQUAL("{", tokens[4].token().to_string());

        CHECK_EQUAL(TokenType::string, tokens[5].token().type());
        CHECK_EQUAL("field1", tokens[5].token().to_string());

        CHECK_EQUAL(TokenType::equal, tokens[6].token().type());
        CHECK_EQUAL("=", tokens[6].token().to_string());

        CHECK_EQUAL(TokenType::numeric_literal, tokens[7].token().type());
        CHECK_EQUAL("1", tokens[7].token().to_string());

        CHECK_EQUAL(TokenType::comma, tokens[8].token().type());
        CHECK_EQUAL(",", tokens[8].token().to_string());

        CHECK_EQUAL(TokenType::string, tokens[9].token().type());
        CHECK_EQUAL("field2", tokens[9].token().to_string());

        CHECK_EQUAL(TokenType::equal, tokens[10].token().type());
        CHECK_EQUAL("=", tokens[10].token().to_string());

        CHECK_EQUAL(TokenType::hex_literal, tokens[11].token().type());
        CHECK_EQUAL("0x02", tokens[11].token().to_string());

        CHECK_EQUAL(TokenType::comma, tokens[12].token().type());
        CHECK_EQUAL(",", tokens[12].token().to_string());

        CHECK_EQUAL(TokenType::string, tokens[13].token().type());
        CHECK_EQUAL("field3", tokens[13].token().to_string());

        CHECK_EQUAL(TokenType::comma, tokens[14].token().type());
        CHECK_EQUAL(",", tokens[14].token().to_string());

        CHECK_EQUAL(TokenType::r_brace, tokens[15].token().type());
        CHECK_EQUAL("}", tokens[15].token().to_string());
    }

    struct InputIsBitfield : public TokenizerFixture
    {
        const std::string s = "bitfield Test : u8 { field1 : 0, field2 : 1, field3 : 2..3, }";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::string);
    };

    TEST_FIXTURE(InputIsBitfield, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        for(std::size_t position = 0, end = sv.length(); position < end; ++position)
        {
            tokenizer.consume(sv, position);
        }

        tokenizer.flush();

        REQUIRE CHECK_EQUAL(21U, tokens.size());

        CHECK_EQUAL(TokenType::type, tokens[0].token().type());
        CHECK_EQUAL("bitfield", tokens[0].token().to_string());

        CHECK_EQUAL(TokenType::string, tokens[1].token().type());
        CHECK_EQUAL("Test", tokens[1].token().to_string());

        CHECK_EQUAL(TokenType::colon, tokens[2].token().type());
        CHECK_EQUAL(":", tokens[2].token().to_string());

        CHECK_EQUAL(TokenType::type, tokens[3].token().type());
        CHECK_EQUAL("u8", tokens[3].token().to_string());

        CHECK_EQUAL(TokenType::l_brace, tokens[4].token().type());
        CHECK_EQUAL("{", tokens[4].token().to_string());

        CHECK_EQUAL(TokenType::string, tokens[5].token().type());
        CHECK_EQUAL("field1", tokens[5].token().to_string());

        CHECK_EQUAL(TokenType::colon, tokens[6].token().type());
        CHECK_EQUAL(":", tokens[6].token().to_string());

        CHECK_EQUAL(TokenType::numeric_literal, tokens[7].token().type());
        CHECK_EQUAL("0", tokens[7].token().to_string());

        CHECK_EQUAL(TokenType::comma, tokens[8].token().type());
        CHECK_EQUAL(",", tokens[8].token().to_string());

        CHECK_EQUAL(TokenType::string, tokens[9].token().type());
        CHECK_EQUAL("field2", tokens[9].token().to_string());

        CHECK_EQUAL(TokenType::colon, tokens[10].token().type());
        CHECK_EQUAL(":", tokens[10].token().to_string());

        CHECK_EQUAL(TokenType::numeric_literal, tokens[11].token().type());
        CHECK_EQUAL("1", tokens[11].token().to_string());

        CHECK_EQUAL(TokenType::comma, tokens[12].token().type());
        CHECK_EQUAL(",", tokens[12].token().to_string());

        CHECK_EQUAL(TokenType::string, tokens[13].token().type());
        CHECK_EQUAL("field3", tokens[13].token().to_string());

        CHECK_EQUAL(TokenType::colon, tokens[14].token().type());
        CHECK_EQUAL(":", tokens[14].token().to_string());

        CHECK_EQUAL(TokenType::numeric_literal, tokens[15].token().type());
        CHECK_EQUAL("2", tokens[15].token().to_string());

        CHECK_EQUAL(TokenType::dot, tokens[16].token().type());
        CHECK_EQUAL(".", tokens[16].token().to_string());

        CHECK_EQUAL(TokenType::dot, tokens[17].token().type());
        CHECK_EQUAL(".", tokens[17].token().to_string());

        CHECK_EQUAL(TokenType::numeric_literal, tokens[18].token().type());
        CHECK_EQUAL("3", tokens[18].token().to_string());

        CHECK_EQUAL(TokenType::comma, tokens[19].token().type());
        CHECK_EQUAL(",", tokens[19].token().to_string());

        CHECK_EQUAL(TokenType::r_brace, tokens[20].token().type());
        CHECK_EQUAL("}", tokens[20].token().to_string());
    }

    struct InputIsUsingStatement : public TokenizerFixture
    {
        const std::string s = "    \n  using Byte = u8;";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::string);
    };

    TEST_FIXTURE(InputIsUsingStatement, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        for(std::size_t position = 0, end = sv.length(); position < end; ++position)
        {
            tokenizer.consume(sv, position);
        }

        tokenizer.flush();

        REQUIRE CHECK_EQUAL(5U, tokens.size());

        CHECK_EQUAL(TokenType::keyword, tokens[0].token().type());
        CHECK_EQUAL("using", tokens[0].token().to_string());

        CHECK_EQUAL(2U, tokens[0].fileInfo().start().line());
        CHECK_EQUAL(3U, tokens[0].fileInfo().start().column());
        CHECK_EQUAL(2U, tokens[0].fileInfo().end().line());
        CHECK_EQUAL(8U, tokens[0].fileInfo().end().column());

        CHECK_EQUAL(TokenType::string, tokens[1].token().type());
        CHECK_EQUAL("Byte", tokens[1].token().to_string());

        CHECK_EQUAL(2U, tokens[1].fileInfo().start().line());
        CHECK_EQUAL(9U, tokens[1].fileInfo().start().column());
        CHECK_EQUAL(2U, tokens[1].fileInfo().end().line());
        CHECK_EQUAL(13U, tokens[1].fileInfo().end().column());

        CHECK_EQUAL(TokenType::equal, tokens[2].token().type());
        CHECK_EQUAL("=", tokens[2].token().to_string());

        CHECK_EQUAL(2U, tokens[2].fileInfo().start().line());
        CHECK_EQUAL(14U, tokens[2].fileInfo().start().column());
        CHECK_EQUAL(2U, tokens[2].fileInfo().end().line());
        CHECK_EQUAL(15U, tokens[2].fileInfo().end().column());

        CHECK_EQUAL(TokenType::type, tokens[3].token().type());
        CHECK_EQUAL("u8", tokens[3].token().to_string());

        CHECK_EQUAL(2U, tokens[3].fileInfo().start().line());
        CHECK_EQUAL(16U, tokens[3].fileInfo().start().column());
        CHECK_EQUAL(2U, tokens[3].fileInfo().end().line());
        CHECK_EQUAL(18U, tokens[3].fileInfo().end().column());

        CHECK_EQUAL(TokenType::end_statement, tokens[4].token().type());
        CHECK_EQUAL(";", tokens[4].token().to_string());

        CHECK_EQUAL(2U, tokens[4].fileInfo().start().line());
        CHECK_EQUAL(18U, tokens[4].fileInfo().start().column());
        CHECK_EQUAL(2U, tokens[4].fileInfo().end().line());
        CHECK_EQUAL(19U, tokens[4].fileInfo().end().column());
    }

    struct InputIsUsingStatementWithUserDefinedType : public TokenizerFixture
    {
        const std::string s = "    \n  using Byte = MyByte;";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::string);
    };

    TEST_FIXTURE(InputIsUsingStatementWithUserDefinedType, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        for(std::size_t position = 0, end = sv.length(); position < end; ++position)
        {
            tokenizer.consume(sv, position);
        }

        tokenizer.flush();

        REQUIRE CHECK_EQUAL(5U, tokens.size());

        CHECK_EQUAL(TokenType::keyword, tokens[0].token().type());
        CHECK_EQUAL("using", tokens[0].token().to_string());

        CHECK_EQUAL(TokenType::string, tokens[1].token().type());
        CHECK_EQUAL("Byte", tokens[1].token().to_string());

        CHECK_EQUAL(TokenType::equal, tokens[2].token().type());
        CHECK_EQUAL("=", tokens[2].token().to_string());

        CHECK_EQUAL(TokenType::string, tokens[3].token().type());
        CHECK_EQUAL("MyByte", tokens[3].token().to_string());

        CHECK_EQUAL(TokenType::end_statement, tokens[4].token().type());
        CHECK_EQUAL(";", tokens[4].token().to_string());
    }

    // Simple struct with default values and inline comments
    struct InputIsStruct : public TokenizerFixture
    {
        const std::string s =
            "struct MessageBlock {" "\n"
            "    " "u16 length = 10;" "\n"
            "    " "u8 messageType = 'c'; // default to c" "\n"
            "}" "\n";

        const boost::string_view sv = boost::string_view(s);
        Token token = Token(sv, 0, 0, TokenType::string);
    };

    TEST_FIXTURE(InputIsStruct, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        for(std::size_t position = 0, end = sv.length(); position < end; ++position)
        {
            tokenizer.consume(sv, position);
        }

        tokenizer.flush();

        REQUIRE CHECK_EQUAL(15U, tokens.size());

        CHECK_EQUAL(TokenType::keyword, tokens[0].token().type());
        CHECK_EQUAL("struct", tokens[0].token().to_string());

        CHECK_EQUAL(1U, tokens[0].fileInfo().start().line());
        CHECK_EQUAL(1U, tokens[0].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[0].fileInfo().end().line());
        CHECK_EQUAL(7U, tokens[0].fileInfo().end().column());

        CHECK_EQUAL(TokenType::string, tokens[1].token().type());
        CHECK_EQUAL("MessageBlock", tokens[1].token().to_string());

        CHECK_EQUAL(1U, tokens[1].fileInfo().start().line());
        CHECK_EQUAL(8U, tokens[1].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[1].fileInfo().end().line());
        CHECK_EQUAL(20U, tokens[1].fileInfo().end().column());

        CHECK_EQUAL(TokenType::l_brace, tokens[2].token().type());
        CHECK_EQUAL("{", tokens[2].token().to_string());

        CHECK_EQUAL(1U, tokens[2].fileInfo().start().line());
        CHECK_EQUAL(21U, tokens[2].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[2].fileInfo().end().line());
        CHECK_EQUAL(22U, tokens[2].fileInfo().end().column());

        CHECK_EQUAL(TokenType::type, tokens[3].token().type());
        CHECK_EQUAL("u16", tokens[3].token().to_string());

        CHECK_EQUAL(2U, tokens[3].fileInfo().start().line());
        CHECK_EQUAL(5U, tokens[3].fileInfo().start().column());
        CHECK_EQUAL(2U, tokens[3].fileInfo().end().line());
        CHECK_EQUAL(8U, tokens[3].fileInfo().end().column());

        CHECK_EQUAL(TokenType::string, tokens[4].token().type());
        CHECK_EQUAL("length", tokens[4].token().to_string());

        CHECK_EQUAL(2U, tokens[4].fileInfo().start().line());
        CHECK_EQUAL(9U, tokens[4].fileInfo().start().column());
        CHECK_EQUAL(2U, tokens[4].fileInfo().end().line());
        CHECK_EQUAL(15U, tokens[4].fileInfo().end().column());

        CHECK_EQUAL(TokenType::equal, tokens[5].token().type());
        CHECK_EQUAL("=", tokens[5].token().to_string());

        CHECK_EQUAL(2U, tokens[5].fileInfo().start().line());
        CHECK_EQUAL(16U, tokens[5].fileInfo().start().column());
        CHECK_EQUAL(2U, tokens[5].fileInfo().end().line());
        CHECK_EQUAL(17U, tokens[5].fileInfo().end().column());

        CHECK_EQUAL(TokenType::numeric_literal, tokens[6].token().type());
        CHECK_EQUAL("10", tokens[6].token().to_string());

        CHECK_EQUAL(2U, tokens[6].fileInfo().start().line());
        CHECK_EQUAL(18U, tokens[6].fileInfo().start().column());
        CHECK_EQUAL(2U, tokens[6].fileInfo().end().line());
        CHECK_EQUAL(20U, tokens[6].fileInfo().end().column());

        CHECK_EQUAL(TokenType::end_statement, tokens[7].token().type());
        CHECK_EQUAL(";", tokens[7].token().to_string());

        CHECK_EQUAL(2U, tokens[7].fileInfo().start().line());
        CHECK_EQUAL(20U, tokens[7].fileInfo().start().column());
        CHECK_EQUAL(2U, tokens[7].fileInfo().end().line());
        CHECK_EQUAL(21U, tokens[7].fileInfo().end().column());

        CHECK_EQUAL(TokenType::type, tokens[8].token().type());
        CHECK_EQUAL("u8", tokens[8].token().to_string());

        CHECK_EQUAL(3U, tokens[8].fileInfo().start().line());
        CHECK_EQUAL(5U, tokens[8].fileInfo().start().column());
        CHECK_EQUAL(3U, tokens[8].fileInfo().end().line());
        CHECK_EQUAL(7U, tokens[8].fileInfo().end().column());

        CHECK_EQUAL(TokenType::string, tokens[9].token().type());
        CHECK_EQUAL("messageType", tokens[9].token().to_string());

        CHECK_EQUAL(3U, tokens[9].fileInfo().start().line());
        CHECK_EQUAL(8U, tokens[9].fileInfo().start().column());
        CHECK_EQUAL(3U, tokens[9].fileInfo().end().line());
        CHECK_EQUAL(19U, tokens[9].fileInfo().end().column());

        CHECK_EQUAL(TokenType::equal, tokens[10].token().type());
        CHECK_EQUAL("=", tokens[10].token().to_string());

        CHECK_EQUAL(3U, tokens[10].fileInfo().start().line());
        CHECK_EQUAL(20U, tokens[10].fileInfo().start().column());
        CHECK_EQUAL(3U, tokens[10].fileInfo().end().line());
        CHECK_EQUAL(21U, tokens[10].fileInfo().end().column());

        CHECK_EQUAL(TokenType::char_literal, tokens[11].token().type());
        CHECK_EQUAL("'c'", tokens[11].token().to_string());

        CHECK_EQUAL(3U, tokens[11].fileInfo().start().line());
        CHECK_EQUAL(22U, tokens[11].fileInfo().start().column());
        CHECK_EQUAL(3U, tokens[11].fileInfo().end().line());
        CHECK_EQUAL(25U, tokens[11].fileInfo().end().column());

        CHECK_EQUAL(TokenType::end_statement, tokens[12].token().type());
        CHECK_EQUAL(";", tokens[12].token().to_string());

        CHECK_EQUAL(3U, tokens[12].fileInfo().start().line());
        CHECK_EQUAL(25U, tokens[12].fileInfo().start().column());
        CHECK_EQUAL(3U, tokens[12].fileInfo().end().line());
        CHECK_EQUAL(26U, tokens[12].fileInfo().end().column());

        CHECK_EQUAL(TokenType::comment, tokens[13].token().type());
        CHECK_EQUAL("// default to c", tokens[13].token().to_string());

        CHECK_EQUAL(3U, tokens[13].fileInfo().start().line());
        CHECK_EQUAL(27U, tokens[13].fileInfo().start().column());
        CHECK_EQUAL(3U, tokens[13].fileInfo().end().line());
        CHECK_EQUAL(42U, tokens[13].fileInfo().end().column());

        CHECK_EQUAL(TokenType::r_brace, tokens[14].token().type());
        CHECK_EQUAL("}", tokens[14].token().to_string());

        CHECK_EQUAL(4U, tokens[14].fileInfo().start().line());
        CHECK_EQUAL(1U, tokens[14].fileInfo().start().column());
        CHECK_EQUAL(4U, tokens[14].fileInfo().end().line());
        CHECK_EQUAL(2U, tokens[14].fileInfo().end().column());
    }

    struct InputIsStructWithAttributes : public TokenizerFixture
    {
        const std::string s =
            "@input" "\n"
            "struct Message {" "\n"
            "    " "fix::Side side;" "\n"
            "    " "fix::Price price;" "\n\n"
            "    " "@big_endian " "\n"
            "    " "u16 flags;" "\n"
            "}"
            "\n";


        const boost::string_view sv = boost::string_view(s);
        Token token = Token(sv, 0, 0, TokenType::string);
    };

    TEST_FIXTURE(InputIsStructWithAttributes, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        for(std::size_t position = 0, end = sv.length(); position < end; ++position)
        {
            tokenizer.consume(sv, position);
        }

        tokenizer.flush();

        REQUIRE CHECK_EQUAL(21U, tokens.size());

        CHECK_EQUAL(TokenType::attribute, tokens[0].token().type());
        CHECK_EQUAL("@input", tokens[0].token().to_string());

        CHECK_EQUAL(TokenType::keyword, tokens[1].token().type());
        CHECK_EQUAL("struct", tokens[1].token().to_string());

        CHECK_EQUAL(TokenType::string, tokens[2].token().type());
        CHECK_EQUAL("Message", tokens[2].token().to_string());

        CHECK_EQUAL(TokenType::l_brace, tokens[3].token().type());
        CHECK_EQUAL("{", tokens[3].token().to_string());

        CHECK_EQUAL(TokenType::string, tokens[4].token().type());
        CHECK_EQUAL("fix", tokens[4].token().to_string());

        CHECK_EQUAL(TokenType::colon, tokens[5].token().type());
        CHECK_EQUAL(":", tokens[5].token().to_string());

        CHECK_EQUAL(TokenType::colon, tokens[6].token().type());
        CHECK_EQUAL(":", tokens[6].token().to_string());

        CHECK_EQUAL(TokenType::string, tokens[7].token().type());
        CHECK_EQUAL("Side", tokens[7].token().to_string());

        CHECK_EQUAL(TokenType::string, tokens[8].token().type());
        CHECK_EQUAL("side", tokens[8].token().to_string());

        CHECK_EQUAL(TokenType::end_statement, tokens[9].token().type());
        CHECK_EQUAL(";", tokens[9].token().to_string());

        CHECK_EQUAL(TokenType::string, tokens[10].token().type());
        CHECK_EQUAL("fix", tokens[10].token().to_string());

        CHECK_EQUAL(TokenType::colon, tokens[11].token().type());
        CHECK_EQUAL(":", tokens[11].token().to_string());

        CHECK_EQUAL(TokenType::colon, tokens[12].token().type());
        CHECK_EQUAL(":", tokens[12].token().to_string());

        CHECK_EQUAL(TokenType::string, tokens[13].token().type());
        CHECK_EQUAL("Price", tokens[13].token().to_string());

        CHECK_EQUAL(TokenType::string, tokens[14].token().type());
        CHECK_EQUAL("price", tokens[14].token().to_string());

        CHECK_EQUAL(TokenType::end_statement, tokens[15].token().type());
        CHECK_EQUAL(";", tokens[15].token().to_string());

        CHECK_EQUAL(TokenType::attribute, tokens[16].token().type());
        CHECK_EQUAL("@big_endian", tokens[16].token().to_string());

        CHECK_EQUAL(TokenType::type, tokens[17].token().type());
        CHECK_EQUAL("u16", tokens[17].token().to_string());

        CHECK_EQUAL(TokenType::string, tokens[18].token().type());
        CHECK_EQUAL("flags", tokens[18].token().to_string());

        CHECK_EQUAL(TokenType::end_statement, tokens[19].token().type());
        CHECK_EQUAL(";", tokens[19].token().to_string());

        CHECK_EQUAL(TokenType::r_brace, tokens[20].token().type());
        CHECK_EQUAL("}", tokens[20].token().to_string());

        CHECK_EQUAL(8U, tokens[20].fileInfo().start().line());
        CHECK_EQUAL(1U, tokens[20].fileInfo().start().column());
        CHECK_EQUAL(8U, tokens[20].fileInfo().end().line());
        CHECK_EQUAL(2U, tokens[20].fileInfo().end().column());
    }

    struct InputIsStructWithArrayAndVectorMember : public TokenizerFixture
    {
        const std::string s =
            "struct Message {"          "\n"
            "    " "u8[10] fixed ;"       "\n"       // array
            "    " "u8 size;"             "\n"
            "    " "u8[size] varstring;"  "\n"
            "}"                         "\n";


        const boost::string_view sv = boost::string_view(s);
        Token token = Token(sv, 0, 0, TokenType::string);
    };

    TEST_FIXTURE(InputIsStructWithArrayAndVectorMember, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        for(std::size_t position = 0, end = sv.length(); position < end; ++position)
        {
            tokenizer.consume(sv, position);
        }

        tokenizer.flush();

        REQUIRE CHECK_EQUAL(19U, tokens.size());

        CHECK_EQUAL(TokenType::keyword, tokens[0].token().type());
        CHECK_EQUAL("struct", tokens[0].token().to_string());

        CHECK_EQUAL(TokenType::string, tokens[1].token().type());
        CHECK_EQUAL("Message", tokens[1].token().to_string());

        CHECK_EQUAL(TokenType::l_brace, tokens[2].token().type());
        CHECK_EQUAL("{", tokens[2].token().to_string());

        CHECK_EQUAL(TokenType::type, tokens[3].token().type());
        CHECK_EQUAL("u8", tokens[3].token().to_string());

        CHECK_EQUAL(TokenType::l_bracket, tokens[4].token().type());
        CHECK_EQUAL("[", tokens[4].token().to_string());

        CHECK_EQUAL(TokenType::numeric_literal, tokens[5].token().type());
        CHECK_EQUAL("10", tokens[5].token().to_string());

        CHECK_EQUAL(TokenType::r_bracket, tokens[6].token().type());
        CHECK_EQUAL("]", tokens[6].token().to_string());

        CHECK_EQUAL(TokenType::string, tokens[7].token().type());
        CHECK_EQUAL("fixed", tokens[7].token().to_string());

        CHECK_EQUAL(TokenType::end_statement, tokens[8].token().type());
        CHECK_EQUAL(";", tokens[8].token().to_string());

        CHECK_EQUAL(TokenType::type, tokens[9].token().type());
        CHECK_EQUAL("u8", tokens[9].token().to_string());

        CHECK_EQUAL(TokenType::string, tokens[10].token().type());
        CHECK_EQUAL("size", tokens[10].token().to_string());

        CHECK_EQUAL(TokenType::end_statement, tokens[11].token().type());
        CHECK_EQUAL(";", tokens[11].token().to_string());

        CHECK_EQUAL(TokenType::type, tokens[12].token().type());
        CHECK_EQUAL("u8", tokens[12].token().to_string());

        CHECK_EQUAL(TokenType::l_bracket, tokens[13].token().type());
        CHECK_EQUAL("[", tokens[13].token().to_string());

        CHECK_EQUAL(TokenType::string, tokens[14].token().type());
        CHECK_EQUAL("size", tokens[14].token().to_string());

        CHECK_EQUAL(TokenType::r_bracket, tokens[15].token().type());
        CHECK_EQUAL("]", tokens[15].token().to_string());

        CHECK_EQUAL(TokenType::string, tokens[16].token().type());
        CHECK_EQUAL("varstring", tokens[16].token().to_string());

        CHECK_EQUAL(TokenType::end_statement, tokens[17].token().type());
        CHECK_EQUAL(";", tokens[17].token().to_string());

        CHECK_EQUAL(TokenType::r_brace, tokens[18].token().type());
        CHECK_EQUAL("}", tokens[18].token().to_string());
    }

    struct InputIsStructWithVectorSizedByNestedField : public TokenizerFixture
    {
        const std::string s =
            "struct Message {"                  "\n"
            "    " "Info info;"                 "\n"
            "    " "u8[info.size] varstring;"   "\n"
            "}"                                 "\n";

        const boost::string_view sv = boost::string_view(s);
        Token token = Token(sv, 0, 0, TokenType::string);
    };

    TEST_FIXTURE(InputIsStructWithVectorSizedByNestedField, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        for(std::size_t position = 0, end = sv.length(); position < end; ++position)
        {
            tokenizer.consume(sv, position);
        }

        tokenizer.flush();

        REQUIRE CHECK_EQUAL(15U, tokens.size());

        CHECK_EQUAL(TokenType::keyword, tokens[0].token().type());
        CHECK_EQUAL("struct", tokens[0].token().to_string());

        CHECK_EQUAL(TokenType::string, tokens[1].token().type());
        CHECK_EQUAL("Message", tokens[1].token().to_string());

        CHECK_EQUAL(TokenType::l_brace, tokens[2].token().type());
        CHECK_EQUAL("{", tokens[2].token().to_string());

        CHECK_EQUAL(TokenType::string, tokens[3].token().type());
        CHECK_EQUAL("Info", tokens[3].token().to_string());

        CHECK_EQUAL(TokenType::string, tokens[4].token().type());
        CHECK_EQUAL("info", tokens[4].token().to_string());

        CHECK_EQUAL(TokenType::end_statement, tokens[5].token().type());
        CHECK_EQUAL(";", tokens[5].token().to_string());

        CHECK_EQUAL(TokenType::type, tokens[6].token().type());
        CHECK_EQUAL("u8", tokens[6].token().to_string());

        CHECK_EQUAL(TokenType::l_bracket, tokens[7].token().type());
        CHECK_EQUAL("[", tokens[7].token().to_string());

        CHECK_EQUAL(TokenType::string, tokens[8].token().type());
        CHECK_EQUAL("info", tokens[8].token().to_string());

        CHECK_EQUAL(TokenType::dot, tokens[9].token().type());
        CHECK_EQUAL(".", tokens[9].token().to_string());

        CHECK_EQUAL(TokenType::string, tokens[10].token().type());
        CHECK_EQUAL("size", tokens[10].token().to_string());

        CHECK_EQUAL(TokenType::r_bracket, tokens[11].token().type());
        CHECK_EQUAL("]", tokens[11].token().to_string());

        CHECK_EQUAL(TokenType::string, tokens[12].token().type());
        CHECK_EQUAL("varstring", tokens[12].token().to_string());

        CHECK_EQUAL(TokenType::end_statement, tokens[13].token().type());
        CHECK_EQUAL(";", tokens[13].token().to_string());

        CHECK_EQUAL(TokenType::r_brace, tokens[14].token().type());
        CHECK_EQUAL("}", tokens[14].token().to_string());
    }

    struct InputIsStructWithFieldLabel : public TokenizerFixture
    {
        const std::string s =
            "struct Message {"                  "\n"
            "  " "115:  Info info;"             "\n"
            "}"                                 "\n";

        const boost::string_view sv = boost::string_view(s);
        Token token = Token(sv, 0, 0, TokenType::string);
    };

    TEST_FIXTURE(InputIsStructWithFieldLabel, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        for(std::size_t position = 0, end = sv.length(); position < end; ++position)
        {
            tokenizer.consume(sv, position);
        }

        tokenizer.flush();

        REQUIRE CHECK_EQUAL(9U, tokens.size());

        CHECK_EQUAL(TokenType::keyword, tokens[0].token().type());
        CHECK_EQUAL("struct", tokens[0].token().to_string());

        CHECK_EQUAL(TokenType::string, tokens[1].token().type());
        CHECK_EQUAL("Message", tokens[1].token().to_string());

        CHECK_EQUAL(TokenType::l_brace, tokens[2].token().type());
        CHECK_EQUAL("{", tokens[2].token().to_string());

        CHECK_EQUAL(TokenType::numeric_literal, tokens[3].token().type());
        CHECK_EQUAL("115", tokens[3].token().to_string());

        CHECK_EQUAL(TokenType::colon, tokens[4].token().type());
        CHECK_EQUAL(":", tokens[4].token().to_string());

        CHECK_EQUAL(TokenType::string, tokens[5].token().type());
        CHECK_EQUAL("Info", tokens[5].token().to_string());

        CHECK_EQUAL(TokenType::string, tokens[6].token().type());
        CHECK_EQUAL("info", tokens[6].token().to_string());

        CHECK_EQUAL(TokenType::end_statement, tokens[7].token().type());
        CHECK_EQUAL(";", tokens[7].token().to_string());

        CHECK_EQUAL(TokenType::r_brace, tokens[8].token().type());
        CHECK_EQUAL("}", tokens[8].token().to_string());
    }

    struct InputIsStructWithKeyValueAttribute : public TokenizerFixture
    {
        const std::string s =
            "struct Message {"                  "\n"
            "  " "@max=100"                     "\n"
            "  " "@min=-100"                    "\n"
            "  " "i8 size;"                     "\n"
            "}"                                 "\n";

        const boost::string_view sv = boost::string_view(s);
        Token token = Token(sv, 0, 0, TokenType::string);
    };

    TEST_FIXTURE(InputIsStructWithKeyValueAttribute, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        for(std::size_t position = 0, end = sv.length(); position < end; ++position)
        {
            tokenizer.consume(sv, position);
        }

        tokenizer.flush();

        REQUIRE CHECK_EQUAL(13U, tokens.size());

        CHECK_EQUAL(TokenType::keyword, tokens[0].token().type());
        CHECK_EQUAL("struct", tokens[0].token().to_string());

        CHECK_EQUAL(TokenType::string, tokens[1].token().type());
        CHECK_EQUAL("Message", tokens[1].token().to_string());

        CHECK_EQUAL(TokenType::l_brace, tokens[2].token().type());
        CHECK_EQUAL("{", tokens[2].token().to_string());

        CHECK_EQUAL(TokenType::attribute, tokens[3].token().type());
        CHECK_EQUAL("@max", tokens[3].token().to_string());

        CHECK_EQUAL(TokenType::equal, tokens[4].token().type());
        CHECK_EQUAL("=", tokens[4].token().to_string());

        CHECK_EQUAL(TokenType::numeric_literal, tokens[5].token().type());
        CHECK_EQUAL("100", tokens[5].token().to_string());

        CHECK_EQUAL(TokenType::attribute, tokens[6].token().type());
        CHECK_EQUAL("@min", tokens[6].token().to_string());

        CHECK_EQUAL(TokenType::equal, tokens[7].token().type());
        CHECK_EQUAL("=", tokens[7].token().to_string());

        CHECK_EQUAL(TokenType::numeric_literal, tokens[8].token().type());
        CHECK_EQUAL("-100", tokens[8].token().to_string());

        CHECK_EQUAL(TokenType::type, tokens[9].token().type());
        CHECK_EQUAL("i8", tokens[9].token().to_string());

        CHECK_EQUAL(TokenType::string, tokens[10].token().type());
        CHECK_EQUAL("size", tokens[10].token().to_string());

        CHECK_EQUAL(TokenType::end_statement, tokens[11].token().type());
        CHECK_EQUAL(";", tokens[11].token().to_string());

        CHECK_EQUAL(TokenType::r_brace, tokens[12].token().type());
        CHECK_EQUAL("}", tokens[12].token().to_string());
    }

    struct InputIsStructWithBlockAttribute : public TokenizerFixture
    {
        const std::string s =
            "struct Message {"                      "\n"
            "  " "@attribute{size != 0}"            "\n"
            "  " "u8 size;"                         "\n"
            "}"                                     "\n";

        const boost::string_view sv = boost::string_view(s);
        Token token = Token(sv, 0, 0, TokenType::string);
    };

    TEST_FIXTURE(InputIsStructWithBlockAttribute, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        for(std::size_t position = 0, end = sv.length(); position < end; ++position)
        {
            tokenizer.consume(sv, position);
        }

        tokenizer.flush();

        REQUIRE CHECK_EQUAL(9U, tokens.size());

        CHECK_EQUAL(TokenType::keyword, tokens[0].token().type());
        CHECK_EQUAL("struct", tokens[0].token().to_string());

        CHECK_EQUAL(1U, tokens[0].fileInfo().start().line());
        CHECK_EQUAL(1U, tokens[0].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[0].fileInfo().end().line());
        CHECK_EQUAL(7U, tokens[0].fileInfo().end().column());

        CHECK_EQUAL(TokenType::string, tokens[1].token().type());
        CHECK_EQUAL("Message", tokens[1].token().to_string());

        CHECK_EQUAL(1U, tokens[1].fileInfo().start().line());
        CHECK_EQUAL(8U, tokens[1].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[1].fileInfo().end().line());
        CHECK_EQUAL(15U, tokens[1].fileInfo().end().column());

        CHECK_EQUAL(TokenType::l_brace, tokens[2].token().type());
        CHECK_EQUAL("{", tokens[2].token().to_string());

        CHECK_EQUAL(1U, tokens[2].fileInfo().start().line());
        CHECK_EQUAL(16U, tokens[2].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[2].fileInfo().end().line());
        CHECK_EQUAL(17U, tokens[2].fileInfo().end().column());

        CHECK_EQUAL(TokenType::attribute, tokens[3].token().type());
        CHECK_EQUAL("@attribute", tokens[3].token().to_string());

        CHECK_EQUAL(2U, tokens[3].fileInfo().start().line());
        CHECK_EQUAL(3U, tokens[3].fileInfo().start().column());
        CHECK_EQUAL(2U, tokens[3].fileInfo().end().line());
        CHECK_EQUAL(13U, tokens[3].fileInfo().end().column());

        CHECK_EQUAL(TokenType::attribute_block, tokens[4].token().type());
        CHECK_EQUAL("{size != 0}", tokens[4].token().to_string());

        CHECK_EQUAL(2U, tokens[4].fileInfo().start().line());
        CHECK_EQUAL(13U, tokens[4].fileInfo().start().column());
        CHECK_EQUAL(2U, tokens[4].fileInfo().end().line());
        CHECK_EQUAL(24U, tokens[4].fileInfo().end().column());

        CHECK_EQUAL(TokenType::type, tokens[5].token().type());
        CHECK_EQUAL("u8", tokens[5].token().to_string());

        CHECK_EQUAL(3U, tokens[5].fileInfo().start().line());
        CHECK_EQUAL(3U, tokens[5].fileInfo().start().column());
        CHECK_EQUAL(3U, tokens[5].fileInfo().end().line());
        CHECK_EQUAL(5U, tokens[5].fileInfo().end().column());

        CHECK_EQUAL(TokenType::string, tokens[6].token().type());
        CHECK_EQUAL("size", tokens[6].token().to_string());

        CHECK_EQUAL(3U, tokens[6].fileInfo().start().line());
        CHECK_EQUAL(6U, tokens[6].fileInfo().start().column());
        CHECK_EQUAL(3U, tokens[6].fileInfo().end().line());
        CHECK_EQUAL(10U, tokens[6].fileInfo().end().column());

        CHECK_EQUAL(TokenType::end_statement, tokens[7].token().type());
        CHECK_EQUAL(";", tokens[7].token().to_string());

        CHECK_EQUAL(3U, tokens[7].fileInfo().start().line());
        CHECK_EQUAL(10U, tokens[7].fileInfo().start().column());
        CHECK_EQUAL(3U, tokens[7].fileInfo().end().line());
        CHECK_EQUAL(11U, tokens[7].fileInfo().end().column());

        CHECK_EQUAL(TokenType::r_brace, tokens[8].token().type());
        CHECK_EQUAL("}", tokens[8].token().to_string());

        CHECK_EQUAL(4U, tokens[8].fileInfo().start().line());
        CHECK_EQUAL(1U, tokens[8].fileInfo().start().column());
        CHECK_EQUAL(4U, tokens[8].fileInfo().end().line());
        CHECK_EQUAL(2U, tokens[8].fileInfo().end().column());
    }

    struct InputIsStructWithNumericConstants : public TokenizerFixture
    {
        const std::string s =
            "struct Message {"                  "\n"
            "  " "const u8 maxSize = 100;"      "\n"
            "  " "const i8 minSize = -20;"      "\n"
            "  " "const f32 f = -1.87;"         "\n"
            "}"                                 "\n";

        const boost::string_view sv = boost::string_view(s);
        Token token = Token(sv, 0, 0, TokenType::string);
    };

    TEST_FIXTURE(InputIsStructWithNumericConstants, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        for(std::size_t position = 0, end = sv.length(); position < end; ++position)
        {
            tokenizer.consume(sv, position);
        }

        tokenizer.flush();

        REQUIRE CHECK_EQUAL(22U, tokens.size());

        CHECK_EQUAL(TokenType::keyword, tokens[0].token().type());
        CHECK_EQUAL("struct", tokens[0].token().to_string());

        CHECK_EQUAL(TokenType::string, tokens[1].token().type());
        CHECK_EQUAL("Message", tokens[1].token().to_string());

        CHECK_EQUAL(TokenType::l_brace, tokens[2].token().type());
        CHECK_EQUAL("{", tokens[2].token().to_string());

        CHECK_EQUAL(TokenType::keyword, tokens[3].token().type());
        CHECK_EQUAL("const", tokens[3].token().to_string());

        CHECK_EQUAL(TokenType::type, tokens[4].token().type());
        CHECK_EQUAL("u8", tokens[4].token().to_string());

        CHECK_EQUAL(TokenType::string, tokens[5].token().type());
        CHECK_EQUAL("maxSize", tokens[5].token().to_string());

        CHECK_EQUAL(TokenType::equal, tokens[6].token().type());
        CHECK_EQUAL("=", tokens[6].token().to_string());

        CHECK_EQUAL(TokenType::numeric_literal, tokens[7].token().type());
        CHECK_EQUAL("100", tokens[7].token().to_string());

        CHECK_EQUAL(TokenType::end_statement, tokens[8].token().type());
        CHECK_EQUAL(";", tokens[8].token().to_string());

        CHECK_EQUAL(TokenType::keyword, tokens[9].token().type());
        CHECK_EQUAL("const", tokens[9].token().to_string());

        CHECK_EQUAL(TokenType::type, tokens[10].token().type());
        CHECK_EQUAL("i8", tokens[10].token().to_string());

        CHECK_EQUAL(TokenType::string, tokens[11].token().type());
        CHECK_EQUAL("minSize", tokens[11].token().to_string());

        CHECK_EQUAL(TokenType::equal, tokens[12].token().type());
        CHECK_EQUAL("=", tokens[12].token().to_string());

        CHECK_EQUAL(TokenType::numeric_literal, tokens[13].token().type());
        CHECK_EQUAL("-20", tokens[13].token().to_string());

        CHECK_EQUAL(TokenType::end_statement, tokens[14].token().type());
        CHECK_EQUAL(";", tokens[14].token().to_string());

        CHECK_EQUAL(TokenType::keyword, tokens[15].token().type());
        CHECK_EQUAL("const", tokens[15].token().to_string());

        CHECK_EQUAL(TokenType::type, tokens[16].token().type());
        CHECK_EQUAL("f32", tokens[16].token().to_string());

        CHECK_EQUAL(TokenType::string, tokens[17].token().type());
        CHECK_EQUAL("f", tokens[17].token().to_string());

        CHECK_EQUAL(TokenType::equal, tokens[18].token().type());
        CHECK_EQUAL("=", tokens[18].token().to_string());

        CHECK_EQUAL(TokenType::float_literal, tokens[19].token().type());
        CHECK_EQUAL("-1.87", tokens[19].token().to_string());

        CHECK_EQUAL(TokenType::end_statement, tokens[20].token().type());
        CHECK_EQUAL(";", tokens[20].token().to_string());

        CHECK_EQUAL(TokenType::r_brace, tokens[21].token().type());
        CHECK_EQUAL("}", tokens[21].token().to_string());
    }

    struct InputIsStructWithCharConstant : public TokenizerFixture
    {
        const std::string s =
            "struct Message {"                  "\n"
            "  " "const u8 maxSize = 'c';"      "\n"
            "}"                                 "\n";

        const boost::string_view sv = boost::string_view(s);
        Token token = Token(sv, 0, 0, TokenType::string);
    };

    TEST_FIXTURE(InputIsStructWithCharConstant, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        for(std::size_t position = 0, end = sv.length(); position < end; ++position)
        {
            tokenizer.consume(sv, position);
        }

        tokenizer.flush();

        REQUIRE CHECK_EQUAL(10U, tokens.size());

        CHECK_EQUAL(TokenType::keyword, tokens[0].token().type());
        CHECK_EQUAL("struct", tokens[0].token().to_string());

        CHECK_EQUAL(TokenType::string, tokens[1].token().type());
        CHECK_EQUAL("Message", tokens[1].token().to_string());

        CHECK_EQUAL(TokenType::l_brace, tokens[2].token().type());
        CHECK_EQUAL("{", tokens[2].token().to_string());

        CHECK_EQUAL(TokenType::keyword, tokens[3].token().type());
        CHECK_EQUAL("const", tokens[3].token().to_string());

        CHECK_EQUAL(TokenType::type, tokens[4].token().type());
        CHECK_EQUAL("u8", tokens[4].token().to_string());

        CHECK_EQUAL(TokenType::string, tokens[5].token().type());
        CHECK_EQUAL("maxSize", tokens[5].token().to_string());

        CHECK_EQUAL(TokenType::equal, tokens[6].token().type());
        CHECK_EQUAL("=", tokens[6].token().to_string());

        CHECK_EQUAL(TokenType::char_literal, tokens[7].token().type());
        CHECK_EQUAL("'c'", tokens[7].token().to_string());

        CHECK_EQUAL(TokenType::end_statement, tokens[8].token().type());
        CHECK_EQUAL(";", tokens[8].token().to_string());

        CHECK_EQUAL(TokenType::r_brace, tokens[9].token().type());
        CHECK_EQUAL("}", tokens[9].token().to_string());
    }

    struct InputIsStructWithStringConstant : public TokenizerFixture
    {
        const std::string s =
            "struct Message {"                      "\n"
            "  " "const u8[] maxSize = \"Error\";"  "\n"
            "}"                                     "\n";

        const boost::string_view sv = boost::string_view(s);
        Token token = Token(sv, 0, 0, TokenType::string);
    };

    TEST_FIXTURE(InputIsStructWithStringConstant, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        for(std::size_t position = 0, end = sv.length(); position < end; ++position)
        {
            tokenizer.consume(sv, position);
        }

        tokenizer.flush();

        REQUIRE CHECK_EQUAL(12U, tokens.size());

        CHECK_EQUAL(TokenType::keyword, tokens[0].token().type());
        CHECK_EQUAL("struct", tokens[0].token().to_string());

        CHECK_EQUAL(TokenType::string, tokens[1].token().type());
        CHECK_EQUAL("Message", tokens[1].token().to_string());

        CHECK_EQUAL(TokenType::l_brace, tokens[2].token().type());
        CHECK_EQUAL("{", tokens[2].token().to_string());

        CHECK_EQUAL(TokenType::keyword, tokens[3].token().type());
        CHECK_EQUAL("const", tokens[3].token().to_string());

        CHECK_EQUAL(TokenType::type, tokens[4].token().type());
        CHECK_EQUAL("u8", tokens[4].token().to_string());

        CHECK_EQUAL(TokenType::l_bracket, tokens[5].token().type());
        CHECK_EQUAL("[", tokens[5].token().to_string());

        CHECK_EQUAL(TokenType::r_bracket, tokens[6].token().type());
        CHECK_EQUAL("]", tokens[6].token().to_string());

        CHECK_EQUAL(TokenType::string, tokens[7].token().type());
        CHECK_EQUAL("maxSize", tokens[7].token().to_string());

        CHECK_EQUAL(TokenType::equal, tokens[8].token().type());
        CHECK_EQUAL("=", tokens[8].token().to_string());

        CHECK_EQUAL(TokenType::string_literal, tokens[9].token().type());
        CHECK_EQUAL("\"Error\"", tokens[9].token().to_string());

        CHECK_EQUAL(TokenType::end_statement, tokens[10].token().type());
        CHECK_EQUAL(";", tokens[10].token().to_string());

        CHECK_EQUAL(TokenType::r_brace, tokens[11].token().type());
        CHECK_EQUAL("}", tokens[11].token().to_string());
    }

    struct InputIsStructWithVariableBlock : public TokenizerFixture
    {
        const std::string s =
            "struct Message {"                      "\n"
            "  " "u8 msgType;"                      "\n\n"

            "  " "variable_block : msgType {"       "\n"
            "  " "case 'A': MsgA,"                  "\n"
            "  " "case 'B': MsgB,"                  "\n"
            "  " "}"                                "\n"
            "}"                                     "\n";

        const boost::string_view sv = boost::string_view(s);
        Token token = Token(sv, 0, 0, TokenType::string);
    };

    TEST_FIXTURE(InputIsStructWithVariableBlock, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        for(std::size_t position = 0, end = sv.length(); position < end; ++position)
        {
            tokenizer.consume(sv, position);
        }

        tokenizer.flush();

        REQUIRE CHECK_EQUAL(22U, tokens.size());

        CHECK_EQUAL(TokenType::keyword, tokens[0].token().type());
        CHECK_EQUAL("struct", tokens[0].token().to_string());

        CHECK_EQUAL(1U, tokens[0].fileInfo().start().line());
        CHECK_EQUAL(1U, tokens[0].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[0].fileInfo().end().line());
        CHECK_EQUAL(7U, tokens[0].fileInfo().end().column());

        CHECK_EQUAL(TokenType::string, tokens[1].token().type());
        CHECK_EQUAL("Message", tokens[1].token().to_string());

        CHECK_EQUAL(1U, tokens[1].fileInfo().start().line());
        CHECK_EQUAL(8U, tokens[1].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[1].fileInfo().end().line());
        CHECK_EQUAL(15U, tokens[1].fileInfo().end().column());

        CHECK_EQUAL(TokenType::l_brace, tokens[2].token().type());
        CHECK_EQUAL("{", tokens[2].token().to_string());

        CHECK_EQUAL(1U, tokens[2].fileInfo().start().line());
        CHECK_EQUAL(16U, tokens[2].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[2].fileInfo().end().line());
        CHECK_EQUAL(17U, tokens[2].fileInfo().end().column());

        CHECK_EQUAL(TokenType::type, tokens[3].token().type());
        CHECK_EQUAL("u8", tokens[3].token().to_string());

        CHECK_EQUAL(2U, tokens[3].fileInfo().start().line());
        CHECK_EQUAL(3U, tokens[3].fileInfo().start().column());
        CHECK_EQUAL(2U, tokens[3].fileInfo().end().line());
        CHECK_EQUAL(5U, tokens[3].fileInfo().end().column());

        CHECK_EQUAL(TokenType::string, tokens[4].token().type());
        CHECK_EQUAL("msgType", tokens[4].token().to_string());

        CHECK_EQUAL(2U, tokens[4].fileInfo().start().line());
        CHECK_EQUAL(6U, tokens[4].fileInfo().start().column());
        CHECK_EQUAL(2U, tokens[4].fileInfo().end().line());
        CHECK_EQUAL(13U, tokens[4].fileInfo().end().column());

        CHECK_EQUAL(TokenType::end_statement, tokens[5].token().type());
        CHECK_EQUAL(";", tokens[5].token().to_string());

        CHECK_EQUAL(2U, tokens[5].fileInfo().start().line());
        CHECK_EQUAL(13U, tokens[5].fileInfo().start().column());
        CHECK_EQUAL(2U, tokens[5].fileInfo().end().line());
        CHECK_EQUAL(14U, tokens[5].fileInfo().end().column());

        CHECK_EQUAL(TokenType::type, tokens[6].token().type());
        CHECK_EQUAL("variable_block", tokens[6].token().to_string());

        CHECK_EQUAL(4U, tokens[6].fileInfo().start().line());
        CHECK_EQUAL(3U, tokens[6].fileInfo().start().column());
        CHECK_EQUAL(4U, tokens[6].fileInfo().end().line());
        CHECK_EQUAL(17U, tokens[6].fileInfo().end().column());

        CHECK_EQUAL(TokenType::colon, tokens[7].token().type());
        CHECK_EQUAL(":", tokens[7].token().to_string());

        CHECK_EQUAL(4U, tokens[7].fileInfo().start().line());
        CHECK_EQUAL(18U, tokens[7].fileInfo().start().column());
        CHECK_EQUAL(4U, tokens[7].fileInfo().end().line());
        CHECK_EQUAL(19U, tokens[7].fileInfo().end().column());

        CHECK_EQUAL(TokenType::string, tokens[8].token().type());
        CHECK_EQUAL("msgType", tokens[8].token().to_string());

        CHECK_EQUAL(4U, tokens[8].fileInfo().start().line());
        CHECK_EQUAL(20U, tokens[8].fileInfo().start().column());
        CHECK_EQUAL(4U, tokens[8].fileInfo().end().line());
        CHECK_EQUAL(27U, tokens[8].fileInfo().end().column());

        CHECK_EQUAL(TokenType::l_brace, tokens[9].token().type());
        CHECK_EQUAL("{", tokens[9].token().to_string());

        CHECK_EQUAL(4U, tokens[9].fileInfo().start().line());
        CHECK_EQUAL(28U, tokens[9].fileInfo().start().column());
        CHECK_EQUAL(4U, tokens[9].fileInfo().end().line());
        CHECK_EQUAL(29U, tokens[9].fileInfo().end().column());

        CHECK_EQUAL(TokenType::keyword, tokens[10].token().type());
        CHECK_EQUAL("case", tokens[10].token().to_string());

        CHECK_EQUAL(5U, tokens[10].fileInfo().start().line());
        CHECK_EQUAL(3U, tokens[10].fileInfo().start().column());
        CHECK_EQUAL(5U, tokens[10].fileInfo().end().line());
        CHECK_EQUAL(7U, tokens[10].fileInfo().end().column());

        CHECK_EQUAL(TokenType::char_literal, tokens[11].token().type());
        CHECK_EQUAL("'A'", tokens[11].token().to_string());

        CHECK_EQUAL(5U, tokens[11].fileInfo().start().line());
        CHECK_EQUAL(8U, tokens[11].fileInfo().start().column());
        CHECK_EQUAL(5U, tokens[11].fileInfo().end().line());
        CHECK_EQUAL(11U, tokens[11].fileInfo().end().column());

        CHECK_EQUAL(TokenType::colon, tokens[12].token().type());
        CHECK_EQUAL(":", tokens[12].token().to_string());

        CHECK_EQUAL(5U, tokens[12].fileInfo().start().line());
        CHECK_EQUAL(11U, tokens[12].fileInfo().start().column());
        CHECK_EQUAL(5U, tokens[12].fileInfo().end().line());
        CHECK_EQUAL(12U, tokens[12].fileInfo().end().column());

        CHECK_EQUAL(TokenType::string, tokens[13].token().type());
        CHECK_EQUAL("MsgA", tokens[13].token().to_string());

        CHECK_EQUAL(5U, tokens[13].fileInfo().start().line());
        CHECK_EQUAL(13U, tokens[13].fileInfo().start().column());
        CHECK_EQUAL(5U, tokens[13].fileInfo().end().line());
        CHECK_EQUAL(17U, tokens[13].fileInfo().end().column());

        CHECK_EQUAL(TokenType::comma, tokens[14].token().type());
        CHECK_EQUAL(",", tokens[14].token().to_string());

        CHECK_EQUAL(5U, tokens[14].fileInfo().start().line());
        CHECK_EQUAL(17U, tokens[14].fileInfo().start().column());
        CHECK_EQUAL(5U, tokens[14].fileInfo().end().line());
        CHECK_EQUAL(18U, tokens[14].fileInfo().end().column());

        CHECK_EQUAL(TokenType::keyword, tokens[15].token().type());
        CHECK_EQUAL("case", tokens[15].token().to_string());

        CHECK_EQUAL(6U, tokens[15].fileInfo().start().line());
        CHECK_EQUAL(3U, tokens[15].fileInfo().start().column());
        CHECK_EQUAL(6U, tokens[15].fileInfo().end().line());
        CHECK_EQUAL(7U, tokens[15].fileInfo().end().column());

        CHECK_EQUAL(TokenType::char_literal, tokens[16].token().type());
        CHECK_EQUAL("'B'", tokens[16].token().to_string());

        CHECK_EQUAL(6U, tokens[16].fileInfo().start().line());
        CHECK_EQUAL(8U, tokens[16].fileInfo().start().column());
        CHECK_EQUAL(6U, tokens[16].fileInfo().end().line());
        CHECK_EQUAL(11U, tokens[16].fileInfo().end().column());

        CHECK_EQUAL(TokenType::colon, tokens[17].token().type());
        CHECK_EQUAL(":", tokens[17].token().to_string());

        CHECK_EQUAL(6U, tokens[17].fileInfo().start().line());
        CHECK_EQUAL(11U, tokens[17].fileInfo().start().column());
        CHECK_EQUAL(6U, tokens[17].fileInfo().end().line());
        CHECK_EQUAL(12U, tokens[17].fileInfo().end().column());

        CHECK_EQUAL(TokenType::string, tokens[18].token().type());
        CHECK_EQUAL("MsgB", tokens[18].token().to_string());

        CHECK_EQUAL(6U, tokens[18].fileInfo().start().line());
        CHECK_EQUAL(13U, tokens[18].fileInfo().start().column());
        CHECK_EQUAL(6U, tokens[18].fileInfo().end().line());
        CHECK_EQUAL(17U, tokens[18].fileInfo().end().column());

        CHECK_EQUAL(TokenType::comma, tokens[19].token().type());
        CHECK_EQUAL(",", tokens[19].token().to_string());

        CHECK_EQUAL(6U, tokens[19].fileInfo().start().line());
        CHECK_EQUAL(17U, tokens[19].fileInfo().start().column());
        CHECK_EQUAL(6U, tokens[19].fileInfo().end().line());
        CHECK_EQUAL(18U, tokens[19].fileInfo().end().column());

        CHECK_EQUAL(TokenType::r_brace, tokens[20].token().type());
        CHECK_EQUAL("}", tokens[20].token().to_string());

        CHECK_EQUAL(7U, tokens[20].fileInfo().start().line());
        CHECK_EQUAL(3U, tokens[20].fileInfo().start().column());
        CHECK_EQUAL(7U, tokens[20].fileInfo().end().line());
        CHECK_EQUAL(4U, tokens[20].fileInfo().end().column());

        CHECK_EQUAL(TokenType::r_brace, tokens[21].token().type());
        CHECK_EQUAL("}", tokens[21].token().to_string());

        CHECK_EQUAL(8U, tokens[21].fileInfo().start().line());
        CHECK_EQUAL(1U, tokens[21].fileInfo().start().column());
        CHECK_EQUAL(8U, tokens[21].fileInfo().end().line());
        CHECK_EQUAL(2U, tokens[21].fileInfo().end().column());
    }
}
