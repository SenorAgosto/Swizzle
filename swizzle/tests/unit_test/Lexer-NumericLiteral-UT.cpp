#include "./platform/UnitTestSupport.hpp"
#include <swizzle/lexer/states/NumericLiteral.hpp>

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

    struct NumericLiteralFixture
    {
        std::deque<TokenInfo> tokens;
        CreateTokenCallback callback = CreateTokenCallback(tokens);

        Token token = Token(boost::string_view(), TokenType::numeric_literal);
        FileInfo info = FileInfo("testfile");

        states::NumericLiteral<CreateTokenCallback> state = states::NumericLiteral<CreateTokenCallback>(callback);

        std::size_t position = 0;
    };

    struct WhenStringIsEmpty : public NumericLiteralFixture
    {
        const std::string s;
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenStringIsEmpty, verifyConstruction)
    {
    }

    struct WhenNextCharIs0 : public NumericLiteralFixture
    {
        const std::string s = "00";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIs0, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::NumericLiteral, tokenState);
        CHECK_EQUAL(TokenType::numeric_literal, token.type());
        CHECK_EQUAL("0", token.to_string());

        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::NumericLiteral, tokenState);
        CHECK_EQUAL(TokenType::numeric_literal, token.type());
        CHECK_EQUAL("00", token.to_string());

        REQUIRE CHECK_EQUAL(0U, tokens.size());
    }

    struct WhenNextCharIs1 : public NumericLiteralFixture
    {
        const std::string s = "01";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIs1, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::NumericLiteral, tokenState);
        CHECK_EQUAL(TokenType::numeric_literal, token.type());
        CHECK_EQUAL("0", token.to_string());

        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::NumericLiteral, tokenState);
        CHECK_EQUAL(TokenType::numeric_literal, token.type());
        CHECK_EQUAL("01", token.to_string());

        REQUIRE CHECK_EQUAL(0U, tokens.size());
    }

    struct WhenNextCharIs2 : public NumericLiteralFixture
    {
        const std::string s = "02";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIs2, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::NumericLiteral, tokenState);
        CHECK_EQUAL(TokenType::numeric_literal, token.type());
        CHECK_EQUAL("0", token.to_string());

        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::NumericLiteral, tokenState);
        CHECK_EQUAL(TokenType::numeric_literal, token.type());
        CHECK_EQUAL("02", token.to_string());

        REQUIRE CHECK_EQUAL(0U, tokens.size());
    }

    struct WhenNextCharIs3 : public NumericLiteralFixture
    {
        const std::string s = "03";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIs3, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::NumericLiteral, tokenState);
        CHECK_EQUAL(TokenType::numeric_literal, token.type());
        CHECK_EQUAL("0", token.to_string());

        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::NumericLiteral, tokenState);
        CHECK_EQUAL(TokenType::numeric_literal, token.type());
        CHECK_EQUAL("03", token.to_string());

        REQUIRE CHECK_EQUAL(0U, tokens.size());
    }

    struct WhenNextCharIs4 : public NumericLiteralFixture
    {
        const std::string s = "04";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIs4, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::NumericLiteral, tokenState);
        CHECK_EQUAL(TokenType::numeric_literal, token.type());
        CHECK_EQUAL("0", token.to_string());

        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::NumericLiteral, tokenState);
        CHECK_EQUAL(TokenType::numeric_literal, token.type());
        CHECK_EQUAL("04", token.to_string());

        REQUIRE CHECK_EQUAL(0U, tokens.size());
    }

    struct WhenNextCharIs5 : public NumericLiteralFixture
    {
        const std::string s = "05";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIs5, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::NumericLiteral, tokenState);
        CHECK_EQUAL(TokenType::numeric_literal, token.type());
        CHECK_EQUAL("0", token.to_string());

        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::NumericLiteral, tokenState);
        CHECK_EQUAL(TokenType::numeric_literal, token.type());
        CHECK_EQUAL("05", token.to_string());

        REQUIRE CHECK_EQUAL(0U, tokens.size());
    }

    struct WhenNextCharIs6 : public NumericLiteralFixture
    {
        const std::string s = "06";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIs6, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::NumericLiteral, tokenState);
        CHECK_EQUAL(TokenType::numeric_literal, token.type());
        CHECK_EQUAL("0", token.to_string());

        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::NumericLiteral, tokenState);
        CHECK_EQUAL(TokenType::numeric_literal, token.type());
        CHECK_EQUAL("06", token.to_string());

        REQUIRE CHECK_EQUAL(0U, tokens.size());
    }

    struct WhenNextCharIs7 : public NumericLiteralFixture
    {
        const std::string s = "07";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIs7, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::NumericLiteral, tokenState);
        CHECK_EQUAL(TokenType::numeric_literal, token.type());
        CHECK_EQUAL("0", token.to_string());

        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::NumericLiteral, tokenState);
        CHECK_EQUAL(TokenType::numeric_literal, token.type());
        CHECK_EQUAL("07", token.to_string());

        REQUIRE CHECK_EQUAL(0U, tokens.size());
    }

    struct WhenNextCharIs8 : public NumericLiteralFixture
    {
        const std::string s = "08";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIs8, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::NumericLiteral, tokenState);
        CHECK_EQUAL(TokenType::numeric_literal, token.type());
        CHECK_EQUAL("0", token.to_string());

        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::NumericLiteral, tokenState);
        CHECK_EQUAL(TokenType::numeric_literal, token.type());
        CHECK_EQUAL("08", token.to_string());

        REQUIRE CHECK_EQUAL(0U, tokens.size());
    }

    struct WhenNextCharIs9 : public NumericLiteralFixture
    {
        const std::string s = "09";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIs9, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::NumericLiteral, tokenState);
        CHECK_EQUAL(TokenType::numeric_literal, token.type());
        CHECK_EQUAL("0", token.to_string());

        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::NumericLiteral, tokenState);
        CHECK_EQUAL(TokenType::numeric_literal, token.type());
        CHECK_EQUAL("09", token.to_string());

        REQUIRE CHECK_EQUAL(0U, tokens.size());
    }

    struct WhenNextCharIsSpace : public NumericLiteralFixture
    {
        const std::string s = "0 ";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsSpace, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::NumericLiteral, tokenState);
        CHECK_EQUAL(TokenType::numeric_literal, token.type());
        CHECK_EQUAL("0", token.to_string());

        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::Init, tokenState);

        REQUIRE CHECK_EQUAL(1U, tokens.size());
        CHECK_EQUAL("0", tokens[0].token().to_string());
        CHECK_EQUAL(TokenType::numeric_literal, tokens[0].token().type());
    }

    struct WhenNextCharIsEqual : public NumericLiteralFixture
    {
        const std::string s = "0=";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsEqual, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::NumericLiteral, tokenState);
        CHECK_EQUAL(TokenType::numeric_literal, token.type());
        CHECK_EQUAL("0", token.to_string());

        CHECK_THROW(state.consume(sv, position++, info, token), swizzle::TokenizerError);
    }

    struct WhenNextCharIsLeftBracket : public NumericLiteralFixture
    {
        const std::string s = "0[";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsLeftBracket, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::NumericLiteral, tokenState);
        CHECK_EQUAL(TokenType::numeric_literal, token.type());
        CHECK_EQUAL("0", token.to_string());

        CHECK_THROW(state.consume(sv, position++, info, token), swizzle::TokenizerError);
    }

    struct WhenNextCharIsRightBracket : public NumericLiteralFixture
    {
        const std::string s = "0]";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsRightBracket, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::NumericLiteral, tokenState);
        CHECK_EQUAL(TokenType::numeric_literal, token.type());
        CHECK_EQUAL("0", token.to_string());

        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::Init, tokenState);

        REQUIRE CHECK_EQUAL(2U, tokens.size());

        CHECK_EQUAL("0", tokens[0].token().to_string());
        CHECK_EQUAL(TokenType::numeric_literal, tokens[0].token().type());

        CHECK_EQUAL("]", tokens[1].token().to_string());
        CHECK_EQUAL(TokenType::r_bracket, tokens[1].token().type());
    }

    struct WhenNextCharIsLeftBrace : public NumericLiteralFixture
    {
        const std::string s = "0{";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsLeftBrace, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::NumericLiteral, tokenState);
        CHECK_EQUAL(TokenType::numeric_literal, token.type());
        CHECK_EQUAL("0", token.to_string());

        CHECK_THROW(state.consume(sv, position++, info, token), swizzle::TokenizerError);
    }

    struct WhenNextCharIsRightBrace : public NumericLiteralFixture
    {
        const std::string s = "0}";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsRightBrace, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::NumericLiteral, tokenState);
        CHECK_EQUAL(TokenType::numeric_literal, token.type());
        CHECK_EQUAL("0", token.to_string());

        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::Init, tokenState);

        REQUIRE CHECK_EQUAL(2U, tokens.size());

        CHECK_EQUAL("0", tokens[0].token().to_string());
        CHECK_EQUAL(TokenType::numeric_literal, tokens[0].token().type());

        CHECK_EQUAL("}", tokens[1].token().to_string());
        CHECK_EQUAL(TokenType::r_brace, tokens[1].token().type());
    }

    struct WhenNextCharIsColon : public NumericLiteralFixture
    {
        const std::string s = "0:";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsColon, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::NumericLiteral, tokenState);
        CHECK_EQUAL(TokenType::numeric_literal, token.type());
        CHECK_EQUAL("0", token.to_string());

        CHECK_THROW(state.consume(sv, position++, info, token), swizzle::TokenizerError);
    }

    struct WhenNextCharIsSemiColon : public NumericLiteralFixture
    {
        const std::string s = "0;";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsSemiColon, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::NumericLiteral, tokenState);
        CHECK_EQUAL(TokenType::numeric_literal, token.type());
        CHECK_EQUAL("0", token.to_string());

        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::Init, tokenState);

        REQUIRE CHECK_EQUAL(2U, tokens.size());

        CHECK_EQUAL("0", tokens[0].token().to_string());
        CHECK_EQUAL(TokenType::numeric_literal, tokens[0].token().type());

        CHECK_EQUAL(";", tokens[1].token().to_string());
        CHECK_EQUAL(TokenType::end_statement, tokens[1].token().type());
    }

    struct WhenNextCharIsDot : public NumericLiteralFixture
    {
        const std::string s = "0.";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsDot, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::NumericLiteral, tokenState);
        CHECK_EQUAL(TokenType::numeric_literal, token.type());
        CHECK_EQUAL("0", token.to_string());

        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::Init, tokenState);

        REQUIRE CHECK_EQUAL(2U, tokens.size());

        CHECK_EQUAL("0", tokens[0].token().to_string());
        CHECK_EQUAL(TokenType::numeric_literal, tokens[0].token().type());

        CHECK_EQUAL(".", tokens[1].token().to_string());
        CHECK_EQUAL(TokenType::dot, tokens[1].token().type());
    }

    struct WhenNextCharIsComma : public NumericLiteralFixture
    {
        const std::string s = "0,";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsComma, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::NumericLiteral, tokenState);
        CHECK_EQUAL(TokenType::numeric_literal, token.type());
        CHECK_EQUAL("0", token.to_string());

        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::Init, tokenState);

        REQUIRE CHECK_EQUAL(2U, tokens.size());

        CHECK_EQUAL("0", tokens[0].token().to_string());
        CHECK_EQUAL(TokenType::numeric_literal, tokens[0].token().type());

        CHECK_EQUAL(",", tokens[1].token().to_string());
        CHECK_EQUAL(TokenType::comma, tokens[1].token().type());
    }

    struct WhenNextCharIsDoubleQuote : public NumericLiteralFixture
    {
        const std::string s = "0\"";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsDoubleQuote, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::NumericLiteral, tokenState);
        CHECK_EQUAL(TokenType::numeric_literal, token.type());
        CHECK_EQUAL("0", token.to_string());

        CHECK_THROW(state.consume(sv, position++, info, token), swizzle::TokenizerError);
    }

    struct WhenNextCharIsAlpha : public NumericLiteralFixture
    {
        const std::string s = "0a";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsAlpha, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::NumericLiteral, tokenState);
        CHECK_EQUAL(TokenType::numeric_literal, token.type());
        CHECK_EQUAL("0", token.to_string());

        CHECK_THROW(state.consume(sv, position++, info, token), swizzle::TokenizerError);
    }

}
