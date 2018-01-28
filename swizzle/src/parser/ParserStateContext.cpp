#include <swizzle/parser/ParserStateContext.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>

#include <cstddef>
#include <cstdint>

namespace swizzle { namespace parser {

    void ParserStateContext::AllocateFieldLabel(const lexer::TokenInfo& token)
    {
        const auto label = token.token().value().to_string();
        
        const auto iter = UsedFieldLabels.find(label);
        if(iter != UsedFieldLabels.cend())
        {
            throw SyntaxError("Duplicate field label value assigned.", token);
        }
        
        UsedFieldLabels.insert(label);
    }
    
    void ParserStateContext::ClearFieldLabels()
    {
        UsedFieldLabels.clear();
    }

}}
