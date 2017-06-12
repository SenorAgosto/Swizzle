#pragma once
#include <swizzle/ast/MatchRule.hpp>
#include <swizzle/ast/MatcherReferenceHolder.hpp>

#include <string>

namespace swizzle { namespace ast { namespace matchers {

    class HasFieldNamed : public MatchRule
    {
    public:
        HasFieldNamed(const std::string& name);

        bool evaluate(Node::smartptr node) override;

    private:
        std::string name_;
    };
}}}

namespace swizzle { namespace ast { namespace matchers { namespace fluent {

    template<class Matcher>
    class HasFieldNamed : private MatcherReferenceHolder<Matcher>
    {
    public:
        HasFieldNamed(Matcher& matcher)
            : MatcherReferenceHolder<Matcher>(matcher)
        {
        }

        Matcher& hasFieldNamed(const std::string& name)
        {
            this->matcher().template append<swizzle::ast::matchers::HasFieldNamed>(name);
            return this->matcher();
        }
    };
}}}}
