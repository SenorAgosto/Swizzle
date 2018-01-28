#include <swizzle/types/utils/AttachAttributes.hpp>

namespace swizzle { namespace types { namespace utils {

    void attachAttributes(NodeStack& attributeStack, ast::Node::smartptr node)
    {
        while(!attributeStack.empty())
        {
            node->append(attributeStack.top());
            attributeStack.pop();
        }
    }
}}}
