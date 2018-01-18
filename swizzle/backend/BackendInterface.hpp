#pragma once

namespace swizzle { namespace ast {
    class AbstractSyntaxTree;
}}

namespace swizzle { namespace parser {
    struct ParserStateContext;
}}

namespace swizzle { namespace backend {

    // the interface a backend must implement
    class BackendInterface
    {
    public:
        virtual ~BackendInterface() = 0 {}
        virtual void generate(const parser::ParsingStateContext& context, const ast::AbstractSyntaxTree& ast) = 0;
    };
}}
