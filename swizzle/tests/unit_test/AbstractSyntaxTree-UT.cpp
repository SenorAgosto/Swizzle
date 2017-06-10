#include "./platform/UnitTestSupport.hpp"

#include <swizzle/ast/AbstractSyntaxTree.hpp>
#include <swizzle/ast/VisitorInterface.hpp>
#include <swizzle/ast/nodes/Attribute.hpp>
#include <swizzle/ast/nodes/AttributeBlock.hpp>
#include <swizzle/ast/nodes/CharLiteral.hpp>
#include <swizzle/ast/nodes/Comment.hpp>
#include <swizzle/ast/nodes/Enum.hpp>
#include <swizzle/ast/nodes/EnumField.hpp>
#include <swizzle/ast/nodes/HexLiteral.hpp>
#include <swizzle/ast/nodes/Import.hpp>
#include <swizzle/ast/nodes/MultilineComment.hpp>
#include <swizzle/ast/nodes/NumericLiteral.hpp>
#include <swizzle/ast/nodes/StringLiteral.hpp>
#include <swizzle/ast/nodes/TypeAlias.hpp>
#include <swizzle/lexer/TokenType.hpp>
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/parser/detail/AppendNode.hpp>

#include <cstddef>

namespace {

    using namespace swizzle::ast;
    using namespace swizzle::lexer;
    using namespace swizzle::parser;

    class TestVisitor : public VisitorInterface
    {
    public:
        virtual ~TestVisitor(){}
        void operator()(Node&) override { root++; }

        void operator()(nodes::Attribute&) override { attribute++; }
        void operator()(nodes::AttributeBlock&) override { attributeBlock++; }
        void operator()(nodes::Bitfield&) override { bitfield++; }
        void operator()(nodes::BitfieldField&) override { bitfieldField++; }
        void operator()(nodes::CharLiteral&) override { charLiteral++; }
        void operator()(nodes::Comment&) override { comment++; }
        void operator()(nodes::Enum&) override { enumCount++; }
        void operator()(nodes::EnumField&) override { enumField++; }
        void operator()(nodes::FieldLabel&) override { fieldLabel++; }
        void operator()(nodes::HexLiteral&) override { hexLiteral++; }
        void operator()(nodes::Import&) override { import++; }
        void operator()(nodes::MultilineComment&) override { multilineComment++; }
        void operator()(nodes::NumericLiteral&) override { numericLiteral++; }
        void operator()(nodes::StringLiteral&) override { stringLiteral++; }
        void operator()(nodes::Struct&) override { structCount++; }
        void operator()(nodes::StructField&) override { structField++; }
        void operator()(nodes::TypeAlias&) override { typeAlias++; }

    public:
        std::size_t root = 0;

        std::size_t attribute = 0;
        std::size_t attributeBlock = 0;
        std::size_t bitfield = 0;
        std::size_t bitfieldField = 0;
        std::size_t charLiteral = 0;
        std::size_t comment = 0;
        std::size_t enumCount = 0;
        std::size_t enumField = 0;
        std::size_t fieldLabel = 0;
        std::size_t hexLiteral = 0;
        std::size_t import = 0;
        std::size_t multilineComment = 0;
        std::size_t numericLiteral = 0;
        std::size_t stringLiteral = 0;
        std::size_t structCount = 0;
        std::size_t structField = 0;
        std::size_t typeAlias = 0;
    };

    struct AbstractSyntaxTreeFixture
    {
        AbstractSyntaxTreeFixture()
        {
            nodeStack.push(ast.root());
        }

        AbstractSyntaxTree ast;
        NodeStack nodeStack;

        TestVisitor visitor;
    };

    TEST_FIXTURE(AbstractSyntaxTreeFixture, verifyAccept)
    {
        CHECK_EQUAL(0U, visitor.root);

        ast.accept(visitor);

        CHECK_EQUAL(1U, visitor.root);
    }

    struct WhenTreeHasAttribute : public AbstractSyntaxTreeFixture
    {
        WhenTreeHasAttribute()
        {
            detail::appendNode<nodes::Attribute>(nodeStack, TokenInfo(Token("@verify", 0, 7, TokenType::attribute), FileInfo("test.swizzle")));
        }
    };

