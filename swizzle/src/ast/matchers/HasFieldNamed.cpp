#include <swizzle/ast/matchers/HasFieldNamed.hpp>

#include <swizzle/ast/DefaultVisitor.hpp>
#include <swizzle/ast/nodes/BitfieldField.hpp>
#include <swizzle/ast/nodes/StructField.hpp>

namespace swizzle { namespace ast { namespace matchers {

    namespace {
        class FieldVisitor : public DefaultVisitor
        {
        public:
            FieldVisitor(const std::string& fieldName)
                : fieldName_(fieldName)
                , found_(false)
            {
            }

            void operator()(AncestorInfo&, nodes::BitfieldField& node) override
            {
                found_ = node.name().token().value() == fieldName_;
            }

            void operator()(AncestorInfo&, nodes::StructField& node) override
            {
                found_ = node.name().token().value() == fieldName_;
            }

            bool found() const
            {
                return found_;
            }

        private:
            std::string fieldName_;
            bool found_;
        };
    }

    HasFieldNamed::HasFieldNamed(const std::string& name)
        : name_(name)
    {
    }

    bool HasFieldNamed::evaluate(VariableBindingInterface& binder, Node::smartptr node)
    {
        FieldVisitor v(name_);
        ancestors_.push(*node);
        
        for(const auto child : node->children())
        {
            child->accept(v, ancestors_);

            if(v.found())
            {
                binder.bind(bindName_, child);
                return true;
            }
        }

        return v.found();
    }
}}}

