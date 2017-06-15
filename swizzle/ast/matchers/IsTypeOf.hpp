#pragma once
#include <swizzle/ast/MatchRule.hpp>
#include <swizzle/ast/MatcherReferenceHolder.hpp>

#include <cstddef>
#include <numeric>

namespace swizzle { namespace ast { namespace matchers {

    template<class... T>
    class IsTypeOf : public MatchRule
    {
    public:
        bool evaluate(Node::smartptr node) override
        {
            static constexpr std::size_t size = sizeof...(T);
            void* results[size] = { (dynamic_cast<T*>(node.get()))... };

            for(std::size_t i = 0; i < size; ++i)
            {
                if(results[i])
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
    class IsTypeOf : private MatcherReferenceHolder<Matcher>
    {
    public:
        IsTypeOf(Matcher& matcher)
            : MatcherReferenceHolder<Matcher>(matcher)
        {
        }

        template<class... T>
        Matcher& isTypeOf()
        {
            this->matcher().template append<swizzle::ast::matchers::IsTypeOf<T...>>();
            return this->matcher();
        }
    };
}}}}