    TEST_FIXTURE(WhenTreeHasAttribute, verifyAccept)
    {
        CHECK_EQUAL(0U, visitor.root);
        CHECK_EQUAL(0U, visitor.attribute);

        ast.accept(visitor);

        CHECK_EQUAL(1U, visitor.root);
        CHECK_EQUAL(1U, visitor.attribute);
    }

    struct WhenTreeHasAttributeBlock : public AbstractSyntaxTreeFixture
    {
        WhenTreeHasAttributeBlock()
        {
            detail::appendNode<nodes::AttributeBlock>(nodeStack, TokenInfo(Token("{size() != 0}", 0, 13, TokenType::attribute), FileInfo("test.swizzle")));
        }
    };

    TEST_FIXTURE(WhenTreeHasAttributeBlock, verifyAccept)
    {
        CHECK_EQUAL(0U, visitor.root);
        CHECK_EQUAL(0U, visitor.attributeBlock);

        ast.accept(visitor);

        CHECK_EQUAL(1U, visitor.root);
        CHECK_EQUAL(1U, visitor.attributeBlock);
    }

    struct WhenTreeHasCharLiteral : public AbstractSyntaxTreeFixture
    {
        WhenTreeHasCharLiteral()
        {
            detail::appendNode<nodes::CharLiteral>(nodeStack, TokenInfo(Token("'a'", 0, 3, TokenType::char_literal), FileInfo("test.swizzle")));
        }
    };

    TEST_FIXTURE(WhenTreeHasCharLiteral, verifyAccept)
    {
        CHECK_EQUAL(0U, visitor.root);
        CHECK_EQUAL(0U, visitor.charLiteral);

        ast.accept(visitor);

        CHECK_EQUAL(1U, visitor.root);
        CHECK_EQUAL(1U, visitor.charLiteral);
    }

    struct WhenTreeHasComment : public AbstractSyntaxTreeFixture
    {
        WhenTreeHasComment()
        {
            detail::appendNode<nodes::Comment>(nodeStack, TokenInfo(Token("// comment", 0, 10, TokenType::comment), FileInfo("test.swizzle")));
        }
    };

    TEST_FIXTURE(WhenTreeHasComment, verifyAccept)
    {
        CHECK_EQUAL(0U, visitor.root);
        CHECK_EQUAL(0U, visitor.comment);

        ast.accept(visitor);

        CHECK_EQUAL(1U, visitor.root);
        CHECK_EQUAL(1U, visitor.comment);
    }

    struct WhenTreeHasEnum : public AbstractSyntaxTreeFixture
    {
        WhenTreeHasEnum()
        {
            const auto keyword = TokenInfo(Token("enum", 0, 4, TokenType::keyword), FileInfo("test.swizzle"));
            const auto name = TokenInfo(Token("MyEnum", 0, 6, TokenType::string), FileInfo("test.swizzle"));

            detail::appendNode<nodes::Enum>(nodeStack, keyword, name, "my_namespace");
        }
    };

    TEST_FIXTURE(WhenTreeHasEnum, verifyAccept)
    {
        CHECK_EQUAL(0U, visitor.root);
        CHECK_EQUAL(0U, visitor.enumCount);

        ast.accept(visitor);

        CHECK_EQUAL(1U, visitor.root);
        CHECK_EQUAL(1U, visitor.enumCount);
    }

    struct WhenTreeHasEnumField : public AbstractSyntaxTreeFixture
    {
        WhenTreeHasEnumField()
        {
            const auto name = TokenInfo(Token("MyEnum", 0, 6, TokenType::string), FileInfo("test.swizzle"));
            const auto underlying = TokenInfo(Token("u8", 0, 2, TokenType::type), FileInfo("test.swizzle"));

            detail::appendNode<nodes::EnumField>(nodeStack, name, underlying);
        }
    };

    TEST_FIXTURE(WhenTreeHasEnumField, verifyAccept)
    {
        CHECK_EQUAL(0U, visitor.root);
        CHECK_EQUAL(0U, visitor.enumField);

        ast.accept(visitor);

        CHECK_EQUAL(1U, visitor.root);
        CHECK_EQUAL(1U, visitor.enumField);
    }

    struct WhenTreeHasHexLiteral : public AbstractSyntaxTreeFixture
    {
        WhenTreeHasHexLiteral()
        {
            detail::appendNode<nodes::HexLiteral>(nodeStack, TokenInfo(Token("0x01", 0, 4, TokenType::hex_literal), FileInfo("test.swizzle")));
        }
    };

