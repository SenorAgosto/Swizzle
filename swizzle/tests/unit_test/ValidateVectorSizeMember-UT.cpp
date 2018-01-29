#include "./ut_support/UnitTestSupport.hpp"
#include <swizzle/parser/detail/ValidateVectorSizeMember.hpp>

#include <swizzle/ast/AbstractSyntaxTree.hpp>
#include <swizzle/ast/nodes/Comment.hpp>
#include <swizzle/ast/nodes/Struct.hpp>
#include <swizzle/ast/nodes/StructField.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/types/NodeStack.hpp>
#include <swizzle/types/utils/AppendNode.hpp>
#include <swizzle/types/TokenStack.hpp>

namespace {

    using namespace swizzle::ast;
    using namespace swizzle::lexer;
    using namespace swizzle::parser;
    using namespace swizzle::types;

    struct ValidateVectorSizeMemberFixture
    {
        Node::smartptr make_struct(const std::string& nameSpace, const std::string& structName)
        {
            const auto info = TokenInfo(Token("struct", 0, 6, TokenType::keyword), FileInfo("test.swizzle"));
            const auto name = TokenInfo(Token(structName, 0, structName.length(), TokenType::string), FileInfo("test.swizzle"));
            const auto node = utils::appendNode<nodes::Struct>(nodeStack, info, name, nameSpace);

            return node;
        }

        Node::smartptr make_field(const std::string& type, const Token& name)
        {
            auto node = utils::appendNode<nodes::StructField>(nodeStack);
            auto& field = static_cast<nodes::StructField&>(*node);
            field.type(type);
            field.name(TokenInfo(name, FileInfo("test.swizzle")));

            return node;
        }

        AbstractSyntaxTree ast;
        NodeStack nodeStack;
        TokenStack tokenStack;
        ParserStateContext context;

        const TokenInfo token = TokenInfo(Token("field3", 0, 6, TokenType::string), FileInfo("test.swizzle"));
    };

    struct WhenTokenStackHasMemberNameFromCurrentStruct : public ValidateVectorSizeMemberFixture
    {
        WhenTokenStackHasMemberNameFromCurrentStruct()
        {
            nodeStack.push(ast.root());

            auto node = make_struct("my_namespace", "MyStruct");
            context.TypeCache["my_namespace::MyStruct"] = node;
            nodeStack.push(node);

            node = make_field("u8", field1);
            node = make_field("u16", field2);
            node = make_field("u16", field3);

            nodeStack.push(node);
            tokenStack.push(TokenInfo(Token("field1", 0, 6, TokenType::string), FileInfo("test.swizzle")));
        }

        const Token field1 = Token("field1", 0, 6, TokenType::string);
        const Token field2 = Token("field2", 0, 6, TokenType::string);
        const Token field3 = Token("field3", 0, 6, TokenType::string);
    };

    TEST_FIXTURE(WhenTokenStackHasMemberNameFromCurrentStruct, verifyValidateVectorSizeMember)
    {
        detail::validateVectorSizeMember(token, nodeStack, tokenStack, context);
    }

    struct WhenTokenStackHasMemberNameAndItsNested : public ValidateVectorSizeMemberFixture
    {
        WhenTokenStackHasMemberNameAndItsNested()
        {
            nodeStack.push(ast.root());

            // my_namespace::struct1
            auto node = make_struct("my_namespace", "struct1");
            context.TypeCache["my_namespace::struct1"] = node;
            nodeStack.push(node);

            node = make_field("other_namespace::struct2", field1);
            node = make_field("u8", field2);
            nodeStack.push(node);

            // other_namespace::struct2
            node = make_struct("other_namespace", "struct2");
            context.TypeCache["other_namespace::struct2"] = node;

            nodeStack.push(node);
            node = make_field("other2::struct3", f1);
            nodeStack.pop();

            // other2::struct3
            node = make_struct("other2", "struct3");
            context.TypeCache["other2::struct3"] = node;

            nodeStack.push(node);
            node = make_field("u8", field2);
            nodeStack.pop();

            // my_namespace::struct1 -> other_namespace::struct2 -> other2::struct3
            // field1.f1.field2
            tokenStack.push(TokenInfo(Token("field1", 0, 6, TokenType::string), FileInfo("test.swizzle")));
            tokenStack.push(TokenInfo(Token("f1", 0, 2, TokenType::string), FileInfo("test.swizzle")));
            tokenStack.push(TokenInfo(Token("field2", 0, 6, TokenType::string), FileInfo("test.swizzle")));
        }

