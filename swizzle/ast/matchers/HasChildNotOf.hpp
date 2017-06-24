#pragma once
#include <swizzle/ast/MatchRule.hpp>

#include <cstddef>
#include <numeric>

namespace swizzle { namespace ast { namespace matchers {

    template<class... T>
    class HasChildNotOf : public MatchRule
    {
    public:
        bool evaluate(VariableBindingInterface& binder, Node::smartptr node) override
        {
            for(const auto child : node->children())
            {
                static constexpr std::size_t size = sizeof...(T);
                void* results[size] = { (dynamic_cast<T*>(child.get()))... };

                std::size_t count = 0;
                for(std::size_t i = 0; i < size; ++i)
                {
                    count += results[i] == nullptr ? 0 : 1;
                }

                // all casts failed, therefore the child node is of a different type
                if(count == 0)
                {
                    binder.bind(bindName_, node);
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

        template<class... T>
        Matcher& hasChildNotOf()
        {
            matcher_.template append<swizzle::ast::matchers::HasChildNotOf<T...>>();
            return matcher_;
        }

    private:
        Matcher& matcher_;
    };
}}}}
