#include <iostream>
#include <iomanip>
#include <array>
#include <vector>


/*
*   template<typename T>
*   void Foo(ParamType param);
*
*   Foo(expr); / function call of deducing "ParamType" and "expr"
*
*   "ParamType" can be
*   1. - pointer or reference / but not universal reference
*   2. - universal reference (&&)
*   3. - nor pointer nor reference
*/

/*
*  Helper function for printing types
*  @ https://stackoverflow.com/questions/81870/is-it-possible-to-print-a-variables-type-in-standard-c
*/
template<typename T>
std::string_view GetTypeName()
{
    using namespace std;

#ifdef __clang__
    string_view p = __PRETTY_FUNCTION__;
    return string_view(p.data() + 34, p.size() - 34 - 1);
#elif defined(__GNUC__)
    string_view p = __PRETTY_FUNCTION__;
#  if __cplusplus < 201402
    return string_view(p.data() + 36, p.size() - 36 - 1);
#  else
    return string_view(p.data() + 49, p.find(';', 49) - 49);
#  endif
#elif defined(_MSC_VER)
    string_view p = __FUNCSIG__;
    return string_view(p.data() + 86, p.size() - 86 - 7); // Changed the constant values for Microsoft Visual Studio Professional 2019 Version 16.11.15
#endif
}

/*
*   Case 1  "ParamType" - pointer or reference / but not universal reference
*
*   Deduce work in next steps :
*   1. if expr - reference, then refer part is ignore
*   2. match expr with ParamType to define T
*/


template<typename T>
void Case11T(T& param)
{
    std::cout << "T :\t" << std::setw(10) << GetTypeName<T>() << "\t";
    std::cout << "param :\t" << std::setw(10) << GetTypeName<decltype(param)>() << std::endl;
}

template<typename T>
void Case12T(const T& param)
{
    std::cout << "T :\t" << std::setw(10) << GetTypeName<T>() << "\t";
    std::cout << "param :\t" << std::setw(10) << GetTypeName<decltype(param)>() << std::endl;
}

template<typename T>
void Case13T(T* param)
{
    std::cout << "T :\t" << std::setw(10) << GetTypeName<T>() << "\t";
    std::cout << "param :\t" << std::setw(10) << GetTypeName<decltype(param)>() << std::endl;
}

void Case1()
{
    int x = 27;
    const int cx = x;
    const int& rx = x;

    std::cout << "Case 1  ""ParamType"" - pointer or reference / but not universal reference" << std::endl;

    std::cout << "Case [T& param] : " << std::endl;

    Case11T(x);    //   T :            int      param :           int&
    Case11T(cx);   //   T :      const int      param :     const int&
    Case11T(rx);   //   T :      const int      param :     const int&

    std::cout << "Case [const T& param] : " << std::endl;

    Case12T(x);    //   T :            int      param :     const int&
    Case12T(cx);   //   T :            int      param :     const int&
    Case12T(rx);   //   T :            int      param :     const int&

    std::cout << "Case [T* param] : " << std::endl;

    const int* px = &x; // pointer to x, as a const int

    Case13T(&x);   //  T :            int      param :           int*
    Case13T(px);   //  T :      const int      param :     const int*
}


/*
*   Case 2 "ParamType"  - universal reference (&&)
*/

template<typename T>
void Case2T(T&& param)
{
    std::cout << "T :\t" << std::setw(10) << GetTypeName<T>() << "\t";
    std::cout << "param :\t" << std::setw(10) << GetTypeName<decltype(param)>() << std::endl;
}

void Case2()
{
    int x = 27;
    const int cx = x;
    const int& rx = x;

    std::cout << "Case 2 ""ParamType""  - universal reference (&&)" << std::endl;

    Case2T(x);      //  T :           int&      param :           int&
    Case2T(cx);     //  T :     const int&      param :     const int&
    Case2T(rx);     //  T :     const int&      param :     const int&
    Case2T(27);     //  T :            int      param :          int&&
}


/*
*   Case 3 "ParamType"  - nor pointer nor reference
*
*   It means that param will be copy of passed object - it will be the new object
*
*   1. If the expr - reference, then the reference part will ignored
*   2. After discarding the reference part, const and volatile also will be discarded
*/

template<typename T>
void Case3T(T param)
{
    std::cout << "T :\t" << std::setw(10) << GetTypeName<T>() << "\t";
    std::cout << "param :\t" << std::setw(10) << GetTypeName<decltype(param)>() << std::endl;
}

void Case3()
{
    int x = 27;
    const int cx = x;
    const int& rx = x;

    std::cout << "Case 3 ""ParamType""  - nor pointer nor reference" << std::endl;

    Case3T(x);      //T :            int      param :            int
    Case3T(cx);     //T :            int      param :            int
    Case3T(rx);     //T :            int      param :            int

    const char* const ptr = "Fun with pointers";

    /*
    *   Here the pointer itself will be copied to param
    *   so it will not be constant pointer (const char* const param), but changebale pointer (const char* param)
    *   to a constant character string
    */
    Case3T(ptr);    //T :     const char*     param :     const char*
}

