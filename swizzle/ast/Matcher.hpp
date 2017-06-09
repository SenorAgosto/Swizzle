#pragma once
#include <swizzle/ast/AbstractSyntaxTree.hpp>
#include <swizzle/ast/MatchRule.hpp>

#include <swizzle/ast/matchers/HasChild.hpp>

#include <deque>
#include <memory>

namespace swizzle { namespace ast {

    template<template<class> class... MatchRules>
    class MatcherImpl : public MatchRules<MatcherImpl<MatchRules>>...
    {
    public:
        MatcherImpl()
            : MatchRules<MatcherImpl>(*this)...
        {
        }

        // evaluate AST
        bool operator()(AbstractSyntaxTree& ast)
        {
            return MatcherImpl::operator()(ast.root());
        }

        // evaluate subtree
        bool operator()(Node::smartptr node)
        {
            for(auto& rule : rules_)
            {
                if(!rule->evaluate(node))
                {
                    return false;
                }
            }

            return true;
        }

        // append a matche rule
        template<class MatchRule, class... Args>
        void append(Args&&... args)
        {
            rules_.emplace_back(std::make_shared<MatchRule>(std::forward<Args>(args)...));
        }

    private:
        std::deque<std::shared_ptr<MatchRule>> rules_;
    };

    using Matcher = MatcherImpl<matchers::fluent::HasChild>;
}}
