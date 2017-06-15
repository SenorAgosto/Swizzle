#pragma once
#include <swizzle/ast/MatchRule.hpp>
#include <swizzle/ast/MatcherReferenceHolder.hpp>

#include <cstddef>
#include <numeric>

namespace swizzle { namespace ast { namespace matchers {

    template<class... T>
    class IsNotTypeOf : public MatchRule
    {
    public:
        bool evaluate(Node::smartptr node) override
        {
            static constexpr std::size_t size = sizeof...(T);
            void* results[size] = { (dynamic_cast<T*>(node.get()))... };

            std::size_t count = 0;
            for(std::size_t i = 0; i < size; ++i)
            {
                count += results[i] == nullptr ? 0 : 1;
            }

            // all casts failed, therefore the child node is of a different type
            if(count == 0)
            {
                return true;
            }

            return false;
        }
    };
}}}

namespace swizzle { namespace ast { namespace matchers { namespace fluent {

    template<class Matcher>
    class IsNotTypeOf : private MatcherReferenceHolder<Matcher>
    {
    public:
        IsNotTypeOf(Matcher& matcher)
            : MatcherReferenceHolder<Matcher>(matcher)
        {
        }

        template<class... T>
        Matcher& isNotTypeOf()
        {
            this->matcher().template append<swizzle::ast::matchers::IsNotTypeOf<T...>>();
            return this->matcher();
        }
    };
}}}}
