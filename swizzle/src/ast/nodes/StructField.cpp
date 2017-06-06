#include <swizzle/ast/nodes/StructField.hpp>

#include <swizzle/ast/VisitorInterface.hpp>
#include <swizzle/Exceptions.hpp>

#include <boost/lexical_cast.hpp>

namespace swizzle { namespace ast { namespace nodes {

    StructField::StructField()
        : arraySize_(0)
        , isConst_(false)
    {
    }

    void StructField::name(const lexer::TokenInfo& name)
    {
        name_ = name;
    }

    const lexer::TokenInfo& StructField::name() const
    {
        return name_;
    }

    void StructField::type(const std::string& type)
    {
        type_ = type;
    }

    std::string StructField::type() const
    {
        return type_;
    }

    void StructField::setConst()
    {
        isConst_ = true;
    }

    bool StructField::isConst() const
    {
        return isConst_;
    }

    void StructField::makeArray(const lexer::TokenInfo& token)
    {
        try
        {
            arraySize_ = boost::lexical_cast<std::ptrdiff_t>(token.token().value());

            if(arraySize_ < 0)
            {
                throw SyntaxError("Expected positive numeric array size", token);
            }
        }
        catch(const boost::bad_lexical_cast&)
        {
            throw SyntaxError("Expected positive numeric array size", token);
        }
    }

    bool StructField::isArray() const
    {
        return arraySize_ != 0;
    }

    void StructField::makeVector(const std::string& sizeMember)
    {
        if(isArray())
        {
            throw ParserError("Internal parser error, struct field cannot be both array and vector.");
        }

        vectorOnField_ = sizeMember;
    }

    bool StructField::isVector() const
    {
        return !vectorOnField_.empty();
    }

    void StructField::accept(VisitorInterface& visitor)
    {
        visitor(*this);

        for(auto child : children())
        {
            child->accept(visitor);
        }
    }
}}}
