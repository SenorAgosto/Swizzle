#include "./ut_support/UnitTestSupport.hpp"

#include <swizzle/ast/Matcher.hpp>
#include <swizzle/ast/nodes/Attribute.hpp>
#include <swizzle/ast/nodes/BaseClass.hpp>
#include <swizzle/ast/nodes/Bitfield.hpp>
#include <swizzle/ast/nodes/BitfieldField.hpp>
#include <swizzle/ast/nodes/CharLiteral.hpp>
#include <swizzle/ast/nodes/Comment.hpp>
#include <swizzle/ast/nodes/Enum.hpp>
#include <swizzle/ast/nodes/EnumField.hpp>
#include <swizzle/ast/nodes/Extern.hpp>
#include <swizzle/ast/nodes/Import.hpp>
#include <swizzle/ast/nodes/MultilineComment.hpp>
#include <swizzle/ast/nodes/Namespace.hpp>
#include <swizzle/ast/nodes/StringLiteral.hpp>
#include <swizzle/ast/nodes/Struct.hpp>
#include <swizzle/ast/nodes/StructField.hpp>
#include <swizzle/ast/nodes/TypeAlias.hpp>

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

            parser.finalize();
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
        CHECK_EQUAL(0U, f0.value());

        const auto field1 = fieldsMatcher.bound("fields_1");
        REQUIRE CHECK(field1);

        const auto& f1 = static_cast<nodes::EnumField&>(*field1);
        CHECK_EQUAL("copper", f1.name().token().to_string());
        CHECK_EQUAL("u8", f1.underlying().token().to_string());
        CHECK_EQUAL(1U, f1.value());

        const auto field2 = fieldsMatcher.bound("fields_2");
        REQUIRE CHECK(field2);

        const auto& f2 = static_cast<nodes::EnumField&>(*field2);
        CHECK_EQUAL("gold", f2.name().token().to_string());
        CHECK_EQUAL("u8", f2.underlying().token().to_string());
        CHECK_EQUAL(2U, f2.value());
    }

    struct WhenInputIsEnumWithAttribute : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "@volatile" "\n"
            "enum Metal : u8 {" "\n"
            "\t" "iron," "\n"
            "\t" "copper," "\n"
            "\t" "gold," "\n"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsEnumWithAttribute, verifyConsume)
    {
        tokenize(sv);
        parse();
    }

    struct WhenInputIsEnumWithKeyValueAttributeNumeric : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "@volatile=1" "\n"
            "enum Metal : u8 {" "\n"
            "\t" "iron," "\n"
            "\t" "copper," "\n"
            "\t" "gold," "\n"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsEnumWithKeyValueAttributeNumeric, verifyConsume)
    {
        tokenize(sv);
        parse();
    }

    struct WhenInputIsEnumWithKeyValueAttributeHex : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "@volatile=0x02" "\n"
            "enum Metal : u8 {" "\n"
            "\t" "iron," "\n"
            "\t" "copper," "\n"
            "\t" "gold," "\n"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsEnumWithKeyValueAttributeHex, verifyConsume)
    {
        tokenize(sv);
        parse();
    }


    struct WhenInputIsEnumWithKeyValueAttributeWithChar : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "@volatile='a'" "\n"
            "enum Metal : u8 {" "\n"
            "\t" "iron," "\n"
            "\t" "copper," "\n"
            "\t" "gold," "\n"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsEnumWithKeyValueAttributeWithChar, verifyConsume)
    {
        tokenize(sv);
        parse();
    }

    struct WhenInputIsEnumWithKeyValueAttributeWithStringLiteral : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "@volatile=\"boo\"" "\n"
            "enum Metal : u8 {" "\n"
            "\t" "iron," "\n"
            "\t" "copper," "\n"
            "\t" "gold," "\n"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsEnumWithKeyValueAttributeWithStringLiteral, verifyConsume)
    {
        tokenize(sv);
        parse();
    }

    struct WhenInputIsEnumWithAttributeOnEnumValue : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "enum Metal : u8 {" "\n"
            "@blah" "\n"
            "\t" "iron," "\n"
            "@blooie" "\n"
            "\t" "copper," "\n"
            "\t" "gold," "\n"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsEnumWithAttributeOnEnumValue, verifyConsume)
    {
        tokenize(sv);
        parse();
    }

    struct WhenInputIsEnumWithKeyValueAttributeOnEnumValueKeyIsNumericLiteral : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "enum Metal : u8 {" "\n"
            "@blah=1" "\n"
            "\t" "iron," "\n"
            "@blooie" "\n"
            "\t" "copper," "\n"
            "\t" "gold," "\n"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsEnumWithKeyValueAttributeOnEnumValueKeyIsNumericLiteral, verifyConsume)
    {
        tokenize(sv);
        parse();
    }

    struct WhenInputIsEnumWithKeyValueAttributeOnEnumValueKeyIsHexLiteral : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "enum Metal : u8 {" "\n"
            "@blah=0x04" "\n"
            "\t" "iron," "\n"
            "@blooie" "\n"
            "\t" "copper," "\n"
            "\t" "gold," "\n"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsEnumWithKeyValueAttributeOnEnumValueKeyIsHexLiteral, verifyConsume)
    {
        tokenize(sv);
        parse();
    }

    struct WhenInputIsEnumWithKeyValueAttributeOnEnumValueKeyIsCharLiteral : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "enum Metal : u8 {" "\n"
            "@blah='a'" "\n"
            "\t" "iron," "\n"
            "@blooie" "\n"
            "\t" "copper," "\n"
            "\t" "gold," "\n"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsEnumWithKeyValueAttributeOnEnumValueKeyIsCharLiteral, verifyConsume)
    {
        tokenize(sv);
        parse();
    }

    struct WhenInputIsEnumWithKeyValueAttributeOnEnumValueKeyIsStringLiteral : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "enum Metal : u8 {" "\n"
            "@blah=\"boo\"" "\n"
            "\t" "iron," "\n"
            "@blooie" "\n"
            "\t" "copper," "\n"
            "\t" "gold," "\n"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsEnumWithKeyValueAttributeOnEnumValueKeyIsStringLiteral, verifyConsume)
    {
        tokenize(sv);
        parse();
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
        CHECK_EQUAL(0U, f0.value());

        const auto field1 = fieldsMatcher.bound("fields_1");
        REQUIRE CHECK(field1);

        const auto& f1 = static_cast<nodes::EnumField&>(*field1);
        CHECK_EQUAL("copper", f1.name().token().to_string());
        CHECK_EQUAL("u8", f1.underlying().token().to_string());
        CHECK_EQUAL(4U, f1.value());

        const auto field2 = fieldsMatcher.bound("fields_2");
        REQUIRE CHECK(field2);

        const auto& f2 = static_cast<nodes::EnumField&>(*field2);
        CHECK_EQUAL("gold", f2.name().token().to_string());
        CHECK_EQUAL("u8", f2.underlying().token().to_string());
        CHECK_EQUAL(5U, f2.value());
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
        CHECK_EQUAL(0U, f0.value());

        const auto field1 = fieldsMatcher.bound("fields_1");
        REQUIRE CHECK(field1);

        const auto& f1 = static_cast<nodes::EnumField&>(*field1);
        CHECK_EQUAL("copper", f1.name().token().to_string());
        CHECK_EQUAL("u8", f1.underlying().token().to_string());
        CHECK_EQUAL(97U, f1.value());

        const auto field2 = fieldsMatcher.bound("fields_2");
        REQUIRE CHECK(field2);

        const auto& f2 = static_cast<nodes::EnumField&>(*field2);
        CHECK_EQUAL("gold", f2.name().token().to_string());
        CHECK_EQUAL("u8", f2.underlying().token().to_string());
        CHECK_EQUAL(98U, f2.value());
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
        CHECK_EQUAL(0U, f0.value());

        const auto field1 = fieldsMatcher.bound("fields_1");
        REQUIRE CHECK(field1);

        const auto& f1 = static_cast<nodes::EnumField&>(*field1);
        CHECK_EQUAL("copper", f1.name().token().to_string());
        CHECK_EQUAL("u8", f1.underlying().token().to_string());
        CHECK_EQUAL(200U, f1.value());

        const auto field2 = fieldsMatcher.bound("fields_2");
        REQUIRE CHECK(field2);

        const auto& f2 = static_cast<nodes::EnumField&>(*field2);
        CHECK_EQUAL("gold", f2.name().token().to_string());
        CHECK_EQUAL("u8", f2.underlying().token().to_string());
        CHECK_EQUAL(201U, f2.value());
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
        CHECK_EQUAL(4U, f0.value());

        const auto field1 = fieldsMatcher.bound("fields_1");
        REQUIRE CHECK(field1);

        const auto& f1 = static_cast<nodes::EnumField&>(*field1);
        CHECK_EQUAL("copper", f1.name().token().to_string());
        CHECK_EQUAL("u8", f1.underlying().token().to_string());
        CHECK_EQUAL(97U, f1.value());

        const auto field2 = fieldsMatcher.bound("fields_2");
        REQUIRE CHECK(field2);

        const auto& f2 = static_cast<nodes::EnumField&>(*field2);
        CHECK_EQUAL("gold", f2.name().token().to_string());
        CHECK_EQUAL("u8", f2.underlying().token().to_string());
        CHECK_EQUAL(200U, f2.value());
    }

    struct WhenInputIsEnumWithDifferentLiteralsAndFieldsBetween : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "enum Metal : u8 {" "\n"
            "\t" "// this is a comment" "\n"
            "\t" "iron = 0x04," "\n"
            "\t" "iron2," "\n"
            "\t" "copper = 'a'," "\n"
            "\t" "// this is a multi-line \\" "\n"
            "\t" "   comment" "\n"
            "\t" "copper2," "\n"
            "\t" "gold = 200," "\n"
            "\t" "gold2," "\n"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsEnumWithDifferentLiteralsAndFieldsBetween, verifyConsume)
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
        CHECK_EQUAL(4U, f0.value());

        const auto field1 = fieldsMatcher.bound("fields_1");
        REQUIRE CHECK(field1);

        const auto& f1 = static_cast<nodes::EnumField&>(*field1);
        CHECK_EQUAL("iron2", f1.name().token().to_string());
        CHECK_EQUAL("u8", f1.underlying().token().to_string());
        CHECK_EQUAL(5U, f1.value());

        const auto field2 = fieldsMatcher.bound("fields_2");
        REQUIRE CHECK(field2);

        const auto& f2 = static_cast<nodes::EnumField&>(*field2);
        CHECK_EQUAL("copper", f2.name().token().to_string());
        CHECK_EQUAL("u8", f2.underlying().token().to_string());
        CHECK_EQUAL(97U, f2.value());

        const auto field3 = fieldsMatcher.bound("fields_3");
        REQUIRE CHECK(field3);

        const auto& f3 = static_cast<nodes::EnumField&>(*field3);
        CHECK_EQUAL("copper2", f3.name().token().to_string());
        CHECK_EQUAL("u8", f3.underlying().token().to_string());
        CHECK_EQUAL(98U, f3.value());

        const auto field4 = fieldsMatcher.bound("fields_4");
        REQUIRE CHECK(field4);

        const auto& f4 = static_cast<nodes::EnumField&>(*field4);
        CHECK_EQUAL("gold", f4.name().token().to_string());
        CHECK_EQUAL("u8", f4.underlying().token().to_string());
        CHECK_EQUAL(200U, f4.value());

        const auto field5 = fieldsMatcher.bound("fields_5");
        REQUIRE CHECK(field5);

        const auto& f5 = static_cast<nodes::EnumField&>(*field5);
        CHECK_EQUAL("gold2", f5.name().token().to_string());
        CHECK_EQUAL("u8", f5.underlying().token().to_string());
        CHECK_EQUAL(201U, f5.value());
    }

    struct WhenInputIsEnumWhichReusesValues : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "enum Metal : u8 {" "\n"
            "\t" "gold = 300," "\n"
            "\t" "silver = 300," "\n"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsEnumWhichReusesValues, verifyConsume)
    {
        tokenize(sv);
        CHECK_THROW(parse(), swizzle::SyntaxError);
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

    struct WhenInputIsBitfield : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "bitfield Field1 : u8 {" "\n"
            "\t" "f1 : 0," "\n"
            "\t" "// comment" "\n"
            "\t" "f2 : 1..2," "\n"
            "\t" "// multi-line \\" "\n"
            "\t" "   comment" "\n"
            "\t" "f3 : 3..4," "\n"
            "\t" "f4 : 5," "\n"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsBitfield, verifyConsume)
    {
        auto matcher = Matcher().getChildrenOf<nodes::Bitfield>().bind("bitfield");
        CHECK(!matcher(parser.ast().root()));

        tokenize(sv);
        parse();

        CHECK(matcher(parser.ast().root()));

        const auto bitfieldNode = matcher.bound("bitfield_0");
        REQUIRE CHECK(bitfieldNode);

        const auto& bitfield = static_cast<nodes::Bitfield&>(*bitfieldNode);
        CHECK_EQUAL("foo::Field1", bitfield.name());
        CHECK_EQUAL("u8", bitfield.underlying().token().value());

        auto fieldMatcher = Matcher().getChildrenOf<nodes::BitfieldField>().bind("fields");
        REQUIRE CHECK(fieldMatcher(bitfieldNode));

        const auto field0 = fieldMatcher.bound("fields_0");
        REQUIRE CHECK(field0);

        const auto& f0 = static_cast<nodes::BitfieldField&>(*field0);
        CHECK_EQUAL("f1", f0.name().token().value());
        CHECK_EQUAL("u8", f0.underlying().token().value());
        CHECK_EQUAL(0, f0.beginBit());
        CHECK_EQUAL(0, f0.endBit());

        const auto field1 = fieldMatcher.bound("fields_1");
        REQUIRE CHECK(field1);

        const auto& f1 = static_cast<nodes::BitfieldField&>(*field1);
        CHECK_EQUAL("f2", f1.name().token().value());
        CHECK_EQUAL("u8", f1.underlying().token().value());
        CHECK_EQUAL(1, f1.beginBit());
        CHECK_EQUAL(2, f1.endBit());

        const auto field2 = fieldMatcher.bound("fields_2");
        REQUIRE CHECK(field2);

        const auto& f2 = static_cast<nodes::BitfieldField&>(*field2);
        CHECK_EQUAL("f3", f2.name().token().value());
        CHECK_EQUAL("u8", f2.underlying().token().value());
        CHECK_EQUAL(3, f2.beginBit());
        CHECK_EQUAL(4, f2.endBit());

        const auto field3 = fieldMatcher.bound("fields_3");
        REQUIRE CHECK(field3);

        const auto& f3 = static_cast<nodes::BitfieldField&>(*field3);
        CHECK_EQUAL("f4", f3.name().token().value());
        CHECK_EQUAL("u8", f3.underlying().token().value());
        CHECK_EQUAL(5, f3.beginBit());
        CHECK_EQUAL(5, f3.endBit());
    }

    struct WhenInputIsBitfieldWithAttribute : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "@vogue" "\n"
            "bitfield Field1 : u8 {" "\n"
            "\t" "f1 : 0," "\n"
            "\t" "f2 : 1..2," "\n"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsBitfieldWithAttribute, verifyConsume)
    {
        tokenize(sv);
        parse();
    }

    struct WhenInputIsBitfieldWithKeyValueAttributeNumericLiteral : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "@vogue=1" "\n"
            "bitfield Field1 : u8 {" "\n"
            "\t" "f1 : 0," "\n"
            "\t" "f2 : 1..2," "\n"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsBitfieldWithKeyValueAttributeNumericLiteral, verifyConsume)
    {
        tokenize(sv);
        parse();
    }

    struct WhenInputIsBitfieldWithKeyValueAttributeHexLiteral : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "@vogue=0x02" "\n"
            "bitfield Field1 : u8 {" "\n"
            "\t" "f1 : 0," "\n"
            "\t" "f2 : 1..2," "\n"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsBitfieldWithKeyValueAttributeHexLiteral, verifyConsume)
    {
        tokenize(sv);
        parse();
    }

    struct WhenInputIsBitfieldWithKeyValueAttributeCharLiteral : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "@vogue='a'" "\n"
            "bitfield Field1 : u8 {" "\n"
            "\t" "f1 : 0," "\n"
            "\t" "f2 : 1..2," "\n"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsBitfieldWithKeyValueAttributeCharLiteral, verifyConsume)
    {
        tokenize(sv);
        parse();
    }

    struct WhenInputIsBitfieldWithKeyValueAttributeStringLiteral : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "@vogue=\"boo\"" "\n"
            "bitfield Field1 : u8 {" "\n"
            "\t" "f1 : 0," "\n"
            "\t" "f2 : 1..2," "\n"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsBitfieldWithKeyValueAttributeStringLiteral, verifyConsume)
    {
        tokenize(sv);
        parse();
    }

    struct WhenInputIsBitfieldWithAttributeOnBitfield : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "bitfield Field1 : u8 {" "\n"
            "@b1"
            "\t" "f1 : 0," "\n"
            "\t" "f2 : 1..2," "\n"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsBitfieldWithAttributeOnBitfield, verifyConsume)
    {
        tokenize(sv);
        parse();
    }

    struct WhenInputIsBitfieldWithAttributeOnBitfieldNumericLiteral : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "bitfield Field1 : u8 {" "\n"
            "@b1=2"
            "\t" "f1 : 0," "\n"
            "\t" "f2 : 1..2," "\n"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsBitfieldWithAttributeOnBitfieldNumericLiteral, verifyConsume)
    {
        tokenize(sv);
        parse();
    }

    struct WhenInputIsBitfieldWithAttributeOnBitfieldHexLiteral : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "bitfield Field1 : u8 {" "\n"
            "@b1=0x08"
            "\t" "f1 : 0," "\n"
            "\t" "f2 : 1..2," "\n"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsBitfieldWithAttributeOnBitfieldHexLiteral, verifyConsume)
    {
        tokenize(sv);
        parse();
    }

    struct WhenInputIsBitfieldWithAttributeOnBitfieldCharLiteral : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "bitfield Field1 : u8 {" "\n"
            "@b1='c'"
            "\t" "f1 : 0," "\n"
            "\t" "f2 : 1..2," "\n"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsBitfieldWithAttributeOnBitfieldCharLiteral, verifyConsume)
    {
        tokenize(sv);
        parse();
    }

    struct WhenInputIsBitfieldWithAttributeOnBitfieldStringLiteral : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "bitfield Field1 : u8 {" "\n"
            "@b1=\"boo\""
            "\t" "f1 : 0," "\n"
            "\t" "f2 : 1..2," "\n"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsBitfieldWithAttributeOnBitfieldStringLiteral, verifyConsume)
    {
        tokenize(sv);
        parse();
    }

    struct WhenInputIsBitfieldReassigningBits_1 : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "bitfield Field1 : u8 {" "\n"
            "\t" "f1 : 0," "\n"
            "\t" "f2 : 0..2," "\n"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsBitfieldReassigningBits_1, verifyConsume)
    {
        tokenize(sv);
        CHECK_THROW(parse(), swizzle::SyntaxError);
    }

    struct WhenInputIsBitfieldReassigningBits_2 : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "bitfield Field1 : u8 {" "\n"
            "\t" "f1 : 0," "\n"
            "\t" "f2 : 1..2," "\n"
            "\t" "f3 : 0," "\n"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsBitfieldReassigningBits_2, verifyConsume)
    {
        tokenize(sv);
        CHECK_THROW(parse(), swizzle::SyntaxError);
    }

    struct WhenInputIsBitfieldNotStartingAtLowBit : ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "bitfield Field1 : u8 {" "\n"
            "\t" "f1 : 7," "\n"
            "\t" "f2 : 6..4," "\n"
            "\t" "f3 : 3," "\n"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsBitfieldNotStartingAtLowBit, verifyConsume)
    {
        tokenize(sv);
        CHECK_THROW(parse(), swizzle::SyntaxError);
    }

    struct WhenInputIsBitfieldAndEndBitIsTooLow : ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "bitfield Field1 : u8 {" "\n"
            "\t" "f1 : 0," "\n"
            "\t" "f2 : 3..2," "\n"
            "\t" "f3 : 5," "\n"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsBitfieldAndEndBitIsTooLow, verifyConsume)
    {
        tokenize(sv);
        CHECK_THROW(parse(), swizzle::SyntaxError);
    }

    struct WhenInputIsBitFieldAndFieldRangeExceedsUnderlyingType : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "bitfield Field1 : u8 {" "\n"
            "\t" "f1 : 5," "\n"
            "\t" "f2 : 6..7," "\n"
            "\t" "f3 : 8," "\n"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsBitFieldAndFieldRangeExceedsUnderlyingType, verifyConsume)
    {
        tokenize(sv);
        CHECK_THROW(parse(), swizzle::SyntaxError);
    }

    struct WhenInputIsStruct : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "struct Myfield {" "\n"
            "\t" "// field 1 is blah blah blah" "\n"
            "\t" "u8 field1;" "\n"
            "\t" "// field 2 will be blah \\" "\n"
            "\t" "   blah blah" "\n"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsStruct, verifyConsume)
    {
        auto matcher = Matcher().getChildrenOf<nodes::Struct>().bind("struct");
        CHECK(!matcher(parser.ast().root()));

        tokenize(sv);
        parse();

        CHECK(matcher(parser.ast().root()));

        const auto node = matcher.bound("struct_0");
        REQUIRE CHECK(node);

        const auto& s = static_cast<nodes::Struct&>(*node);
        CHECK_EQUAL("foo::Myfield", s.name());

        auto fieldsMatcher = Matcher().getChildrenOf<nodes::StructField>().bind("fields");
        REQUIRE CHECK(fieldsMatcher(node));

        const auto f0_node = fieldsMatcher.bound("fields_0");
        REQUIRE CHECK(f0_node);

        const auto& f0 = static_cast<nodes::StructField&>(*f0_node);
        CHECK_EQUAL("field1", f0.name().token().value());
        CHECK_EQUAL("u8", f0.type());
        CHECK(!f0.isConst());
        CHECK(!f0.isArray());
        CHECK(!f0.isVector());
    }

    struct WhenInputIsNestedStruct : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n\n"

            "// struct 1" "\n"
            "struct Struct1 {" "\n"
            "\t" "u8 field1;" "\n"
            "}" "\n\n"

            "// struct 2" "\n"
            "struct Struct2 {" "\n"
            "\t" "f32 slope;" "\n"
            "\t" "Struct1 s1;" "\n"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsNestedStruct, verifyConsume)
    {
        auto matcher = Matcher().getChildrenOf<nodes::Struct>().bind("struct");
        CHECK(!matcher(parser.ast().root()));

        tokenize(sv);
        parse();

        CHECK(matcher(parser.ast().root()));

        const auto node = matcher.bound("struct_1");
        REQUIRE CHECK(node);

        auto fieldMatcher = Matcher().getChildrenOf<nodes::StructField>().bind("fields");
        CHECK(fieldMatcher(node));

        const auto field1_node = fieldMatcher.bound("fields_1");
        REQUIRE CHECK(field1_node);

        const auto& s1 = static_cast<nodes::StructField&>(*field1_node);
        CHECK_EQUAL("foo::Struct1", s1.type());
        CHECK_EQUAL("s1", s1.name().token().value());
    }

    struct WhenInputIsNestedStruct_2 : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n\n"

            "// struct 1" "\n"
            "struct Struct1 {" "\n"
            "\t" "u8 field1;" "\n"
            "}" "\n\n"

            "// struct 2" "\n"
            "struct Struct2 {" "\n"
            "\t" "f32 slope;" "\n"
            "\t" "foo::Struct1 s1;" "\n"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsNestedStruct_2, verifyConsume)
    {
        tokenize(sv);
        parse();
    }
    
    struct WhenInputIsStructWithBaseClass : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "struct Struct1 { u8 field1; }" "\n"
            "struct Struct2 : Struct1 { u8 field1; }"
        );
    };

    TEST_FIXTURE(WhenInputIsStructWithBaseClass, verifyConsume)
    {
        auto matcher = Matcher().getChildrenOf<nodes::Struct>().bind("struct");
        CHECK(!matcher(parser.ast().root()));

        tokenize(sv);
        parse();
        
        REQUIRE CHECK(matcher(parser.ast().root()));
        const auto struct_node = matcher.bound("struct_1");
        
        auto baseClassMatcher = Matcher().getChildrenOf<nodes::BaseClass>().bind("base");
        CHECK(baseClassMatcher(struct_node));
        
        const auto base_node = baseClassMatcher.bound("base_0");
        const auto& base = static_cast<nodes::BaseClass&>(*base_node);
        
        CHECK_EQUAL("foo::Struct1", base.name());
    }
    
    struct WhenInputIsStructWithChainedInheritance : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "struct Struct1 { u8 field1; }" "\n"
            "struct Struct2 : Struct1 { u8 field1; }"
            "struct Struct3 : Struct2 { u8 field1; }"
        );
    };

    TEST_FIXTURE(WhenInputIsStructWithChainedInheritance, verifyConsume)
    {
        auto matcher = Matcher().getChildrenOf<nodes::Struct>().bind("struct");
        CHECK(!matcher(parser.ast().root()));
        
        tokenize(sv);
        parse();
        
        REQUIRE CHECK(matcher(parser.ast().root()));
        const auto struct_node = matcher.bound("struct_2");

        auto baseClassMatcher = Matcher().getChildrenOf<nodes::BaseClass>().bind("base");
        CHECK(baseClassMatcher(struct_node));
        
        const auto base_node = baseClassMatcher.bound("base_0");
        const auto& base = static_cast<nodes::BaseClass&>(*base_node);
        
        CHECK_EQUAL("foo::Struct2", base.name());   // foo::Struct2 is the base of foo::Struct3
    }

    struct WhenInputIsStructWithMultipleInheritance : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "struct Struct1 { u8 field1; }" "\n"
            "struct Struct2 { u8 field1; }"
            "struct Struct3 : Struct1, Struct2 { u8 field1; }"
        );
    };

    TEST_FIXTURE(WhenInputIsStructWithMultipleInheritance, verifyConsume)
    {
        auto matcher = Matcher().getChildrenOf<nodes::Struct>().bind("struct");
        CHECK(!matcher(parser.ast().root()));
        
        tokenize(sv);
        parse();
        
        REQUIRE CHECK(matcher(parser.ast().root()));
        const auto struct_node = matcher.bound("struct_2");

        auto baseClassMatcher = Matcher().getChildrenOf<nodes::BaseClass>().bind("base");
        CHECK(baseClassMatcher(struct_node));
        
        const auto bases = baseClassMatcher.all_bound("base");
        REQUIRE CHECK_EQUAL(2U, bases.size());
        /* TODO: Matcher::all_bound() needs to preserve the order of the nodes, which means Matcher::variables_
           data structure needs to be changed to preserve the order of nodes matched.
        CHECK_EQUAL("foo::Struct1", static_cast<nodes::BaseClass&>(*bases[0]).name());
        CHECK_EQUAL("foo::Struct2", static_cast<nodes::BaseClass&>(*bases[1]).name());
        */
    }

    struct WhenInputIsStructWithAttributeOnBaseClass : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "struct Struct1 { u8 field1; }" "\n"
            "struct Struct2 { u8 field1; }"
            "struct Struct3 : @base_1 Struct1, @base_2 Struct2 { u8 field1; }"
        );
    };
    
    TEST_FIXTURE(WhenInputIsStructWithAttributeOnBaseClass, verifyConsume)
    {
        tokenize(sv);
        parse();
    }

    struct WhenInputIsStructWithCharAttributeOnBaseClass : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "struct Struct1 { u8 field1; }" "\n"
            "struct Struct2 { u8 field1; }"
            "struct Struct3 : @base='1' Struct1, @base='2' Struct2 { u8 field1; }"
        );
    };
    
    TEST_FIXTURE(WhenInputIsStructWithCharAttributeOnBaseClass, verifyConsume)
    {
        tokenize(sv);
        parse();
    }
    
    struct WhenInputIsStructWithNumAttributeOnBaseClass : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "struct Struct1 { u8 field1; }" "\n"
            "struct Struct2 { u8 field1; }"
            "struct Struct3 : @base=1 Struct1, @base=2 Struct2 { u8 field1; }"
        );
    };
    
    TEST_FIXTURE(WhenInputIsStructWithNumAttributeOnBaseClass, verifyConsume)
    {
        tokenize(sv);
        parse();
    }

    struct WhenInputIsStructWithHexAttributeOnBaseClass : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "struct Struct1 { u8 field1; }" "\n"
            "struct Struct2 { u8 field1; }"
            "struct Struct3 : @base=0x01 Struct1, @base=0x02 Struct2 { u8 field1; }"
        );
    };
    
    TEST_FIXTURE(WhenInputIsStructWithHexAttributeOnBaseClass, verifyConsume)
    {
        tokenize(sv);
        parse();
    }

    struct WhenInputIsStructWithStringAttributeOnBaseClass : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "struct Struct1 { u8 field1; }" "\n"
            "struct Struct2 { u8 field1; }"
            "struct Struct3 : @base=\"b1\" Struct1, @base=\"b2\" Struct2 { u8 field1; }"
        );
    };
    
    TEST_FIXTURE(WhenInputIsStructWithStringAttributeOnBaseClass, verifyConsume)
    {
        tokenize(sv);
        parse();
    }

    struct WhenInputIsStructWithBlockAttributeOnBaseClass : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "struct Struct1 { u8 field1; }" "\n"
            "struct Struct2 { u8 field1; }"
            "struct Struct3 : @base{whatever} Struct1, @base{whatever2} Struct2 { u8 field1; }"
        );
    };

    TEST_FIXTURE(WhenInputIsStructWithBlockAttributeOnBaseClass, verifyConsume)
    {
        tokenize(sv);
        parse();
    }

    struct WhenInputIsStructWithCommentBeforeBaseClass : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "struct Struct1 { u8 field1; }" "\n"
            "struct Struct2 { u8 field1; }"
            "struct Struct3 : // comment \n"
                "Struct1, Struct2 { u8 field1; }"
        );
    };

    TEST_FIXTURE(WhenInputIsStructWithCommentBeforeBaseClass, verifyConsume)
    {
        tokenize(sv);
        parse();
    }

    struct WhenInputIsStructWithMultiLineCommentBeforeBaseClass : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "struct Struct1 { u8 field1; }" "\n"
            "struct Struct2 { u8 field1; }"
            "struct Struct3 : // comment \\\n"
                " comment continues \n"
                "Struct1, Struct2 { u8 field1; }"
        );
    };

    TEST_FIXTURE(WhenInputIsStructWithMultiLineCommentBeforeBaseClass, verifyConsume)
    {
        tokenize(sv);
        parse();
    }

    struct WhenInputIsStructWithAttribute : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "@amigo" "\n"
            "struct Struct1 {" "\n"
                "u8 field;" "\n"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsStructWithAttribute, verifyConsume)
    {
        auto matcher = Matcher().getChildrenOf<nodes::Struct>().bind("struct");
        CHECK(!matcher(parser.ast().root()));

        tokenize(sv);
        parse();

        CHECK(matcher(parser.ast().root()));

        const auto node = matcher.bound("struct_0");
        REQUIRE CHECK(node);

        auto attributeMatcher = Matcher().getChildrenOf<nodes::Attribute>().bind("attribute");
        CHECK(attributeMatcher(node));

        const auto attributeNode = attributeMatcher.bound("attribute_0");
        REQUIRE CHECK(attributeNode);

        const auto& attribute = static_cast<nodes::Attribute&>(*attributeNode);
        CHECK_EQUAL("@amigo", attribute.info().token().value());
    }

    struct WhenInputIsStructWithFieldLabels : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "struct Struct1 {" "\n"
            "\t" "10: u8 msgType;" "\n"
            "\t" "20: u8 status;" "\n"
            "}"
        );
    };
    
    TEST_FIXTURE(WhenInputIsStructWithFieldLabels, verifyConsume)
    {
        tokenize(sv);
        parse();
    }

    struct WhenInputIsStructWithDuplicateFieldLabels : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "struct Struct1 {" "\n"
            "\t" "10: u8 msgType;" "\n"
            "\t" "10: u8 status;" "\n"
            "}"
        );
    };
    
    TEST_FIXTURE(WhenInputIsStructWithDuplicateFieldLabels, verifyConsume)
    {
        tokenize(sv);
        CHECK_THROW(parse(), swizzle::SyntaxError);
    }

    struct WhenInputIsStructWithFieldLabelsOnAConstField : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "struct Struct1 {" "\n"
            "\t" "10: const u8 msgType = 'A';" "\n"
            "\t" "20: u8 status;" "\n"
            "}"
        );
    };
    
    TEST_FIXTURE(WhenInputIsStructWithFieldLabelsOnAConstField, verifyConsume)
    {
        tokenize(sv);
        parse();
    }

    struct WhenInputIsStructWithFieldLabelsAndAttributes : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "struct Struct1 {" "\n"
            "\t" "10: u8 msgType;" "\n"
            "\t" "// field comment" "\n"
            "\t" "20: @optional u8 status;  // comment" "\n"
            "}"
        );
    };
    
    TEST_FIXTURE(WhenInputIsStructWithFieldLabelsAndAttributes, verifyConsume)
    {
        tokenize(sv);
        parse();
        
        // TODO: validate AST
    }

    struct WhenInputIsStructWithMemberFieldMissingType : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "struct Struct1 {" "\n"
            "\t"    "10: heartbeat;"
            "}"
        );
    };
    
    TEST_FIXTURE(WhenInputIsStructWithMemberFieldMissingType, verifyConsume)
    {
        tokenize(sv);
        
        try
        {
            parse();
        }
        catch(const swizzle::SyntaxError& e)
        {
            CHECK_EQUAL(3U, e.token().fileInfo().start().line());
            CHECK_EQUAL(6U, e.token().fileInfo().start().column());
        }
    }

    struct WhenInputIsStructWithEnumFieldGettingDefaultValue : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "enum E : u16 {" "\n"
            "\t" "e1 = 1," "\n"
            "}" "\n"
            "struct S {" "\n"
            "\t" "E e = E::e1;"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsStructWithEnumFieldGettingDefaultValue, verifyConsume)
    {
        tokenize(sv);
        parse();
    }

    struct WhenInputIsMismatchedEnumAssignment : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "enum E : u16 {" "\n"
            "\t" "e1 = 1," "\n"
            "}" "\n"
            "enum E2 : u8 {" "\n"
            "\t" "e2 = 2," "\n"
            "}"
            "struct S {" "\n"
            "\t" "E2 e = E::e1;"
            "}"
        );
    };
    
    TEST_FIXTURE(WhenInputIsMismatchedEnumAssignment, verifyConsume)
    {
        tokenize(sv);
        CHECK_THROW(parse(), swizzle::SyntaxError);
    }

    struct WhenInputIsStructWithEnumFieldAssignedToInteger : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "enum E : u16 {" "\n"
            "\t" "e1 = 1," "\n"
            "}" "\n"
            "struct S {" "\n"
            "\t" "u32 e = E::e1;"
            "}"
        );
    };
    
    TEST_FIXTURE(WhenInputIsStructWithEnumFieldAssignedToInteger, verifyConsume)
    {
        tokenize(sv);
        CHECK_THROW(parse(), swizzle::SyntaxError);
    }

    struct WhenInputIsStructWithArray : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "struct Struct1 {"
                "u8[10] name;"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsStructWithArray, verifyConsume)
    {
        tokenize(sv);
        parse();
    }

    struct WhenInputIsStructWithVector : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "struct Struct1 {"
                "u8 size;" "\n"
                "u8[size] name;" "\n"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsStructWithVector, verifyConsume)
    {
        auto matcher = Matcher().getChildrenOf<nodes::Struct>().bind("struct");
        CHECK(!matcher(parser.ast().root()));
        
        tokenize(sv);
        parse();

        CHECK(matcher(parser.ast().root()));
        
        const auto node = matcher.bound("struct_0");
        REQUIRE CHECK(node);
        
        const auto& s = static_cast<nodes::Struct&>(*node);
        CHECK_EQUAL("foo::Struct1", s.name());

        auto fieldsMatcher = Matcher().getChildrenOf<nodes::StructField>().bind("fields");
        REQUIRE CHECK(fieldsMatcher(node));
        
        const auto f0_node = fieldsMatcher.bound("fields_0");
        REQUIRE CHECK(f0_node);
        
        const auto& f0 = static_cast<nodes::StructField&>(*f0_node);
        CHECK_EQUAL("size", f0.name().token().value());
        CHECK_EQUAL("u8", f0.type());
        CHECK(!f0.isConst());
        CHECK(!f0.isArray());
        CHECK(!f0.isVector());
        
        const auto f1_node = fieldsMatcher.bound("fields_1");
        REQUIRE CHECK(f1_node);
        
        const auto& f1 = static_cast<nodes::StructField&>(*f1_node);
        CHECK_EQUAL("name", f1.name().token().value());
        CHECK_EQUAL("u8", f1.type());
        CHECK(!f1.isConst());
        CHECK(!f1.isArray());
        CHECK(f1.isVector());
    }

    struct WhenInputIsStructWithVectorAndNestedSizeMember : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "struct Struct1 {"
                "u8 size;" "\n"
            "}"
            "struct Struct2 {"
                "Struct1 s1;"
                "u16[s1.size] name;"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsStructWithVectorAndNestedSizeMember, verifyConsume)
    {
        tokenize(sv);
        parse();
    }

    struct WhenInputIsStructWithDoubleNestedVectorSizeMember : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "struct Struct1 {"
                "u8 size;" "\n"
            "}"
            "struct Struct2 {"
                "Struct1 s1;"
            "}"
            "struct Struct3 {"
                "Struct2 s2;"
                "f32[s2.s1.size] buffer;"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsStructWithDoubleNestedVectorSizeMember, verifyConsume)
    {
        auto matcher = Matcher().getChildrenOf<nodes::Struct>().bind("struct");
        CHECK(!matcher(parser.ast().root()));
        
        tokenize(sv);
        parse();
        
        CHECK(matcher(parser.ast().root()));
        
        // we're interested in the 3rd struct defined
        const auto node = matcher.bound("struct_2");
        REQUIRE CHECK(node);
        
        const auto& s = static_cast<nodes::Struct&>(*node);
        CHECK_EQUAL("foo::Struct3", s.name());
        
        auto fieldsMatcher = Matcher().getChildrenOf<nodes::StructField>().bind("fields");
        REQUIRE CHECK(fieldsMatcher(node));

        const auto f1_node = fieldsMatcher.bound("fields_1");
        REQUIRE CHECK(f1_node);
        
        const auto& f1 = static_cast<nodes::StructField&>(*f1_node);
        CHECK_EQUAL("buffer", f1.name().token().value());
        CHECK_EQUAL("f32", f1.type());
        CHECK(!f1.isConst());
        CHECK(!f1.isArray());
        CHECK(f1.isVector());
    }

    struct WhenInputIsStructWithFieldAttribute : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "struct Struct1 {" "\n"
                "@align=\"left\" @padding=' '" "\n"
                "u8[20] name;" "\n"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsStructWithFieldAttribute, verifyConsume)
    {
        tokenize(sv);
        parse();

        auto structMatcher = Matcher().getChildrenOf<nodes::Struct>().bind("struct");
        auto structFieldMatcher = Matcher().getChildrenOf<nodes::StructField>().bind("field");
        auto attributeMatcher = Matcher().getChildrenOf<nodes::Attribute>().bind("attribute");

        REQUIRE CHECK(structMatcher(parser.ast().root()));
        const auto structNode = structMatcher.bound("struct_0");
        REQUIRE CHECK(structNode);

        REQUIRE CHECK(structFieldMatcher(structNode));
        const auto structFieldNode = structFieldMatcher.bound("field_0");
        REQUIRE CHECK(structFieldNode);

        REQUIRE CHECK(attributeMatcher(structFieldNode));

        auto attribute0Node = attributeMatcher.bound("attribute_0");
        REQUIRE CHECK(attribute0Node);

        const auto& attribute0 = static_cast<nodes::Attribute&>(*attribute0Node);
        CHECK_EQUAL("@padding", attribute0.info().token().value());

        REQUIRE CHECK_EQUAL(1U, attribute0Node->children().size());
        const auto& value0 = dynamic_cast<nodes::CharLiteral&>(*attribute0Node->children()[0]);
        CHECK_EQUAL("' '", value0.info().token().value());

        auto attribute1Node = attributeMatcher.bound("attribute_1");
        REQUIRE CHECK(attribute1Node);

        const auto& attribute1 = static_cast<nodes::Attribute&>(*attribute1Node);
        CHECK_EQUAL("@align", attribute1.info().token().value());

        REQUIRE CHECK_EQUAL(1U, attribute1Node->children().size());
        const auto& value1 = dynamic_cast<nodes::StringLiteral&>(*attribute1Node->children()[0]);
        CHECK_EQUAL("\"left\"", value1.info().token().value());
    }

    struct WhenInputIsABitfieldUsedInAStruct : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "bitfield Bitfield1 : u8 {"
                "field1 : 0,"
                "field2 : 1..2,"
                "field3 : 3,"
                "field4 : 4,"
                "field5 : 5,"
                "field6 : 6,"
                "field7 : 7,"
            "}"
            "struct Struct1 {"
                "Bitfield1 b1;"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsABitfieldUsedInAStruct, verifyConsume)
    {
        tokenize(sv);
        parse();
    }

    struct WhenInputIsEnumUsedInAStruct : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "enum Metal : u8 {" "\n"
            "\t" "iron," "\n"
            "\t" "copper," "\n"
            "\t" "gold," "\n"
            "}"
            "struct Struct1 {"
                "Metal metal;"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsEnumUsedInAStruct, verifyConsume)
    {
        tokenize(sv);
        parse();
    }

    struct WhenInputIsStructWithConstMemberAssignedNumericLiteral : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "struct Struct1 {"
                "const u8 value = 10;"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsStructWithConstMemberAssignedNumericLiteral, verifyConsume)
    {
        tokenize(sv);
        parse();

        // TODO: validate AST
    }

    struct WhenInputIsStructWithConstMemberAssignedNumericLiteralWhichOverflowsUnderlying : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "struct Struct1 {"
                "const u8 value = 300;"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsStructWithConstMemberAssignedNumericLiteralWhichOverflowsUnderlying, verifyConsume)
    {
        tokenize(sv);
        CHECK_THROW(parse(), std::runtime_error);
    }

    struct WhenInputIsStructWithConstMemberAssignedHexLiteral : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "struct Struct1 {"
                "const u8 value = 0x0a;"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsStructWithConstMemberAssignedHexLiteral, verifyConsume)
    {
        tokenize(sv);
        parse();
    }

    struct WhenInputIsStructWithConstMemberAssignedHexLiteralWhichOverflowsUnderyling : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "struct Struct1 {"
                "const u8 value = 0x0afa;"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsStructWithConstMemberAssignedHexLiteralWhichOverflowsUnderyling, verifyConsume)
    {
        tokenize(sv);
        CHECK_THROW(parse(), std::runtime_error);
    }

    struct WhenInputIsStructWithConstMemberAssignedCharLiteral : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "struct Struct1 {"
                "const u8 value = 'c';"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsStructWithConstMemberAssignedCharLiteral, verifyConsume)
    {
        tokenize(sv);
        parse();
    }

    struct WhenInputIsStructWithConstMemberAssignedStringLiteral : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "struct Struct1 {"
                "const u8[5] value = \"hello\";"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsStructWithConstMemberAssignedStringLiteral, verifyConsume)
    {
        tokenize(sv);
        parse();

        // TODO: validate AST
    }

    struct WhenInputIsStructWithConstMemberAssignedStringLiteralWhichWouldBeTruncated : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "struct Struct1 {"
                "const u8[4] value = \"hello\";"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsStructWithConstMemberAssignedStringLiteralWhichWouldBeTruncated, verifyConsume)
    {
        tokenize(sv);
        CHECK_THROW(parse(), swizzle::SyntaxError);
    }

    struct WhenInputIsStructWithAMemberAssignedNumericLiteral : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "struct Struct1 {"
                "u8 value = 10;"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsStructWithAMemberAssignedNumericLiteral, verifyConsume)
    {
        tokenize(sv);
        parse();
    }

    struct WhenInputIsStructWithAMemberAssignedHexLiteral : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "struct Struct1 {"
                "u8 value = 0x0f;"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsStructWithAMemberAssignedHexLiteral, verifyConsume)
    {
        tokenize(sv);
        parse();
    }

    struct WhenInputIsStructWithAMemberAssignedCharLiteral : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "struct Struct1 {"
                "u8 value = 'c';"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsStructWithAMemberAssignedCharLiteral, verifyConsume)
    {
        tokenize(sv);
        parse();
    }

    struct WhenInputIsStructWithAMemberAssignedStringLiteral : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "struct Struct1 {"
                "u8[5] value = \"hello\";"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsStructWithAMemberAssignedStringLiteral, verifyConsume)
    {
        tokenize(sv);
        parse();

        // TODO: validate AST
    }

    struct WhenInputIsStructWithVariableBlock : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "struct Struct1 {"
                "u8 field1;"
            "}"
            "struct Struct2 {"
                "u16 field1;"
            "}"
            "struct Struct3 {"
                "u32 field1;"
            "}"
            "struct Struct4 {"
                "u8 blockType;"
                "variable_block : blockType {"
                    "case 'a': Struct1,"
                    "case 'b': Struct2,"
                    "case 'c': Struct3,"
                "}"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsStructWithVariableBlock, verifyConsume)
    {
        tokenize(sv);
        parse();

        // TODO: validate the AST
    }

    struct WhenInputIsStructWithVariableBlockAndNestedField : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;"
            "struct Struct1 {"
                "u8 blockType;"
            "}"
            "struct Struct2 {"
                "Struct1 s1;"
            "}"
            "struct Struct3 {"
                "u8 foo;"
                "u8 bar;"
            "}"
            "struct Struct4 {"
                "u16 foo;"
                "u16 bar;"
            "}"
            "struct Struct5 {"
                "Struct2 s2;"
                "variable_block : s2.s1.blockType {"
                    "case 'a': Struct3,"
                    "case 'b': Struct4,"
                "}"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsStructWithVariableBlockAndNestedField, verifyConsume)
    {
        tokenize(sv);
        parse();

        // TODO: validate the AST
    }

    struct WhenInputIsStructCaseStatementsAreNumericLiteral : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "struct Struct1 {"
                "u8 field1;"
            "}"
            "struct Struct2 {"
                "u16 field1;"
            "}"
            "struct Struct3 {"
                "u32 field1;"
            "}"
            "struct Struct4 {"
                "u8 blockType;"
                "variable_block : blockType {"
                    "case 1: Struct1,"
                    "case 2: Struct2,"
                    "case 3: Struct3,"
                "}"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsStructCaseStatementsAreNumericLiteral, verifyConsume)
    {
        tokenize(sv);
        parse();

        // TODO: validate the AST
    }

    struct WhenInputIsStructCaseStatementsAreHexLiteral : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "struct Struct1 {"
                "u8 field1;"
            "}"
            "struct Struct2 {"
                "u16 field1;"
            "}"
            "struct Struct3 {"
                "u32 field1;"
            "}"
            "struct Struct4 {"
                "u8 blockType;"
                "variable_block : blockType {"
                    "case 0x01: Struct1,"
                    "case 0x02: Struct2,"
                    "case 0x03: Struct3,"
                "}"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsStructCaseStatementsAreHexLiteral, verifyConsume)
    {
        tokenize(sv);
        parse();

        // TODO: validate the AST
    }

    struct WhenInputIsStructCaseStatementsAreHexLiteralThatOverflowTheSwitchField : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "struct Struct1 {"
                "u8 field1;"
            "}"
            "struct Struct2 {"
                "u16 field1;"
            "}"
            "struct Struct3 {"
                "u32 field1;"
            "}"
            "struct Struct4 {"
                "u8 blockType;"
                "variable_block : blockType {"
                    "case 0x01: Struct1,"
                    "case 0x02: Struct2,"
                    "case 0xffaa: Struct3,"
                "}"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsStructCaseStatementsAreHexLiteralThatOverflowTheSwitchField, verifyConsume)
    {
        tokenize(sv);
        CHECK_THROW(parse(), std::runtime_error);
    }

    struct WhenInputIsStructCaseStatementsAreStringLiteral : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "struct Struct1 {"
                "u8 field1;"
            "}"
            "struct Struct2 {"
                "u16 field1;"
            "}"
            "struct Struct3 {"
                "u32 field1;"
            "}"
            "struct Struct4 {"
                "u8[4] blockType;"
                "variable_block : blockType {"
                    "case \"ab\": Struct1,"
                    "case \"a\": Struct2,"
                    "case \"b\": Struct3,"
                "}"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsStructCaseStatementsAreStringLiteral, verifyConsume)
    {
        tokenize(sv);
        parse();
    }

    struct WhenInputIsStructCaseStatementsAreStringLiteralAndSwitchingFieldIsVector : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "struct Struct1 {"
                "u8 field1;"
            "}"
            "struct Struct2 {"
                "u16 field1;"
            "}"
            "struct Struct3 {"
                "u32 field1;"
            "}"
            "struct Struct4 {"
                "u8 length;"
                "u8[length] blockType;"
                "variable_block : blockType {"
                    "case \"ab\": Struct1,"
                    "case \"a\": Struct2,"
                    "case \"b\": Struct3,"
                "}"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsStructCaseStatementsAreStringLiteralAndSwitchingFieldIsVector, verifyConsume)
    {
        tokenize(sv);
        parse();
    }

    struct WhenInputIsStructCaseStatementsAreStringLiteralAndCaseOverflowsSwitchField : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "struct Struct1 {"
                "u8 field1;"
            "}"
            "struct Struct2 {"
                "u16 field1;"
            "}"
            "struct Struct3 {"
                "u32 field1;"
            "}"
            "struct Struct4 {"
                "u8[2] blockType;"
                "variable_block : blockType {"
                    "case \"abc\": Struct1,"
                    "case \"a\": Struct2,"
                    "case \"b\": Struct3,"
                "}"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsStructCaseStatementsAreStringLiteralAndCaseOverflowsSwitchField, verifyConsume)
    {
        tokenize(sv);
        CHECK_THROW(parse(), swizzle::SyntaxError);
    }

    struct WhenInputIsStructCaseStatementsAreStringLiteralAndSwitchFieldIsNotArrayOrVector : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "struct Struct1 {"
                "u8 field1;"
            "}"
            "struct Struct2 {"
                "u16 field1;"
            "}"
            "struct Struct3 {"
                "u32 field1;"
            "}"
            "struct Struct4 {"
                "u8 blockType;"
                "variable_block : blockType {"
                    "case \"ab\": Struct1,"
                    "case \"a\": Struct2,"
                    "case \"b\": Struct3,"
                "}"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsStructCaseStatementsAreStringLiteralAndSwitchFieldIsNotArrayOrVector, verifyConsume)
    {
        tokenize(sv);
        CHECK_THROW(parse(), swizzle::SyntaxError);
    }


    struct WhenInputIsStructWithVerifyBlockAndCaseValueOverflowsSwitchField : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;"
            "struct Struct1 {"
                "u8 field1;"
                "f32 field2;"
            "}"
            "struct Struct2 {"
                "f64 field1;"
                "f64 field2;"
            "}"
            "struct Struct3 {"
                "u8 blockType;"
                "variable_block : blockType {"
                    "case 200: Struct1,"
                    "case 300: Struct2,"
                "}"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsStructWithVerifyBlockAndCaseValueOverflowsSwitchField, verifyConsume)
    {
        tokenize(sv);
        CHECK_THROW(parse(), std::runtime_error);
    }

    struct WhenInputIsStructWithNoFields : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "struct S1 {"
                " // comment " "\n"
            "}"
        );
    };

    TEST_FIXTURE(WhenInputIsStructWithNoFields, verifyConsume)
    {
        tokenize(sv);
        parse();
    }

    struct WhenInputIsAUsingStatementOfBuiltinType : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "using Indicator = u8;" "\n"
        );
    };

    TEST_FIXTURE(WhenInputIsAUsingStatementOfBuiltinType, verifyConsume)
    {
        auto matcher = Matcher().getChildrenOf<nodes::TypeAlias>().bind("using");
        CHECK(!matcher(parser.ast().root()));

        tokenize(sv);
        parse();

        CHECK(matcher(parser.ast().root()));

        const auto node = matcher.bound("using_0");
        REQUIRE CHECK(node);

        const auto& typeAlias = static_cast<nodes::TypeAlias&>(*node);
        CHECK_EQUAL("Indicator", typeAlias.aliasedType().token().value());
        CHECK_EQUAL("u8", typeAlias.existingType().token().value());
    }

    struct WhenInputIsAUsingStatementOfDefinedStruct : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "struct S1{u8 field1;}" "\n"

            "using Struct1 = S1;"
        );
    };

    TEST_FIXTURE(WhenInputIsAUsingStatementOfDefinedStruct, verifyConsume)
    {
        tokenize(sv);
        parse();
    }

    struct WhenInputIsAUsingStatementOfAnExternalType : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "extern ns1::Type1;" "\n"
            "namespace foo;" "\n"
            "using String = ns1::Type1;"
        );
    };
    
    TEST_FIXTURE(WhenInputIsAUsingStatementOfAnExternalType, verifyConsume)
    {
        tokenize(sv);
        parse();
    }
    
    struct WhenInputIsAUsingStatementWithAttribute : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "@attribute" "\n"
            "using Indicator = u8;"
        );
    };

    TEST_FIXTURE(WhenInputIsAUsingStatementWithAttribute, verifyConsume)
    {
        auto matcher = Matcher().getChildrenOf<nodes::TypeAlias>().bind("alias");
        CHECK(!matcher(parser.ast().root()));
        
        tokenize(sv);
        parse();

        CHECK(matcher(parser.ast().root()));
        
        const auto node = matcher.bound("alias_0");
        REQUIRE CHECK(node);
        
        // ensure our type alias has an attribute under it
        auto attributeMatcher = Matcher().getChildrenOf<nodes::Attribute>().bind("attribute");
        attributeMatcher(node);
        
        const auto a0_node = attributeMatcher.bound("attribute_0");
        REQUIRE CHECK(a0_node);
        
        const auto& attribute = static_cast<nodes::Attribute&>(*a0_node);
        CHECK_EQUAL("@attribute", attribute.info().token().value());
    }

    struct WhenInputIsAUsingStatementWithKeyValueAttribute : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "@attribute=\"value\"" "\n"
            "using Indicator = u8;"
        );
    };

    TEST_FIXTURE(WhenInputIsAUsingStatementWithKeyValueAttribute, verifyConsume)
    {
        auto matcher = Matcher().getChildrenOf<nodes::TypeAlias>().bind("alias");
        CHECK(!matcher(parser.ast().root()));
        
        tokenize(sv);
        parse();

        CHECK(matcher(parser.ast().root()));
        
        const auto node = matcher.bound("alias_0");
        REQUIRE CHECK(node);
        
        // ensure our type alias has attribute key/value under it
        auto attributeMatcher = Matcher().getChildrenOf<nodes::Attribute>().bind("attribute");
        attributeMatcher(node);
        
        const auto a0_node = attributeMatcher.bound("attribute_0");
        REQUIRE CHECK(a0_node);
        
        const auto& attribute = static_cast<nodes::Attribute&>(*a0_node);
        CHECK_EQUAL("@attribute", attribute.info().token().value());
        
        // the value of the attribute is a child of the attribute
        auto attributeValueMatcher = Matcher().getChildrenOf<nodes::StringLiteral>().bind("value");
        attributeValueMatcher(a0_node);
        
        const auto v0_node = attributeValueMatcher.bound("value_0");
        REQUIRE CHECK(v0_node);
        
        const auto& value = static_cast<nodes::StringLiteral&>(*v0_node);
        CHECK_EQUAL("\"value\"", value.info().token().value());
    }

    struct WhenInputIsAUsingStatementWithKeyValueAttributeAndCharLiteral : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "@attribute='a'" "\n"
            "using Indicator = u8;"
        );
    };

    TEST_FIXTURE(WhenInputIsAUsingStatementWithKeyValueAttributeAndCharLiteral, verifyConsume)
    {
        tokenize(sv);
        parse();

        // TODO: validate AST
    }

    struct WhenInputIsAUsingStatementWithKeyValueAttributeAndHexLiteral : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "@attribute=0x02" "\n"
            "using Indicator = u8;"
        );
    };

    TEST_FIXTURE(WhenInputIsAUsingStatementWithKeyValueAttributeAndHexLiteral, verifyConsume)
    {
        tokenize(sv);
        parse();

        // TODO: validate AST
    }

    struct WhenInputIsAusingStatementWithKeyValueAttributeAndNumericLiteral : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "@attribute=42" "\n"
            "using Indicator = f64;"
        );
    };

    TEST_FIXTURE(WhenInputIsAusingStatementWithKeyValueAttributeAndNumericLiteral, verifyConsume)
    {
        tokenize(sv);
        parse();

        // TODO: validate AST
    }

    struct WhenInputIsAUsingStatementButTypeIsNotDefined : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "namespace foo;" "\n"
            "using Indicator = bar::UndefinedType;" "\n"
        );
    };

    TEST_FIXTURE(WhenInputIsAUsingStatementButTypeIsNotDefined, verifyConsume)
    {
        tokenize(sv);
        CHECK_THROW(parse(), swizzle::SyntaxError);
    }
    
    struct WhenInputIsRealistic : public ParserFixture
    {
        const boost::string_view sv = boost::string_view(
            "extern ns1::Type1;" "\n"
            "extern ns2::Type2;" "\n"
            "\n"
            "namespace foo;" "\n"
            "\n"
            "using String = ns1::Type1;" "\n"
            
            "enum TransactionType : u8 {" "\n"
            "\t"    "New = '0'," "\n"
            "\t"    "Cancel = '1'," "\n"
            "}" "\n"
            
            "@inbound" "\n"
            "struct Transaction {"
            "\t" "TransactionType transactionType;" "\n"
            "}"
        );
    };
    
    TEST_FIXTURE(WhenInputIsRealistic, verifyConsume)
    {
        tokenize(sv);
        parse();
    }
}
