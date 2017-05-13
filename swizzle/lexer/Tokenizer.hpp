#pragma once 

#include <swizzle/lexer/FileInfo.hpp>
#include <swizzle/lexer/Token.hpp>
#include <swizzle/lexer/TokenProducer.hpp>
#include <swizzle/lexer/TokenizerState.hpp>
#include <swizzle/lexer/TokenizerStatesPack.hpp>

#include <boost/utility/string_view.hpp>
#include <cstddef>
#include <string>

namespace swizzle { namespace lexer {

    template<class CreateTokenCallback>
    class Tokenizer : private TokenProducer<CreateTokenCallback>
    {
    public:
        Tokenizer(const std::string& filename, CreateTokenCallback callback)
            : TokenProducer<CreateTokenCallback>(callback)
            , filename_(filename)
            , states_(callback)
            , fileInfo_(filename)
            , state_(TokenizerState::Init)
        {
        }

        void consume(const boost::string_view& source, const std::size_t position)
        {
            state_ = states_.consume(state_, source, position, fileInfo_, token_);
        }

        void flush()
        {
            this->produceToken(token_, fileInfo_);
        }

    private:
        std::string filename_;
        TokenizerStatesPack<CreateTokenCallback> states_;

        FileInfo fileInfo_;
        Token token_;
        TokenizerState state_;
    };
}}
