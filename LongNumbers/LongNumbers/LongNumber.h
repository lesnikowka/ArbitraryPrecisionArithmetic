#pragma once

#include <vector>
#include <cstdint>
#include <string>

#define TYPE uint8_t
#define BIG_TYPE uint64_t

class LongNumber
{
public:
	LongNumber();
	LongNumber(const LongNumber& n);
	LongNumber(LongNumber&& n);
	LongNumber(const std::vector<TYPE>& data, bool negative);
	LongNumber(std::vector<TYPE>&& data, bool negative);
	LongNumber& operator=(const LongNumber& n);
	LongNumber& operator=(LongNumber&& n);
	void setData(const std::vector<TYPE>& data, bool negative);
	void setData(std::vector<TYPE>&& data, bool negative);
	std::pair<std::vector<TYPE>, bool> getData() const;

	LongNumber operator+(const LongNumber& n) const;
	void operator+=(const LongNumber& n) const;
	LongNumber operator-(const LongNumber& n) const;
	void operator-=(const LongNumber& n) const;


	std::string getString() const;
	
private:

	static void _checkZero(LongNumber& n);
	static BIG_TYPE _getMaxTypeValue();

	std::vector<TYPE> _data;
	bool _negative;
};