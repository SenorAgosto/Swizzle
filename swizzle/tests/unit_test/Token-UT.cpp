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
        const Token token("value", TokenType::comment);
        
        CHECK_EQUAL("value", token.to_string());
        CHECK_EQUAL(TokenType::comment, token.type());
    }
    
    TEST(verifySettingType)
    {
        Token token("value", TokenType::comment);
        
        CHECK_EQUAL("value", token.to_string());
        CHECK_EQUAL(TokenType::comment, token.type());

        token.type(TokenType::multiline_comment);
        CHECK_EQUAL(TokenType::multiline_comment, token.type());
    }

    TEST(verifyExpand)
    {
        const std::string s = "This is a string";
        const boost::string_view sv(s);

        Token token(boost::string_view(s.c_str(), 2), TokenType::string);
        token.expand(sv, 2);

        CHECK_EQUAL("This", token.value());
        CHECK_EQUAL("This", token.to_string());
    }

    TEST(verifyExpandTruncatesOnOverrun)
    {
        const std::string s = "This is a string";
        const boost::string_view sv(s);

        Token token(boost::string_view(s.c_str(), 2), TokenType::string);
        token.expand(sv, 100);

        CHECK_EQUAL(s, token.to_string());
    }

    TEST(verifyEmpty)
    {
        const std::string s = " \t\r \na";
        const boost::string_view sv(s);

        Token token(boost::string_view(s.c_str(), 0), TokenType::string);
        CHECK(token.empty());

        token.expand(sv);   // ' '
        CHECK(token.empty());

        token.expand(sv);   // \t
        CHECK(token.empty());

        token.expand(sv);   // \r
        CHECK(token.empty());

        token.expand(sv);   // ' '
        CHECK(token.empty());

        token.expand(sv);   // \n
        CHECK(token.empty());

        token.expand(sv);   // a
        CHECK(!token.empty());

        const Token token2(boost::string_view(s.c_str(), 0), TokenType::string_literal);
        CHECK(!token.empty());
    }

    TEST(verifyClear)
    {
        Token token("value", TokenType::comment);

        CHECK_EQUAL("value", token.to_string());
        CHECK_EQUAL(TokenType::comment, token.type());

        token.clear();
        
        CHECK_EQUAL("", token.to_string());
        CHECK_EQUAL(TokenType::string, token.type());
        CHECK(token.empty());
    }
}
