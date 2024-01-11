#include <iostream>
#include <algorithm>
#include <functional>
#include <ranges>
#include <string>
#include <vector>

std::string s1 = "abcd";
std::string s2 = "xyz";
struct Point { int x, y; } p1{2, 0}, p2{1, 1}; 

template<class T>
const T& max_1(const T& lhs, const T& rhs) {
    return lhs < rhs ? rhs : lhs;
}

template<class T, class Proj>
const T& max2_1(const T& lhs, const T& rhs, Proj proj) {
    return proj(lhs) < proj(rhs) ? rhs : lhs;
}

template<class T, class Proj = std::identity>
const T& max2_2(const T& lhs, const T& rhs, Proj proj = {}) {
    return std::invoke(proj, lhs) < std::invoke(proj, rhs) ? rhs : lhs;
}

template<class T, class Comp = std::less<void>, class Proj = std::identity>
const T& max3(const T& lhs, const T& rhs, Comp comp = {}, Proj proj = {}) {
//  return comp(std::invoke(proj, lhs), std::invoke(proj, rhs)) ? rhs : lhs;                comp가 멤버함수이면 error
    return std::invoke(comp, std::invoke(proj, lhs), std::invoke(proj, rhs)) ? rhs : lhs;
}

int main(void) {
    auto ret1_1 = max_1(s1, s2);                // 사전순으로 비교
    auto ret1_2 = max_1(s1.size(), s2.size());  // 문자열의 길이로 비교

    std::cout << ret1_1 << std::endl;         // ok
    std::cout << ret1_2 << std::endl;         // 길이가 출력됨...

//-------------------------------------------------------------------------------------------------------------

    /* 알고리즘 함수가 사용하는 정책(비교 방식)을 변경하고 싶다.
        방법 1. 이항 조건자(binary predicator) 사용
            std::sort(v.begin(), b.end(), [](auto& a, auto& b){ return a.size() < b.size(); });     // C++ STL
        방법 2. 단한 조건자(unary predicator) 사용
            sorted(str_list, key = lambda x : len(x));                                              // Python
        방법 3. "방법 1"과 "방법 2" 모두 사용
            => C++20 Range Algorithm 의 원리
    */

    auto ret2_1 = max2_1(s1, s2, [](auto& param){ return param.size(); });
    auto ret2_2 = max2_2(s1, s2);                                               // default 정책
    auto ret2_3 = max2_1(s1, s2, [](auto& param){ return param.size(); });      // 단항 조건자
    auto ret2_4 = max2_2(s1, s2, &std::string::size);                           // 멤버 함수포인터

    std::cout << ret2_1 << std::endl;
    std::cout << ret2_2 << std::endl;
    std::cout << ret2_3 << std::endl;
    std::cout << ret2_4 << std::endl;

    auto ret2_5 = max2_2(p1, p2, &Point::y);                                    // 멤버 변수 포인터
    std::cout << ret2_5.x << ", " << ret2_5.y << std::endl;

//-------------------------------------------------------------------------------------------------------------
    /*
        비교 함수객체도 변경하고 싶은 경우
            std::less{}     => '<' 연산자
            std::greater{}  => '>' 연산자
    */

    auto ret3_1 = max3(s1, s2);
    auto ret3_2 = max3(s1, s2, std::greater{});
    auto ret3_3 = max3(s1, s2, {}, &std::string::size);
    auto ret3_4 = max3(s1, s2, std::greater{}, &std::string::size);

    std::cout << ret3_1 << std::endl;
    std::cout << ret3_2 << std::endl;
    std::cout << ret3_3 << std::endl;
    std::cout << ret3_4 << std::endl;

//-------------------------------------------------------------------------------------------------------------

    /* std::ranges
        header : <algorithm>
        알고리즘 "비교 정책" 교체 가능
        Projection 전달 가능(pointer to member 사용 가능)
        반복자 구간이 아닌 "컨테이너를 전달" 받는다.
    */

    auto ret4_1 = std::ranges::max(s1, s2, std::greater{}, &std::string::size);
    auto ret4_2 = std::ranges::max(s1, s2, std::ranges::greater{}, &std::string::size);

    std::cout << ret4_1 << std::endl;
    std::cout << ret4_2 << std::endl;

    std::vector<std::string> v = { "hello", "a", "xxx", "zz" };

    std::sort(v.begin(), v.end());                                      // 반복자 구간
    std::ranges::sort(v, std::ranges::greater{}, &std::string::size);   // 컨테이너

    for(auto element : v)
        std::cout << element << std::endl;

    /*
        max3 는 함수 템플릿 이지만,
        std::ranges::max 는 함수 객체(템플릿) 이다.
    */

    return 0;
}