#include "./ut_support/UnitTestSupport.hpp"
#include <swizzle/types/utils/StackInvert.hpp>

namespace {

    using namespace swizzle::lexer;
    using namespace swizzle::types;

    struct InvertFixture
    {
        InvertFixture()
        {
            tokenStack.emplace(TokenInfo(Token("foo", 0, 3, TokenType::string), FileInfo("test.swizzle")));
            tokenStack.emplace(TokenInfo(Token("bar", 0, 3, TokenType::string), FileInfo("test.swizzle")));
            tokenStack.emplace(TokenInfo(Token("baz", 0, 3, TokenType::string), FileInfo("test.swizzle")));
        }

        TokenStack tokenStack;
    };

    TEST_FIXTURE(InvertFixture, verifyInvert)
    {
        TokenStack stack = utils::invert(tokenStack);

        CHECK_EQUAL("foo", stack.top().token().to_string()); stack.pop();
        CHECK_EQUAL("bar", stack.top().token().to_string()); stack.pop();
        CHECK_EQUAL("baz", stack.top().token().to_string()); stack.pop();
    }
}
