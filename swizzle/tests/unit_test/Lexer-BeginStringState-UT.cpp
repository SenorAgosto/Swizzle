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
        states::BeginStringState<CreateTokenCallback> state = states::BeginStringState<CreateTokenCallback>(callback);

        std::size_t position = 0;
    };

    struct WhenInputIsEmpty : public TokenizerFixture
    {
        const std::string s = "";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::string);
    };

    TEST_FIXTURE(WhenInputIsEmpty, verifyConstruction)
    {
    }

    struct WhenNextCharacterIsSlash : public TokenizerFixture
    {
        const std::string s = "f/";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::string);
    };

    TEST_FIXTURE(WhenNextCharacterIsSlash, verifyConsumeSpace)
    {
        CHECK_EQUAL(0U, tokens.size());

        // consume f
        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::BeginString, tokenState);
        CHECK_EQUAL(TokenType::string, token.type());
        CHECK_EQUAL("f", token.to_string());

        // consume /
        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::FirstSlash, tokenState);
        CHECK_EQUAL(TokenType::comment, token.type());
        CHECK_EQUAL("/", token.to_string());

        REQUIRE CHECK_EQUAL(1U, tokens.size());

        CHECK_EQUAL("f", tokens[0].token().to_string());
        CHECK_EQUAL(TokenType::string, tokens[0].token().type());

        CHECK_EQUAL(1U, tokens[0].fileInfo().start().line());
        CHECK_EQUAL(1U, tokens[0].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[0].fileInfo().end().line());
        CHECK_EQUAL(2U, tokens[0].fileInfo().end().column());
    }

    struct WhenNextCharacterIsSingleQuote : public TokenizerFixture
    {
        const std::string s = "f'";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::string);
    };

    TEST_FIXTURE(WhenNextCharacterIsSingleQuote, verifyConsumeSpace)
    {
        CHECK_EQUAL(0U, tokens.size());

        // consume f
        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::BeginString, tokenState);
        CHECK_EQUAL(TokenType::string, token.type());
        CHECK_EQUAL("f", token.to_string());

        // consume '
        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::CharLiteral, tokenState);
        CHECK_EQUAL(TokenType::char_literal, token.type());
        CHECK_EQUAL("'", token.to_string());

        REQUIRE CHECK_EQUAL(1U, tokens.size());

        CHECK_EQUAL("f", tokens[0].token().to_string());
        CHECK_EQUAL(TokenType::string, tokens[0].token().type());

        CHECK_EQUAL(1U, tokens[0].fileInfo().start().line());
        CHECK_EQUAL(1U, tokens[0].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[0].fileInfo().end().line());
        CHECK_EQUAL(2U, tokens[0].fileInfo().end().column());
    }

    struct WhenNextCharacterIsDoubleQuote : public TokenizerFixture
    {
        const std::string s = "f\"";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::string);
    };

    TEST_FIXTURE(WhenNextCharacterIsDoubleQuote, verifyConsumeSpace)
    {
        CHECK_EQUAL(0U, tokens.size());

        // consume f
        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::BeginString, tokenState);
        CHECK_EQUAL(TokenType::string, token.type());
        CHECK_EQUAL("f", token.to_string());

        // consume "
        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::StringLiteral, tokenState);
        CHECK_EQUAL(TokenType::string_literal, token.type());
        CHECK_EQUAL("\"", token.to_string());

        REQUIRE CHECK_EQUAL(1U, tokens.size());

        CHECK_EQUAL("f", tokens[0].token().to_string());
        CHECK_EQUAL(TokenType::string, tokens[0].token().type());

        CHECK_EQUAL(1U, tokens[0].fileInfo().start().line());
        CHECK_EQUAL(1U, tokens[0].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[0].fileInfo().end().line());
        CHECK_EQUAL(2U, tokens[0].fileInfo().end().column());
    }

    struct WhenLastCharacterIsSemiColon : public TokenizerFixture
    {
        const std::string s = "fld1;";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::string);
    };

    TEST_FIXTURE(WhenLastCharacterIsSemiColon, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        // consume f
        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::BeginString, tokenState);
        CHECK_EQUAL(TokenType::string, token.type());
        CHECK_EQUAL("f", token.to_string());

        // consume l
        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::BeginString, tokenState);
        CHECK_EQUAL(TokenType::string, token.type());
        CHECK_EQUAL("fl", token.to_string());

        // consume d
        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::BeginString, tokenState);
        CHECK_EQUAL(TokenType::string, token.type());
        CHECK_EQUAL("fld", token.to_string());

        // consume 1
        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::BeginString, tokenState);
        CHECK_EQUAL(TokenType::string, token.type());
        CHECK_EQUAL("fld1", token.to_string());

        // consume ;
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

        Token token = Token(sv, 0, 0, TokenType::string);
    };

    TEST_FIXTURE(WhenNextCharacterIsAttribute, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        // consume f
        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::BeginString, tokenState);
        CHECK_EQUAL(TokenType::string, token.type());
        CHECK_EQUAL("f", token.to_string());

        // consume @
        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::Init, tokenState);

        REQUIRE CHECK_EQUAL(2U, tokens.size());

        CHECK_EQUAL("f", tokens[0].token().to_string());
        CHECK_EQUAL(TokenType::string, tokens[0].token().type());

        CHECK_EQUAL(1U, tokens[0].fileInfo().start().line());
        CHECK_EQUAL(1U, tokens[0].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[0].fileInfo().end().line());
        CHECK_EQUAL(2U, tokens[0].fileInfo().end().column());

        CHECK_EQUAL("@", tokens[1].token().to_string());
        CHECK_EQUAL(TokenType::attribute, tokens[1].token().type());

        CHECK_EQUAL(1U, tokens[1].fileInfo().start().line());
        CHECK_EQUAL(2U, tokens[1].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[1].fileInfo().end().line());
        CHECK_EQUAL(3U, tokens[1].fileInfo().end().column());
    }

    struct WhenNextCharacterIsComma : public TokenizerFixture
    {
        const std::string s = "f,";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::string);
    };

    TEST_FIXTURE(WhenNextCharacterIsComma, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        // consume f
        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::BeginString, tokenState);
        CHECK_EQUAL(TokenType::string, token.type());
        CHECK_EQUAL("f", token.to_string());

        // consume ,
        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::Init, tokenState);

        REQUIRE CHECK_EQUAL(2U, tokens.size());

        CHECK_EQUAL("f", tokens[0].token().to_string());
        CHECK_EQUAL(TokenType::string, tokens[0].token().type());

        CHECK_EQUAL(1U, tokens[0].fileInfo().start().line());
        CHECK_EQUAL(1U, tokens[0].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[0].fileInfo().end().line());
        CHECK_EQUAL(2U, tokens[0].fileInfo().end().column());

        CHECK_EQUAL(",", tokens[1].token().to_string());
        CHECK_EQUAL(TokenType::comma, tokens[1].token().type());

        CHECK_EQUAL(1U, tokens[1].fileInfo().start().line());
        CHECK_EQUAL(2U, tokens[1].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[1].fileInfo().end().line());
        CHECK_EQUAL(3U, tokens[1].fileInfo().end().column());
    }

    struct WhenNextCharacterIsColon : public TokenizerFixture
    {
        const std::string s = "f:";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::string);
    };

    TEST_FIXTURE(WhenNextCharacterIsColon, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        // consume f
        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::BeginString, tokenState);
        CHECK_EQUAL(TokenType::string, token.type());
        CHECK_EQUAL("f", token.to_string());

        // consume :
        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::Init, tokenState);

        REQUIRE CHECK_EQUAL(2U, tokens.size());

        CHECK_EQUAL("f", tokens[0].token().to_string());
        CHECK_EQUAL(TokenType::string, tokens[0].token().type());

        CHECK_EQUAL(1U, tokens[0].fileInfo().start().line());
        CHECK_EQUAL(1U, tokens[0].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[0].fileInfo().end().line());
        CHECK_EQUAL(2U, tokens[0].fileInfo().end().column());

        CHECK_EQUAL(":", tokens[1].token().to_string());
        CHECK_EQUAL(TokenType::colon, tokens[1].token().type());

        CHECK_EQUAL(1U, tokens[1].fileInfo().start().line());
        CHECK_EQUAL(2U, tokens[1].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[1].fileInfo().end().line());
        CHECK_EQUAL(3U, tokens[1].fileInfo().end().column());
    }

    struct WhenNextCharacterIsLeftBrace : public TokenizerFixture
    {
        const std::string s = "f{";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::string);
    };

    TEST_FIXTURE(WhenNextCharacterIsLeftBrace, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        // consume f
        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::BeginString, tokenState);
        CHECK_EQUAL(TokenType::string, token.type());
        CHECK_EQUAL("f", token.to_string());

        // consume {
        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::Init, tokenState);

        REQUIRE CHECK_EQUAL(2U, tokens.size());

        CHECK_EQUAL("f", tokens[0].token().to_string());
        CHECK_EQUAL(TokenType::string, tokens[0].token().type());

        CHECK_EQUAL(1U, tokens[0].fileInfo().start().line());
        CHECK_EQUAL(1U, tokens[0].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[0].fileInfo().end().line());
        CHECK_EQUAL(2U, tokens[0].fileInfo().end().column());

        CHECK_EQUAL("{", tokens[1].token().to_string());
        CHECK_EQUAL(TokenType::l_brace, tokens[1].token().type());

        CHECK_EQUAL(1U, tokens[1].fileInfo().start().line());
        CHECK_EQUAL(2U, tokens[1].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[1].fileInfo().end().line());
        CHECK_EQUAL(3U, tokens[1].fileInfo().end().column());
    }

    struct WhenNextCharacterIsRightBrace : public TokenizerFixture
    {
        const std::string s = "f}";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::string);
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

        CHECK_EQUAL(1U, tokens[0].fileInfo().start().line());
        CHECK_EQUAL(1U, tokens[0].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[0].fileInfo().end().line());
        CHECK_EQUAL(2U, tokens[0].fileInfo().end().column());

        CHECK_EQUAL("}", tokens[1].token().to_string());
        CHECK_EQUAL(TokenType::r_brace, tokens[1].token().type());

        CHECK_EQUAL(1U, tokens[1].fileInfo().start().line());
        CHECK_EQUAL(2U, tokens[1].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[1].fileInfo().end().line());
        CHECK_EQUAL(3U, tokens[1].fileInfo().end().column());
    }

    struct WhenNextCharacterIsLeftBracket : public TokenizerFixture
    {
        const std::string s = "f[";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::string);
    };

    TEST_FIXTURE(WhenNextCharacterIsLeftBracket, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        // consume f
        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::BeginString, tokenState);
        CHECK_EQUAL(TokenType::string, token.type());
        CHECK_EQUAL("f", token.to_string());

        // consume [
        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::Init, tokenState);

        REQUIRE CHECK_EQUAL(2U, tokens.size());

        CHECK_EQUAL("f", tokens[0].token().to_string());
        CHECK_EQUAL(TokenType::string, tokens[0].token().type());

        CHECK_EQUAL(1U, tokens[0].fileInfo().start().line());
        CHECK_EQUAL(1U, tokens[0].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[0].fileInfo().end().line());
        CHECK_EQUAL(2U, tokens[0].fileInfo().end().column());

        CHECK_EQUAL("[", tokens[1].token().to_string());
        CHECK_EQUAL(TokenType::l_bracket, tokens[1].token().type());

        CHECK_EQUAL(1U, tokens[1].fileInfo().start().line());
        CHECK_EQUAL(2U, tokens[1].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[1].fileInfo().end().line());
        CHECK_EQUAL(3U, tokens[1].fileInfo().end().column());
    }

    struct WhenNextCharacterIsRightBracket : public TokenizerFixture
    {
        const std::string s = "f]";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::string);
    };

    TEST_FIXTURE(WhenNextCharacterIsRightBracket, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        // consume f
        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::BeginString, tokenState);
        CHECK_EQUAL(TokenType::string, token.type());
        CHECK_EQUAL("f", token.to_string());

        // consume ]
        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::Init, tokenState);

        REQUIRE CHECK_EQUAL(2U, tokens.size());

        CHECK_EQUAL("f", tokens[0].token().to_string());
        CHECK_EQUAL(TokenType::string, tokens[0].token().type());

        CHECK_EQUAL(1U, tokens[0].fileInfo().start().line());
        CHECK_EQUAL(1U, tokens[0].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[0].fileInfo().end().line());
        CHECK_EQUAL(2U, tokens[0].fileInfo().end().column());

        CHECK_EQUAL("]", tokens[1].token().to_string());
        CHECK_EQUAL(TokenType::r_bracket, tokens[1].token().type());

        CHECK_EQUAL(1U, tokens[1].fileInfo().start().line());
        CHECK_EQUAL(2U, tokens[1].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[1].fileInfo().end().line());
        CHECK_EQUAL(3U, tokens[1].fileInfo().end().column());
    }

    struct WhenNextCharacterIsEqual : public TokenizerFixture
    {
        const std::string s = "f=";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::string);
    };

    TEST_FIXTURE(WhenNextCharacterIsEqual, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        // consume f
        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::BeginString, tokenState);
        CHECK_EQUAL(TokenType::string, token.type());
        CHECK_EQUAL("f", token.to_string());

        // consume =
        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::Init, tokenState);

        REQUIRE CHECK_EQUAL(2U, tokens.size());

        CHECK_EQUAL("f", tokens[0].token().to_string());
        CHECK_EQUAL(TokenType::string, tokens[0].token().type());

        CHECK_EQUAL(1U, tokens[0].fileInfo().start().line());
        CHECK_EQUAL(1U, tokens[0].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[0].fileInfo().end().line());
        CHECK_EQUAL(2U, tokens[0].fileInfo().end().column());

        CHECK_EQUAL("=", tokens[1].token().to_string());
        CHECK_EQUAL(TokenType::equal, tokens[1].token().type());

        CHECK_EQUAL(1U, tokens[1].fileInfo().start().line());
        CHECK_EQUAL(2U, tokens[1].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[1].fileInfo().end().line());
        CHECK_EQUAL(3U, tokens[1].fileInfo().end().column());
    }

    struct WhenNextCharacterIsDot : public TokenizerFixture
    {
        const std::string s = "f.";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::string);
    };

    TEST_FIXTURE(WhenNextCharacterIsDot, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        // consume f
        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::BeginString, tokenState);
        CHECK_EQUAL(TokenType::string, token.type());
        CHECK_EQUAL("f", token.to_string());

        // consume .
        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::Init, tokenState);

        REQUIRE CHECK_EQUAL(2U, tokens.size());

        CHECK_EQUAL("f", tokens[0].token().to_string());
        CHECK_EQUAL(TokenType::string, tokens[0].token().type());

        CHECK_EQUAL(1U, tokens[0].fileInfo().start().line());
        CHECK_EQUAL(1U, tokens[0].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[0].fileInfo().end().line());
        CHECK_EQUAL(2U, tokens[0].fileInfo().end().column());

        CHECK_EQUAL(".", tokens[1].token().to_string());
        CHECK_EQUAL(TokenType::dot, tokens[1].token().type());

        CHECK_EQUAL(1U, tokens[1].fileInfo().start().line());
        CHECK_EQUAL(2U, tokens[1].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[1].fileInfo().end().line());
        CHECK_EQUAL(3U, tokens[1].fileInfo().end().column());
    }

    struct WhenNextCharacterIsSpace : public TokenizerFixture
    {
        const std::string s = "f a ";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::string);
    };

    TEST_FIXTURE(WhenNextCharacterIsSpace, verifyConsumeSpace)
    {
        CHECK_EQUAL(0U, tokens.size());

        // consume f
        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::BeginString, tokenState);
        CHECK_EQUAL(TokenType::string, token.type());
        CHECK_EQUAL("f", token.to_string());

        // consume ' '
        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::Init, tokenState);

        REQUIRE CHECK_EQUAL(1U, tokens.size());

        CHECK_EQUAL("f", tokens[0].token().to_string());
        CHECK_EQUAL(TokenType::string, tokens[0].token().type());

        CHECK_EQUAL(1U, tokens[0].fileInfo().start().line());
        CHECK_EQUAL(1U, tokens[0].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[0].fileInfo().end().line());
        CHECK_EQUAL(2U, tokens[0].fileInfo().end().column());
    }

    struct WhenNextCharacterIsTab : public TokenizerFixture
    {
        const std::string s = "f\t";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::string);
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

        CHECK_EQUAL(1U, tokens[0].fileInfo().start().line());
        CHECK_EQUAL(1U, tokens[0].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[0].fileInfo().end().line());
        CHECK_EQUAL(2U, tokens[0].fileInfo().end().column());
    }

    struct WhenNextCharacterIsReturn : public TokenizerFixture
    {
        const std::string s = "f\r";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::string);
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

        CHECK_EQUAL(1U, tokens[0].fileInfo().start().line());
        CHECK_EQUAL(1U, tokens[0].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[0].fileInfo().end().line());
        CHECK_EQUAL(2U, tokens[0].fileInfo().end().column());
    }

    struct WhenNextCharacterIsNewLine : public TokenizerFixture
    {
        const std::string s = "f\n";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::string);
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

        CHECK_EQUAL(1U, tokens[0].fileInfo().start().line());
        CHECK_EQUAL(1U, tokens[0].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[0].fileInfo().end().line());
        CHECK_EQUAL(2U, tokens[0].fileInfo().end().column());
    }
}
