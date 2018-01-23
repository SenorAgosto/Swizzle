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
        enum class Depth { One, All };
        
    public:
    
        virtual ~Node(){}
        virtual void accept(VisitorInterface& visitor, Node& parent, const Depth depth = Depth::All);

        const std::deque<Node::smartptr>& children() const;
        void append(Node::smartptr node);

        bool empty() const;

    private:
        std::deque<Node::smartptr> children_;
    };
}}
