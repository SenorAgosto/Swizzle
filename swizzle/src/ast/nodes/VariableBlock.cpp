#include <swizzle/ast/nodes/VariableBlock.hpp>
#include <swizzle/ast/VisitorInterface.hpp>

namespace swizzle { namespace ast { namespace nodes {

    VariableBlock::VariableBlock(const lexer::TokenInfo& variableBlockInfo)
        : variableBlockInfo_(variableBlockInfo)
    {
    }

    const lexer::TokenInfo& VariableBlock::variableBlockInfo() const
    {
        return variableBlockInfo_;
    }

    void VariableBlock::variableOnField(const lexer::TokenInfo& variableOnField)
    {
        variableOnFieldInfo_ = variableOnField;
    }

    const lexer::TokenInfo& VariableBlock::variableOnField() const
    {
        return variableOnFieldInfo_;
    }

    void VariableBlock::accept(VisitorInterface& visitor, Node& parent, const Node::Depth depth)
    {
        visitor(parent, *this);
        if(depth == Depth::One) return;
        
        for(auto& child : children())
        {
            auto parent = this;
            child->accept(visitor, *parent, depth);
        }
    }
}}}
