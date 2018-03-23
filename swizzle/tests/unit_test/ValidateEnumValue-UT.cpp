#include "./ut_support/UnitTestSupport.hpp"
#include <swizzle/types/utils/ValidateEnumValue.hpp>
#include <swizzle/types/utils/AppendNode.hpp>

#include <swizzle/ast/nodes/Enum.hpp>
#include <swizzle/ast/nodes/EnumField.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/ParserStateContext.hpp>

namespace {

    using namespace swizzle;
    
    struct ValidateEnumValueFixture
    {
        parser::ParserStateContext context;
    };

    struct WhenTokenIsValidEnumValue : public  ValidateEnumValueFixture
    {
        WhenTokenIsValidEnumValue()
        {
            context.CurrentNamespace = "foo";
            context.SymbolTable.insert("foo::E", types::SymbolInfo("foo::E", types::SymbolType::Struct, node));
            
            node->append(new ast::nodes::EnumField(enum_value, underlying));
        }
        
        const boost::string_view e = boost::string_view("enum E : u8 { e1 = 1, )");
        const lexer::TokenInfo enum_decl = lexer::TokenInfo(lexer::Token(e, 0, 4, lexer::TokenType::keyword), lexer::FileInfo("test.swizzle"));
        const lexer::TokenInfo enum_name = lexer::TokenInfo(lexer::Token(e, 5, 1, lexer::TokenType::string), lexer::FileInfo("test.swizzle"));
        const lexer::TokenInfo underlying = lexer::TokenInfo(lexer::Token(e, 9, 2, lexer::TokenType::type), lexer::FileInfo("test.swizzle"));
        const lexer::TokenInfo enum_value = lexer::TokenInfo(lexer::Token(e, 14, 2, lexer::TokenType::string), lexer::FileInfo("test.swizzle"));
        
        const boost::string_view sv = boost::string_view("E::e1");
        const lexer::TokenInfo token = lexer::TokenInfo(lexer::Token(sv, 0, 5, lexer::TokenType::string), lexer::FileInfo("test.swizzle"));
        
        ast::Node::smartptr node = new ast::nodes::Enum(enum_decl, enum_name, "foo");
    };

    TEST_FIXTURE(WhenTokenIsValidEnumValue, verifyValidateEnumValue)
    {
        CHECK(types::utils::validateEnumValue(context, "foo::E", token));
    }
    
    struct WhenTokenIsNotEnumValue : public ValidateEnumValueFixture
    {
        WhenTokenIsNotEnumValue()
        {
            context.SymbolTable.insert("foo::E", types::SymbolInfo("foo::E", types::SymbolType::Struct, node));
        }
        
        const boost::string_view e = boost::string_view("enum E : u8 { e1 = 1, )");
        const lexer::TokenInfo enum_decl = lexer::TokenInfo(lexer::Token(e, 0, 4, lexer::TokenType::keyword), lexer::FileInfo("test.swizzle"));
        const lexer::TokenInfo enum_name = lexer::TokenInfo(lexer::Token(e, 5, 1, lexer::TokenType::string), lexer::FileInfo("test.swizzle"));
        const lexer::TokenInfo underlying = lexer::TokenInfo(lexer::Token(e, 9, 2, lexer::TokenType::type), lexer::FileInfo("test.swizzle"));
        const lexer::TokenInfo enum_value = lexer::TokenInfo(lexer::Token(e, 14, 2, lexer::TokenType::string), lexer::FileInfo("test.swizzle"));
        
        const boost::string_view sv = boost::string_view("not_enum_value");
        const lexer::TokenInfo token = lexer::TokenInfo(lexer::Token(sv, 0, 14, lexer::TokenType::string), lexer::FileInfo("test.swizzle"));
        
        ast::Node::smartptr node = new ast::nodes::Enum(enum_decl, enum_name, "foo");
    };
    
    TEST_FIXTURE(WhenTokenIsNotEnumValue, verifyValidateEnumValue)
    {
        CHECK(!types::utils::validateEnumValue(context, "foo:E", token));
    }

    struct WhenTokenIsUndefinedEnumValue : public  ValidateEnumValueFixture
    {
        WhenTokenIsUndefinedEnumValue()
        {
            context.SymbolTable.insert("foo::E", types::SymbolInfo("foo::E", types::SymbolType::Struct, node));
        }
        
        const boost::string_view e = boost::string_view("enum E : u8 { e1 = 1, )");
        const lexer::TokenInfo enum_decl = lexer::TokenInfo(lexer::Token(e, 0, 4, lexer::TokenType::keyword), lexer::FileInfo("test.swizzle"));
        const lexer::TokenInfo enum_name = lexer::TokenInfo(lexer::Token(e, 5, 1, lexer::TokenType::string), lexer::FileInfo("test.swizzle"));
        const lexer::TokenInfo underlying = lexer::TokenInfo(lexer::Token(e, 9, 2, lexer::TokenType::type), lexer::FileInfo("test.swizzle"));
        const lexer::TokenInfo enum_value = lexer::TokenInfo(lexer::Token(e, 14, 2, lexer::TokenType::string), lexer::FileInfo("test.swizzle"));
        
        const boost::string_view sv = boost::string_view("E::e1");
        const lexer::TokenInfo token = lexer::TokenInfo(lexer::Token(sv, 0, 5, lexer::TokenType::string), lexer::FileInfo("test.swizzle"));
        
        ast::Node::smartptr node = new ast::nodes::Enum(enum_decl, enum_name, "foo");
    };
    
    TEST_FIXTURE(WhenTokenIsUndefinedEnumValue, verifyValidateEnumValue)
    {
        CHECK_THROW(types::utils::validateEnumValue(context, "foo::E", token), std::exception);
    }
}
