#include "./ut_support/UnitTestSupport.hpp"
#include <swizzle/lexer/states/AttributeState.hpp>

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
        states::AttributeState<CreateTokenCallback> state = states::AttributeState<CreateTokenCallback>(callback);

        std::size_t position = 0;
    };

    TEST_FIXTURE(AttributeStateFixture, verifyConstruction)
    {
    }

    struct WhenNextCharIsAlpha : public AttributeStateFixture
    {
        const std::string s = "a";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::attribute);
    };

    TEST_FIXTURE(WhenNextCharIsAlpha, verifyConsume)
    {
        // consume a
        const auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::Attribute, tokenState);
        CHECK_EQUAL(TokenType::attribute, token.type());
        CHECK_EQUAL("a", token.to_string());
    }

    struct WhenNextCharIsNumeric : public AttributeStateFixture
    {
        const std::string s = "1";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::attribute);
    };

    TEST_FIXTURE(WhenNextCharIsNumeric, verifyConsume)
    {
        // consume 1
        const auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::Attribute, tokenState);
        CHECK_EQUAL(TokenType::attribute, token.type());
        CHECK_EQUAL("1", token.to_string());
    }

    struct WhenNextCharIsUnderbar : public AttributeStateFixture
    {
        const std::string s = "_";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::attribute);
    };

    TEST_FIXTURE(WhenNextCharIsUnderbar, verifyConsume)
    {
        // consume _
        const auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::Attribute, tokenState);
        CHECK_EQUAL(TokenType::attribute, token.type());
        CHECK_EQUAL("_", token.to_string());
    }

    struct WhenNextCharIsDash : public AttributeStateFixture
    {
        const std::string s = "-";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::attribute);
    };

    TEST_FIXTURE(WhenNextCharIsDash, verifyConsume)
    {
        // consume -
        const auto tokenState = state.consume(sv, position++, info, token);

        CHECK_EQUAL(TokenizerState::Attribute, tokenState);
        CHECK_EQUAL(TokenType::attribute, token.type());
        CHECK_EQUAL("-", token.to_string());
    }

    struct WhenNextCharIsEqual : public AttributeStateFixture
    {
        const std::string s = "a=";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::attribute);
    };

    TEST_FIXTURE(WhenNextCharIsEqual, verifyConsume)
    {
        // consume a
        auto tokenState = state.consume(sv, position++, info, token);
        CHECK_EQUAL(TokenizerState::Attribute, tokenState);

        // consume {
        tokenState = state.consume(sv, position++, info, token);
        CHECK_EQUAL(TokenizerState::Init, tokenState);

        REQUIRE CHECK_EQUAL(2U, tokens.size());

        CHECK_EQUAL(TokenType::attribute, tokens[0].token().type());
        CHECK_EQUAL("a", tokens[0].token().to_string());

        CHECK_EQUAL(1U, tokens[0].fileInfo().start().line());
        CHECK_EQUAL(1U, tokens[0].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[0].fileInfo().end().line());
        CHECK_EQUAL(2U, tokens[0].fileInfo().end().column());

        CHECK_EQUAL(TokenType::equal, tokens[1].token().type());
        CHECK_EQUAL("=", tokens[1].token().to_string());

        CHECK_EQUAL(1U, tokens[1].fileInfo().start().line());
        CHECK_EQUAL(2U, tokens[1].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[1].fileInfo().end().line());
        CHECK_EQUAL(3U, tokens[1].fileInfo().end().column());
    }

    struct WhenNextCharIsBrace : public AttributeStateFixture
    {
        const std::string s = "a{";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::attribute);
    };

    TEST_FIXTURE(WhenNextCharIsBrace, verifyConsume)
    {
        // consume a
        auto tokenState = state.consume(sv, position++, info, token);
        CHECK_EQUAL(TokenizerState::Attribute, tokenState);

        // consume {
        tokenState = state.consume(sv, position++, info, token);
        CHECK_EQUAL(TokenizerState::AttributeBlock, tokenState);

        REQUIRE CHECK_EQUAL(1U, tokens.size());

        CHECK_EQUAL(TokenType::attribute, tokens[0].token().type());
        CHECK_EQUAL("a", tokens[0].token().to_string());

        CHECK_EQUAL(1U, tokens[0].fileInfo().start().line());
        CHECK_EQUAL(1U, tokens[0].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[0].fileInfo().end().line());
        CHECK_EQUAL(2U, tokens[0].fileInfo().end().column());
    }

    struct WhenNextCharIsSpace : public AttributeStateFixture
    {
        const std::string s = "a ";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::attribute);
    };

    TEST_FIXTURE(WhenNextCharIsSpace, verifyConsume)
    {
        // consume a
        auto tokenState = state.consume(sv, position++, info, token);
        CHECK_EQUAL(TokenizerState::Attribute, tokenState);

        // consume space
        tokenState = state.consume(sv, position++, info, token);
        CHECK_EQUAL(TokenizerState::Init, tokenState);

        REQUIRE CHECK_EQUAL(1U, tokens.size());

        CHECK_EQUAL(TokenType::attribute, tokens[0].token().type());
        CHECK_EQUAL("a", tokens[0].token().to_string());

        CHECK_EQUAL(1U, tokens[0].fileInfo().start().line());
        CHECK_EQUAL(1U, tokens[0].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[0].fileInfo().end().line());
        CHECK_EQUAL(2U, tokens[0].fileInfo().end().column());
    }

    struct WhenNextCharIsTab : public AttributeStateFixture
    {
        const std::string s = "a\t";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::attribute);
    };

    TEST_FIXTURE(WhenNextCharIsTab, verifyConsume)
    {
        // consume a
        auto tokenState = state.consume(sv, position++, info, token);
        CHECK_EQUAL(TokenizerState::Attribute, tokenState);

        // consume tab
        CHECK_THROW(state.consume(sv, position++, info, token), swizzle::TokenizerSyntaxError);
    }

    struct WhenNextCharIsReturn : public AttributeStateFixture
    {
        const std::string s = "a\r";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::attribute);
    };

    TEST_FIXTURE(WhenNextCharIsReturn, verifyConsume)
    {
        // consume a
        auto tokenState = state.consume(sv, position++, info, token);
        CHECK_EQUAL(TokenizerState::Attribute, tokenState);

        // consume \r
        tokenState = state.consume(sv, position++, info, token);
        CHECK_EQUAL(TokenizerState::Init, tokenState);

        REQUIRE CHECK_EQUAL(1U, tokens.size());

        CHECK_EQUAL(TokenType::attribute, tokens[0].token().type());
        CHECK_EQUAL("a", tokens[0].token().to_string());

        CHECK_EQUAL(1U, tokens[0].fileInfo().start().line());
        CHECK_EQUAL(1U, tokens[0].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[0].fileInfo().end().line());
        CHECK_EQUAL(2U, tokens[0].fileInfo().end().column());
    }

    struct WhenNextCharIsNewLine : public AttributeStateFixture
    {
        const std::string s = "a\n";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::attribute);
    };

    TEST_FIXTURE(WhenNextCharIsNewLine, verifyConsume)
    {
        // consume a
        auto tokenState = state.consume(sv, position++, info, token);
        CHECK_EQUAL(TokenizerState::Attribute, tokenState);

        // consume \n
        tokenState = state.consume(sv, position++, info, token);
        CHECK_EQUAL(TokenizerState::Init, tokenState);

        REQUIRE CHECK_EQUAL(1U, tokens.size());

        CHECK_EQUAL(TokenType::attribute, tokens[0].token().type());
        CHECK_EQUAL("a", tokens[0].token().to_string());

        CHECK_EQUAL(1U, tokens[0].fileInfo().start().line());
        CHECK_EQUAL(1U, tokens[0].fileInfo().start().column());
        CHECK_EQUAL(1U, tokens[0].fileInfo().end().line());
        CHECK_EQUAL(2U, tokens[0].fileInfo().end().column());
    }

    struct WhenNextCharIsInvalid : public AttributeStateFixture
    {
        const std::string s = "a]";
        const boost::string_view sv = boost::string_view(s);

        Token token = Token(sv, 0, 0, TokenType::attribute);
    };

    TEST_FIXTURE(WhenNextCharIsInvalid, verifyConsume)
    {
        // consume a
        auto tokenState = state.consume(sv, position++, info, token);
        CHECK_EQUAL(TokenizerState::Attribute, tokenState);

        // consume ]
        CHECK_THROW(state.consume(sv, position++, info, token), swizzle::TokenizerError);
    }

}
