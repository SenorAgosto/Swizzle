#pragma once
#include <swizzle/ast/MatchRule.hpp>
#include <swizzle/ast/MatcherReferenceHolder.hpp>

namespace swizzle { namespace ast { namespace matchers {

    class HasChild : public MatchRule
    {
    public:
        bool evaluate(Node::smartptr node) override;
    };
}}}

namespace swizzle { namespace ast { namespace matchers { namespace fluent {

    template<class Matcher>
    class HasChild : protected MatcherReferenceHolder<Matcher>
    {
    public:
        HasChild(Matcher& matcher)
            : MatcherReferenceHolder<Matcher>(matcher)
        {
        }

        Matcher& hasChild()
        {
            this->matcher().template append<swizzle::ast::matchers::HasChild>();
            return this->matcher();
        }
    };
}}}}
