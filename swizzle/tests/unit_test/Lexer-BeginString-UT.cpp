#include "./platform/UnitTestSupport.hpp"
#include <swizzle/lexer/states/BeginStringState.hpp>

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
        states::BeginString<CreateTokenCallback> state = states::BeginString<CreateTokenCallback>(callback);

        std::size_t position = 0;
    };

    struct WhenInputIsEmpty : public TokenizerFixture
    {
        const std::string s = "";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(boost::string_view(), TokenType::string);
    };

    TEST_FIXTURE(WhenInputIsEmpty, verifyConstruction)
    {
    }

    struct WhenNextCharacterIsSlash : public TokenizerFixture
    {
        const std::string s = "f/";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(boost::string_view(), TokenType::string);
    };

    TEST_FIXTURE(WhenNextCharacterIsSlash, verifyConsumeSpace)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::BeginString, tokenState);
        CHECK_EQUAL(TokenType::string, token.type());
        CHECK_EQUAL("f", token.to_string());

        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::FirstSlash, tokenState);

        REQUIRE CHECK_EQUAL(1U, tokens.size());
        CHECK_EQUAL("f", tokens[0].token().to_string());
        CHECK_EQUAL(TokenType::string, tokens[0].token().type());
    }

    struct WhenNextCharacterIsSingleQuote : public TokenizerFixture
    {
        const std::string s = "f'";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(boost::string_view(), TokenType::string);
    };

    TEST_FIXTURE(WhenNextCharacterIsSingleQuote, verifyConsumeSpace)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::BeginString, tokenState);
        CHECK_EQUAL(TokenType::string, token.type());
        CHECK_EQUAL("f", token.to_string());

        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::CharLiteral, tokenState);

        REQUIRE CHECK_EQUAL(1U, tokens.size());
        CHECK_EQUAL("f", tokens[0].token().to_string());
        CHECK_EQUAL(TokenType::string, tokens[0].token().type());
    }

    struct WhenNextCharacterIsDoubleQuote : public TokenizerFixture
    {
        const std::string s = "f\"";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(boost::string_view(), TokenType::string);
    };

    TEST_FIXTURE(WhenNextCharacterIsDoubleQuote, verifyConsumeSpace)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::BeginString, tokenState);
        CHECK_EQUAL(TokenType::string, token.type());
        CHECK_EQUAL("f", token.to_string());

        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::StringLiteral, tokenState);

        REQUIRE CHECK_EQUAL(1U, tokens.size());
        CHECK_EQUAL("f", tokens[0].token().to_string());
        CHECK_EQUAL(TokenType::string, tokens[0].token().type());
    }

    struct WhenLastCharacterIsSemiColon : public TokenizerFixture
    {
        const std::string s = "fld1;";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(boost::string_view(), TokenType::string);
    };

    TEST_FIXTURE(WhenLastCharacterIsSemiColon, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::BeginString, tokenState);
        CHECK_EQUAL(TokenType::string, token.type());
        CHECK_EQUAL("f", token.to_string());

        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::BeginString, tokenState);
        CHECK_EQUAL(TokenType::string, token.type());
        CHECK_EQUAL("fl", token.to_string());

        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::BeginString, tokenState);
        CHECK_EQUAL(TokenType::string, token.type());
        CHECK_EQUAL("fld", token.to_string());

        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::BeginString, tokenState);
        CHECK_EQUAL(TokenType::string, token.type());
        CHECK_EQUAL("fld1", token.to_string());

        tokenState = state.consume(sv, position++, info, token);

        REQUIRE CHECK_EQUAL(2U, tokens.size());

        CHECK_EQUAL("fld1", tokens[0].token().to_string());
        CHECK_EQUAL(TokenType::string, tokens[0].token().type());
        CHECK_EQUAL(1U, tokens[0].fileInfo().start().line());
        CHECK_EQUAL(1U, tokens[0].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[0].fileInfo().end().line());
        CHECK_EQUAL(5U, tokens[0].fileInfo().end().column());

        CHECK_EQUAL(";", tokens[1].token().to_string());
        CHECK_EQUAL(TokenType::end_statement, tokens[1].token().type());
        CHECK_EQUAL(1U, tokens[1].fileInfo().start().line());
        CHECK_EQUAL(5U, tokens[1].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[1].fileInfo().end().line());
        CHECK_EQUAL(6U, tokens[1].fileInfo().end().column());
    }

    struct WhenNextCharacterIsAttribute : public TokenizerFixture
    {
        const std::string s = "f@";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(boost::string_view(), TokenType::string);
    };

    TEST_FIXTURE(WhenNextCharacterIsAttribute, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::BeginString, tokenState);
        CHECK_EQUAL(TokenType::string, token.type());
        CHECK_EQUAL("f", token.to_string());

        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::Init, tokenState);

        REQUIRE CHECK_EQUAL(2U, tokens.size());

        CHECK_EQUAL("f", tokens[0].token().to_string());
        CHECK_EQUAL(TokenType::string, tokens[0].token().type());

        CHECK_EQUAL("@", tokens[1].token().to_string());
        CHECK_EQUAL(TokenType::attribute, tokens[1].token().type());
    }

    struct WhenNextCharacterIsColon : public TokenizerFixture
    {
        const std::string s = "f:";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(boost::string_view(), TokenType::string);
    };

    TEST_FIXTURE(WhenNextCharacterIsColon, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::BeginString, tokenState);
        CHECK_EQUAL(TokenType::string, token.type());
        CHECK_EQUAL("f", token.to_string());

        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::Init, tokenState);

        REQUIRE CHECK_EQUAL(2U, tokens.size());

        CHECK_EQUAL("f", tokens[0].token().to_string());
        CHECK_EQUAL(TokenType::string, tokens[0].token().type());

        CHECK_EQUAL(":", tokens[1].token().to_string());
        CHECK_EQUAL(TokenType::colon, tokens[1].token().type());
    }

    struct WhenNextCharacterIsLeftBrace : public TokenizerFixture
    {
        const std::string s = "f{";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(boost::string_view(), TokenType::string);
    };

    TEST_FIXTURE(WhenNextCharacterIsLeftBrace, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::BeginString, tokenState);
        CHECK_EQUAL(TokenType::string, token.type());
        CHECK_EQUAL("f", token.to_string());

        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::Init, tokenState);

        REQUIRE CHECK_EQUAL(2U, tokens.size());

        CHECK_EQUAL("f", tokens[0].token().to_string());
        CHECK_EQUAL(TokenType::string, tokens[0].token().type());

        CHECK_EQUAL("{", tokens[1].token().to_string());
        CHECK_EQUAL(TokenType::l_brace, tokens[1].token().type());
    }

    struct WhenNextCharacterIsRightBrace : public TokenizerFixture
    {
        const std::string s = "f}";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(boost::string_view(), TokenType::string);
    };

    TEST_FIXTURE(WhenNextCharacterIsRightBrace, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::BeginString, tokenState);
        CHECK_EQUAL(TokenType::string, token.type());
        CHECK_EQUAL("f", token.to_string());

        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::Init, tokenState);

        REQUIRE CHECK_EQUAL(2U, tokens.size());

        CHECK_EQUAL("f", tokens[0].token().to_string());
        CHECK_EQUAL(TokenType::string, tokens[0].token().type());

        CHECK_EQUAL("}", tokens[1].token().to_string());
        CHECK_EQUAL(TokenType::r_brace, tokens[1].token().type());
    }

    struct WhenNextCharacterIsLeftBracket : public TokenizerFixture
    {
        const std::string s = "f[";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(boost::string_view(), TokenType::string);
    };

    TEST_FIXTURE(WhenNextCharacterIsLeftBracket, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::BeginString, tokenState);
        CHECK_EQUAL(TokenType::string, token.type());
        CHECK_EQUAL("f", token.to_string());

        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::Init, tokenState);

        REQUIRE CHECK_EQUAL(2U, tokens.size());

        CHECK_EQUAL("f", tokens[0].token().to_string());
        CHECK_EQUAL(TokenType::string, tokens[0].token().type());

        CHECK_EQUAL("[", tokens[1].token().to_string());
        CHECK_EQUAL(TokenType::l_bracket, tokens[1].token().type());
    }

    struct WhenNextCharacterIsRightBracket : public TokenizerFixture
    {
        const std::string s = "f]";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(boost::string_view(), TokenType::string);
    };

    TEST_FIXTURE(WhenNextCharacterIsRightBracket, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::BeginString, tokenState);
        CHECK_EQUAL(TokenType::string, token.type());
        CHECK_EQUAL("f", token.to_string());

        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::Init, tokenState);

        REQUIRE CHECK_EQUAL(2U, tokens.size());

        CHECK_EQUAL("f", tokens[0].token().to_string());
        CHECK_EQUAL(TokenType::string, tokens[0].token().type());

        CHECK_EQUAL("]", tokens[1].token().to_string());
        CHECK_EQUAL(TokenType::r_bracket, tokens[1].token().type());
    }

    struct WhenNextCharacterIsEqual : public TokenizerFixture
    {
        const std::string s = "f=";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(boost::string_view(), TokenType::string);
    };

    TEST_FIXTURE(WhenNextCharacterIsEqual, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::BeginString, tokenState);
        CHECK_EQUAL(TokenType::string, token.type());
        CHECK_EQUAL("f", token.to_string());

        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::Init, tokenState);

        REQUIRE CHECK_EQUAL(2U, tokens.size());

        CHECK_EQUAL("f", tokens[0].token().to_string());
        CHECK_EQUAL(TokenType::string, tokens[0].token().type());

        CHECK_EQUAL("=", tokens[1].token().to_string());
        CHECK_EQUAL(TokenType::equal, tokens[1].token().type());
    }

    struct WhenNextCharacterIsDot : public TokenizerFixture
    {
        const std::string s = "f.";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(boost::string_view(), TokenType::string);
    };

    TEST_FIXTURE(WhenNextCharacterIsDot, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::BeginString, tokenState);
        CHECK_EQUAL(TokenType::string, token.type());
        CHECK_EQUAL("f", token.to_string());

        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::Init, tokenState);

        REQUIRE CHECK_EQUAL(2U, tokens.size());

        CHECK_EQUAL("f", tokens[0].token().to_string());
        CHECK_EQUAL(TokenType::string, tokens[0].token().type());

        CHECK_EQUAL(".", tokens[1].token().to_string());
        CHECK_EQUAL(TokenType::dot, tokens[1].token().type());
    }

    struct WhenNextCharacterIsSpace : public TokenizerFixture
    {
        const std::string s = "f ";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(boost::string_view(), TokenType::string);
    };

    TEST_FIXTURE(WhenNextCharacterIsSpace, verifyConsumeSpace)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::BeginString, tokenState);
        CHECK_EQUAL(TokenType::string, token.type());
        CHECK_EQUAL("f", token.to_string());

        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::Init, tokenState);

        REQUIRE CHECK_EQUAL(1U, tokens.size());
        CHECK_EQUAL("f", tokens[0].token().to_string());
        CHECK_EQUAL(TokenType::string, tokens[0].token().type());
    }

    struct WhenNextCharacterIsTab : public TokenizerFixture
    {
        const std::string s = "f\t";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(boost::string_view(), TokenType::string);
    };

    TEST_FIXTURE(WhenNextCharacterIsTab, verifyConsumeSpace)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::BeginString, tokenState);
        CHECK_EQUAL(TokenType::string, token.type());
        CHECK_EQUAL("f", token.to_string());

        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::Init, tokenState);

        REQUIRE CHECK_EQUAL(1U, tokens.size());
        CHECK_EQUAL("f", tokens[0].token().to_string());
        CHECK_EQUAL(TokenType::string, tokens[0].token().type());
    }

    struct WhenNextCharacterIsReturn : public TokenizerFixture
    {
        const std::string s = "f\r";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(boost::string_view(), TokenType::string);
    };

    TEST_FIXTURE(WhenNextCharacterIsReturn, verifyConsumeSpace)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::BeginString, tokenState);
        CHECK_EQUAL(TokenType::string, token.type());
        CHECK_EQUAL("f", token.to_string());

        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::Init, tokenState);

        REQUIRE CHECK_EQUAL(1U, tokens.size());
        CHECK_EQUAL("f", tokens[0].token().to_string());
        CHECK_EQUAL(TokenType::string, tokens[0].token().type());
    }

    struct WhenNextCharacterIsNewLine : public TokenizerFixture
    {
        const std::string s = "f\n";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(boost::string_view(), TokenType::string);
    };

    TEST_FIXTURE(WhenNextCharacterIsNewLine, verifyConsumeSpace)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::BeginString, tokenState);
        CHECK_EQUAL(TokenType::string, token.type());
        CHECK_EQUAL("f", token.to_string());

        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::Init, tokenState);

        REQUIRE CHECK_EQUAL(1U, tokens.size());
        CHECK_EQUAL("f", tokens[0].token().to_string());
        CHECK_EQUAL(TokenType::string, tokens[0].token().type());
    }
}
