#include "./ut_support/UnitTestSupport.hpp"

#include <swizzle/ast/Matcher.hpp>
#include <swizzle/ast/nodes/Comment.hpp>
#include <swizzle/ast/nodes/Enum.hpp>
#include <swizzle/ast/nodes/EnumField.hpp>
#include <swizzle/ast/nodes/Extern.hpp>
#include <swizzle/ast/nodes/Import.hpp>
#include <swizzle/ast/nodes/MultilineComment.hpp>
#include <swizzle/ast/nodes/Namespace.hpp>

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

    struct WhenInputIsImportAndFileDoesntExist : public ParserFixture
    {
        const boost::string_view sv= boost::string_view("import Flan;");
    };

    TEST_FIXTURE(WhenInputIsImportAndFileDoesntExist, verifyConsume)
    {
        tokenize(sv);
        CHECK_THROW(parse(), swizzle::ParserError);
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

    struct WhenInputIsExtern : public ParserFixture
    {
        const boost::string_view sv = boost::string_view("extern Magui;");
    };

    TEST_FIXTURE(WhenInputIsExtern, verifyConsume)
    {
        auto matcher = Matcher().getChildrenOf<nodes::Extern>().bind("extern");
        CHECK(!matcher(parser.ast().root()));

        tokenize(sv);
        parse();

        CHECK(matcher(parser.ast().root()));

        const auto node = matcher.bound("extern_0");
        REQUIRE CHECK(node);

        const auto& external = static_cast<nodes::Extern&>(*node);
        CHECK_EQUAL("Magui", external.externType().token().to_string());
    }

    struct WhenInputIsExternWithNamespace : public ParserFixture
    {
        const boost::string_view sv = boost::string_view("extern foo::Magui;");
    };

    TEST_FIXTURE(WhenInputIsExternWithNamespace, verifyConsume)
    {
        auto matcher = Matcher().getChildrenOf<nodes::Extern>().bind("extern");
        CHECK(!matcher(parser.ast().root()));

        tokenize(sv);
        parse();

        CHECK(matcher(parser.ast().root()));

        const auto node = matcher.bound("extern_0");
        REQUIRE CHECK(node);

        const auto& external = static_cast<nodes::Extern&>(*node);
        CHECK_EQUAL("foo::Magui", external.externType().token().to_string());
    }

    struct WhenInputIsExternWithMultipleNamespaces : public ParserFixture
    {
        const boost::string_view sv = boost::string_view("extern foo::bar::baz::Magui;");
    };

    TEST_FIXTURE(WhenInputIsExternWithMultipleNamespaces, verifyConsume)
    {
        auto matcher = Matcher().getChildrenOf<nodes::Extern>().bind("extern");
        CHECK(!matcher(parser.ast().root()));

        tokenize(sv);
        parse();

        CHECK(matcher(parser.ast().root()));

        const auto node = matcher.bound("extern_0");
        REQUIRE CHECK(node);

        const auto& external = static_cast<nodes::Extern&>(*node);
        CHECK_EQUAL("foo::bar::baz::Magui", external.externType().token().to_string());
    }

    struct WhenInputIsNamespace : public ParserFixture
    {
        const boost::string_view sv = boost::string_view("namespace foo;");
    };

    TEST_FIXTURE(WhenInputIsNamespace, verifyConsume)
    {
        auto matcher = Matcher().getChildrenOf<nodes::Namespace>().bind("namespace");
        CHECK(!matcher(parser.ast().root()));

        tokenize(sv);
        parse();

        CHECK(matcher(parser.ast().root()));

        const auto node = matcher.bound("namespace_0");
        REQUIRE CHECK(node);

        const auto& nameSpace = static_cast<nodes::Namespace&>(*node);
        CHECK_EQUAL("foo", nameSpace.info().token().to_string());
    }

    struct WhenInputIsNestedNamespace : public ParserFixture
    {
        const boost::string_view sv = boost::string_view("namespace foo::bar;");
    };

    TEST_FIXTURE(WhenInputIsNestedNamespace, verifyConsume)
    {
        auto matcher = Matcher().getChildrenOf<nodes::Namespace>().bind("namespace");
        CHECK(!matcher(parser.ast().root()));

        tokenize(sv);
        parse();

        CHECK(matcher(parser.ast().root()));

        const auto node = matcher.bound("namespace_0");
        REQUIRE CHECK(node);

        const auto& nameSpace = static_cast<nodes::Namespace&>(*node);
        CHECK_EQUAL("foo::bar", nameSpace.info().token().to_string());
    }

    struct WhenInputIsMultipleNestedNamespaces : public ParserFixture
    {
        const boost::string_view sv = boost::string_view("namespace foo::bar::baz;");
    };

    TEST_FIXTURE(WhenInputIsMultipleNestedNamespaces, verifyConsume)
    {
        auto matcher = Matcher().getChildrenOf<nodes::Namespace>().bind("namespace");
        CHECK(!matcher(parser.ast().root()));

        tokenize(sv);
        parse();

        CHECK(matcher(parser.ast().root()));

        const auto node = matcher.bound("namespace_0");
        REQUIRE CHECK(node);

        const auto& nameSpace = static_cast<nodes::Namespace&>(*node);
        CHECK_EQUAL("foo::bar::baz", nameSpace.info().token().to_string());
    }

    struct WhenInputIsEnum : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "enum Metal : u8 {" "\n"
            "\t" "iron," "\n"
            "\t" "copper," "\n"
            "\t" "gold," "\n"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsEnum, verifyConsume)
    {
        auto matcher = Matcher().getChildrenOf<nodes::Enum>().bind("enum");
        CHECK(!matcher(parser.ast().root()));

        tokenize(sv);
        parse();

        CHECK(matcher(parser.ast().root()));

        const auto node = matcher.bound("enum_0");
        REQUIRE CHECK(node);

        const auto& Enum = static_cast<nodes::Enum&>(*node);
        CHECK_EQUAL("foo::Metal", Enum.name());
        CHECK_EQUAL("u8", Enum.underlying().token().to_string());

        auto fieldsMatcher = Matcher().getChildrenOf<nodes::EnumField>().bind("fields");
        CHECK(fieldsMatcher(node));

        const auto field0 = fieldsMatcher.bound("fields_0");
        REQUIRE CHECK(field0);

        const auto& f0 = static_cast<nodes::EnumField&>(*field0);
        CHECK_EQUAL("iron", f0.name().token().to_string());
        CHECK_EQUAL("u8", f0.underlying().token().to_string());
        CHECK_EQUAL(0U, boost::get<std::uint8_t>(f0.value()));

        const auto field1 = fieldsMatcher.bound("fields_1");
        REQUIRE CHECK(field1);

        const auto& f1 = static_cast<nodes::EnumField&>(*field1);
        CHECK_EQUAL("copper", f1.name().token().to_string());
        CHECK_EQUAL("u8", f1.underlying().token().to_string());
        CHECK_EQUAL(1U, boost::get<std::uint8_t>(f1.value()));

        const auto field2 = fieldsMatcher.bound("fields_2");
        REQUIRE CHECK(field2);

        const auto& f2 = static_cast<nodes::EnumField&>(*field2);
        CHECK_EQUAL("gold", f2.name().token().to_string());
        CHECK_EQUAL("u8", f2.underlying().token().to_string());
        CHECK_EQUAL(2U, boost::get<std::uint8_t>(f2.value()));
    }

    struct WhenInputIsEnumWithHexLiteral : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "enum Metal : u8 {" "\n"
            "\t" "iron," "\n"
            "\t" "copper = 0x04," "\n"
            "\t" "gold," "\n"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsEnumWithHexLiteral, verifyConsume)
    {
        auto matcher = Matcher().getChildrenOf<nodes::Enum>().bind("enum");
        CHECK(!matcher(parser.ast().root()));

        tokenize(sv);
        parse();

        CHECK(matcher(parser.ast().root()));

        const auto node = matcher.bound("enum_0");
        REQUIRE CHECK(node);

        const auto& Enum = static_cast<nodes::Enum&>(*node);
        CHECK_EQUAL("foo::Metal", Enum.name());
        CHECK_EQUAL("u8", Enum.underlying().token().to_string());

        auto fieldsMatcher = Matcher().getChildrenOf<nodes::EnumField>().bind("fields");
        CHECK(fieldsMatcher(node));

        const auto field0 = fieldsMatcher.bound("fields_0");
        REQUIRE CHECK(field0);

        const auto& f0 = static_cast<nodes::EnumField&>(*field0);
        CHECK_EQUAL("iron", f0.name().token().to_string());
        CHECK_EQUAL("u8", f0.underlying().token().to_string());
        CHECK_EQUAL(0U, boost::get<std::uint8_t>(f0.value()));

        const auto field1 = fieldsMatcher.bound("fields_1");
        REQUIRE CHECK(field1);

        const auto& f1 = static_cast<nodes::EnumField&>(*field1);
        CHECK_EQUAL("copper", f1.name().token().to_string());
        CHECK_EQUAL("u8", f1.underlying().token().to_string());
        CHECK_EQUAL(4U, boost::get<std::uint8_t>(f1.value()));

        const auto field2 = fieldsMatcher.bound("fields_2");
        REQUIRE CHECK(field2);

        const auto& f2 = static_cast<nodes::EnumField&>(*field2);
        CHECK_EQUAL("gold", f2.name().token().to_string());
        CHECK_EQUAL("u8", f2.underlying().token().to_string());
        CHECK_EQUAL(5U, boost::get<std::uint8_t>(f2.value()));
    }

    struct WhenInputIsEnumWithCharLiteral : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "enum Metal : u8 {" "\n"
            "\t" "iron," "\n"
            "\t" "copper = 'a'," "\n"
            "\t" "gold," "\n"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsEnumWithCharLiteral, verifyConsume)
    {
        auto matcher = Matcher().getChildrenOf<nodes::Enum>().bind("enum");
        CHECK(!matcher(parser.ast().root()));

        tokenize(sv);
        parse();

        CHECK(matcher(parser.ast().root()));

        const auto node = matcher.bound("enum_0");
        REQUIRE CHECK(node);

        const auto& Enum = static_cast<nodes::Enum&>(*node);
        CHECK_EQUAL("foo::Metal", Enum.name());
        CHECK_EQUAL("u8", Enum.underlying().token().to_string());

        auto fieldsMatcher = Matcher().getChildrenOf<nodes::EnumField>().bind("fields");
        CHECK(fieldsMatcher(node));

        const auto field0 = fieldsMatcher.bound("fields_0");
        REQUIRE CHECK(field0);

        const auto& f0 = static_cast<nodes::EnumField&>(*field0);
        CHECK_EQUAL("iron", f0.name().token().to_string());
        CHECK_EQUAL("u8", f0.underlying().token().to_string());
        CHECK_EQUAL(0U, boost::get<std::uint8_t>(f0.value()));

        const auto field1 = fieldsMatcher.bound("fields_1");
        REQUIRE CHECK(field1);

        const auto& f1 = static_cast<nodes::EnumField&>(*field1);
        CHECK_EQUAL("copper", f1.name().token().to_string());
        CHECK_EQUAL("u8", f1.underlying().token().to_string());
        CHECK_EQUAL(97U, boost::get<std::uint8_t>(f1.value()));

        const auto field2 = fieldsMatcher.bound("fields_2");
        REQUIRE CHECK(field2);

        const auto& f2 = static_cast<nodes::EnumField&>(*field2);
        CHECK_EQUAL("gold", f2.name().token().to_string());
        CHECK_EQUAL("u8", f2.underlying().token().to_string());
        CHECK_EQUAL(98U, boost::get<std::uint8_t>(f2.value()));
    }

    struct WhenInputIsEnumWithNumericLiteral : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "enum Metal : u8 {" "\n"
            "\t" "iron," "\n"
            "\t" "copper = 200," "\n"
            "\t" "gold," "\n"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsEnumWithNumericLiteral, verifyConsume)
    {
        auto matcher = Matcher().getChildrenOf<nodes::Enum>().bind("enum");
        CHECK(!matcher(parser.ast().root()));

        tokenize(sv);
        parse();

        CHECK(matcher(parser.ast().root()));

        const auto node = matcher.bound("enum_0");
        REQUIRE CHECK(node);

        const auto& Enum = static_cast<nodes::Enum&>(*node);
        CHECK_EQUAL("foo::Metal", Enum.name());
        CHECK_EQUAL("u8", Enum.underlying().token().to_string());

        auto fieldsMatcher = Matcher().getChildrenOf<nodes::EnumField>().bind("fields");
        CHECK(fieldsMatcher(node));

        const auto field0 = fieldsMatcher.bound("fields_0");
        REQUIRE CHECK(field0);

        const auto& f0 = static_cast<nodes::EnumField&>(*field0);
        CHECK_EQUAL("iron", f0.name().token().to_string());
        CHECK_EQUAL("u8", f0.underlying().token().to_string());
        CHECK_EQUAL(0U, boost::get<std::uint8_t>(f0.value()));

        const auto field1 = fieldsMatcher.bound("fields_1");
        REQUIRE CHECK(field1);

        const auto& f1 = static_cast<nodes::EnumField&>(*field1);
        CHECK_EQUAL("copper", f1.name().token().to_string());
        CHECK_EQUAL("u8", f1.underlying().token().to_string());
        CHECK_EQUAL(200U, boost::get<std::uint8_t>(f1.value()));

        const auto field2 = fieldsMatcher.bound("fields_2");
        REQUIRE CHECK(field2);

        const auto& f2 = static_cast<nodes::EnumField&>(*field2);
        CHECK_EQUAL("gold", f2.name().token().to_string());
        CHECK_EQUAL("u8", f2.underlying().token().to_string());
        CHECK_EQUAL(201U, boost::get<std::uint8_t>(f2.value()));
    }

    struct WhenInputIsEnumWithDifferentLiterals : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "enum Metal : u8 {" "\n"
            "\t" "iron = 0x04," "\n"
            "\t" "copper = 'a'," "\n"
            "\t" "gold = 200," "\n"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsEnumWithDifferentLiterals, verifyConsume)
    {
        auto matcher = Matcher().getChildrenOf<nodes::Enum>().bind("enum");
        CHECK(!matcher(parser.ast().root()));

        tokenize(sv);
        parse();

        CHECK(matcher(parser.ast().root()));

        const auto node = matcher.bound("enum_0");
        REQUIRE CHECK(node);

        const auto& Enum = static_cast<nodes::Enum&>(*node);
        CHECK_EQUAL("foo::Metal", Enum.name());
        CHECK_EQUAL("u8", Enum.underlying().token().to_string());

        auto fieldsMatcher = Matcher().getChildrenOf<nodes::EnumField>().bind("fields");
        CHECK(fieldsMatcher(node));

        const auto field0 = fieldsMatcher.bound("fields_0");
        REQUIRE CHECK(field0);

        const auto& f0 = static_cast<nodes::EnumField&>(*field0);
        CHECK_EQUAL("iron", f0.name().token().to_string());
        CHECK_EQUAL("u8", f0.underlying().token().to_string());
        CHECK_EQUAL(4U, boost::get<std::uint8_t>(f0.value()));

        const auto field1 = fieldsMatcher.bound("fields_1");
        REQUIRE CHECK(field1);

        const auto& f1 = static_cast<nodes::EnumField&>(*field1);
        CHECK_EQUAL("copper", f1.name().token().to_string());
        CHECK_EQUAL("u8", f1.underlying().token().to_string());
        CHECK_EQUAL(97U, boost::get<std::uint8_t>(f1.value()));

        const auto field2 = fieldsMatcher.bound("fields_2");
        REQUIRE CHECK(field2);

        const auto& f2 = static_cast<nodes::EnumField&>(*field2);
        CHECK_EQUAL("gold", f2.name().token().to_string());
        CHECK_EQUAL("u8", f2.underlying().token().to_string());
        CHECK_EQUAL(200U, boost::get<std::uint8_t>(f2.value()));
    }

    struct WhenInputIsEnumWithOutOfRangeNumericLiteral : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "enum Metal : u8 {" "\n"
            "\t" "gold = 300," "\n"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsEnumWithOutOfRangeNumericLiteral, verifyConsume)
    {
        tokenize(sv);
        CHECK_THROW(parse(), swizzle::SyntaxError);
    }

    struct WhenInputIsEnumWithOutOfRangeHexLiteral : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "enum Metal : u8 {" "\n"
            "\t" "gold = 0xabba," "\n"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsEnumWithOutOfRangeHexLiteral, verifyConsume)
    {
        tokenize(sv);
        CHECK_THROW(parse(), swizzle::SyntaxError);
    }
}
