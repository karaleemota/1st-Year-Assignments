#ifndef LENGTHMISMATCH_H
#define LENGTHMISMATCH_H

#include <string>
#include <stdexcept>

class LengthMismatch : public std::exception
{
public:
	LengthMismatch(int input1, int input2);
	const char* what() const noexcept;
private:
	std::string msg;
};

#endif