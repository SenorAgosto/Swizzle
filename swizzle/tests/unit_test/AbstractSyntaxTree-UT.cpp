#include "./ut_support/UnitTestSupport.hpp"

#include <swizzle/ast/AncestorInfo.hpp>
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
#include <swizzle/ast/nodes/Namespace.hpp>
#include <swizzle/ast/nodes/NumericLiteral.hpp>
#include <swizzle/ast/nodes/StringLiteral.hpp>
#include <swizzle/ast/nodes/TypeAlias.hpp>
#include <swizzle/ast/nodes/VariableBlock.hpp>
#include <swizzle/ast/nodes/VariableBlockCase.hpp>

#include <swizzle/lexer/TokenType.hpp>
#include <swizzle/types/NodeStack.hpp>
#include <swizzle/types/utils/AppendNode.hpp>

#include <cstddef>

namespace {

    using namespace swizzle::ast;
    using namespace swizzle::lexer;
    using namespace swizzle::parser;
    using namespace swizzle::types;

    class TestVisitor : public VisitorInterface
    {
    public:
        virtual ~TestVisitor(){}
        void operator()(AncestorInfo&, Node&) override {}

        void operator()(AncestorInfo&, nodes::Attribute&) override { attribute++; }
        void operator()(AncestorInfo&, nodes::AttributeBlock&) override { attributeBlock++; }
        void operator()(AncestorInfo&, nodes::BaseClass&) override { baseClasses++; }
        void operator()(AncestorInfo&, nodes::Bitfield&) override { bitfield++; }
        void operator()(AncestorInfo&, nodes::BitfieldField&) override { bitfieldField++; }
        void operator()(AncestorInfo&, nodes::DefaultFloatValue&) override { defaultFloat++; }
        void operator()(AncestorInfo&, nodes::DefaultStringValue&) override { defaultString++; }
        void operator()(AncestorInfo&, nodes::DefaultValue&) override { defaultValue++; }
        void operator()(AncestorInfo&, nodes::CharLiteral&) override { charLiteral++; }
        void operator()(AncestorInfo&, nodes::Comment&) override { comment++; }
        void operator()(AncestorInfo&, nodes::Enum&) override { enumCount++; }
        void operator()(AncestorInfo&, nodes::EnumField&) override { enumField++; }
        void operator()(AncestorInfo&, nodes::Extern&) override { externCount++; }
        void operator()(AncestorInfo&, nodes::FieldLabel&) override { fieldLabel++; }
        void operator()(AncestorInfo&, nodes::HexLiteral&) override { hexLiteral++; }
        void operator()(AncestorInfo&, nodes::Import&) override { import++; }
        void operator()(AncestorInfo&, nodes::MultilineComment&) override { multilineComment++; }
        void operator()(AncestorInfo&, nodes::Namespace&) override { nameSpace++; }
        void operator()(AncestorInfo&, nodes::NumericLiteral&) override { numericLiteral++; }
        void operator()(AncestorInfo&, nodes::Root&) override { root++; }
        void operator()(AncestorInfo&, nodes::StringLiteral&) override { stringLiteral++; }
        void operator()(AncestorInfo&, nodes::Struct&) override { structCount++; }
        void operator()(AncestorInfo&, nodes::StructField&) override { structField++; }
        void operator()(AncestorInfo&, nodes::TypeAlias&) override { typeAlias++; }
        void operator()(AncestorInfo&, nodes::VariableBlock&) override { varBlock++; }
        void operator()(AncestorInfo&, nodes::VariableBlockCase&) override { varBlockCase++; }

    public:
        std::size_t root = 0;

        std::size_t attribute = 0;
        std::size_t attributeBlock = 0;
        std::size_t baseClasses = 0;
        std::size_t bitfield = 0;
        std::size_t bitfieldField = 0;
        std::size_t charLiteral = 0;
        std::size_t comment = 0;
        std::size_t defaultFloat = 0;
        std::size_t defaultString = 0;
        std::size_t defaultValue = 0;
        std::size_t enumCount = 0;
        std::size_t enumField = 0;
        std::size_t externCount = 0;
        std::size_t fieldLabel = 0;
        std::size_t hexLiteral = 0;
        std::size_t import = 0;
        std::size_t multilineComment = 0;
        std::size_t nameSpace = 0;
        std::size_t numericLiteral = 0;
        std::size_t stringLiteral = 0;
        std::size_t structCount = 0;
        std::size_t structField = 0;
        std::size_t typeAlias = 0;
        std::size_t varBlock = 0;
        std::size_t varBlockCase = 0;
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
            utils::appendNode<nodes::Attribute>(nodeStack, TokenInfo(Token("@verify", 0, 7, TokenType::attribute), FileInfo("test.swizzle")));
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
            utils::appendNode<nodes::AttributeBlock>(nodeStack, TokenInfo(Token("{size() != 0}", 0, 13, TokenType::attribute), FileInfo("test.swizzle")));
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
            utils::appendNode<nodes::CharLiteral>(nodeStack, TokenInfo(Token("'a'", 0, 3, TokenType::char_literal), FileInfo("test.swizzle")));
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
            utils::appendNode<nodes::Comment>(nodeStack, TokenInfo(Token("// comment", 0, 10, TokenType::comment), FileInfo("test.swizzle")));
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

            utils::appendNode<nodes::Enum>(nodeStack, keyword, name, "my_namespace");
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

            utils::appendNode<nodes::EnumField>(nodeStack, name, underlying);
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
            utils::appendNode<nodes::HexLiteral>(nodeStack, TokenInfo(Token("0x01", 0, 4, TokenType::hex_literal), FileInfo("test.swizzle")));
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
            utils::appendNode<nodes::Import>(nodeStack, TokenInfo(Token("blahblah", 0, 8, TokenType::string), FileInfo("test.swizzle")), "blahblah.swizzle");
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
            utils::appendNode<nodes::MultilineComment>(nodeStack, TokenInfo(Token("// multiline comment", 0, 21, TokenType::multiline_comment), FileInfo("test.swizzle")));
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
            utils::appendNode<nodes::NumericLiteral>(nodeStack, TokenInfo(Token("42", 0, 2, TokenType::numeric_literal), FileInfo("test.swizzle")));
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
            utils::appendNode<nodes::StringLiteral>(nodeStack, TokenInfo(Token("\"blah\"", 0, 6, TokenType::string_literal), FileInfo("test.swizzle")));
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

            utils::appendNode<nodes::TypeAlias>(nodeStack, usingClause, newType);
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
