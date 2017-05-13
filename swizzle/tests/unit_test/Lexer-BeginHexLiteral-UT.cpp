#include "./platform/UnitTestSupport.hpp"
#include <swizzle/lexer/states/BeginHexLiteralState.hpp>

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

    struct BeginHexLiteralFixture
    {
        std::deque<TokenInfo> tokens;
        CreateTokenCallback callback = CreateTokenCallback(tokens);

        Token token = Token(boost::string_view(), TokenType::hex_literal);
        FileInfo info = FileInfo("testfile");

        states::BeginHexLiteral<CreateTokenCallback> state = states::BeginHexLiteral<CreateTokenCallback>(callback);

        std::size_t position = 0;
    };

    TEST_FIXTURE(BeginHexLiteralFixture, verifyConstruction)
    {
    }

    struct WhenNextCharIsNumber : public BeginHexLiteralFixture
    {
        const std::string s = "1";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsNumber, verifyConsume)
    {
        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::NumericLiteral, tokenState);
        CHECK_EQUAL("1", token.to_string());
    }

    struct WhenNextCharIsX : public BeginHexLiteralFixture
    {
        const std::string s = "x";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsX, verifyConsume)
    {
        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::HexLiteral, tokenState);
        CHECK_EQUAL("x", token.to_string());
    }

    struct WhenNextCharIsSpace : public BeginHexLiteralFixture
    {
        const std::string s = " ";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsSpace, verifyConsume)
    {
        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::Init, tokenState);
        CHECK(token.empty());
    }

    struct WhenNextCharIsTab : public BeginHexLiteralFixture
    {
        const std::string s = "\t";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsTab, verifyConsume)
    {
        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::Init, tokenState);
        CHECK(token.empty());
    }

    struct WhenNextCharIsReturn : public BeginHexLiteralFixture
    {
        const std::string s = "\r";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsReturn, verifyConsume)
    {
        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::Init, tokenState);
        CHECK(token.empty());
    }

    struct WhenNextCharIsNewLine : public BeginHexLiteralFixture
    {
        const std::string s = "\n";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsNewLine, verifyConsume)
    {
        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::Init, tokenState);
        CHECK(token.empty());
    }

    struct WhenNextCharIsRightBracket : public BeginHexLiteralFixture
    {
        const std::string s = "]";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsRightBracket, verifyConsume)
    {
        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::Init, tokenState);

        REQUIRE CHECK_EQUAL(1U, tokens.size());
        CHECK_EQUAL(TokenType::r_bracket, tokens[0].token().type());
        CHECK_EQUAL("]", tokens[0].token().to_string());
    }

    struct WhenNextCharIsRightBrace : public BeginHexLiteralFixture
    {
        const std::string s = "}";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsRightBrace, verifyConsume)
    {
        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::Init, tokenState);

        REQUIRE CHECK_EQUAL(1U, tokens.size());
        CHECK_EQUAL(TokenType::r_brace, tokens[0].token().type());
        CHECK_EQUAL("}", tokens[0].token().to_string());
    }

    struct WhenNextCharIsDot : public BeginHexLiteralFixture
    {
        const std::string s = ".";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsDot, verifyConsume)
    {
        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::Init, tokenState);

        REQUIRE CHECK_EQUAL(1U, tokens.size());
        CHECK_EQUAL(TokenType::dot, tokens[0].token().type());
        CHECK_EQUAL(".", tokens[0].token().to_string());
    }

    struct WhenNextCharIsSemiColon : public BeginHexLiteralFixture
    {
        const std::string s = ";";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsSemiColon, verifyConsume)
    {
        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::Init, tokenState);

        REQUIRE CHECK_EQUAL(1U, tokens.size());
        CHECK_EQUAL(TokenType::end_statement, tokens[0].token().type());
        CHECK_EQUAL(";", tokens[0].token().to_string());
    }

    struct WhenNextCharIsComma : public BeginHexLiteralFixture
    {
        const std::string s = ",";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsComma, verifyConsume)
    {
        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::Init, tokenState);

        REQUIRE CHECK_EQUAL(1U, tokens.size());
        CHECK_EQUAL(TokenType::comma, tokens[0].token().type());
        CHECK_EQUAL(",", tokens[0].token().to_string());
    }

    struct WhenNextCharIsInvalid : public BeginHexLiteralFixture
    {
        const std::string s = "[";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsInvalid, verifyConsume)
    {
        CHECK_THROW(state.consume(sv, position++, info, token), swizzle::TokenizerError);
    }
}
