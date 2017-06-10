#include <swizzle/ast/matchers/HasChild.hpp>

namespace swizzle { namespace ast { namespace matchers {

    bool HasChild::evaluate(Node::smartptr node)
    {
        return !node->empty();
    }
}}}
