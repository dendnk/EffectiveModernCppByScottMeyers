#include "UniformInitialization.h"
#include <vector>
#include <atomic>


class ClassA
{
	int xx{ 0 };	//	OK, the value = 0 by default
	int yy = 0;	//	OK
	//int zz(0);	//	Error
	bool bb = false;
	double dd = 0;
public:
	ClassA() {}
	ClassA(int i) {}
	ClassA(int i, bool b) {}	// constructor 3
	ClassA(int i, double d) {}	// constructor 4

	ClassA(std::initializer_list<long double> i1) {}	// constructor with init_list

	operator float() const;		// conversion to float
};


template<typename T, typename ...Args>
void DoWork(Args... params)
{
	T localObject1(std::forward<Args>(params)...);		// use round brackets
	T localObject2{ std::forward<Args>(params)... };	// use figure brackets
}


void UniformInitializationMain()
{
	int x(0);
	int y = 0;
	int z{ 0 };
	int zz = { 0 }; //	the same as : int z{0};

	ClassA a1;		//	default constructor
	ClassA a2 = a1;	//	copy constructor
	a1 = a2;		//	operator=()

	std::vector<int> v1{ 1,3,5 };	// v1 contains 1,3,5 by default

	// Non-copy objects (std::atomic for example) can be init with {} and (), but not with opertor=()
	std::atomic<int> ai1{ 0 };	//	OK
	std::atomic<int> ai2(0);	//	OK
	std::atomic<int> ai3 = 0;	//	Error!

	double d1 = 1;
	double d2 = 2;
	double d3 = 3;

	//int sum1{ d1 + d2 + d3 };	//	{} ban narrowing conversion

	//int sum2(d1 + d2 + d3);		//	OK, Narrowing to int
	//int sum3 = d1 + d2 + d3;	//	OK
	auto sum4 = static_cast<int>(d1 + d2 + d3);

	ClassA a3(10);	//	constructor with argument

	ClassA a4();	//	Syntax analysis recognize it like function declaration with returning type Widget

	ClassA a5{};	//	constructor without arguments

	ClassA a6(10, true);	// constructor 3
	ClassA a7{ 10, true };	// constructor 3, if not constructor with init_list, otherwise - constructor with init_list
	ClassA a8(10, 5.0);		// constructor 4
	ClassA a9{ 10, 5.0 };	// constructor 4, if not constructor with init_list, otherwise - constructor with init_list

	//ClassA a7{ 10, true }; 10 and true will convert to long double
	//ClassA a9{ 10, 5.0 }; 10 and 5.0 will convert to long double

	ClassA a10(a9);		// use copy constructor
	ClassA a11{ a9 };	// use figure braces, so it will call constructor with init_list, a9 will convert to float, and float will convert to long double

	ClassA a12(std::move(a9));		//	use move constructor
	ClassA a13{ std::move(a9) };	//  use figure braces, so it will call constructor with init_list (same as for a11)

	/*
	*	constructor with init_list dominateeven if has no good match
	*
	*	so if we have only one constructor with init_list
	*	ClassA(std::initializer_list<bool> i); without any additional implicit conversions
	*	it will be used for
	*
	*	ClassA a{10,5.0};
	*
	*	even if we have the the accurate constructor with brackets () : ClassA(int,double)
	*/

	/*
	*	When we use empty figure bracers {} for object creating, which supported by constructor wit init_list,
	*	will be used constructor by default , because empty {} means lack of arguments, and not empty init_list
	*
	*	ClassB
	*	{
	*	public:
	*		ClassB();								// constuctor by default
	*		ClassB(std::initialiser_list<int> i);	// constuctor with init_list
	*
	*		// no implicit conversions functions
	*	}
	*
	*	ClassB b1;			//	constructor by default
	*	ClassB b2{};		//	constructor by default
	*	ClassB b3();		//	it recognized as function declaration
	*	ClassB b4({});		//	constuctor with init_list with empty init_list
	*	ClassB b5{ {} };	//	constuctor with init_list with empty init_list
	*/

	//Good example std::vector;

	std::vector<int> v2(10, 20);	// use constructor without init_list, create vector with 10 elements, value of each is 20
	std::vector<int> v3{ 10, 20 };	// use constuctor with init_list, create vector with 2 elements 10 and 20


	//	Interesting problem can be here inside templates

	DoWork<std::vector<int>>(10, 20);

	/*
	*	in this case localObject1 will be std::vector with 10 elements, each value will be 20
	*	and localObject2 will be std::vector with 2 elements : 10 , 20
	*/
}
