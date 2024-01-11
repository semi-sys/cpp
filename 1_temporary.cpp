#include <iostream>

class Point
{
public:
    int x,y;
    Point(int x, int y) : x(x), y(y)    { std::cout << "Point()" << x << y << std::endl; }
    ~Point()                            { std::cout << "~Point()"<< x << y << std::endl ; }

    void set(int a, int b) { x = a; y = b; } 
};

void test1() {
    Point pt(1,2);  // named object     자신을 선언한 블록을 벗어날 때 파괴.
    Point (3,4);    // unnamed object   자신을 선언한 문장의 끝에서 파괴. "임시객체(temporary)" 
    std::cout << "--------------" << std::endl;
}

//--------------------------------------------------------------------------------------------

void test2() {
    Point pt(1,2);

    pt.x = 10;
    //Point(1, 2).x = 10;       // 임시객체는 rvalue 이다.
    Point(1, 2).set(10, 20);    // 상수는 아니다. 멤버함수 호출 가능.

    Point* p1 = &pt;
    //Point* p2 = &Point(1,2);   // 주소가 있지만 주소 연산자를 사용할 수 없다. (this 포인터는 사용 가능.)

    Point& r1 = pt;
    //Point& r2 = Point(1, 2);
    const Point& r3 = Point(1, 2);  // 임시객체의 lifespan 연장.
    Point&& r4 = Point(1, 2);       // rvalue reference
}

//--------------------------------------------------------------------------------------------

void foo(const Point& pt) {std::cout << "foo" << std::endl; }
void test3() {
    // Point pt(1,2);
    // foo(pt);         // pt가 더 이상 사용하지 않지만, 계속 남아있음.
    foo(Point(1,2));    // 객체가 함수의 인자로만 사용된다면 임시객체를 사용하는 것이 효율적이다.
                        // 함수 인자는 반드시 "const lvalue reference" 로 받아야 한다.
                        // foo(Point{1 , 2}), foo({1 , 2}) 로도 전달 가능.
}

//--------------------------------------------------------------------------------------------

#include <string>
#include <string_view>
void foo(const std::string& str) {}
void goo(std::string_view str) {}   // call by value
void test4() {
    foo("Pracetice make perfect");  // => foo(std::string("Pracetice make perfect")), 생성자를 호출하여 임시객체 생성
        // 문자열의 복사본도 힙에 생성한다.(짧은 문자열의 경우 SSO 기술 사용)

    goo("Pracetice make perfect");
        // string_view == { 문자열 주소, 문자열 길이 }
        // 문자열의 복사본을 생성하지 않고, 상수 메모리의 문자열을 가리킨다.
        // Read-Only 문자열을 함수 인자로 받는 경우 const std::string& 보다 std::string_view가 효율적이다.
}

//--------------------------------------------------------------------------------------------

Point global_pt(1, 2);
void func1(Point p1) {}                             // call by value
void func2(Point& p2) {}                            // call by reference
Point func3(void) { return global_pt; }             // return by value      ==> 임시객체를 반환
Point& func4(void) { return global_pt; }            // return by reference
Point& func5(void) { Point pt(1,2); return pt; }    // 파괴된 객체를 return, caller가 객체에 접근시 runtime error

class Counter {
    int count{0};
public:
    Counter increment() {
        ++count;
        return *this;
    }
    int get() const { return count; }
};

void test5() {
    //func3().x = 10;       error, assignment to rvalue
    func4().x = 10;

    Counter c;
    c.increment().increment().increment();  // 임시객체가 3번 생성됨.
    std::cout << c.get() << std::endl;      // Print 1
}

//--------------------------------------------------------------------------------------------

struct Base {
    int value = 10;
    Base() = default;
    Base(const Base& b) : value(b.value) {
        std::cout << "Copy Constructor" << std::endl;
    }
};
struct Derived : public Base { int value = 10; };
void test6() {
    Derived d;
    std:: cout << d.value << std::endl; // 20;
    std::cout << static_cast<Base&>(d).value << std::endl;  // 10, 참조 캐스팅
    std::cout << static_cast<Base>(d).value << std::endl;   // 10, 값 캐스팅

    //static_case<Base>(d).value = 10;  값 캐스팅은 복사생성자를 호출하여 임시객체를 생성한다.
}

int main()
{
    /*  임시객체(temporary)

        사용자가 명시적으로 만드는 경우
            - 함수 인자로 객체를 전달할 때.

        컴파일러에 의해 생성되는 경우
            - 함수 인자로 객체가 아닌 생성자 인자를 전달할 때.
            - 함수가 값으로 객체를 반환할 때.
            - 객체를 값으로 캐스팅 할 때.

    */
}