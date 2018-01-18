#pragma once
#include <swizzle/backend/BackendInterface.hpp>

namespace swizzle { namespace plugins { namespace print {

    class PrintPlugin : public backend::BackendInterface
    {
    public:
        PrintPlugin(void* /* serviceInterface */) {}
        void generate(const parser::ParserStateContext& context, ast::AbstractSyntaxTree& ast) override;
    };
}}}
