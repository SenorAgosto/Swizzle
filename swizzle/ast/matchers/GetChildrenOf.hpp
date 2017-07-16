#pragma once
#include <swizzle/ast/MatchRule.hpp>

#include <boost/lexical_cast.hpp>
#include <cstddef>
#include <numeric>

namespace swizzle { namespace ast { namespace matchers {

    // capture the nodes of all children matching these types,
    // save them in "field_<x>"
    template<class... T>
    class GetChildrenOf : public MatchRule
    {
    public:
        bool evaluate(VariableBindingInterface& binder, Node::smartptr node) override
        {
            bool returnValue = false;

            for(const auto child : node->children())
            {
                static constexpr std::size_t size = sizeof...(T);
                void* results[size] = { (dynamic_cast<T*>(child.get()))... };

                bool hasMatch = false;
                for(std::size_t i = 0; i < size; ++i)
                {
                    if(results[i])
                    {
                        hasMatch = true;
                        returnValue = true;

                        break;
                    }
                }

                if(hasMatch)
                {
                    binder.bind(bindName_ + "_" + boost::lexical_cast<std::string>(matchCount_++), child);
                }
            }

            return returnValue;
        }

    private:
        std::size_t matchCount_ = 0;
    };
}}}

namespace swizzle { namespace ast { namespace matchers { namespace fluent {

    template<class Matcher>
    class GetChildrenOf
    {
    public:
        GetChildrenOf(Matcher& matcher)
            : matcher_(matcher)
        {
        }

        template<class... T>
        Matcher& getChildrenOf()
        {
            matcher_.template append<swizzle::ast::matchers::GetChildrenOf<T...>>();
            return matcher_;
        }

    private:
        Matcher& matcher_;
    };
}}}}
