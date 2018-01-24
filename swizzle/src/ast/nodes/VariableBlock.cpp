#include <swizzle/ast/nodes/VariableBlock.hpp>

#include <swizzle/ast/AncestorInfo.hpp>
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

    void VariableBlock::accept(VisitorInterface& visitor, AncestorInfo& ancestors, const Node::Depth depth)
    {
        visitor(ancestors, *this);
        
        if(depth == Depth::One) return;
        ancestors.push(*this);
        
        for(auto& child : children())
        {
            child->accept(visitor, ancestors, depth);
        }
        
        ancestors.pop();
    }
}}}
