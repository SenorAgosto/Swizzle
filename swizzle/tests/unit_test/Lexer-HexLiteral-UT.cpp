#include "./platform/UnitTestSupport.hpp"
#include <swizzle/lexer/states/HexLiteralState.hpp>

#include <swizzle/Exceptions.hpp>
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

    struct HexLiteralFixture
    {
        std::deque<TokenInfo> tokens;
        CreateTokenCallback callback = CreateTokenCallback(tokens);

        Token token = Token(boost::string_view(), TokenType::hex_literal);
        FileInfo info = FileInfo("testfile");

        states::HexLiteralState<CreateTokenCallback> state = states::HexLiteralState<CreateTokenCallback>(callback);

        std::size_t position = 0;
    };

    TEST_FIXTURE(HexLiteralFixture, verifyConstruction)
    {
    }

    struct WhenNextCharIs0 : public HexLiteralFixture
    {
        const std::string s = "0";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIs0, verifyConsumer)
    {
        const auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::HexLiteral, tokenState);
        CHECK_EQUAL(TokenType::hex_literal, token.type());
        CHECK_EQUAL("0", token.to_string());
    }

    struct WhenNextCharIs1 : public HexLiteralFixture
    {
        const std::string s = "1";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIs1, verifyConsumer)
    {
        const auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::HexLiteral, tokenState);
        CHECK_EQUAL(TokenType::hex_literal, token.type());
        CHECK_EQUAL("1", token.to_string());
    }

    struct WhenNextCharIs2 : public HexLiteralFixture
    {
        const std::string s = "2";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIs2, verifyConsumer)
    {
        const auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::HexLiteral, tokenState);
        CHECK_EQUAL(TokenType::hex_literal, token.type());
        CHECK_EQUAL("2", token.to_string());
    }

    struct WhenNextCharIs3 : public HexLiteralFixture
    {
        const std::string s = "3";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIs3, verifyConsumer)
    {
        const auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::HexLiteral, tokenState);
        CHECK_EQUAL(TokenType::hex_literal, token.type());
        CHECK_EQUAL("3", token.to_string());
    }

    struct WhenNextCharIs4 : public HexLiteralFixture
    {
        const std::string s = "4";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIs4, verifyConsumer)
    {
        const auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::HexLiteral, tokenState);
        CHECK_EQUAL(TokenType::hex_literal, token.type());
        CHECK_EQUAL("4", token.to_string());
    }

    struct WhenNextCharIs5 : public HexLiteralFixture
    {
        const std::string s = "5";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIs5, verifyConsumer)
    {
        const auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::HexLiteral, tokenState);
        CHECK_EQUAL(TokenType::hex_literal, token.type());
        CHECK_EQUAL("5", token.to_string());
    }

    struct WhenNextCharIs6 : public HexLiteralFixture
    {
        const std::string s = "6";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIs6, verifyConsumer)
    {
        const auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::HexLiteral, tokenState);
        CHECK_EQUAL(TokenType::hex_literal, token.type());
        CHECK_EQUAL("6", token.to_string());
    }

    struct WhenNextCharIs7 : public HexLiteralFixture
    {
        const std::string s = "7";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIs7, verifyConsumer)
    {
        const auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::HexLiteral, tokenState);
        CHECK_EQUAL(TokenType::hex_literal, token.type());
        CHECK_EQUAL("7", token.to_string());
    }

    struct WhenNextCharIs8 : public HexLiteralFixture
    {
        const std::string s = "8";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIs8, verifyConsumer)
    {
        const auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::HexLiteral, tokenState);
        CHECK_EQUAL(TokenType::hex_literal, token.type());
        CHECK_EQUAL("8", token.to_string());
    }

    struct WhenNextCharIs9 : public HexLiteralFixture
    {
        const std::string s = "9";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIs9, verifyConsumer)
    {
        const auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::HexLiteral, tokenState);
        CHECK_EQUAL(TokenType::hex_literal, token.type());
        CHECK_EQUAL("9", token.to_string());
    }

    struct WhenNextCharIsA : public HexLiteralFixture
    {
        const std::string s = "aA";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsA, verifyConsumer)
    {
        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::HexLiteral, tokenState);
        CHECK_EQUAL(TokenType::hex_literal, token.type());
        CHECK_EQUAL("a", token.to_string());

        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::HexLiteral, tokenState);
        CHECK_EQUAL(TokenType::hex_literal, token.type());
        CHECK_EQUAL("aA", token.to_string());
    }

    struct WhenNextCharIsB : public HexLiteralFixture
    {
        const std::string s = "bB";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsB, verifyConsumer)
    {
        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::HexLiteral, tokenState);
        CHECK_EQUAL(TokenType::hex_literal, token.type());
        CHECK_EQUAL("b", token.to_string());

        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::HexLiteral, tokenState);
        CHECK_EQUAL(TokenType::hex_literal, token.type());
        CHECK_EQUAL("bB", token.to_string());
    }

    struct WhenNextCharIsC : public HexLiteralFixture
    {
        const std::string s = "cC";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsC, verifyConsumer)
    {
        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::HexLiteral, tokenState);
        CHECK_EQUAL(TokenType::hex_literal, token.type());
        CHECK_EQUAL("c", token.to_string());

        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::HexLiteral, tokenState);
        CHECK_EQUAL(TokenType::hex_literal, token.type());
        CHECK_EQUAL("cC", token.to_string());
    }

    struct WhenNextCharIsD : public HexLiteralFixture
    {
        const std::string s = "dD";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsD, verifyConsumer)
    {
        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::HexLiteral, tokenState);
        CHECK_EQUAL(TokenType::hex_literal, token.type());
        CHECK_EQUAL("d", token.to_string());

        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::HexLiteral, tokenState);
        CHECK_EQUAL(TokenType::hex_literal, token.type());
        CHECK_EQUAL("dD", token.to_string());
    }

    struct WhenNextCharIsE : public HexLiteralFixture
    {
        const std::string s = "eE";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsE, verifyConsumer)
    {
        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::HexLiteral, tokenState);
        CHECK_EQUAL(TokenType::hex_literal, token.type());
        CHECK_EQUAL("e", token.to_string());

        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::HexLiteral, tokenState);
        CHECK_EQUAL(TokenType::hex_literal, token.type());
        CHECK_EQUAL("eE", token.to_string());
    }

    struct WhenNextCharIsF : public HexLiteralFixture
    {
        const std::string s = "fF";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsF, verifyConsumer)
    {
        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::HexLiteral, tokenState);
        CHECK_EQUAL(TokenType::hex_literal, token.type());
        CHECK_EQUAL("f", token.to_string());

        tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::HexLiteral, tokenState);
        CHECK_EQUAL(TokenType::hex_literal, token.type());
        CHECK_EQUAL("fF", token.to_string());
    }

    struct WhenNextCharIsNonHex : public HexLiteralFixture
    {
        const std::string s = "g";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsNonHex, verifyConsumer)
    {
        CHECK_THROW(state.consume(sv, position++, info, token), swizzle::TokenizerError);
    }

    struct WhenNextCharIsSpace : public HexLiteralFixture
    {
        const std::string s = "f ";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsSpace, verifyConsumer)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::HexLiteral, tokenState);
        CHECK_EQUAL(TokenType::hex_literal, token.type());
        CHECK_EQUAL("f", token.to_string());

        tokenState = state.consume(sv, position++, info, token);

        REQUIRE CHECK_EQUAL(1U, tokens.size());
        CHECK_EQUAL(TokenType::hex_literal, tokens[0].token().type());
        CHECK_EQUAL("f", tokens[0].token().to_string());
    }

    struct WhenNextCharIsRightBracket : public HexLiteralFixture
    {
        const std::string s = "f]";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsRightBracket, verifyConsumer)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::HexLiteral, tokenState);
        CHECK_EQUAL(TokenType::hex_literal, token.type());
        CHECK_EQUAL("f", token.to_string());

        tokenState = state.consume(sv, position++, info, token);

        REQUIRE CHECK_EQUAL(2U, tokens.size());

        CHECK_EQUAL(TokenType::hex_literal, tokens[0].token().type());
        CHECK_EQUAL("f", tokens[0].token().to_string());

        CHECK_EQUAL(TokenType::r_bracket, tokens[1].token().type());
        CHECK_EQUAL("]", tokens[1].token().to_string());
    }

    struct WhenNextCharIsRightBrace : public HexLiteralFixture
    {
        const std::string s = "}";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsRightBrace, verifyConsume)
    {
        CHECK_THROW(state.consume(sv, position++, info, token), swizzle::TokenizerError);
    }

    struct WhenNextCharIsDot : public HexLiteralFixture
    {
        const std::string s = ".";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsDot, verifyConsume)
    {
        CHECK_THROW(state.consume(sv, position++, info, token), swizzle::TokenizerError);
    }

    struct WhenNextCharIsSemiColon : public HexLiteralFixture
    {
        const std::string s = "f;";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsSemiColon, verifyConsumer)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::HexLiteral, tokenState);
        CHECK_EQUAL(TokenType::hex_literal, token.type());
        CHECK_EQUAL("f", token.to_string());

        tokenState = state.consume(sv, position++, info, token);

        REQUIRE CHECK_EQUAL(2U, tokens.size());

        CHECK_EQUAL(TokenType::hex_literal, tokens[0].token().type());
        CHECK_EQUAL("f", tokens[0].token().to_string());

        CHECK_EQUAL(TokenType::end_statement, tokens[1].token().type());
        CHECK_EQUAL(";", tokens[1].token().to_string());
    }

    struct WhenNextCharIsComma : public HexLiteralFixture
    {
        const std::string s = "f,";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsComma, verifyConsumer)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::HexLiteral, tokenState);
        CHECK_EQUAL(TokenType::hex_literal, token.type());
        CHECK_EQUAL("f", token.to_string());

        tokenState = state.consume(sv, position++, info, token);

        REQUIRE CHECK_EQUAL(2U, tokens.size());

        CHECK_EQUAL(TokenType::hex_literal, tokens[0].token().type());
        CHECK_EQUAL("f", tokens[0].token().to_string());

        CHECK_EQUAL(TokenType::comma, tokens[1].token().type());
        CHECK_EQUAL(",", tokens[1].token().to_string());
    }
}
