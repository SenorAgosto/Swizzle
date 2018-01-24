#pragma once
#include <swizzle/ast/Node.hpp>

#include <cstddef>
#include <functional>
#include <utility>
#include <vector>

namespace swizzle { namespace ast {

    // A stack-line structure used to store and retrieve information about ancestor nodes in the AST
    class AncestorInfo
    {
    public:
        using AncestorContainer = std::vector<std::reference_wrapper<const Node>>;
        
        using iterator = AncestorContainer::iterator;
        using const_iterator = AncestorContainer::const_iterator;
        
    public:
        const ast::Node& top() const;
        
        void push(const ast::Node& node);
        void pop();
        
        const_iterator cbegin() const;
        iterator begin();

        const_iterator cend() const;
        iterator end();

        const_iterator find(const ast::Node& node) const;
        iterator find(const ast::Node& node);
        
        // return parent of @node or end(), root is its own parent
        const_iterator parent_of(const ast::Node& node) const;
        iterator parent_of(const ast::Node& node);
        
        // return @node's parent's parent or end()
        const_iterator grandparent_of(const ast::Node& node) const;
        iterator grandparent_of(const ast::Node& node);
        
        // return a pair (depth, found)
        // depth is the depth in AST, root node is depth 0.
        // found is true if @node was found, false otherwise
        std::pair<std::size_t, bool> depth_of(const ast::Node& node);
        
        std::size_t depth() const;   // return depth of the ancestor info
        
    private:
        AncestorContainer ancestors_;
    };
}}
