#include "UseOverrideFunctions.h"
#include <memory>
#include <vector>


class Base
{
public:
	virtual void DoSomeWork()
	{}
};

class Derived : public Base
{
public:
	virtual void DoSomeWork() override
	{};
};


/*
*	C++11 requirement:
*	Reference qualifiers of virtual functions should be equal
*/

class ClassZ
{
public:
	void DoSomeWork()&		// Reference qualifier; this version used if *this will be lvalue
	{}

	void DoSomeWork()&&		// Reference qualifier; this version used if *this will be rvalue
	{}
};

/*
*	Reference qualifier can be used for different usage lavalue and rvalue objects
*/

class ClassX
{
public:
	using DataType = std::vector<double>;

	DataType& data()&
	{
		return values;	// For lvalue ClassX return lvalue
	}

	DataType&& data()&&
	{
		return std::move(values);	// For rvalue ClassX return rvalue
	}

private:
	DataType values;
};


/*
*	Using override can show potential problems with derived virtual functions (mismatch in argumanets const qualifiers, names and others)
*	Also can show how many functions you need to update, if try change signature
*
*	Also we have keyword "final" - it will stop overriding virtual funtion in derived class
*	Also final can be used for class - class cannot be used like base class
*/


void UseOverrideFunctionsMain()
{
	std::unique_ptr<Base> upb = std::make_unique<Derived>();

	upb->DoSomeWork();

	// ClassZ makeWidget(); // Fabric function - return rvalue
	// ClassZ z;			// common object - return lvalue

	// z.DoSomeWork();				// Calll ClassZ::DoSomeWork for lvalue (void DoSomeWork()&)
	// makeWidget().DoSomeWork();	// Calll ClassZ::DoSomeWork for rvalue (void DoSomeWork()&&)

	/*
	*	If we want to make function thataccept only lvalue - arguments
	*	void doSmth(ClassZ& z);	//	Accept only lvalu ClassZ
	*/

	ClassX x;

	auto val1 = x.data();	// call lvalue data(), val1 created by copy

	// makeClassX() - factory function to create ClassX

	//auto val2 = makeClassX().data();	// call rvalue data(), val2 created by moving
}


