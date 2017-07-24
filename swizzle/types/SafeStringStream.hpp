#pragma once 
#include <boost/utility/string_view.hpp>
#include <cstdint>

namespace swizzle { namespace types {

    // NOTE: this is *not* a fully functioning istringstream class.
    // it doesn't handle whitespace separation of fields & a host
    // of other normal use cases
    class safe_istringstream
    {
    public:
        safe_istringstream(const boost::string_view& input);

        safe_istringstream& operator>>(std::uint8_t& i);
        safe_istringstream& operator>>(std::int8_t& i);
        safe_istringstream& operator>>(std::uint16_t& i);
        safe_istringstream& operator>>(std::int16_t& i);
        safe_istringstream& operator>>(std::uint32_t& i);
        safe_istringstream& operator>>(std::int32_t& i);
        safe_istringstream& operator>>(std::uint64_t& i);
        safe_istringstream& operator>>(std::int64_t& i);

        void hex_stream(const bool isHex) { hex_ = isHex; }
        bool hex_stream() const { return hex_; }

    private:
        boost::string_view input_;
        bool hex_;
    };

    struct HexStreamTag {} const static hex_stream = HexStreamTag();
    safe_istringstream& operator>>(safe_istringstream& is, const HexStreamTag&);
}}
