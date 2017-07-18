#include "./ut_support/UnitTestSupport.hpp"

#include <swizzle/ast/Matcher.hpp>
#include <swizzle/ast/nodes/Comment.hpp>

#include <swizzle/lexer/Tokenizer.hpp>
#include <swizzle/parser/Parser.hpp>

#include <boost/utility/string_view.hpp>
#include <deque>

namespace {

    using namespace swizzle::ast;
    using namespace swizzle::lexer;
    using namespace swizzle::parser;

    struct CreateTokenCallback
    {
        CreateTokenCallback(std::deque<TokenInfo>& tokens)
            : tokens_(tokens)
        {
        }

        void operator()(const TokenInfo& token)
        {
            tokens_.push_back(token);
        }

    private:
        std::deque<TokenInfo>& tokens_;
    };

    struct ParserFixture
    {
        void tokenize(const boost::string_view& sv)
        {
            for(std::size_t position = 0, end = sv.length(); position < end; ++position)
            {
                tokenizer.consume(sv, position);
            }

            tokenizer.flush();
        }

        void parse()
        {
            for(const auto token : tokens)
            {
                parser.consume(token);
            }
        }

        std::deque<TokenInfo> tokens;

        CreateTokenCallback callback = CreateTokenCallback(tokens);
        Tokenizer<CreateTokenCallback> tokenizer = Tokenizer<CreateTokenCallback>("test.swizzle", callback);

        Parser parser;
    };

    TEST_FIXTURE(ParserFixture, verifyConstruction)
    {
    }

    struct WhenInputIsComment : public ParserFixture
    {
        const boost::string_view sv = boost::string_view("// comment");
    };

    TEST_FIXTURE(WhenInputIsComment, verifyConsume)
    {
        auto matcher = Matcher().hasChildOf<nodes::Comment>();
        CHECK(!matcher(parser.ast().root()));

        tokenize(sv);
        parse();

        CHECK(matcher(parser.ast().root()));
    }
}
