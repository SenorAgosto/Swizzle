#pragma once
#include <swizzle/Exceptions.hpp>

#include <cstdint>
#include <unordered_set>
#include <type_traits>

namespace swizzle { namespace types {

    // Work flow:
    //     field_value(enum_value->assign_field_value(token));
    //     enum_value->increment()
    //
    //     OR, if the value is being set
    //
    //     enum_value->value(v);
    //     field_value(enum_value->assign_field_value(token));
    //     enum_value->increment()
    
    class EnumValueInterface
    {
    public:
        virtual ~EnumValueInterface(){}
        
        virtual void value(const std::uint64_t) = 0;
        virtual std::uint64_t value() const = 0; // for unit testing only
        virtual void increment() = 0;
        
        virtual std::uint64_t assign_field_value(const lexer::TokenInfo& token) const = 0;
    };


    template<typename T>
    class EnumValue : public EnumValueInterface
    {
    public:
        EnumValue();
        
        void value(const std::uint64_t v) override;
        std::uint64_t value() const override;
        
        void increment() override;
        std::uint64_t assign_field_value(const lexer::TokenInfo& token) const override;
        
    private:
        mutable std::unordered_set<std::uint64_t> allocated_;
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
        
        static_assert(TypeOkay, "EnumValue must be of type uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, uint64_t, int64_t");
    }
    
    template<typename T>
    void EnumValue<T>::value(const std::uint64_t v)
    {
        rolled_over_ = false;
        value_ = v;
    }

    template<typename T>
    std::uint64_t EnumValue<T>::value() const
    {
        return value_;
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
            throw SyntaxError("Enum value overflows underlying type", token);
        }

        const auto iter = allocated_.find(value_);
        if(iter != allocated_.cend())
        {
            throw SyntaxError("Enum field value previously assigned", token);
        }
        
        allocated_.insert(value_);
        return value_;
    }
}}
