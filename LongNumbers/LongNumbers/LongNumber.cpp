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

LongNumber::LongNumber(LongNumber&& n)
	: _data(std::move(n._data))
	, _negative(n._negative)
{
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

LongNumber& LongNumber::operator=(LongNumber&& n)
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

	// for all positive or all negative

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

bool LongNumber::operator==(const LongNumber& n)
{
	return _negative == n._negative && _data == n._data;
}

std::string LongNumber::getString() const
{
	return std::string();
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
	return static_cast<BIG_TYPE>(std::numeric_limits<TYPE>::max());
}