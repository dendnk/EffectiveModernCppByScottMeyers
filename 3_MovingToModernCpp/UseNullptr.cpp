#include "UseNullptr.h"
#include <memory>
#include <mutex>


class ClassAA
{
public:
	ClassAA() {}
	~ClassAA() {}
};


/*
*	Let's assume that you have some functions, which should be called when specified mutex will be locked.
*	Every function get the secific pointer
*/

int f1(std::shared_ptr<ClassAA> spA)
{
	return int(42);
}

double f2(std::unique_ptr<ClassAA> upA)
{
	return 42.;
}

bool f3(ClassAA* pA)
{
	return true;
}


template<typename FuncType, typename MuxType, typename PtrType>
//auto LockAndCall(FuncType func, MuxType mutex, PtrType ptr) -> decltype(func(ptr))
decltype(auto) LockAndCall(FuncType func, MuxType& mutex, PtrType ptr)	// In C++14 can be used decltype(auto)
{
	std::lock_guard<MuxType> LG{ mutex };
	return func(ptr);
}


void UseNullptrMain()
{
	/*
	*	Literal 0 has type int, and it not a pointer
	*	NULL also not a pointer
	*/

	// Let's assume
	//void f(int);
	//void f(bool);
	//void f(void*);

	//f(0);		//	Call f(int), not f(void*)
	// f(NULL);	//	may not compile, usually call f(int), and never call f(void*)

	/*
	*	nullptr has type std::nullptr_t
	*	std::nullptr_t type converted to all types of common pointers
	*	so nullptr can be used as pointer for all types
	*/

	//f(nullptr);	//	Call f(void*)

	/*
	*	auto result = dindRecord();
	*	if (result == 0) {} // not clear, is resulat a pointer, but if we use nullptr, - it will be clear that result has pointer type
	*	if (result == nullptr) {}
	*/

	std::mutex f1m;
	std::mutex f2m;
	std::mutex f3m;

	//using MuxGuard = std::lock_guard<std::mutex>; //	use alias

	//{
	//	MuxGuard g(f1m);			// Lock mutex for f1
	//	auto result = f1(0);		// Pass 0 to f1
	//}								// Unlock mutex for f1

	//{
	//	MuxGuard g(f2m);			// Lock mutex for f2
	//	auto result = f2(NULL);		// Pass NULL to f2
	//}								// Unlock mutex for f2

	//{
	//	MuxGuard g(f3m);			// Lock mutex for f3
	//	auto result = f3(nullptr);	// Pass nullptr to f3
	//}								// Unlock mutex for f3

	//	The code above is very sad (duplicated code, 0 and NULL used instead of nullptr). So to fix it we make template LockAndCall

	//auto result1 = LockAndCall(f1, f1m, 0);			//	Error	: because 0 has type int and it cannot be pointer to std::shared_ptr<ClassAA>
	//auto result2 = LockAndCall(f2, f2m, NULL);		//	Error	: because NULL has integer type (int, long) cannot be pointer to std::unique_ptr<ClassAA>
	auto result3 = LockAndCall(f3, f3m, nullptr);		//	OK		: ptr has type std::nullptr_t, while passing it to function f3, it implicity converts to ClassAA*, because std::nullptr_t can be converted to any pointer type

	/*
	*	Try to avoid using overrides with int types and pointer types
	*/
}
