#include "LongNumber.h"

#include <unordered_map>

namespace
{
	std::unordered_map<char, TYPE> charToTYPE =
	{
		{'0', 0},
		{'1', 1},
		{'2', 2},
		{'3', 3},
		{'4', 4},
		{'5', 5},
		{'6', 6},
		{'7', 7},
		{'8', 8},
		{'9', 9},
	};

	std::unordered_map<TYPE, char> typeToChar =
	{
		{0, '0'},
		{1, '1'},
		{2, '2'},
		{3, '3'},
		{4, '4'},
		{5, '5'},
		{6, '6'},
		{7, '7'},
		{8, '8'},
		{9, '9'},
	};
}

LongNumber::LongNumber()
	: _negative(false)
{
}

LongNumber::LongNumber(const LongNumber& n)
	: _data(n._data)
	, _negative(n._negative)
{
}

LongNumber::LongNumber(LongNumber&& n) noexcept
	: _data(std::move(n._data))
	, _negative(n._negative)
{
}

LongNumber::LongNumber(std::string s)
{
	if (s.size() == 0)
	{
		return;
	}
	else if (s[0] == '-')
	{
		_negative = true;
	}

	for (long long i = s.size() - 1; i >= (int)(s[0] == '-'); i--)
	{
		_data.push_back(charToTYPE[s[i]]);
	}

	_checkZero(*this);
}

LongNumber& LongNumber::operator=(const LongNumber& n)
{
	_data = n._data;
	_negative = n._negative;

	return *this;
}

LongNumber& LongNumber::operator=(LongNumber&& n) noexcept
{
	_data = std::move(n._data);
	_negative = n._negative;

	return *this;
}

LongNumber LongNumber::operator+(const LongNumber& n) const
{
	if (_negative && !n._negative)
	{
		return n - (-(*this));
	}
	if (!_negative && n._negative)
	{
		return *this - (-n);
	}

	LongNumber result;

	if (_negative && n._negative)
	{
		result._negative = true;
	}

	size_t minSize = std::min(_data.size(), n._data.size());
	size_t maxSize = std::min(_data.size(), n._data.size());

	BIG_TYPE resVal = 0;
	TYPE quot = 0;
	TYPE rem = 0;

	for (size_t i = 0; i < minSize; i++)
	{
		resVal = static_cast<BIG_TYPE>(_data[i])
			+ static_cast<BIG_TYPE>(n._data[i]) + 
			static_cast<BIG_TYPE>(quot);
		quot = resVal / (_getMaxTypeValue() + 1);
		rem = resVal % (_getMaxTypeValue() + 1);
		
		result._data.push_back(rem);
	}

	for (size_t i = minSize; i < _data.size(); i++)
	{
		resVal = static_cast<BIG_TYPE>(_data[i]) +
			static_cast<BIG_TYPE>(quot);
		quot = resVal / (_getMaxTypeValue() + 1);
		rem = resVal % (_getMaxTypeValue() + 1);
		
		result._data.push_back(rem);
	}

	for (size_t i = minSize; i < n._data.size(); i++)
	{
		resVal = static_cast<BIG_TYPE>(n._data[i]) +
			static_cast<BIG_TYPE>(quot);
		quot = resVal / (_getMaxTypeValue() + 1);
		rem = resVal % (_getMaxTypeValue() + 1);

		result._data.push_back(rem);
	}

	result._data.push_back(quot);
	_checkZero(result);

	return result;
}

void LongNumber::operator+=(const LongNumber& n)
{
	*this = *this + n;
}

LongNumber LongNumber::operator-(const LongNumber& n) const
{
	if (_negative && !n._negative)
	{
		return (-(*this)) + n;
	}
	if (!_negative && n._negative)
	{
		return *this + (-n);
	}

	LongNumber result;

	if (!_negative && !n._negative)
	{
		if (*this < n)
		{
			result = n - *this;
			result._negative = true;
			return result;
		}
		else 
		{
			result._negative = false;
		}
	}

	if (_negative && n._negative)
	{
		if (n._absIsGreater(*this))
		{
			result = n - *this;
			result._negative = false;
			return result;
		}
		else
		{
			result._negative = true;
		}
	}

	size_t minSize = std::min(_data.size(), n._data.size());
	size_t maxSize = std::min(_data.size(), n._data.size());

	BIG_TYPE fut = 0;
	BIG_TYPE val = 0;
	BIG_TYPE curDataVal = 0;

	for (size_t i = 0; i < minSize; i++)
	{
		curDataVal = static_cast<BIG_TYPE>(_data[i]) - fut;
		

		if (curDataVal >= n._data[i])
		{
			val = curDataVal - n._data[i];
			fut = 0;
		}
		else
		{
			fut = 1;
			val = static_cast<BIG_TYPE>(curDataVal) + _getMaxTypeValue() + 1
				- static_cast<BIG_TYPE>(n._data[i]);
		}
		result._data.push_back(val);
	}

	for (size_t i = minSize; i < _data.size(); i++)
	{
		curDataVal = static_cast<BIG_TYPE>(_data[i]) - fut;

		if (curDataVal >= 0)
		{
			val = curDataVal;
			fut = 0;
		}
		else {
			val = curDataVal + _getMaxTypeValue();
			fut = 1;
		}

		result._data.push_back(val);
	}

	_checkZero(result);

	return result;
}

