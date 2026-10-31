/******************************************************************************/
/*** File-Name: General.cpp                                                 ***/
/*** Author...: João da Rocha Labrego                                       ***/
/*** E-Mail...: joaolabrego@gmail.com                                       ***/
/*** Purpose..: General implementation.                                     ***/
/******************************************************************************/

#include "General.h"

/*************************************/
SIGN General::ToSign(const char signal)
/*************************************/
{
	return signal == char(SIGNAL::NEGATIVE) ? SIGN::NEGATIVE : SIGN::POSITIVE;
}
/***************************************/
SIGN General::ToSign(const SIGNAL signal)
/***************************************/
{
	return signal == SIGNAL::NEGATIVE ? SIGN::NEGATIVE : SIGN::POSITIVE;
}
/*****************************************/
SIGNAL General::ToSignal(const char signal)
/*****************************************/
{
	return signal == char(SIGNAL::NEGATIVE) ? SIGNAL::NEGATIVE : SIGNAL::POSITIVE;
}
/***************************************/
SIGNAL General::ToSignal(const SIGN sign)
/***************************************/
{
	return sign == SIGN::NEGATIVE ? SIGNAL::NEGATIVE : SIGNAL::POSITIVE;
}
/************************************/
SIGN General::NotSign(const SIGN sign)
/************************************/
{
	return sign == SIGN::NEGATIVE ? SIGN::POSITIVE : SIGN::NEGATIVE;
}
/********************************************/
SIGNAL General::NotSignal(const SIGNAL signal)
/********************************************/
{
	return signal == SIGNAL::NEGATIVE ? SIGNAL::POSITIVE : SIGNAL::NEGATIVE;
}
/***************************************/
bool General::IsSignal(const char signal)
/***************************************/
{
	return signal == char(SIGNAL::NEGATIVE) || signal == char(SIGNAL::POSITIVE);
}
/************************************************************/
COMPARISON General::NotComparison(const COMPARISON comparison)
/************************************************************/
{
	return comparison == COMPARISON::MINOR ? COMPARISON::MAJOR : comparison == COMPARISON::MAJOR ? COMPARISON::MINOR : COMPARISON::EQUAL;
}
/********************************************/
size_t General::GetPrecision(bool ignoreExtra)
/********************************************/
{
	return DECIMAL_PLACES + (counterPrecision && !ignoreExtra ? DECIMAL_PLACES * 2 : 1);
}
/*************************/
void General::UpPrecision()
/*************************/
{
	++counterPrecision;
}
/***************************/
void General::DownPrecision()
/***************************/
{
	if (counterPrecision) 
		--counterPrecision;
}
/*******************************************************************************/
string General::PadLeft(const string& value, size_t length, const string& filler)
/*******************************************************************************/
{
	size_t missingZeroes = length - value.size() % length;

	if (missingZeroes < length)
		return Replicate(filler, missingZeroes).append(value);

	return value;
}
/********************************************************************************/
string General::PadRight(const string& value, size_t length, const string& filler)
/********************************************************************************/
{
	size_t missingZeroes = length - value.size() % length;
	if (missingZeroes < length)
		return Replicate(filler, missingZeroes).insert(0, value);

	return value;
}
/*******************************************/
string General::LeftTrim(const string& value)
/*******************************************/
{
	size_t length = value.size(),
		i;

	for (i = 0; i < length && value.at(i) == '0'; i++);
	if (i == length)
		return "0";
	else if (i)
		return value.substr(i);

	return value;
}
/********************************************/
string General::RightTrim(const string& value)
/********************************************/
{
	size_t length = value.size(),
		i;

	for (i = length; i && value.at(i - 1) == '0'; i--);
	if (!i)
		return "";
	else if (i < length)
		return value.substr(0, i);

	return value;
}
/*****************************************/
string General::Agroup(const string& value)
/*****************************************/
{
	string result;
	size_t i = value.size();

	while (i > THOUSANDS_LENGTH)
		result.insert(0, string(1, THOUSANDS_SEPARATOR).append(value.substr(i -= THOUSANDS_LENGTH, THOUSANDS_LENGTH)));

	return result.insert(0, value.substr(0, i));
}
/******************************************************************************************/
string General::Replace(const string& value, const string& oldValue, const string& newValue)
/******************************************************************************************/
{
	size_t length = value.size(),
		oldLength = oldValue.size();
	string result;

	if (length && oldLength && oldLength <= length)
	{
		for (size_t i = 0; i < length;)
		{
			if (value.substr(i, oldLength) == oldValue)
			{
				result.append(newValue);
				i += oldLength;
			}
			else
				result.push_back(value.at(i++));
		}
	}
	else
		result = value;

	return result;
}
/****************************************************************/
string General::Replicate(const string& value, const size_t count)
/****************************************************************/
{
	size_t lengthvalue = value.size(),
		limit;
	string result;

	if (lengthvalue && count)
	{
		limit = count / lengthvalue;
		for (size_t i = 0; i < limit; ++i) result.append(value);
		result.append(value.substr(0, count % lengthvalue));
	}

	return result;
}
/******************************************/
bool General::IsNumeric(const string& value)
/******************************************/
{
	size_t length = value.size(),
		i;

	for (i = 0; i < length && isdigit(value.at(i)); ++i);

	return i == length;
}
/************************************/
void General::Decrement(string& value)
/************************************/
{
	size_t i;

	for (i = value.size(); i && value.at(i - 1) == '0'; i--)
		value.at(i - 1) = '9';
	--value.at(i - 1);
}
/*************************************************************************************************************************************
The complete format of a numeric string is "-999,999,999,999...". 
The '-' character can also be '+' and, if it is '+', it is optional.
The character ',' is optional and separates thousands.
The character '.' it is also optional and separates the decimal places.
Finally, the 3 consecutive points, also optional, inform that the expression represents a periodic tithe.
For the expression to be considered a periodic tithe, it is necessary to repeat at least 3 times its period, followed by the ellipsis.
*************************************************************************************************************************************/
/*************************************************************************************/
bool General::Parse(string value, char& signal, string& wholePart, string& decimalPart)
/*************************************************************************************/
{
	size_t decimalPosition,
		lengthDecimals,
		precision;
	bool ellipsed;
	string repetend;

	if (value.empty())
		return false;
	signal = value.front();
	if (General::IsSignal(signal))
		value.erase(0, 1);
	else if (isdigit(signal))
		signal = char(SIGNAL::POSITIVE);
	else
		return false;
	decimalPosition = value.find(DECIMAL_SEPARATOR);
	if (decimalPosition == string::npos)
	{
		wholePart = LeftTrim(Replace(value, string(1, THOUSANDS_SEPARATOR), ""));
		decimalPart.clear();
	}
	else if (decimalPosition && decimalPosition < value.size() - 1)
	{
		wholePart = LeftTrim(Replace(value.substr(0, decimalPosition), string(1, THOUSANDS_SEPARATOR), ""));
		lengthDecimals = (decimalPart = Replace(RightTrim(value.substr(decimalPosition + 1)), ELLIPSIS, "#")).size();
		if (ellipsed = decimalPart.back() == '#')
		{
			decimalPart.pop_back();
			--lengthDecimals;
		}
		if (!IsNumeric(decimalPart))
			return false;
		precision = General::GetPrecision() * DEC::DIGIT_LENGTH;
		if (ellipsed || lengthDecimals > DECIMAL_PLACES * DEC::DIGIT_LENGTH)
		{
			if ((repetend = Repetend(decimalPart)).empty())
			{
				if (lengthDecimals < precision)
				{
					Decrement(decimalPart);
					decimalPart = PadRight(decimalPart, precision, "9");
				}
			}
			else
				decimalPart = PadRight(decimalPart, precision, repetend);
		}
		else if (lengthDecimals < precision)
			decimalPart = PadRight(decimalPart, DEC::DIGIT_LENGTH, "0");
		decimalPart = decimalPart.substr(0, precision);
	}
	else
		return false;

	return IsNumeric(wholePart);
}
/******************************************************************/
size_t General::CountDigits(const string& value, const string digit)
/******************************************************************/
{
	size_t lengthDigit = digit.size(),
		counter = 0;

	for (size_t i = value.size(); i >= lengthDigit && value.substr(i - lengthDigit, lengthDigit) == digit; i -= lengthDigit)
		++counter;
	return counter;
}
/*******************************************/
string General::Repetend(const string& value)
/*******************************************/
{
	string digit,
		result;
	size_t lastcounter = 0,
		counter,
		length = value.size();

	if (length)
	{
		for (size_t i = length - 1; i > 0; i--)
		{
			counter = CountDigits(value, digit = value.substr(i, length - i));
			if (counter >= THOUSANDS_LENGTH && counter >= lastcounter)
			{
				lastcounter = counter;
				result = digit;
			}
		}
	}
	return result;
}
/****************************************************************/
VECTOR General::Repetend(const string& value, const size_t times)
/****************************************************************/
{
	string repetend = Repetend(value);
	VECTOR result;

	if (repetend.empty())
		return VECTOR(GetPrecision() * times, 0);
	repetend = General::PadRight(repetend, GetPrecision() * DEC::DIGIT_LENGTH * times, repetend);
	for (size_t i = repetend.size(); i;)
		result.push_back(stoull(repetend.substr(i -= DEC::DIGIT_LENGTH, DEC::DIGIT_LENGTH)));

	return result;
}
/************************************************************************************/
COMPARISON General::Compare(const VECTOR& leftComparand, const VECTOR& rightComparand)
/************************************************************************************/
{
	size_t leftLength = leftComparand.size(),
		rightLength = rightComparand.size();
	DIGIT left,
		right;

	if (leftLength < rightLength)
		return COMPARISON::MINOR;
	if (leftLength > rightLength)
		return COMPARISON::MAJOR;
	for (size_t i = leftLength; i;)
	{
		left = leftComparand.at(--i);
		right = rightComparand.at(i);
		if (left < right)
			return COMPARISON::MINOR;
		if (left > right)
			return COMPARISON::MAJOR;
	}

	return COMPARISON::EQUAL;
}
/************************************/
void General::Normalize(VECTOR& value)
/************************************/
{
	size_t length = value.size(),
		i;

	if (length)
	{
		for (i = length; i && !value.at(i - 1); --i);
		if (i)
		{
			if (i < length)
				value.erase(value.begin() + i, value.end());
		}
		else
			ToZero(value);
	}
}
/*******************************************************************************/
VECTOR General::SubVector(const VECTOR& value, const size_t start, size_t length)
/*******************************************************************************/
{
	size_t lengthValue = value.size(),
		end;
	VECTOR result;

	if (lengthValue && start < lengthValue)
	{
		if (length == string::npos)
			length = value.size();
		if ((end = start + length) > lengthValue)
			result.assign(value.begin() + start, value.end());
		else
			result.assign(value.begin() + start, value.begin() + end);
	}
	
	return result;
}
/**********************************************************/
VECTOR& General::Assign(VECTOR& target, const VECTOR& value)
/**********************************************************/
{
	return (target.assign(value.begin(), value.end()), target);
}
/********************************************************/
VECTOR& General::Insert(VECTOR& target, const DIGIT value)
/********************************************************/
{
	return (target.insert(target.begin(), value), target);
}
/****************************************************************************/
VECTOR& General::Insert(VECTOR& target, const size_t count, const DIGIT value)
/****************************************************************************/
{
	return (target.insert(target.begin(), count, value), target);
}
/**********************************************************/
VECTOR& General::Insert(VECTOR& target, const VECTOR& value)
/**********************************************************/
{
	return (target.insert(target.begin(), value.begin(), value.end()), target);
}
/********************************************************/
VECTOR& General::Append(VECTOR& target, const DIGIT value)
/********************************************************/
{
	return (target.push_back(value), target);
}
/****************************************************************************/
VECTOR& General::Append(VECTOR& target, const size_t count, const DIGIT value)
/****************************************************************************/
{
	return (target.insert(target.end(), count, value), target);
}
/**********************************************************/
VECTOR& General::Append(VECTOR& target, const VECTOR& value)
/**********************************************************/
{
	return (target.insert(target.end(), value.begin(), value.end()), target);
}
/*************************************/
VECTOR& General::ToZero(VECTOR& target)
/*************************************/
{
	return (target.assign(ZERO.begin(), ZERO.end()), target);
}
/************************************/
VECTOR& General::ToOne(VECTOR& target)
/************************************/
{
	return (target.assign(ONE.begin(), ONE.end()), target);
}
/***************************************/
bool General::IsZero(const VECTOR& value)
/***************************************/
{
	return value.size() == 1 && !value.front();
}
/*********************************************/
bool General::IsNullOrZero(const VECTOR& value)
/*********************************************/
{
	return value.empty() || (value.size() == 1 && !value.front());
}
/**************************************/
bool General::IsOne(const VECTOR& value)
/**************************************/
{
	return value.size() == 1 && value.front() == 1;
}
/**************************************/
bool General::IsOdd(const VECTOR& value)
/**************************************/
{
	return value.size() && (value.front() & 0x1);
}
/***************************************/
bool General::IsEven(const VECTOR& value)
/***************************************/
{
	return value.size() && !(value.front() & 0x1);
}
/******************************/
string General::NegativeSignal()
/******************************/
{
	string result = std::use_facet<std::moneypunct<char>>(std::locale("")).negative_sign();

	return result.empty() ? "-" : result;
}
/******************************/
string General::PositiveSignal()
/******************************/
{
	string result = std::use_facet<std::moneypunct<char>>(std::locale("")).positive_sign();

	return result.empty() ? "+" : result;
}
/********************************/
string General::DecimalSeparator()
/********************************/
{
	string result = string(1, std::use_facet< std::numpunct<char> >(std::locale("")).decimal_point());

	return result.empty() ? "," : result;
}
/*********************************/
string General::ThousandSeparator()
/*********************************/
{
	string result = string(1, std::use_facet<std::numpunct<char>>(std::locale("")).thousands_sep());

	return result.empty() ? "." : result;
}
/******************************/
size_t General::ThousandLength()
/******************************/
{
	size_t result = std::use_facet<std::numpunct<char>>(std::locale("")).grouping().front();

	return result;
}
/********************************/
bool General::ShowPositiveSignal()
/********************************/
{
	return std::use_facet<std::moneypunct<char>>(std::locale("")).positive_sign().size();
}
/*************************/
bool General::ShowGrouped()
/*************************/
{
	return string(1, std::use_facet<std::numpunct<char>>(std::locale("")).thousands_sep()).size();
}

/***********/
/*** EOF ***/
/***********/