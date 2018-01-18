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
        virtual ~BackendInterface() {}
        virtual void generate(const parser::ParserStateContext& context, ast::AbstractSyntaxTree& ast) = 0;
    };
}}