/*
*   Arguments-arrays
*/

template<typename T, std::size_t N>
constexpr std::size_t GetArraySize(T(&)[N]) noexcept
{
    return N;
    /*
    *   declartion constexpr give to us ability to get result at compile time
    *   so we can use it for declaration othe arrays with the same size
    */
}


void ArgumentsArrays()
{
    const char name[] = "Briggs";   // name type        - const char[7]
    const char* ptrToName = name;   // ptrTo Name type  - const char*

    std::cout << "Arguments-Arrays" << std::endl;

    // We get parameter by value
    std::cout << "We get parameter by value" << std::endl;
    Case3T(name);       //T :     const char*     param :     const char*   name : const char[7]
    Case3T(ptrToName);  //T :     const char*     param :     const char*

    /*
    *   Function with such argument     void FuncWithArrayArgument(int param[]);
    *   will be equal to this           void FuncWithArrayArgument(int* param);
    *   that's why we have  const char* result
    */

    // If we get paramter by reference
    std::cout << "We get parameter by reference" << std::endl;
    Case11T(name);      //T :     const char[7]   param : const char(&)[7] (reference to array)
    Case11T(ptrToName); //T :     const char*     param : const char*&

    int keyValues[] = { 1,3,5,7,9,11,22,35 };

    std::array<int, GetArraySize(keyValues)> mappedValues;

    for (const auto& value : mappedValues)
    {
        std::cout << value << " ";
    }
    std::cout << std::endl;
}


/*
*   Arguments-functions
*/

void SomeFunc(int, double)
{

}


void ArgumentsFunctions()
{
    std::cout << "Arguments-Functions" << std::endl;

    // We pass fucntion by value to template function
    std::cout << "Pass by value" << std::endl;
    Case3T(SomeFunc);   //T :     void(__cdecl *)(int,double)     param : void(__cdecl *)(int,double)

    // We pass fucntion by reference to template function
    std::cout << "Pass by reference" << std::endl;
    Case11T(SomeFunc);  //T :     void(int,double)        param : void(__cdecl &)(int,double)
}


/*
*   Deduce auto
*/

void SomeOtherFunc(int, double) {};

template<typename T>
void DeduceAutoT(std::initializer_list<T> initList)
{
    std::cout << "T :\t" << std::setw(10) << GetTypeName<T>() << "\t";
    std::cout << "initList :\t" << std::setw(10) << GetTypeName<decltype(initList)>() << std::endl;
}

void DeduceAuto()
{
    /*
    *   Deducing auto similar to deducing templates but with one exception
    */

    auto        x = 27;     //  Case 3 (x - nor pointer nor reference)
    const auto  cx = x;     //  Case 3 (cx - nor pointer nor reference)
    const auto& rx = x;     //  Case 1 (rx - reference (not universal))

    auto&& uref1 = x;       // x    -   int and lvalue;         uref1 type :    &int
    auto&& uref2 = cx;      // cx   -   const int and lvalue;   uref2 type :    const int&
    auto&& uref3 = 27;      // 27   -   int and rvalue;         uref3 type :    int&&

    const char name[] = "R. N. Briggs"; //  char type   -   const char[13]
    auto arr1 = name;                   //  arr1 type   -   const char*
    auto& arr2 = name;                  //  arr2 type   -   const char(&)[13]

    auto func1 = SomeOtherFunc;         //  func1 type  -   void(*)(int,double)
    auto& func2 = SomeOtherFunc;        //  func2 type  -   void(&)(int,double)


    /* Declaration int with init value 27 */
    // In C++98
    int x1 = 27;
    int x2(27);

    // In C++11
    int x3 = 27;    // support previous declaration
    int x4(27);    // support previous declaration
    int x5 = { 27 };// new
    int x6{ 27 };   // new

    // What if we replace type by auto?
    auto x11 = 27;      //  type int, value = 27
    auto x12(27);       //  type int, value = 27
    auto x13 = { 27 };  //  std::initializer_list<int>, value {27}
    auto x14{ 13 };     //  should be std::initializer_list<int>, value {27}, but the compiler shows int ???

    /*
    *   if we use {}, it will have the std::initializer_list type
    */

    auto xx = { 11,23,9 };
    Case3T(xx);

    // Case3T({ 101,230,90 }); will not compile, template functions should have arguments as std::initializeer_list

    DeduceAutoT({ 101,230,90 }); //T    -   int;    initList    -   std::initializer_list<int>

    // In C++14
    /*
    *   Allow using auto for returning types of functions
    *   Allow using auto for arguments in lambdas
    *
    *   But in such cases it will be used deducing template type, not deducing auto
    *
    *   auto CreateInitList()
    *   {
    *       return { 1,2,3 }; // cannot deduce a type as an initializer list is not an expression
    *   }
    *
    */

    std::vector<int> v;

    auto ResetV = [&v](const auto& NewValue)
    {
        v = NewValue;
    };

    // ResetV({ 1,2,3 }); can't deduce type for {1,2,3,}
}


/*
*   decltype
*/

class Widget
{

};

template<typename T>
class CustomVector
{
public:
    T& operator[](std::size_t index);
};

