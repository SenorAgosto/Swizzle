#include <swizzle/ast/AncestorInfo.hpp>
#include <iterator>

namespace swizzle { namespace ast {

        const ast::Node& AncestorInfo::top() const
        {
            return ancestors_.back().get();
        }
    
        void AncestorInfo::push(const ast::Node& node)
        {
            ancestors_.push_back(node);
        }
    
        void AncestorInfo::pop()
        {
            if(!ancestors_.empty())
            {
                ancestors_.erase(std::prev(ancestors_.end()));
            }
        }

        AncestorInfo::const_iterator AncestorInfo::cbegin() const
        {
            return ancestors_.cbegin();
        }
    
        AncestorInfo::iterator AncestorInfo::begin()
        {
            return ancestors_.begin();
        }

        AncestorInfo::const_iterator AncestorInfo::cend() const
        {
            return ancestors_.cend();
        }
    
        AncestorInfo::iterator AncestorInfo::end()
        {
            return ancestors_.end();
        }

        AncestorInfo::const_iterator AncestorInfo::find(const ast::Node& node) const
        {
            for(const_iterator iter = ancestors_.cbegin(), end = ancestors_.cend(); iter != end; ++iter)
            {
                if(&iter->get() == &node) return iter;
            }
            
            return ancestors_.cend();
        }
    
        AncestorInfo::iterator AncestorInfo::find(const ast::Node& node)
        {
            for(iterator iter = ancestors_.begin(), end = ancestors_.end(); iter != end; ++iter)
            {
                if(&iter->get() == &node) return iter;
            }
            
            return ancestors_.end();
        }
    
        AncestorInfo::const_iterator AncestorInfo::parent_of(const ast::Node& node) const
        {
            const_iterator iter = find(node);
            return (iter == cbegin() || iter == cend()) ? iter : std::prev(iter);
        }
    
        AncestorInfo::iterator AncestorInfo::parent_of(const ast::Node& node)
        {
            iterator iter = find(node);
            return (iter == cbegin() || iter == end()) ? iter : std::prev(iter);
        }

        AncestorInfo::const_iterator AncestorInfo::grandparent_of(const ast::Node& node) const
        {
            const_iterator iter = parent_of(node);
            return (iter == cbegin() || iter == cend()) ? iter : parent_of(iter->get());
        }

        AncestorInfo::iterator AncestorInfo::grandparent_of(const ast::Node& node)
        {
            iterator iter = parent_of(node);
            return (iter == begin() || iter == end()) ? iter : parent_of(iter->get());
        }

        std::pair<std::size_t, bool> AncestorInfo::depth_of(const ast::Node& node)
        {
            const_iterator iter = find(node);
            if(iter != cend())
            {
                const auto distance = std::distance(cbegin(), iter);
                return std::make_pair(distance, true);
            }
            
            return std::make_pair(0, false);
        }
    
        std::size_t AncestorInfo::depth() const
        {
            return ancestors_.size();
        }

}}
