#include <iostream>
#include <vector>


/*
*	Let's assume that we have function, that get Widget class and return std::vector<bool>,
*	where bools shows if Widget has some defeined properties
*/

class Widget
{};

std::vector<bool> Features(const Widget& W)
{
	return std::vector<bool>{false, true, false};
}

void ProcessWidget(const Widget& W, bool& Priority)
{

}

int main()
{
	// It will work

	Widget W;

	bool Priority1 = Features(W)[1];

	ProcessWidget(W, Priority1);

	/*
	* 	bool Priority1 = Features(W)[1];
	*	ProcessWidget(W, Priority1);
	*
	*	Features(W) return std::vector<bool> for which we call operator[1],
	*	that will return std::vector<bool>::reference, which will be converted to bool
	*	for initialization Priority1
	*
	*	But if we replace bool type to auto :
	*
	*	auto Priority1 = Features(W)[1];
	*	ProcessWidget(W, Priority1);
	*
	*	ProcessWidget(W, Priority1) will have undefined behavior
	*	because Priority0 will have type std::vector<bool>::reference, not bool
	*
	*	But we can use explicitly typed initializer idiom
	*/

	auto Priority0 = static_cast<bool>(Features(W)[0]);
	ProcessWidget(W, Priority0);
}
