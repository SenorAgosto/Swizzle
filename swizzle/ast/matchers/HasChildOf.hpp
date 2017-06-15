#pragma once
#include <swizzle/ast/MatchRule.hpp>
#include <swizzle/ast/MatcherReferenceHolder.hpp>

#include <cstddef>
#include <numeric>

namespace swizzle { namespace ast { namespace matchers {

    template<class... T>
    class HasChildOf : public MatchRule
    {
    public:
        bool evaluate(Node::smartptr node) override
        {
            for(const auto child : node->children())
            {
                static constexpr std::size_t size = sizeof...(T);
                void* results[size] = { (dynamic_cast<T*>(child.get()))... };

                for(std::size_t i = 0; i < size; ++i)
                {
                    if(results[i])
                    {
                        return true;
                    }
                }
            }

            return false;
        }
    };
}}}

namespace swizzle { namespace ast { namespace matchers { namespace fluent {

    template<class Matcher>
    class HasChildOf : private MatcherReferenceHolder<Matcher>
    {
    public:
        HasChildOf(Matcher& matcher)
            : MatcherReferenceHolder<Matcher>(matcher)
        {
        }

        template<class... T>
        Matcher& hasChildOf()
        {
            this->matcher().template append<swizzle::ast::matchers::HasChildOf<T...>>();
            return this->matcher();
        }
    };
}}}}
