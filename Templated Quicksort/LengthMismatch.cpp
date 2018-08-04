#include "LengthMismatch.h"

LengthMismatch::LengthMismatch(int input1, int input2)
{
	msg = "Input data structures have lengths "+
		std::to_string(input1)+" and "+std::to_string(input2);
}
const char* LengthMismatch::what() const noexcept
 {
    return msg.c_str();
 }