void LongNumber::operator-=(const LongNumber& n)
{
	*this = *this - n;
}

LongNumber LongNumber::operator*(const LongNumber& n)
{
	LongNumber result;
	BIG_TYPE val = 0;
	BIG_TYPE quot = 0;
	BIG_TYPE rem = 0;

	for (size_t i = 0; i < n._data.size(); i++)
	{
		LongNumber tempResult;
		for (size_t j = 0; j < _data.size(); j++)
		{
			val = static_cast<BIG_TYPE>(_data[j]) *
				static_cast<BIG_TYPE>(n._data[i]) + quot;
			quot = val / (_getMaxTypeValue() + 1);
			rem = val % (_getMaxTypeValue() + 1);
			tempResult._data.push_back(rem);
		}
		while (quot > 0)
		{
			rem = quot % (_getMaxTypeValue() + 1);
			quot /= (_getMaxTypeValue() + 1);
			tempResult._data.push_back(rem);
		}
		_checkZero(tempResult);
		std::vector<TYPE> empty(i);
		tempResult._data.insert(tempResult._data.begin(), empty.begin(), empty.end());
		result += tempResult;
	}

	result._negative = _negative != n._negative;
	_checkZero(result);
	return result;
}

void LongNumber::operator*=(const LongNumber& n)
{
	*this = *this * n;
}

LongNumber LongNumber::operator-() const
{
	LongNumber result(*this);

	result._negative = !result._negative;

	return result;
}

bool LongNumber::operator==(const LongNumber& n) const
{
	return _data.size() == 0 && n._data.size() == 0 || _negative == n._negative && _data == n._data;
}

bool LongNumber::operator<(const LongNumber& n) const
{
	if (!_negative && !n._negative)
	{
		return _absIsLess(n);
	}
	else if (_negative && n._negative)
	{
		return _absIsGreater(n);
	}
	if (_data.size() == 0 && n._data.size() == 0)
	{
		return false;
	}
	if (!_negative && n._negative)
	{
		return false;
	}

	return true;
}

bool LongNumber::operator>(const LongNumber& n) const
{
	return !((*this) < n || (*this) == n);
}

bool LongNumber::operator<=(const LongNumber& n) const
{
	return !((*this) > n);
}

bool LongNumber::operator>=(const LongNumber& n) const
{
	return !((*this) < n);
}

std::string LongNumber::getString() const
{
	if (_data.size() == 0)
	{
		return "0";
	}

	std::string s;

	if (_negative)
	{
		s += "-";
	}

	for (long long i = _data.size() - 1; i >= 0; i--)
	{
		s += typeToChar[_data[i]];
	}

	return s;
}

bool LongNumber::_absIsLess(const LongNumber& n) const
{
	if (_data.size() < n._data.size())
	{
		return true;
	}
	else if (_data.size() > n._data.size())
	{
		return false;
	}

	for (long long i = _data.size() - 1; i >= 0; i--)
	{
		if (_data[i] < n._data[i])
		{
			return true;
		}
		else if (_data[i] > n._data[i])
		{
			return false;
		}
	}

	return false;
}

bool LongNumber::_absIsGreater(const LongNumber& n) const
{
	if (_data.size() > n._data.size())
	{
		return true;
	}
	else if (_data.size() < n._data.size())
	{
		return false;
	}

	for (long long i = _data.size() - 1; i >= 0; i--)
	{
		if (_data[i] < n._data[i])
		{
			return false;
		}
		else if (_data[i] > n._data[i])
		{
			return true;
		}
	}

	return false;
}

bool LongNumber::_absIsEqual(const LongNumber& n) const
{
	if (_data.size() != n._data.size())
	{
		return false;
	}

	return _data == n._data;
}

void LongNumber::_checkZero(LongNumber& n)
{
	long long firstNonZero = -1;

	for (long long i = n._data.size() - 1; i >= 0; i--)
	{
		if (n._data[i] != 0)
		{
			firstNonZero = i;
			break;
		}
	}

	long long shift = firstNonZero + 1LL;

	std::vector<TYPE> cropped(n._data.begin(), n._data.begin() + shift);
	n._data = std::move(cropped);
}

BIG_TYPE LongNumber::_getMaxTypeValue()
{
	return 9;
}
