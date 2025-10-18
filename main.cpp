#include "iter/iter.hpp"
#include "collection/collection.hpp"
#include <iostream>

int main() {
    auto vector = coll::Vec<int>{3, 2, 1, 0};
    auto set = coll::Set<int>{6, 5, 4};

    vector.iter()
        .chain(set.iter())
        .each([](int x) { std::cout << x << " "; })  // Выводит: 3 2 1 0 4 5 6
        .map<int>([](int x) -> int { return x + 1; })  // Преобразует: 4, 3, 2, 1, 5, 6, 7
        .for_each([](int x) { std::cout << x << "\n"; });  // Выводит преобразованные значения
    
    return 0;
}