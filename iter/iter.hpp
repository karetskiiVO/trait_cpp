#include <functional>
#include <optional>
#include <utility>
#include <iostream>
#include <vector>
 
namespace iter {
    template <typename BaseT, typename DataT>
    struct IterTrait {
        static std::optional<DataT> next(BaseT& base);
    };

    template <typename BaseT, typename DataT>
    std::optional<DataT> IterTrait<BaseT, DataT>::next(BaseT& base) {
        throw std::runtime_error("aboba");
    }
 
    template<typename IterT, typename DataT> 
    struct Filter {
        Filter(IterT baseiter, std::function<bool(DataT)> predicat) : baseiter(baseiter), predicat(predicat) {}
 
        Filter() = default;
    protected:
        IterT baseiter;
        std::function<bool(DataT)> predicat;
 
        friend struct IterTrait<Filter<IterT, DataT>, DataT>;
    };
 
    template <typename IterT, typename DataT>
    struct IterTrait<Filter<IterT, DataT>, DataT> {
        static std::optional<DataT> next(Filter<IterT, DataT>& filter) {
            while (true) {
                std::optional<DataT> data = filter.baseiter.next();
                
                if (!data) {
                    return std::nullopt;
                }
                if (filter.predicat(*data)) {
                    return data;
                }
            }
        }
    };

    template <typename IterT, typename FromT, typename ToT>
    struct Map {
        Map(IterT baseiter, std::function<ToT(FromT)> mapper) : baseiter(baseiter), mapper(mapper) {}
 
        Map() = default;
    private:
        IterT baseiter;
        std::function<ToT(FromT)> mapper;
 
        friend struct IterTrait<Map<IterT, FromT, ToT>, ToT>;
    };
 
    template <typename IterT, typename FromT, typename ToT>
    struct IterTrait<Map<IterT, FromT, ToT>, ToT> {
        static std::optional<ToT> next(Map<IterT, FromT, ToT>& map) {
            std::optional<FromT> data = map.baseiter.next();
            if (!data) {
                return std::nullopt;
            }
            return map.mapper(*data);
        }
    };
 
    template <typename BaseT, typename DataT>
    class Iter  {
        using Self = Iter<BaseT, DataT>;
 
        BaseT iter;
    
        Iter() = default;
    public:
        Iter (BaseT base) :  iter(base) {}
 
        Iter<Filter<Self, DataT>, DataT> filter(std::function<bool(DataT)> predicat) {
            return Iter<Filter<Self, DataT>, DataT>(
                Filter<Self, DataT>(*this, predicat)
            );
        }
 
        std::optional<DataT> next() {
            return IterTrait<BaseT, DataT>::next(iter);
        }
 
        template <typename ToT>
        Iter<Map<Self, DataT, ToT>, ToT> map(std::function<ToT(DataT)> mapper) {
            return Iter<Map<Self, DataT, ToT>, ToT>(
                Map<Self, DataT, ToT>(*this, mapper)
            );
        }
    
        // map_while

        // template <typename Collection = std::vector<BaseT>>
        // Collection take(size_t n) {
        //     auto res = Collection();

        //     for (size_t i = 0; i < n; i++) {
        //         auto v = next();

        //         if (!val) break;

        //         res.insert(*val);
        //     }

        //     return res;
        // } 

        // take while

        // template <typename Collection = std::vector<BaseT>>
        // Collection collect() {
        //     auto res = Collection();

        //     while (true) {
        //         auto v = next();

        //         if (!val) break;

        //         res.insert(*val);
        //     }

        //     return res;
        // } 

        // collect into

        void for_each(std::function<void(DataT)> func) {
            while (true) {
                auto val = next();
                if (!val) return;

                func(*val);
            }
        }
    
        // chain/join iters

        // zip, convert to pairs

        // skip

        // skip_while/skip_until

        // filter_map

        // flat_map

        // enumerate

        // fold - типа просуммировать

        // reduce то же самое но без стартового значения

        // all

        // any

        // unzip
    };
 
    template <typename ForwardIter> 
    struct PairIters {
        PairIters(ForwardIter begin, ForwardIter end) : begin(begin), end(end) {}

    private:
        ForwardIter begin;
        ForwardIter end;
 
        friend struct IterTrait<PairIters<ForwardIter>, decltype(*ForwardIter())>;
    };
 
    template <typename ForwardIter>
    struct IterTrait<PairIters<ForwardIter>, decltype(*ForwardIter())> {
        static std::optional<decltype(*ForwardIter())> next(PairIters<ForwardIter>& pair) {
            if (pair.begin == pair.end) {
                return std::nullopt;
            }

            ForwardIter begin = pair.begin;
            pair.begin++;
            return *begin;
        }
    };
 
    template <typename Collection>
    struct CollectionIter : public PairIters<decltype(Collection().begin())> {
        CollectionIter(Collection& collection) : 
            PairIters<decltype(Collection().begin())>(collection.begin(), collection.end()) {}
    };

    template <typename Collection>
    struct IterTrait<CollectionIter<Collection>, decltype(*Collection().begin())> {
        static std::optional<decltype(*Collection().begin())> next(CollectionIter<Collection>& iter) {
            if (iter.begin == iter.end) {
                return std::nullopt;
            }

            auto begin = iter.begin;
            iter.begin++;
            return *begin;
        }
    };
}
 
int main() {
    auto collection = std::vector<int>{1, 2, 3, 4, 5};
    auto collection_iter = iter::CollectionIter(collection);
    auto iter = iter::Iter<decltype(collection_iter), int>(collection_iter);
 
    iter
        // .filter([] (int x) { 
        //     std::cout << x << std::endl;
        //     return x % 2 == 0;
        // })
        //.map<int>([] (int x) { return x * 2; })
        .for_each([] (int x) { std::cout << x << std::endl; });
}