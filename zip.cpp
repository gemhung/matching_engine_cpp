#include <iostream>
#include <tuple>
#include <type_traits>
using namespace std;

template<size_t...I>
struct Vec{};

template<class...Types>
struct zip ;

template<size_t...I, size_t...J, class...Types>
struct zip<Vec<I...>, Vec<J...>, Types...> {
    using type = typename zip<Vec<(I*J)...>, Types...>::type;
};

template<size_t...I>
struct zip<Vec<I...>> {
    using type = Vec<I...>;
};

int main() {
    //std::tuple<1,2,3> t;
    zip<Vec<1,2,3>, Vec<4,5,6>, Vec<7,8,9>, Vec<10,11,12>>::type x;
    std::is_same<decltype(x), Vec<1*4*7*10, 2*5*8*11, 3*6*9*12>> a;
    static_assert(a);

    zip<Vec<5,6,7,8>, Vec<1,2,3,4>, Vec<9,10,11,12>>::type x2;
    std::is_same<decltype(x2), Vec<45,120,231, 384>> a2;
    static_assert(a2);
    
    cout<<"passed"<<endl;
    return 0;
}
