#include <swizzle/ast/matchers/HasChild.hpp>

namespace swizzle { namespace ast { namespace matchers {

    bool HasChild::evaluate(VariableBindingInterface& binder, Node::smartptr node)
    {
        if(!node->empty())
        {
            binder.bind(bindName_, node);
            return true;
        }

        return false;
    }
}}}
