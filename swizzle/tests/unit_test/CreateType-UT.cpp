#include "./platform/UnitTestSupport.hpp"

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/TokenStack.hpp>
#include <swizzle/parser/detail/CreateType.hpp>

#include <boost/utility/string_view.hpp>

namespace {

    using namespace swizzle::lexer;
    using namespace swizzle::parser;

    struct CreateTokenFixture
    {
        CreateTokenFixture()
        {
            tokenStack.emplace(Token(sv, 0, 3, TokenType::string), FileInfo("test.swizzle", LineInfo(1, 0), LineInfo(1, 4)));
            tokenStack.emplace(Token(sv, 6, 3, TokenType::string), FileInfo("test.swizzle", LineInfo(1, 6), LineInfo(1, 9)));
            tokenStack.emplace(Token(sv, 10, 6, TokenType::string), FileInfo("test.swizzle", LineInfo(1, 11), LineInfo(1, 17)));
        }

        const boost::string_view sv = "foo::bar::MyType";
        TokenStack tokenStack;
    };

    TEST_FIXTURE(CreateTokenFixture, verifyCreateType)
    {
        CHECK_EQUAL(3U, tokenStack.size());

        const auto info = detail::createType(tokenStack);

        CHECK_EQUAL("foo::bar::MyType", info.token().to_string());
        CHECK_EQUAL(0U, tokenStack.size());
    }

    struct WhenTokensNotOnTheSameLine : public CreateTokenFixture
    {
        WhenTokensNotOnTheSameLine()
        {
            tokenStack.pop();
            tokenStack.emplace(Token(sv, 10, 6, TokenType::string), FileInfo("test.swizzle", LineInfo(2, 1), LineInfo(2, 7)));
        }
    };

    TEST_FIXTURE(WhenTokensNotOnTheSameLine, verifyCreateType)
    {
        CHECK_EQUAL(3U, tokenStack.size());
        CHECK_THROW(detail::createType(tokenStack), swizzle::ParserError);
    }
}
