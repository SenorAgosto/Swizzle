#pragma once 

namespace swizzle { namespace ast {

    class AbstractTreeVisitorInterface
    {
    public:
        virtual ~AbstractTreeVisitorInterface(){}
        virtual void operator()(AbstractSyntaxTree& tree) = 0;
    };
}}
