#pragma once
#include <swizzle/ast/Node.hpp>
#include <string>

namespace swizzle { namespace ast {

    class VariableBindingInterface
    {
    public:
        virtual ~VariableBindingInterface(){}
        
        virtual void bind(const std::string& name, Node::smartptr node) = 0;
        virtual void bind(const std::string& name, Node& node) = 0;
    };
}}
