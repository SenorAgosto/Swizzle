#include "./Print.hpp"
#include "./PrintVisitor.hpp"

#include <swizzle/ast/AbstractSyntaxTree.hpp>
#include <swizzle/parser/ParserStateContext.hpp>

#include <PluginFactory/MakePluginMethods.hpp>

namespace swizzle { namespace plugins { namespace print {

    void PrintPlugin::generate(const parser::ParserStateContext&, ast::AbstractSyntaxTree& ast)
    {
        PrintVisitor visitor;
        ast.accept(visitor);
    }
}}}

MAKE_PLUGIN_METHODS(swizzle::backend::BackendInterface, swizzle::plugins::print::PrintPlugin)