    TEST_FIXTURE(WhenTreeHasHexLiteral, verifyAccept)
    {
        CHECK_EQUAL(0U, visitor.root);
        CHECK_EQUAL(0U, visitor.hexLiteral);

        ast.accept(visitor);

        CHECK_EQUAL(1U, visitor.root);
        CHECK_EQUAL(1U, visitor.hexLiteral);
    }

    struct WhenTreeHasImport : public AbstractSyntaxTreeFixture
    {
        WhenTreeHasImport()
        {
            detail::appendNode<nodes::Import>(nodeStack, TokenInfo(Token("blahblah", 0, 8, TokenType::string), FileInfo("test.swizzle")), "blahblah.swizzle");
        }
    };

    TEST_FIXTURE(WhenTreeHasImport, verifyAccept)
    {
        CHECK_EQUAL(0U, visitor.root);
        CHECK_EQUAL(0U, visitor.import);

        ast.accept(visitor);

        CHECK_EQUAL(1U, visitor.root);
        CHECK_EQUAL(1U, visitor.import);
    }

    struct WhenTreeHasMultilineComment : public AbstractSyntaxTreeFixture
    {
        WhenTreeHasMultilineComment()
        {
            detail::appendNode<nodes::MultilineComment>(nodeStack, TokenInfo(Token("// multiline comment", 0, 21, TokenType::multiline_comment), FileInfo("test.swizzle")));
        }
    };

    TEST_FIXTURE(WhenTreeHasMultilineComment, verifyAccept)
    {
        CHECK_EQUAL(0U, visitor.root);
        CHECK_EQUAL(0U, visitor.multilineComment);

        ast.accept(visitor);

        CHECK_EQUAL(1U, visitor.root);
        CHECK_EQUAL(1U, visitor.multilineComment);
    }

    struct WhenTreeHasNumericLiteral : public AbstractSyntaxTreeFixture
    {
        WhenTreeHasNumericLiteral()
        {
            detail::appendNode<nodes::NumericLiteral>(nodeStack, TokenInfo(Token("42", 0, 2, TokenType::numeric_literal), FileInfo("test.swizzle")));
        }
    };

    TEST_FIXTURE(WhenTreeHasNumericLiteral, verifyAccept)
    {
        CHECK_EQUAL(0U, visitor.root);
        CHECK_EQUAL(0U, visitor.numericLiteral);

        ast.accept(visitor);

        CHECK_EQUAL(1U, visitor.root);
        CHECK_EQUAL(1U, visitor.numericLiteral);
    }

    struct WhenTreeHasStringLiteral : public AbstractSyntaxTreeFixture
    {
        WhenTreeHasStringLiteral()
        {
            detail::appendNode<nodes::StringLiteral>(nodeStack, TokenInfo(Token("\"blah\"", 0, 6, TokenType::string_literal), FileInfo("test.swizzle")));
        }
    };

    TEST_FIXTURE(WhenTreeHasStringLiteral, verifyAccept)
    {
        CHECK_EQUAL(0U, visitor.root);
        CHECK_EQUAL(0U, visitor.stringLiteral);

        ast.accept(visitor);

        CHECK_EQUAL(1U, visitor.root);
        CHECK_EQUAL(1U, visitor.stringLiteral);
    }

    struct WhenTreeHasTypeAlias : public AbstractSyntaxTreeFixture
    {
        WhenTreeHasTypeAlias()
        {
            const auto usingClause = TokenInfo(Token("using", 0, 5, TokenType::keyword), FileInfo("test.swizzle"));
            const auto newType = TokenInfo(Token("MyType", 0, 5, TokenType::string_literal), FileInfo("test.swizzle"));

            detail::appendNode<nodes::TypeAlias>(nodeStack, usingClause, newType);
        }
    };

    TEST_FIXTURE(WhenTreeHasTypeAlias, verifyAccept)
    {
        CHECK_EQUAL(0U, visitor.root);
        CHECK_EQUAL(0U, visitor.typeAlias);

        ast.accept(visitor);

        CHECK_EQUAL(1U, visitor.root);
        CHECK_EQUAL(1U, visitor.typeAlias);
    }
}
