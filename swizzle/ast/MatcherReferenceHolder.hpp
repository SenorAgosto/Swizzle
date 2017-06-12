#pragma once 

namespace swizzle { namespace ast {

    // Introducing this class so each matcher doesn't need
    // it's own reference to the Matcher class, which would
    // waste 8 bytes per matcher on 64bit platforms
    template<class Matcher>
    class MatcherReferenceHolder
    {
    public:
        MatcherReferenceHolder(Matcher& matcher)
        {
            matcher_ = &matcher;
        }

        Matcher& matcher()
        {
            return *matcher_;
        }

        const Matcher& matcher() const
        {
            return *matcher_;
        }

    private:
        static Matcher* matcher_;
    };

    // static
    template<class Matcher>
    Matcher* MatcherReferenceHolder<Matcher>::matcher_ = nullptr;
}}
