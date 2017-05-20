#include "./platform/UnitTestSupport.hpp"
#include <swizzle/lexer/states/AttributeBlockState.hpp>

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

    struct AttributeStateFixture
    {
        std::deque<TokenInfo> tokens;
        CreateTokenCallback callback = CreateTokenCallback(tokens);

        FileInfo info = FileInfo("testfile");
        states::AttributeBlockState<CreateTokenCallback> state = states::AttributeBlockState<CreateTokenCallback>(callback);

        std::size_t position = 0;
    };

    TEST_FIXTURE(AttributeStateFixture, verifyConstruction)
    {
    }

    struct WhenNextCharIsRightBrace : public AttributeStateFixture
    {
        const std::string s = "a}";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::attribute_block);
    };

    TEST_FIXTURE(WhenNextCharIsRightBrace, verifyConsume)
    {
        // consume a
        auto tokenState = state.consume(sv, position++, info, token);
        CHECK_EQUAL(TokenizerState::AttributeBlock, tokenState);

        // consume }
        tokenState = state.consume(sv, position++, info, token);
        CHECK_EQUAL(TokenizerState::Init, tokenState);

        REQUIRE CHECK_EQUAL(2U, tokens.size());

        CHECK_EQUAL(TokenType::attribute_block, tokens[0].token().type());
        CHECK_EQUAL("a", tokens[0].token().to_string());

        CHECK_EQUAL(TokenType::r_brace, tokens[1].token().type());
        CHECK_EQUAL("}", tokens[1].token().to_string());
    }

    struct WhenNextCharIsAlpha : public AttributeStateFixture
    {
        const std::string s = "a";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::attribute_block);
    };

    TEST_FIXTURE(WhenNextCharIsAlpha, verifyConsume)
    {
        // consume a
        const auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::AttributeBlock, tokenState);
        CHECK_EQUAL(TokenType::attribute_block, token.type());
        CHECK_EQUAL("a", token.to_string());
    }

    struct WhenNextCharIsNumeric : public AttributeStateFixture
    {
        const std::string s = "1";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::attribute_block);
    };

    TEST_FIXTURE(WhenNextCharIsNumeric, verifyConsume)
    {
        // consume 1
        const auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::AttributeBlock, tokenState);
        CHECK_EQUAL(TokenType::attribute_block, token.type());
        CHECK_EQUAL("1", token.to_string());
    }

    struct WhenNextCharIsUnderbar : public AttributeStateFixture
    {
        const std::string s = "_";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::attribute_block);
    };

    TEST_FIXTURE(WhenNextCharIsUnderbar, verifyConsume)
    {
        // consume _
        const auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::AttributeBlock, tokenState);
        CHECK_EQUAL(TokenType::attribute_block, token.type());
        CHECK_EQUAL("_", token.to_string());
    }

    struct WhenNextCharIsDash : public AttributeStateFixture
    {
        const std::string s = "-";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::attribute_block);
    };

    TEST_FIXTURE(WhenNextCharIsDash, verifyConsume)
    {
        // consume -
        const auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::AttributeBlock, tokenState);
        CHECK_EQUAL(TokenType::attribute_block, token.type());
        CHECK_EQUAL("-", token.to_string());
    }

    struct WhenNextCharIsEqual : public AttributeStateFixture
    {
        const std::string s = "=";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::attribute_block);
    };

    TEST_FIXTURE(WhenNextCharIsEqual, verifyConsume)
    {
        // consume =
        const auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::AttributeBlock, tokenState);
        CHECK_EQUAL(TokenType::attribute_block, token.type());
        CHECK_EQUAL("=", token.to_string());
    }

    struct WhenNextCharIsLeftBracket : public AttributeStateFixture
    {
        const std::string s = "[";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::attribute_block);
    };

    TEST_FIXTURE(WhenNextCharIsLeftBracket, verifyConsume)
    {
        // consume [
        const auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::AttributeBlock, tokenState);
        CHECK_EQUAL(TokenType::attribute_block, token.type());
        CHECK_EQUAL("[", token.to_string());
    }

    struct WhenNextCharIsRightBracket : public AttributeStateFixture
    {
        const std::string s = "]";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::attribute_block);
    };

    TEST_FIXTURE(WhenNextCharIsRightBracket, verifyConsume)
    {
        // consume [
        const auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::AttributeBlock, tokenState);
        CHECK_EQUAL(TokenType::attribute_block, token.type());
        CHECK_EQUAL("]", token.to_string());
    }

    struct WhenNextCharIsAt : public AttributeStateFixture
    {
        const std::string s = "@";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::attribute_block);
    };

    TEST_FIXTURE(WhenNextCharIsAt, verifyConsume)
    {
        // consume @
        const auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::AttributeBlock, tokenState);
        CHECK_EQUAL(TokenType::attribute_block, token.type());
        CHECK_EQUAL("@", token.to_string());
    }

    struct WhenNextCharIsLeftParen : public AttributeStateFixture
    {
        const std::string s = "(";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::attribute_block);
    };

    TEST_FIXTURE(WhenNextCharIsLeftParen, verifyConsume)
    {
        // consume (
        const auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::AttributeBlock, tokenState);
        CHECK_EQUAL(TokenType::attribute_block, token.type());
        CHECK_EQUAL("(", token.to_string());
    }

    struct WhenNextCharIsRightParen : public AttributeStateFixture
    {
        const std::string s = ")";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::attribute_block);
    };

    TEST_FIXTURE(WhenNextCharIsRightParen, verifyConsume)
    {
        // consume )
        const auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::AttributeBlock, tokenState);
        CHECK_EQUAL(TokenType::attribute_block, token.type());
        CHECK_EQUAL(")", token.to_string());
    }

    struct WhenNextCharIsPlus : public AttributeStateFixture
    {
        const std::string s = "+";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::attribute_block);
    };

    TEST_FIXTURE(WhenNextCharIsPlus, verifyConsume)
    {
        // consume +
        const auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::AttributeBlock, tokenState);
        CHECK_EQUAL(TokenType::attribute_block, token.type());
        CHECK_EQUAL("+", token.to_string());
    }

    struct WhenNextCharIsStar : public AttributeStateFixture
    {
        const std::string s = "*";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::attribute_block);
    };

    TEST_FIXTURE(WhenNextCharIsStar, verifyConsume)
    {
        // consume *
        const auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::AttributeBlock, tokenState);
        CHECK_EQUAL(TokenType::attribute_block, token.type());
        CHECK_EQUAL("*", token.to_string());
    }

    struct WhenNextCharIsSlash : public AttributeStateFixture
    {
        const std::string s = "/";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::attribute_block);
    };

    TEST_FIXTURE(WhenNextCharIsSlash, verifyConsume)
    {
        // consume /
        const auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::AttributeBlock, tokenState);
        CHECK_EQUAL(TokenType::attribute_block, token.type());
        CHECK_EQUAL("/", token.to_string());
    }

    struct WhenNextCharIsBlackslash : public AttributeStateFixture
    {
        const std::string s = "\\";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::attribute_block);
    };

    TEST_FIXTURE(WhenNextCharIsBlackslash, verifyConsume)
    {
        // consume backslash
        const auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::AttributeBlock, tokenState);
        CHECK_EQUAL(TokenType::attribute_block, token.type());
        CHECK_EQUAL("\\", token.to_string());
    }
}
