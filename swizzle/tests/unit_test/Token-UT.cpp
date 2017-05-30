#include "./platform/UnitTestSupport.hpp"
#include <swizzle/lexer/Token.hpp>

#include <boost/utility/string_view.hpp>
#include <stdexcept>

namespace {

    using namespace swizzle::lexer;
    
    TEST(verifyInstantation)
    {
        const Token token;

        CHECK_EQUAL("", token.to_string());
        CHECK_EQUAL(TokenType::string, token.type());
    }
    
    TEST(verifyInstantationWithValueAndType)
    {
        const Token token("value", 0, 5, TokenType::comment);
        
        CHECK_EQUAL("value", token.to_string());
        CHECK_EQUAL(TokenType::comment, token.type());
    }
    
    TEST(verifySettingType)
    {
        Token token("value", 0, 5, TokenType::comment);
        
        CHECK_EQUAL("value", token.to_string());
        CHECK_EQUAL(TokenType::comment, token.type());

        token.type(TokenType::multiline_comment);
        CHECK_EQUAL(TokenType::multiline_comment, token.type());
    }

    TEST(verifyExpand)
    {
        const std::string s = "This is a string";
        const boost::string_view sv(s);

        Token token(sv, 0, 2, TokenType::string);
        token.expand(2);

        CHECK_EQUAL("This", token.value());
        CHECK_EQUAL("This", token.to_string());
    }

    TEST(verifyExpandTruncatesOnOverrun)
    {
        const std::string s = "This is a string";
        const boost::string_view sv(s);

        Token token(sv, 0, 5, TokenType::string);
        token.expand(100);

        CHECK_EQUAL(s, token.to_string());
    }

    TEST(verifyContract)
    {
        const std::string s = "100..00";
        const boost::string_view sv(s);

        Token token(sv, 0, 4, TokenType::float_literal);

        CHECK_EQUAL("100.", token.to_string());
        token.contract();
        CHECK_EQUAL("100", token.to_string());
    }

    TEST(verifyEmpty)
    {
        const std::string s = " \t\r \na";
        const boost::string_view sv(s);

        Token token(sv, 0, 0, TokenType::string);
        CHECK(token.empty());

        token.expand(1);   // ' '
        CHECK(token.empty());

        token.expand(1);   // \t
        CHECK(token.empty());

        token.expand(1);   // \r
        CHECK(token.empty());

        token.expand(1);   // ' '
        CHECK(token.empty());

        token.expand(1);   // \n
        CHECK(token.empty());

        token.expand(1);   // a
        CHECK(!token.empty());

        const Token token2(sv, 0, 0, TokenType::string_literal);
        CHECK(!token.empty());
    }
}
