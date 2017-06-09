#pragma once
#include <swizzle/ast/MatchRule.hpp>

namespace swizzle { namespace ast { namespace matchers {

    template<class T>
    class HasChildNotOf : public MatchRule
    {
    public:
        bool evaluate(Node::smartptr node) override
        {
            for(const auto child : node->children())
            {
                const auto ptr = dynamic_cast<T*>(child.get());
                if(ptr == nullptr)
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
    class HasChildNotOf
    {
    public:
        HasChildNotOf(Matcher& matcher)
            : matcher_(matcher)
        {
        }

        template<class T>
        Matcher& hasChildNotOf()
        {
            matcher_.template append<swizzle::ast::matchers::HasChildNotOf<T>>();
            return matcher_;
        }

    private:
        Matcher& matcher_;
    };
}}}}
