#include "./ut_support/UnitTestSupport.hpp"

#include <swizzle/ast/Matcher.hpp>
#include <swizzle/ast/nodes/Comment.hpp>
#include <swizzle/ast/nodes/Import.hpp>
#include <swizzle/ast/nodes/MultilineComment.hpp>

#include <swizzle/lexer/Tokenizer.hpp>
#include <swizzle/parser/Parser.hpp>

#include <boost/filesystem.hpp>
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

    struct WhenInputIsMultilineComment : public ParserFixture
    {
        const boost::string_view sv = boost::string_view("// multi-line \\\n   comment");
    };

    TEST_FIXTURE(WhenInputIsMultilineComment, verifyConsume)
    {
        auto matcher = Matcher().hasChildOf<nodes::MultilineComment>();
        CHECK(!matcher(parser.ast().root()));

        tokenize(sv);
        parse();

        CHECK(matcher(parser.ast().root()));
    }

    struct WhenInputIsImport : public ParserFixture
    {
        WhenInputIsImport()
        {
            boost::filesystem::fstream file(testFile, std::ios::out | std::ios::app);
        }

        ~WhenInputIsImport()
        {
            boost::filesystem::remove(testFile);
        }

        const boost::string_view sv = boost::string_view("import Flan;");
        const boost::filesystem::path testFile = boost::filesystem::path("Flan.swizzle");
    };

    TEST_FIXTURE(WhenInputIsImport, verifyConsume)
    {
        auto matcher = Matcher().getChildrenOf<nodes::Import>().bind("import");
        CHECK(!matcher(parser.ast().root()));

        tokenize(sv);
        parse();

        CHECK(matcher(parser.ast().root()));

        const auto node = matcher.bound("import_0");
        REQUIRE CHECK(node);

        const auto& import = static_cast<nodes::Import&>(*node);
        CHECK_EQUAL("Flan.swizzle", import.path());
    }

    struct WhenInputIsImportWithNamespace : public ParserFixture
    {
        WhenInputIsImportWithNamespace()
        {
            boost::filesystem::create_directories(testFile.parent_path());
            boost::filesystem::fstream file(testFile, std::ios::out | std::ios::app);
        }

        ~WhenInputIsImportWithNamespace()
        {
            boost::filesystem::remove(testFile);
            boost::filesystem::remove_all("./foo");
        }

        const boost::string_view sv = boost::string_view("import foo::bar::Flan;");
        const boost::filesystem::path testFile = boost::filesystem::path("foo/bar/Flan.swizzle");
    };

    TEST_FIXTURE(WhenInputIsImportWithNamespace, verifyConsume)
    {
        auto matcher = Matcher().getChildrenOf<nodes::Import>().bind("import");
        CHECK(!matcher(parser.ast().root()));

        tokenize(sv);
        parse();

        CHECK(matcher(parser.ast().root()));

        const auto node = matcher.bound("import_0");
        REQUIRE CHECK(node);

        const auto& import = static_cast<nodes::Import&>(*node);
        CHECK_EQUAL("foo/bar/Flan.swizzle", import.path());
    }
}
