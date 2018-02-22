#pragma once
#include <swizzle/ast/Depth.hpp>

#include <UsingIntrusivePtrIn/UsingIntrusivePtrIn.hpp>
#include <UsingIntrusivePtrIn/details/SingleThreadedReferenceCountBase.hpp>

#include <boost/intrusive_ptr.hpp>
#include <deque>

namespace swizzle { namespace ast {
    class AncestorInfo;
    class VisitorInterface;
}}

namespace swizzle { namespace ast {
    
    class Node : public UsingIntrusivePtrIn::UsingIntrusivePtrIn<Node, UsingIntrusivePtrIn::details::SingleThreadedReferenceCountBase>
    {
    public:
        using smartptr = boost::intrusive_ptr<Node>;
        
    public:
    
        virtual ~Node(){}
        virtual void accept(VisitorInterface& visitor, AncestorInfo& ancestors, const Depth depth = Depth::All);

        const std::deque<Node::smartptr>& children() const;
        void append(Node::smartptr node);

        bool empty() const;

    private:
        std::deque<Node::smartptr> children_;
    };
}}
