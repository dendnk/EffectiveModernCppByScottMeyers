#include "UseAlias.h"
#include <memory>
#include <unordered_map>
#include <string>
#include <string_view>
#include <list>


class ClassBB
{
public:
	ClassBB(){}
	~ClassBB(){}
};

/*
*	Let's assume example making synonym for linked list, which use custom memory distributor MyAlloc
*/

// MyAllocList<T> - synonym for std::list<T,MyAlloc<T>>:
template<typename T>
using MyAllocList = std::list<T, MyAlloc<T>>;

// If we will use typedef
// MyAllocList<T> - synonym for std::list<T,MyAlloc<T>>:
template<typename T>
struct MyAlloList2
{
	typedef std::list<T, MyAlloc<T>> type;
};

template<typename T>
class CC	// ClassCC<T> contains MyAllocList2<T> as member data
{
	typename MyAlloList2<T>::type list;
	/*
	*	Here MyAlloList2<T>::type refrence to type, that depend on type of (T).
	*	So MyAlloList2<T>::type is dependent type . so name of it should have typename keyword
	*/
};

// In alias case
template<typename T>
class DD	// ClassDD<T> contains MyAllocList<T> as member data
{
	MyAlloList<T> list; // nor use ::type, nor typename
};


void UseAliasMain()
{
	// Pretty long type =)
	//std::unique_ptr<std::unordered_map<std::string, std::string>> MapSS;

	// To fix it - we can use typedef
	// typedef std::unique_ptr<std::unordered_map<std::string, std::string>> UPtrToMapSS;

	// C++11 offer alis decalration
	using UPtrToMapSS = std::unique_ptr<std::unordered_map<std::string, std::string>>;

	// FP - synonym for pointer to function with int,const std::string_view& arguments and no return // C++17 provide string_view
	//typedef void (*FP)(int, const std::string_view&);

	// The same with alias help
	using FP = void (*)(int, const std::string_view&);

	/*
	*	Alias decalration can be templated, and typedef not
	*/

	MyAllocList<ClassBB> lBB;

	MyAlloList2<ClassBB>::type lBB2;

	/*
	*	typedef not support templatization
	*	alias templates not demand suffix "::type", and in templates not demand prefix "typename", unlike typedef
	*	C++14 offer alias templates to all convertations of type properties C++11
	*
	*	std::remove_const<T>::type	->	std::remove_const_t<T>
	*/
}
