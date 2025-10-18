#include <iter.hpp>

namespace coll {
    #include <vector>
    #include <set>
    #include <functional>
    #include <memory>
    
    template <typename CollectionT>
    class Collection : public CollectionT {
    public:
        using CollectionT::CollectionT;

        iter::Iter<iter::CollectionIter<CollectionT>> iter() {
            return iter::Iter<iter::CollectionIter<CollectionT>>(*this);
        }
    };

    template <class _Ty, class _Alloc = ::std::allocator<_Ty>>
    using Vec = Collection<std::vector<_Ty, _Alloc>>;

    template <class _Kty, class _Pr = ::std::less<_Kty>, class _Alloc = ::std::allocator<_Kty>>
    using Set = std::set<_Kty, _pr, _Alloc>;
};