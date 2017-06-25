#pragma once
#include <swizzle/ast/AbstractSyntaxTree.hpp>
#include <swizzle/ast/MatchRule.hpp>
#include <swizzle/ast/Node.hpp>
#include <swizzle/ast/VariableBindingInterface.hpp>

#include <swizzle/ast/matchers/HasChild.hpp>
#include <swizzle/ast/matchers/HasChildOf.hpp>
#include <swizzle/ast/matchers/HasChildNotOf.hpp>
#include <swizzle/ast/matchers/HasFieldNamed.hpp>
#include <swizzle/ast/matchers/IsTypeOf.hpp>
#include <swizzle/ast/matchers/IsNotTypeOf.hpp>

#include <deque>
#include <memory>
#include <string>
#include <unordered_map>

namespace swizzle { namespace ast {

    // NOTED: matcher uses a fluent interface, each of the
    // supplied MatchRules here adds a method (or methods) to the Matcher
    // interface.
    //
    // e.g. auto m = Matche().hasChildOf<swizzle::ast::nodes::Comment>();
    //
    // The methods will typically be the lowercase of the fluent type name,
    // they may or may not take arguments.
    template<template<class> class... MatchRules>
    class MatcherImpl
        : public MatchRules<MatcherImpl<MatchRules...>>...
        , public VariableBindingInterface
    {
    public:
        MatcherImpl()
            : MatchRules<MatcherImpl>(*this)...
        {
        }

        MatcherImpl(const MatcherImpl& other)
            : MatchRules<MatcherImpl>(*this)...
            , rules_(other.rules_)
            , variables_(other.variables_)
        {
        }

        MatcherImpl(MatcherImpl&& other)
            : MatchRules<MatcherImpl>(*this)...
            , rules_(std::move(other.rules_))
            , variables_(std::move(other.variables_))
        {
        }

        MatcherImpl& operator=(const MatcherImpl& other)
        {
            rules_ = other.rules_;
            variables_ = other.variables_;

            return *this;
        }

        MatcherImpl& operator=(MatcherImpl&& other)
        {
            rules_ = std::move(other.rules_);
            variables_ = std::move(other.variables_);
            
            return *this;
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
                if(!rule->evaluate(*this, node))
                {
                    return false;
                }
            }

            return true;
        }

        // append a match rule
        template<class MatchRule, class... Args>
        void append(Args&&... args)
        {
            rules_.emplace_back(std::make_shared<MatchRule>(std::forward<Args>(args)...));
        }

        // if a match occurs save the node in @variables_ as @name
        MatcherImpl& bind(const std::string& name)
        {
            auto& rule = rules_.back();
            rule->bind_variable(name);

            return *this;
        }

        // bind @name to @node
        void bind(const std::string& name, Node::smartptr node) override
        {
            variables_[name] = node;
        }

        // return a node bound to @name, otherwise return null
        Node::smartptr bound(const std::string& name)
        {
            const auto iter = variables_.find(name);
            if(iter == variables_.cend())
            {
                return nullptr;
            }

            return iter->second;
        }

    private:
        std::deque<std::shared_ptr<MatchRule>> rules_;
        std::unordered_map<std::string, ast::Node::smartptr> variables_;
    };


    template<template<class> class... UserMatchRules>
    using ExtensibleMatcher = MatcherImpl<
          matchers::fluent::HasChild
        , matchers::fluent::HasChildOf
        , matchers::fluent::HasChildNotOf
        , matchers::fluent::HasFieldNamed
        , matchers::fluent::IsTypeOf
        , matchers::fluent::IsNotTypeOf
        , UserMatchRules...>;

    using Matcher = ExtensibleMatcher<>;
}}
