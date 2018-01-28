#include <swizzle/parser/detail/ValidateVectorSizeMember.hpp>

#include <swizzle/Exceptions.hpp>

#include <swizzle/ast/Matcher.hpp>
#include <swizzle/ast/matchers/HasFieldNamed.hpp>
#include <swizzle/ast/matchers/IsTypeOf.hpp>
#include <swizzle/ast/nodes/Struct.hpp>
#include <swizzle/ast/nodes/StructField.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/types/IsIntegerType.hpp>
#include <swizzle/types/utils/StackInvert.hpp>
#include <swizzle/types/utils/StackToList.hpp>
#include <swizzle/types/TokenList.hpp>

#include "./ContainsNamespace.hpp"

namespace swizzle { namespace parser { namespace detail {

    namespace {
        void validateTokenStack(const types::TokenStack& tokenStack, const lexer::FileInfo& info)
        {
            if(tokenStack.empty())
            {
                throw SyntaxErrorWithoutToken("Token stack should contain vector size member tokens", " it empty.", info);
            }
        }

        ast::Node::smartptr validateNodeStack(const types::NodeStack& nodeStack, const lexer::FileInfo& info)
        {
            if(nodeStack.empty())
            {
                throw SyntaxErrorWithoutToken("Node stack empty, expected top of node stack to be ast::nodes::StructField", " it empty", info);
            }

            types::NodeStack nodes = nodeStack;

            auto isStructField = ast::Matcher().isTypeOf<ast::nodes::StructField>();
            if(!isStructField(nodes.top()))
            {
                throw SyntaxErrorWithoutToken("Expected top of node stack to be ast::nodes::StructField", " unexpected type", info);
            }

            const auto field = nodes.top();
            nodes.pop();

            auto isStruct = ast::Matcher().isTypeOf<ast::nodes::Struct>();
            if(!isStruct(nodes.top()))
            {
                throw SyntaxErrorWithoutToken("Expected node below top of node stack to be ast::nodes::Struct", " unexpected type", info);
            }

            auto structure = nodes.top();
            nodes.emplace(std::move(field));

            return structure;
        }
    }

    void validateVectorSizeMember(const lexer::TokenInfo& tokenInfo, const types::NodeStack& nodeStack, const types::TokenStack& tokenStack, const ParserStateContext& context)
    {
        validateTokenStack(tokenStack, tokenInfo.fileInfo());
        auto structure = validateNodeStack(nodeStack, tokenInfo.fileInfo());

        types::TokenStack stack = tokenStack;
        stack = types::utils::invert(stack);

        bool last = false;
        auto isStructField = ast::Matcher().isTypeOf<ast::nodes::StructField>();

        const types::TokenList list = types::utils::to_list(stack);
        for(const auto token : list)
        {
            if(last)
            {
                throw SyntaxError("Invalidly formatted vector size member", " intermediate member is integer type not struct", token);
            }

            auto matcher = ast::Matcher().isTypeOf<ast::nodes::Struct>().hasFieldNamed(token.token().to_string()).bind("field");
            if(matcher(structure))
            {
                auto fieldNode = matcher.bound("field");
                if(isStructField(fieldNode))
                {
                    auto field = static_cast<ast::nodes::StructField&>(*fieldNode);
                    const auto ts = field.type();
                    auto type = boost::string_view(ts);

                    if(types::IsIntegerType(type))
                    {
                        last = true;
                    }
                    else if(types::IsType(type))
                    {
                        throw SyntaxError("Vector size member is constructed from unsupported type. Type must be integeral.", " non-integral type", token);
                    }
                    else
                    {
                        const auto typeName = containsNamespace(type) ? type.to_string() : context.CurrentNamespace + "::" + type.to_string();
                        const auto iter = context.TypeCache.find(typeName);
                        if(iter == context.TypeCache.end())
                        {
                            throw SyntaxError("Vector size member invalid", " undefined type: " + typeName, tokenInfo);
                        }

                        structure = iter->second;
                    }
                }
                else
                {
                    throw SyntaxError("Vector size member invalid", " construction from incorrect type", tokenInfo);
                }
            }
            else
            {
                const auto& s = static_cast<ast::nodes::Struct&>(*structure);
                throw SyntaxError("Vector size member invalid", " references to unknown field (" + token.token().to_string() + ") in type: " + s.name(), tokenInfo);
            }
        }

        if(!last)
        {
            throw SyntaxError("Vector size member invalid, must end in integral type", " non-integer type", tokenInfo);
        }
    }
}}}
