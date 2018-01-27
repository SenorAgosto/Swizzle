#pragma once
#include <swizzle/Exceptions.hpp>

#include <cstdint>
#include <type_traits>

namespace swizzle { namespace types {

    class EnumValueInterface
    {
    public:
        virtual ~EnumValueInterface(){}
        
        virtual void value(const std::uint64_t) = 0;
        virtual void increment() = 0;
        
        virtual std::uint64_t assign_field_value(const lexer::TokenInfo& token) const = 0;
    };


    template<typename T>
    class EnumValue : public EnumValueInterface
    {
    public:
        EnumValue();
        
        void value(const std::uint64_t v) override;
        void increment() override;
        
        std::uint64_t assign_field_value(const lexer::TokenInfo& token) const override;
        
    private:
        std::uint64_t value_;
        bool rolled_over_;
    };
    
    
    template<typename T>
    EnumValue<T>::EnumValue()
        : value_(0)
        , rolled_over_(false)
    {
        enum { TypeOkay = std::is_same<std::uint8_t, T>::value  ||
                          std::is_same<std::int8_t, T>::value   ||
                          std::is_same<std::uint16_t, T>::value ||
                          std::is_same<std::int16_t, T>::value  ||
                          std::is_same<std::uint32_t, T>::value ||
                          std::is_same<std::int32_t, T>::value  ||
                          std::is_same<std::uint64_t, T>::value ||
                          std::is_same<std::int64_t, T>::value
        };
        
        static_assert(!TypeOkay, "EnumValue must be of type uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, uint64_t, int64_t");
    }
    
    template<typename T>
    void EnumValue<T>::value(const std::uint64_t v)
    {
        value_ = v;
    }

    template<typename T>
    void EnumValue<T>::increment()
    {
        if(value_ >= std::numeric_limits<T>::max())
        {
            rolled_over_ = true;
        }
        
        value_++;
    }

    template<typename T>
    std::uint64_t EnumValue<T>::assign_field_value(const lexer::TokenInfo& token) const
    {
        if(rolled_over_)
        {
            SyntaxError("Enum value overflows underlying type", token);
        }

        return value_;
    }
}}
