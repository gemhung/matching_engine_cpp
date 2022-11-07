#include <type_traits>
#include <utility>
#include <iostream>
#include <climits>

using namespace std;
// DEFINITION: Compile-time integer vector defined as: 
template<int...I>
struct Vector{};

template<class...Types>
struct zip ;

template<int...I, int...J, class...Types>
struct zip<Vector<I...>, Vector<J...>, Types...> {
    using type = typename zip<Vector<(I*J)...>, Types...>::type; // Note that it might overflow for 'I*J' !
};

// template specialization for zip<Vector<...>>
template<int...I>
struct zip<Vector<I...>> {
    using type = Vector<I...>;
};

// The code below will assume a 'zip' metafunction is used, but feel free to use a different approach. 
// If you do, please adjust the static assert accordingly. 

// TEST
int main() {
    // Test case #1
    static_assert(std::is_same<zip<Vector<1, 2, 3>, Vector<4, 5, 6>, Vector<7, 8, 9>>::type, Vector<28,80,162>>::value, "not match !");
    
   // TASK: Add more test cases here
    // Test case #2
    static_assert(std::is_same<zip<Vector<1, 2, 3>, Vector<4, 5, 6>, Vector<7, 8, 9>>::type, Vector<1*4*7,2*5*8,3*6*9>>::value, "not match !");
    
    // Test case #3
    static_assert(std::is_same<zip<Vector<1>>::type, Vector<1>>::value, "not match !");
   
    // Test case #4
    static_assert(std::is_same<zip<Vector<INT_MAX>>::type, Vector<INT_MAX>>::value, "not match !");
    
    // Test case #5
    static_assert(std::is_same<zip<Vector<INT_MIN>>::type, Vector<INT_MIN>>::value, "not match !");
    
    // Test case #6
    static_assert(std::is_same<zip<Vector<1>, Vector<4>, Vector<7>>::type, Vector<1*4*7>>::value, "not match !");
    
    // Test case #7
    static_assert(std::is_same<zip<Vector<1, 2>, Vector<4, 5>, Vector<7, 8>>::type, Vector<1*4*7,2*5*8>>::value, "not match !");
        
    // Test case #8
    static_assert(std::is_same<zip<Vector<4, 5>, Vector<1, 2>, Vector<7, 8>>::type, Vector<1*4*7,2*5*8>>::value, "not match !");
    
    // Test case #9
    static_assert(std::is_same<zip<Vector<-1, -2>, Vector<4, 5>, Vector<7, 8>>::type, Vector<-1*4*7,-2*5*8>>::value, "not match !");
    
    // Test case #10
    static_assert(std::is_same<zip<Vector<>>::type, Vector<>>::value, "not match !");
    
    // Test case #11
    static_assert(std::is_same<zip<Vector<>, Vector<>>::type,Vector<>>::value, "not match !");
        
    // Test case #12
    static_assert(std::is_same<zip<Vector<>, Vector<>, Vector<>>::type,Vector<>>::value, "not match !");
    
    // TASK: Ensure it compiles before submission
    return 0;
}
