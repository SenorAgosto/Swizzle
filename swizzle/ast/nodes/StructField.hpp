#pragma once 
#include <swizzle/ast/Node.hpp>
#include <swizzle/lexer/TokenInfo.hpp>

#include <string>

namespace swizzle { namespace ast {
    class VisitorInterface;
}}

namespace swizzle { namespace ast { namespace nodes {

    class StructField : public Node
    {
    public:
        StructField();

        void name(const lexer::TokenInfo& name);
        const lexer::TokenInfo& name() const;

        void type(const std::string& type);
        std::string type() const;

        void setConst();
        bool isConst() const;

        void makeArray(const lexer::TokenInfo& token);
        bool isArray() const;

        // @sizeMember must already be a member of the structure
        void makeVector(const std::string& sizeMember);
        bool isVector() const;

        void accept(VisitorInterface& visitor) override;

    private:
        lexer::TokenInfo name_;
        std::string type_;
        std::string vectorOnField_;
        std::size_t arraySize_;
        bool isConst_;
    };
}}}
