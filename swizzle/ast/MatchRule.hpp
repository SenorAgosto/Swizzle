#pragma once 
#include <swizzle/ast/Node.hpp>

namespace swizzle { namespace ast {

    class MatchRule
    {
    public:
        virtual ~MatchRule(){}
        virtual bool evaluate(Node::smartptr node) = 0;
    };
}}
