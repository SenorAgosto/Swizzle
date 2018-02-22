#pragma once
#include <swizzle/ast/MatchRule.hpp>
#include <swizzle/ast/matchers/HasChildOfValueVisitor.hpp>

#include <cstddef>
#include <numeric>

namespace swizzle { namespace ast { namespace matchers {

    template<class... T>
    class HasChildOf : public MatchRule
    {
    public:
        bool evaluate(VariableBindingInterface& binder, Node::smartptr node) override
        {
            for(const auto child : node->children())
            {
                static constexpr std::size_t size = sizeof...(T);
                void* results[size] = { (dynamic_cast<T*>(child.get()))... };

                for(std::size_t i = 0; i < size; ++i)
                {
                    if(results[i])
                    {
                        if(nodeValue_.empty())
                        {
                            binder.bind(bindName_, node);
                            return true;
                        }
                        
                        AncestorInfo ancestors;
                        HasChildOfValueVisitor visitor(nodeValue_);
                        
                        static_cast<Node*>(results[i])->accept(visitor, ancestors, Depth::One);
                        
                        if(visitor.value_matches())
                        {
                            binder.bind(bindName_, node);
                            return true;
                        }
                    }
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

        template<class... T>
        Matcher& hasChildOf()
        {
            matcher_.template append<swizzle::ast::matchers::HasChildOf<T...>>();
            return matcher_;
        }

    private:
        Matcher& matcher_;
    };
}}}}
