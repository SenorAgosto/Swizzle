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
        std::string constructError(const std::string& s, const lexer::FileInfo& info, const std::string& error)
        {
            std::stringstream ss;
            ss  << info.filename() << ":" << info.start().line() << ":" << info.start().column() << ":"
                << s << error;

            return ss.str();
        }

        // TODO: use ncurses to print the line and put squiggles under the error
        std::string constructError(const std::string& s, const lexer::TokenInfo& info, const std::string& error)
        {
            std::stringstream ss;
            ss  << info.fileInfo().filename() << ":" << info.fileInfo().start().line() << ":" << info.fileInfo().start().column() << ": "
                << s << error
                << ", found " << info.token().type() << " (" << info.token().to_string() << ")";

            return ss.str();
        }

        std::string constructError(const std::string& s, const parser::ParserState state)
        {
            std::stringstream ss;
            ss << s << static_cast<std::int64_t>(state);

            return ss.str();
        }
    }

    AliasedTypeIsNotDefinedException::AliasedTypeIsNotDefinedException(const std::string& existingType, const lexer::FileInfo& fileInfo)
        : ParserError(constructError("Syntax Error: ", fileInfo, " attempting to alias an undefined type '" + existingType + "'"))
    {
    }

    InvalidStreamInput::InvalidStreamInput(const std::string& s)
        : std::runtime_error("Invalid character encountered in safe_istringstream: '" + s + "'")
    {
    }

    ParserError::ParserError(const std::string& reason)
        : std::runtime_error(reason)
    {
    }

    RedefinitionOfAliasNameException::RedefinitionOfAliasNameException(const std::string& aliasName, const lexer::FileInfo& fileInfo)
        : ParserError(constructError("Syntax Error: ", fileInfo, " redefinition of alias name '" + aliasName + "'"))
    {
    }

    RedefinitionOfStructTypeException::RedefinitionOfStructTypeException(const std::string& structName, const lexer::FileInfo& fileInfo)
        : ParserError(constructError("Syntax Error: ", fileInfo, " redefinition of struct name '" + structName + "'"))
    {
    }

    StreamEmpty::StreamEmpty()
        : std::runtime_error("safe_istringstream empty when insertion operator called.")
    {
    }

    StreamInputCausesOverflow::StreamInputCausesOverflow(const std::string& s)
        : std::runtime_error("Input caused value overflow in safe_istringstream: '" + s + "'")
    {
    }

    StreamInputCausesUnderflow::StreamInputCausesUnderflow(const std::string& s)
        : std::runtime_error("Input caused value underflow in safe_istringstream: '" + s + "'")
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

    TokenizerSyntaxError::TokenizerSyntaxError(const lexer::FileInfo& info, const std::string& reason)
        : TokenizerError(constructError("Syntax Error: ", info, reason))
    {
    }

    SyntaxError::SyntaxError(const std::string& error, const lexer::TokenInfo& token)
        : ParserError(constructError("Syntax Error: ", token, error))
        , token_(token)
    {
    }

    SyntaxError::SyntaxError(const std::string& error, const std::string& found, const lexer::TokenInfo& token)
        : ParserError(constructError("Syntax Error: ", token.fileInfo(), error + ", found " + found))
        , token_(token)
    {
    }

    SyntaxErrorWithoutToken::SyntaxErrorWithoutToken(const std::string& error, const std::string& found, const lexer::FileInfo& info)
        : ParserError(constructError("Syntax Error: ", info, error + ", found " + found))
        , info_(info)
    {
    }

    UnknownParserState::UnknownParserState(const parser::ParserState state)
        : ParserError(constructError("Unknown Parser State: ", state))
    {
    }
}
