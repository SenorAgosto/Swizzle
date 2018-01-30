#include <swizzle/parser/Parser.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>

#include <sstream>

namespace swizzle { namespace parser {

    Parser::Parser()
        : state_(ParserState::Init)
    {
        nodeStack_.push(ast_.root());
    }

    void Parser::consume(const lexer::TokenInfo& token)
    {
        state_ = states_.consume(state_, token, nodeStack_, attributeStack_, tokenStack_, context_);
    }

    void Parser::finalize() const
    {
        if((state_ != ParserState::Init) && (state_ != ParserState::TranslationUnitMain))
        {
            std::stringstream ss;
            ss << "Parse ended in unexpected state: " << state_;

            throw ParserError(ss.str());
        }        
    }
    
    void Parser::reset()
    {
        state_ = ParserState::Init;
    }

	const ast::AbstractSyntaxTree& Parser::ast() const
	{
		return ast_;
	}
	    
    ast::AbstractSyntaxTree& Parser::ast()
    {
        return ast_;
    }
    
    const ParserStateContext& Parser::context() const
    {
        return context_;
    }
}}
