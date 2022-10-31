#include <iostream>
#include <iomanip>


/*
*   template<typename T>
*   void Foo(ParamType param);
*
*   Foo(expr); / function call of deducing "ParamType" and "expr"
*
*   "ParamType" can be
*   1. - pointer or reference / but not universal reference
*   2. - universal reference (&&)
*   3. - not pointer and not reference
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
void Case11Deduce(T& param)
{
    std::cout << "T :\t" << std::setw(10) << GetTypeName<T>() << "\t";
    std::cout << "param :\t" << std::setw(10) << GetTypeName<decltype(param)>() << std::endl;
}

template<typename T>
void Case12Deduce(const T& param)
{
    std::cout << "T :\t" << std::setw(10) << GetTypeName<T>() << "\t";
    std::cout << "param :\t" << std::setw(10) << GetTypeName<decltype(param)>() << std::endl;
}

template<typename T>
void Case13Deduce(T* param)
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

    Case11Deduce(x);    //   T :            int      param :           int&
    Case11Deduce(cx);   //   T :      const int      param :     const int&
    Case11Deduce(rx);   //   T :      const int      param :     const int&

    std::cout << "Case [const T& param] : " << std::endl;

    Case12Deduce(x);    //   T :            int      param :     const int&
    Case12Deduce(cx);   //   T :            int      param :     const int&
    Case12Deduce(rx);   //   T :            int      param :     const int&

    std::cout << "Case [T* param] : " << std::endl;

    const int* px = &x; // pointer to x, as a const int

    Case13Deduce(&x);   //  T :            int      param :           int*
    Case13Deduce(px);   //  T :      const int      param :     const int*
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
*   Case 3 "ParamType"  - not pointer and not reference
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
    std::cout << "ParamType :\t" << std::setw(10) << GetTypeName<decltype(param)>() << std::endl;
}

void Case3()
{
    int x = 27;
    const int cx = x;
    const int& rx = x;

    std::cout << "Case 3 ""ParamType""  - not pointer and not reference" << std::endl;

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


int main()
{
    Case1();
    Case2();
    Case3();
}
