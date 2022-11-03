#include "UseDeletedFunctions.h"


class FF
{
public:
	FF() {}
	~FF() {}

	FF(const FF&) = delete;
	FF& operator=(const FF&) = delete;
};

bool IsLucky(int number);
bool IsLucky(char) = delete;	//	Discard chars
bool IsLucky(bool) = delete;	//	Discard booleans
bool IsLucky(double) = delete;	//	Discard double and float


/*
*	Assume, template function should process all pointers except the void* and char*
*	To make it, we should mark delete this instances
*/
template<typename T>
void processPointer(T* ptr);

template<>
void processPointer(void* ptr) = delete;

template<>
void processPointer(char* ptr) = delete;

/*
*	And also their constant versions
*/

template<>
void processPointer(const void* ptr) = delete;

template<>
void processPointer(const char* ptr) = delete;

/*
*	And also their const volatile void* and  const volatile char* versions
*/



void UseDeletedFunctionsMain()
{
	FF f1{};
	FF f2 = f1;		//	Deleted copy constructor
	FF f3{ f1 };	//	Deleted copy constructor
}
