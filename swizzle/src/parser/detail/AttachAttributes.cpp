#include <swizzle/parser/detail/AttachAttributes.hpp>

namespace swizzle { namespace parser { namespace detail {

    void attachAttributes(NodeStack& attributeStack, ast::Node::smartptr node)
    {
        while(!attributeStack.empty())
        {
            node->append(attributeStack.top());
            attributeStack.pop();
        }
    }
}}}
