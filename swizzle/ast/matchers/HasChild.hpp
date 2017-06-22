#pragma once
#include <swizzle/ast/MatchRule.hpp>

namespace swizzle { namespace ast { namespace matchers {

    class HasChild : public MatchRule
    {
    public:
        bool evaluate(Node::smartptr node) override;
    };
}}}

namespace swizzle { namespace ast { namespace matchers { namespace fluent {

    template<class Matcher>
    class HasChild
    {
    public:
        HasChild(Matcher& matcher)
            : matcher_(matcher)
        {
        }

        Matcher& hasChild()
        {
            matcher_.template append<swizzle::ast::matchers::HasChild>();
            return matcher_;
        }

    private:
        Matcher& matcher_;
    };
}}}}