bool f(const Widget& w)     //  decltype(w)  -   const Widget&
                            //  decltype(f)  -   bool(const Widget&)
{
    return true;
}

void authenticateUser()
{

}


/*
*   Function that get container, authentificate user and returning index operation result
*   Here decltype help to deduce returning type (it will return the same type as c[i] will have)
*   for case std::vector<bool> c[i] will have type std::vector<bool>::reference and not bool
*   Here auto show that will be used trailing return type (C++11)
*/

//template<typename Container, typename Index>
//auto authAndAccess(Container& c, Index i) -> decltype(c[i]) // C++11, work but need to improve
//{
//    authenticateUser();
//    return c[i];
//}

template<typename Container, typename Index>
auto authAndAccess(Container& c, Index i) -> decltype(std::forward<Container>(c)[i]) // C++11, final version
{
    authenticateUser();
    return std::forward<Container>(c)[i];
}

/*
*   In C++14
*   we can ommit railing return type "-> decltype(c[i])"
*   and it will work but not really correct
*   here auto means type deduce and compilators will return function type from funtion implementation
*/
//template<typename Container, typename Index>
//auto authAndAccess(Container& c, Index i) // have no correct behavior
//{
//    authenticateUser();
//    return c[i]; // returning type deduce from c[i]
//}

/*
*   The problem here will be that operator[] for most containers with T type objects return &T
*   but for templates reference will be ignored
*
*   std::deque<int> d;
*   authAndAccess(d, 5) = 10;
*
*   Here we authenticate User, return d[5] and the try to set 10 to it.
*   It will not work : d[5] will return type  &int,
*   but auto will discard & (reference part) and function will return type int;
*   and it will be rvalue, which we try set with value 10. So it will not compile
*
*   So to fix it need to use decltype(auto) in returning part of function
*/


//template<typename Container, typename Index>
//decltype(auto) authAndAccess(Container& c, Index i) // C++14, work but still need to improve
//{
//    authenticateUser();
//    return c[i]; // returning type deduce from c[i]
//}

/*
*   Here decltype(auto) means:
*   auto        -   show that type must be deduced
*   decltype    -   show that for deducing needs to use decltype rules
*
*   decltype(auto) can be used also for variables
*
*   Why we want to improve it?
*
*   std::deque<std::string> makeStringDeque(); // fabric function
*
*   // we try to make copy of fifth element of deque, that will be returned by function makeStringDeque()
*   auto s = authAndAccess(makeStringDeque(),5)
*                               ^
*                               |
*                             rvalue
*   So here we should make template function accept lvalu and rvalue.
*   To make it we will use universal reference &&
*/

//template<typename Container, typename Index>
//decltype(auto) authAndAccess(Container&& c, Index i) // C++14, now c - universal reference, but still can be improved
//{
//    authenticateUser();
//    return c[i]; // returning type deduce from c[i]
//}

/*
*   Now the problem with missed std::forward<Container>(c)[i]
*/

template<typename Container, typename Index>
decltype(auto) authAndAccess(Container&& c, Index i) // C++14 - final version
{
    authenticateUser();
    return std::forward<Container>(c)[i];
}



void DecltypeMain()
{
    const int i = 0;                //  decltype(i)  -   const int



    struct Point
    {
        int x;                      //  decltype(Point::x)   -   int
        int y;                      //  decltype(Point::u)   -   int
    };

    Widget w;                       //  decltype(w)   -   Widget

    if (f(w)) {}                    //  decltype(f(w))  -   bool

    CustomVector<int> CV;           //  decltype(CV)  -   CustomVector<int>

    //if (CV[0] == 0) {}              //  decltype(CV[0]) -   int&

    std::vector<bool> a{ 1 };
    auto b = a[0];                  //  a[0]    -   std::vector<bool>::reference

    /*
    *   decltype(auto) can be used also for vaiables
    */

    const Widget& cw = w;

    auto myWidget1 = cw;            //  myWidget1   -   Widget
    decltype(auto) myWidget2 = cw;  //  myWidget2   -   const Widget&

    int x = 0;
    std::cout << GetTypeName<decltype(x)>() << std::endl;   //  int
    std::cout << GetTypeName<decltype((x))>() << std::endl; // int&

    /*
    *   decltype(x) : return int
    *   here the x - name of variable, so the decltype give type of this name
    *   But for Lvalue-expression, more complex that name, decltype give garanty, that
    *   returning type will be always lvalue-reference, that's why in case     *
    *   decltype((x)) - will return int&
    *
    *   So in C++14 it important attentively use with function returning decltype(auto)
    *
    *   decltype(auto) f1()
    *   {
    *       int x = 0;
    *       ...
    *       return x;   // decltype(x) - int, so f1 will return int
    *   }
    *
    *   decltype(auto) f2()
    *   {
    *       int x = 0;
    *       ...
    *       return (x);   // decltype((x)) - int&, so f1 will return int&
    *   }
    */
}


int main()
{
    //Case1();
    //Case2();
    //Case3();

    //ArgumentsArrays();
    //ArgumentsFunctions();

    //DeduceAuto();

    DecltypeMain();
}
