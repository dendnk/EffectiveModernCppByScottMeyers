#include "UseScopedEnum.h"
#include <cstdint>


//	Unscoped
enum ColorsA
{
	Black,
	Red,
	White
};

//	Scoped
enum class ColorsB
{
	Blue,
	Green,
	Purple
};

//	forward declaration
enum class EStatus;	// The base type is int

// It can be changed
enum class EState : std::uint32_t;	// Now the base type for EState  std::unit32_t (from <cstdint>)

enum EStage : std::uint32_t; // forward declaration of unscoped enum can be done only with setting default type for it;

void continueProcessing(EStatus s);


void UseScopedEnumMain()
{

}
