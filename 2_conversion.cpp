class Int32 {
    int value;
public:
    Int32()      : value(0) {}
    Int32(int n) : value(n) {}

    operator int() const { return value; }
    // Int32(const Int32&) = delete;
};

void test1() {
    int   pn; // primary type
    Int32 un; // user type

    pn = un;    // 변환연산자, pn = un.operator int()    
    un = pn;    // un.Int32() 는 불가능
                // 1. 대입연산자, un.operator=(pn)
                // 2. 변환생성자, un = Int32(pn)

}

void test2() {
    Int32 n1(3);
    Int32 n2 = 3;   // Int32 n2 = Int32(3)
                    // C++14 까지
                    //  1. Int32(int) 를 호출해 임시객체 생성
                    //  2. 생성된 임시책체를
                    //      ~ C++98 : 복사생성자
                    //      C++11 ~ : 이동생성자
                    //  대부분 컴파일러가 최적화를 통해 임시객체 생성을 제거하지만, 문법적으로 남아있음.
                    //  Int32(const Int32&) = delete; 이면 compile error 발생.
                    // C++17 이후
                    //  임시객체를 생성하지 않고, Int32(int) 만 호출.
    Int32 n3{3};
    Int32 n4 = {3};

    n1 = 3;         // n1 = Int32(3);
                    //  1. Int32(int)를 사용해 임시객체 생성
                    //  2. 디폴트 대입 연산자를 사용해 n1에 대입.
                    //  대부분 컴파일러가 최적화를 통해 임시객체 생성을 제거하지만, 문법적으로 남아있음.
                    //  Int32& operator=(const Int32&) = delete; 이면 compile error 발생.
}

int main()
{
    test1();
    test2();
}