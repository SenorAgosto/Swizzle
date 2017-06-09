#pragma once
#include <swizzle/ast/MatchRule.hpp>

namespace swizzle { namespace ast { namespace matchers {

    template<class T>
    class HasChildOf : public MatchRule
    {
    public:
        bool evaluate(Node::smartptr node) override
        {
            for(const auto child : node->children())
            {
                if(dynamic_cast<T*>(child.get()))
                {
                    return true;
                }
            }

            return false;
        }
    };
}}}

namespace swizzle { namespace ast { namespace matchers { namespace fluent {

    template<class Matcher>
    class HasChildOf
    {
    public:
        HasChildOf(Matcher& matcher)
            : matcher_(matcher)
        {
        }

        template<class T>
        Matcher& hasChildOf()
        {
            matcher_.template append<swizzle::ast::matchers::HasChildOf<T>>();
            return matcher_;
        }

    private:
        Matcher& matcher_;
    };
}}}}
