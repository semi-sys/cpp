#include <iostream>
#include <new>
#include <memory>

class Point {
    int x, y;
public:
    Point(int x, int y) : x{x}, y{y}    { std::cout << "Point(int, int)" << std::endl; }
    ~Point()                            { std::cout << "~Point()" << std::endl; }
};

void test1(void)
{
    /* opeartor new() / operator delete()

        [[nodiscard]]] void* operator new(std::size_t);
        void operator delete (void* ptr) noexcept;

        header : <new>
        메모리를 할당/해지 하는 C++ 표준 함수
        std namespace가 아닌 global namespace
     */

    Point* p1 = new Point(1, 2);         // malloc + constructor
    delete p1;                           // destructor + free

    void* p2 = operator new(sizeof(Point)); // malloc
    Point* p3 = new(p2) Point(1, 2);        // call constructor on 'p1', placement new, (p1 == p2)
    p3->~Point();                           // destructor
    operator delete(p3);                    // free

//------------------------------------------------------------------------------------------
    /* construct_at, destory_at
        header : <memory>
    */

    Point *p4 = static_cast<Point*>(operator new(sizeof(Point)));    // 정확한 type을 받고 싶으면 cast.
    std::construct_at(p4, 1, 2);                                     // 생성자를 명시적으로 호출, 내부에서 placement new 를 사용, (C++20 ~)
    std::destroy_at(p4);                                             // 소멸자를 명시적으로 호출 (C++17 ~)
}

void test2(void)
{
    /* Point 객체 1개를 힙에 생성 */
    Point* p1 = new Point(0, 0);
    delete p1;

    /* Point 객체 3개를 힙에
    연속된 공간에(배열형태로) 생성하려면...* /
//  Point* p2 = new Point[3];                           // error, default 생성자가 선언되지 않음.
    Point* p3 = new Point[3]{{1, 1}, {2, 0}, {3, 5}};   // C++11 부터 지원
    delete[] p3;

    /* 개수가 3개보다 많아지면...? */
    Point* p4 = static_cast<Point*>(operator new(sizeof(Point) * 10));
    for(int i = 0; i < 10; i++)
    {
//      new(&p4[i]) Point(i, i);
        std::construct_at(&p4[i], i, i);
    }

    for(int i = 0; i < 10; i++)
    {
//      p4[i].~Point(i);
        std::construct_at(&p4[i], i, i);
    }

    operator delete(p4);
}

#include <vector>
struct X
{
    X() { std::cout << "constructor" << std::endl; }
    ~X() { std::cout << "destructor" << std::endl; }
};

void test3(void) {
    /* vector v 
        array 주소
        size : 사용중인 element 개수
        capacity : array의 크기, sizeof(array) == sizeof(element) * capacity
    */

    std::vector<X> v(10);   // 생성자 10번 호출

    std::cout << "---------------------------------" << std::endl;

    v.resize(7); // 효율을 위해 메모리는 줄이지 않더라도, 소멸자는 호출해야 하지 않을까?

    std::cout << v.size() << std::endl;
    std::cout << v.capacity() << std::endl;

    std::cout << "---------------------------------" << std::endl;

    v.resize(8); // 1개 더 쓴다고 했으니, 생성자 1번 호출

    std::cout << v.size() << std::endl;
    std::cout << v.capacity() << std::endl;

    std::cout << "---------------------------------" << std::endl;
}

int main()
{
    test1();
    test2();
    test3();
    
    return 0;
}