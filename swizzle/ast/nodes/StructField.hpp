#pragma once 
#include <swizzle/ast/Node.hpp>
#include <swizzle/lexer/TokenInfo.hpp>

#include <cstddef>
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
        std::ptrdiff_t arraySize() const;

        // @sizeMember must already be a member of the structure
        void makeVector(const lexer::TokenInfo& sizeMember);
        bool isVector() const;
        const lexer::TokenInfo& vectorSizeMember() const;

        void accept(VisitorInterface& visitor, Node& parent, const Node::Depth depth = Node::Depth::All) override;

    private:
        lexer::TokenInfo name_;
        std::string type_;

        lexer::TokenInfo vectorOnField_;
        std::ptrdiff_t arraySize_;   // this has to be signed so we can detect and report errant negative sizes
        bool isConst_;
        bool isVector_;
    };
}}}
