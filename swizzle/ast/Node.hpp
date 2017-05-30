#pragma once
#include <UsingIntrusivePtrIn/UsingIntrusivePtrIn.hpp>
#include <UsingIntrusivePtrIn/details/SingleThreadedReferenceCountBase.hpp>

#include <boost/intrusive_ptr.hpp>
#include <deque>

namespace swizzle { namespace ast {
    class VisitorInterface;
}}

namespace swizzle { namespace ast {

    class Node : public UsingIntrusivePtrIn::UsingIntrusivePtrIn<Node, UsingIntrusivePtrIn::details::SingleThreadedReferenceCountBase>
    {
    public:
        using smartptr = boost::intrusive_ptr<Node>;

        virtual ~Node(){}
        virtual void accept(VisitorInterface& visitor);

        const std::deque<Node::smartptr>& children() const;
        void append(Node::smartptr node);

        bool empty() const;

    private:
        std::deque<Node::smartptr> children_;
    };
}}
