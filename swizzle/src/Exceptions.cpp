#include <swizzle/Exceptions.hpp>

#include <swizzle/lexer/FileInfo.hpp>
#include <swizzle/lexer/Token.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/lexer/TokenizerState.hpp>
#include <swizzle/parser/ParserState.hpp>

#include <cstdint>
#include <sstream>

namespace swizzle {

    namespace {
        std::string constructError(const std::string& s, const lexer::TokenizerState& state)
        {
            std::stringstream ss;
            ss << s << static_cast<std::int64_t>(state);

            return ss.str();
        }

        // TODO: use ncurses to print the line and put squiggles under the error
        std::string constructFileLocationString(const std::string& s, const lexer::TokenInfo& info, const std::string& error)
        {
            std::stringstream ss;
            ss  << s
                << info.fileInfo().end().line() << ":" << info.fileInfo().end().column()
                << ": " << error;

            return ss.str();
        }

        std::string constructParserStateError(const std::string& s, const parser::ParserState state)
        {
            std::stringstream ss;
            ss << s << static_cast<std::int64_t>(state);

            return ss.str();
        }
    }

    ParserError::ParserError(const std::string& reason)
        : std::runtime_error(reason)
    {
    }

    TokenizerError::TokenizerError(const std::string& reason)
        : std::runtime_error(reason)
    {
    }

    UnknownTokenizerState::UnknownTokenizerState(const lexer::TokenizerState state)
        : TokenizerError(constructError("Unknown Tokenizer State: ", state))
    {
    }

    SyntaxError::SyntaxError(const std::string& error, const lexer::TokenInfo& info)
        : ParserError(constructFileLocationString("Syntax Error: ", info, error))
    {
    }

    UnknownParserState::UnknownParserState(const parser::ParserState state)
        : ParserError(constructParserStateError("Unknown Parser State: ", state))
    {
    }
}
