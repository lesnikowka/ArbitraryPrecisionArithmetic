#include "LongNumber.h"


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

LongNumber::LongNumber(int64_t n)
{
	_negative = n < 0;
}

LongNumber::LongNumber(const std::vector<TYPE>& data, bool negative)
	: _data(data)
	, _negative(negative)
{
}

LongNumber::LongNumber(std::vector<TYPE>&& data, bool negative)
	: _data(std::move(data))
	, _negative(negative)
{
}

LongNumber& LongNumber::operator=(const LongNumber& n)
{
	_data = n._data;
	_negative = n._negative;
}

LongNumber& LongNumber::operator=(LongNumber&& n) noexcept
{
	_data = std::move(n._data);
	_negative = n._negative;
}

void LongNumber::setData(const std::vector<TYPE>& data, bool negative)
{
	_data = data;
	_negative = negative;
}

void LongNumber::setData(std::vector<TYPE>&& data, bool negative)
{
	_data = std::move(data);
	_negative = negative;
}

std::pair<std::vector<TYPE>, bool> LongNumber::getData() const
{
	return std::make_pair(_data, _negative);
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

	TYPE quot = 0;

	for (size_t i = 0; i < minSize; i++)
	{
		BIG_TYPE resVal = static_cast<BIG_TYPE>(_data[i])
			+ static_cast<BIG_TYPE>(n._data[i]) + 
			static_cast<BIG_TYPE>(quot);
		TYPE quot = resVal / _getMaxTypeValue();
		TYPE rem = resVal % _getMaxTypeValue();
		
		result._data.push_back(rem);
	}

	for (size_t i = minSize; i < _data.size(); i++)
	{
		BIG_TYPE resVal = static_cast<BIG_TYPE>(_data[i]) +
			static_cast<BIG_TYPE>(quot);
		TYPE quot = resVal / _getMaxTypeValue();
		TYPE rem = resVal % _getMaxTypeValue();
		
		result._data.push_back(rem);
	}

	for (size_t i = minSize; i < n._data.size(); i++)
	{
		BIG_TYPE resVal = static_cast<BIG_TYPE>(n._data[i]) +
			static_cast<BIG_TYPE>(quot);
		TYPE quot = resVal / _getMaxTypeValue();
		TYPE rem = resVal % _getMaxTypeValue();

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
			val = static_cast<BIG_TYPE>(curDataVal) + _getMaxTypeValue()
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

LongNumber LongNumber::operator-() const
{
	LongNumber result(*this);

	result._negative = result._negative ? false : true;

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
	return std::string();
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
	long long firstNonZero = 0;

	for (long long i = n._data.size() - 1; i >= 0; i--)
	{
		if (n._data[i] != 0)
		{
			firstNonZero = i;
			break;
		}
	}

	std::vector<TYPE> cropped(n._data.begin(), n._data.begin() + firstNonZero);
	n._data = std::move(cropped);
}

BIG_TYPE LongNumber::_getMaxTypeValue()
{
	//return static_cast<BIG_TYPE>(std::numeric_limits<TYPE>::max());
	return 9;
}
