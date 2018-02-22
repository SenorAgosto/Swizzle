#pragma once
#include <swizzle/ast/Node.hpp>
#include <swizzle/ast/VariableBindingInterface.hpp>

#include <string>
#include <unordered_map>

namespace swizzle { namespace ast {

    class MatchRule
    {
    public:
        virtual ~MatchRule(){}

        virtual bool evaluate(VariableBindingInterface& binder, Node::smartptr node) = 0;
        virtual void bind_variable(const std::string& name) { bindName_ = name; }
        virtual void value_of(const std::string& value) { nodeValue_ = value; }
        
    protected:
        std::string bindName_;
        std::string nodeValue_;
    };
}}
