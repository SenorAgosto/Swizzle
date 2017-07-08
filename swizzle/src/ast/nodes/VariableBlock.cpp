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

    void VariableBlock::accept(VisitorInterface& visitor)
    {
        visitor(*this);

        for(auto& child : children())
        {
            child->accept(visitor);
        }
    }
}}}