        const Token field1 = Token("field1", 0, 6, TokenType::string);
        const Token f1 = Token("f1", 0, 2, TokenType::string);
        const Token field2 = Token("field2", 0, 6, TokenType::string);
    };

    TEST_FIXTURE(WhenTokenStackHasMemberNameAndItsNested, verifyValidateVectorSizeMember)
    {
        detail::validateVectorSizeMember(token, nodeStack, tokenStack, context);
    }

    struct WhenTokenStackHasMemberNameAndItsNestedOneFieldInCurrentNamespace : public ValidateVectorSizeMemberFixture
    {
        WhenTokenStackHasMemberNameAndItsNestedOneFieldInCurrentNamespace()
        {
            nodeStack.push(ast.root());
            context.CurrentNamespace = "my_namespace";

            // my_namespace::struct1
            auto node = make_struct("", "struct1");
            context.TypeCache["my_namespace::struct1"] = node;
            nodeStack.push(node);

            node = make_field("other_namespace::struct2", field1);
            node = make_field("u8", field2);
            nodeStack.push(node);

            // other_namespace::struct2
            node = make_struct("other_namespace", "struct2");
            context.TypeCache["other_namespace::struct2"] = node;

            nodeStack.push(node);
            node = make_field("my_namespace::struct3", f1);
            nodeStack.pop();

            // other2::struct3
            node = make_struct("", "struct3");
            context.TypeCache["my_namespace::struct3"] = node;

            nodeStack.push(node);
            node = make_field("u8", field2);
            nodeStack.pop();

            // my_namespace::struct1 -> other_namespace::struct2 -> other2::struct3
            // field1.f1.field2
            tokenStack.push(TokenInfo(Token("field1", 0, 6, TokenType::string), FileInfo("test.swizzle")));
            tokenStack.push(TokenInfo(Token("f1", 0, 2, TokenType::string), FileInfo("test.swizzle")));
            tokenStack.push(TokenInfo(Token("field2", 0, 6, TokenType::string), FileInfo("test.swizzle")));
        }

        const Token field1 = Token("field1", 0, 6, TokenType::string);
        const Token f1 = Token("f1", 0, 2, TokenType::string);
        const Token field2 = Token("field2", 0, 6, TokenType::string);
    };

    TEST_FIXTURE(WhenTokenStackHasMemberNameAndItsNestedOneFieldInCurrentNamespace, verifyValidateVectorSizeMember)
    {
        detail::validateVectorSizeMember(token, nodeStack, tokenStack, context);
    }

    struct WhenTokenStackHasMemberNameAndTypeIsNotInteger : public ValidateVectorSizeMemberFixture
    {
        WhenTokenStackHasMemberNameAndTypeIsNotInteger()
        {
            nodeStack.push(ast.root());

            auto node = make_struct("my_namespace", "MyStruct");
            context.TypeCache["my_namespace::MyStruct"] = node;
            nodeStack.push(node);

            node = make_field("f32", field1);
            nodeStack.push(node);

            tokenStack.push(TokenInfo(Token("field1", 0, 6, TokenType::string), FileInfo("test.swizzle")));
        }

        const Token field1 = Token("field1", 0, 6, TokenType::string);
    };

    TEST_FIXTURE(WhenTokenStackHasMemberNameAndTypeIsNotInteger, verifyValidateVectorSizeMemberThrows)
    {
        CHECK_THROW(detail::validateVectorSizeMember(token, nodeStack, tokenStack, context), swizzle::SyntaxError);
    }

    struct WhenTokenStackHasMemberAndTypeIsNotDefinedInTypeCache : public ValidateVectorSizeMemberFixture
    {
        WhenTokenStackHasMemberAndTypeIsNotDefinedInTypeCache()
        {
            nodeStack.push(ast.root());

            auto node = make_struct("my_namespace", "MyStruct");
            context.TypeCache["my_namespace::MyStruct"] = node;
            nodeStack.push(node);

            node = make_field("u8", field2);
            nodeStack.push(node);

            tokenStack.push(TokenInfo(Token("field1", 0, 6, TokenType::string), FileInfo("test.swizzle")));
        }

