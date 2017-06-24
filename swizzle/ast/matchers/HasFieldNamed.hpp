#pragma once
#include <swizzle/ast/MatchRule.hpp>

#include <string>

namespace swizzle { namespace ast { namespace matchers {

    class HasFieldNamed : public MatchRule
    {
    public:
        HasFieldNamed(const std::string& name);

        bool evaluate(VariableBindingInterface& binder, Node::smartptr node) override;

    private:
        std::string name_;
    };
}}}

namespace swizzle { namespace ast { namespace matchers { namespace fluent {

    template<class Matcher>
    class HasFieldNamed
    {
    public:
        HasFieldNamed(Matcher& matcher)
            : matcher_(matcher)
        {
        }

        Matcher& hasFieldNamed(const std::string& name)
        {
            matcher_.template append<swizzle::ast::matchers::HasFieldNamed>(name);
            return matcher_;
        }

    private:
        Matcher& matcher_;
    };
}}}}