        const Token field2 = Token("field2", 0, 6, TokenType::string);
    };

    TEST_FIXTURE(WhenTokenStackHasMemberAndTypeIsNotDefinedInTypeCache, verifyValidateSizeMembersThrows)
    {
        CHECK_THROW(detail::validateVectorSizeMember(token, nodeStack, tokenStack, context), swizzle::SyntaxError);
    }

    struct WhenTokenStackHasMemberAndTheLastEntryIsAStruct : public ValidateVectorSizeMemberFixture
    {
        WhenTokenStackHasMemberAndTheLastEntryIsAStruct()
        {
            nodeStack.push(ast.root());
            context.CurrentNamespace = "my_namespace";

            // create ThatStruct
            auto node = make_struct("my_namespace", "ThatStruct");
            context.TypeCache["my_namespace::ThatStruct"] = node;

            node = make_struct("my_namespace", "MyStruct");
            context.TypeCache["my_namespace::MyStruct"] = node;
            nodeStack.push(node);

            node = make_field("my_namespace::ThatStruct", field1);
            nodeStack.push(node);

            tokenStack.push(TokenInfo(Token("field1", 0, 6, TokenType::string), FileInfo("test.swizzle")));
        }

        const Token field1 = Token("field1", 0, 6, TokenType::string);
    };

    TEST_FIXTURE(WhenTokenStackHasMemberAndTheLastEntryIsAStruct, verifyValidateVectorMemberThrows)
    {
        CHECK_THROW(detail::validateVectorSizeMember(token, nodeStack, tokenStack, context), swizzle::SyntaxError);
    }

    struct WhenTokenStackIsEmpty : public ValidateVectorSizeMemberFixture
    {
    };

    TEST_FIXTURE(WhenTokenStackIsEmpty, verifyValidateVectorSizeMemberThrows)
    {
        CHECK_THROW(detail::validateVectorSizeMember(token, nodeStack, tokenStack, context), swizzle::SyntaxErrorWithoutToken);
    }

    struct WhenNodeStackIsEmpty : public ValidateVectorSizeMemberFixture
    {
        WhenNodeStackIsEmpty()
        {
            tokenStack.push(TokenInfo(Token("field1", 0, 6, TokenType::string), FileInfo("test.swizzle")));
        }
    };

    TEST_FIXTURE(WhenNodeStackIsEmpty, verifyValidateVectorSizeMemberThrows)
    {
        CHECK_THROW(detail::validateVectorSizeMember(token, nodeStack, tokenStack, context), swizzle::SyntaxErrorWithoutToken);
    }

    struct WhenTopOfNodeStackIsNotStructField : public ValidateVectorSizeMemberFixture
    {
        WhenTopOfNodeStackIsNotStructField()
        {
            nodeStack.push(ast.root());

            const auto node = utils::appendNode<nodes::Comment>(nodeStack, TokenInfo(Token("// blah", 0, 7, TokenType::comment), FileInfo("test.swizzle")));
            nodeStack.push(node);
            tokenStack.push(token);
        }
    };

    TEST_FIXTURE(WhenTopOfNodeStackIsNotStructField, verifyCreateMemberThrows)
    {
        CHECK_THROW(detail::validateVectorSizeMember(token, nodeStack, tokenStack, context), swizzle::SyntaxErrorWithoutToken);
    }

    struct WhenSecondItemInNodeStackIsNotStruct : public ValidateVectorSizeMemberFixture
    {
        WhenSecondItemInNodeStackIsNotStruct()
        {
            nodeStack.push(ast.root());

            const auto info = TokenInfo(Token("// blah", 0, 7, TokenType::comment), FileInfo("test.swizzle"));

            auto node = utils::appendNode<nodes::Comment>(nodeStack, info);
            nodeStack.push(node);

            node = utils::appendNode<nodes::StructField>(nodeStack);
            nodeStack.push(node);
        }
    };

    TEST_FIXTURE(WhenSecondItemInNodeStackIsNotStruct, verifyCreateMemberThrows)
    {
        CHECK_THROW(detail::validateVectorSizeMember(token, nodeStack, tokenStack, context), swizzle::SyntaxErrorWithoutToken);
    }
}
