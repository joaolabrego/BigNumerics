/******************************************************************************/
/*** File-Name: BcdNumeric.cpp	                                            ***/
/*** Author...: João da Rocha Labrego                                       ***/
/*** E-Mail...: joaolabrego@gmail.com                                       ***/
/*** Purpose..: BcdNumeric implementation.                                  ***/
/******************************************************************************/

#include "BcdNumeric.h"

/***************************************************************/
COMPARISON BcdNumeric::Compare(DOUBLE_DIGIT rightComparand) const
/***************************************************************/
{
	size_t length = data.size();
	DOUBLE_DIGIT value;

	if (!length)
		return COMPARISON::MINOR;
	if (length > 2)
		return COMPARISON::MAJOR;
	value = data.front();
	if (length == 2)
		value += (DOUBLE_DIGIT(data.back()) * DEC::BASE);
	if (value < rightComparand)
		return COMPARISON::MINOR;
	if (value > rightComparand)
		return COMPARISON::MAJOR;

	return COMPARISON::EQUAL;
}
/*****************************************************/
BcdNumeric& BcdNumeric::operator =(const string& value)
/*****************************************************/
{
	char signal;
	string wholePart,
		decimalPart;

	ToNull();
	if (General::Parse(value, signal, wholePart, decimalPart))
	{
		wholePart = General::PadLeft(wholePart);
		for (size_t i = wholePart.size(); i;)
			data.push_back(stoul(wholePart.substr(i -= DEC::DIGIT_LENGTH, DEC::DIGIT_LENGTH)));
		if (decimalPart.size() && decimalPart.front() >= '5') // to round.
			++*this;
	}

	return *this;
}
/***************************************************/
string BcdNumeric::ToString(const bool grouped) const
/***************************************************/
{
	string result;

	if (IsNull())
		return UNDEFINED;
	if (IsZero())
		return "0";
	for (size_t i = data.size(); i;)
		result.append(General::PadLeft(to_string(data.at(--i))));
	result = General::LeftTrim(result);
	if (grouped)
		return General::Agroup(result);

	return result;
}
/*************************************************/
BcdNumeric& BcdNumeric::ToValue(DOUBLE_DIGIT value)
/*************************************************/
{
	ToNull();
	do
		data.push_back(value % DEC::BASE);
	while (value /= DEC::BASE);

	return *this;
}
/********************************************************/
BcdNumeric BcdNumeric::Add(const BcdNumeric& addend) const
/********************************************************/
{
	VECTOR sum;
	size_t leftLength = data.size(),
		rightLength = addend.data.size(),
		i;
	DIGIT carry = 0;

	sum.resize(leftLength, 0);
	for (i = 0; i < rightLength; ++i)
	{
		carry += (data.at(i) + addend.data.at(i));
		sum.at(i) = carry % DEC::BASE;
		carry /= DEC::BASE;
	}
	for (; i < leftLength; ++i)
	{
		carry += data.at(i);
		sum.at(i) = carry % DEC::BASE;
		carry /= DEC::BASE;
	}
	if (carry)
		sum.push_back(carry);

	return sum;
}
/***************************************************************/
BcdNumeric BcdNumeric::operator +(const BcdNumeric& addend) const
/***************************************************************/
{
	BcdNumeric sum;
	size_t leftLength = data.size(),
		rightLength = addend.data.size();

	if (leftLength && rightLength)
	{
		if (IsZero())
			return addend;
		if (addend.IsZero())
			return *this;
		if (leftLength < rightLength)
			return BcdNumeric(addend.Add(data).data);
		sum = BcdNumeric(Add(addend.data).data);
	}

	return sum;
}
/***********************************/
BcdNumeric& BcdNumeric::operator ++()
/***********************************/
{
	size_t length = data.size(),
		i;

	if (length)
	{
		for (i = 0; i < length && data.at(i) == DEC::MAXIMUM_VALUE; ++i)
			data.at(i) = 0;
		if (i == length)
			data.push_back(1);
		else
			++data.at(i);
	}

	return *this;
}
/*******************************************************************/
BcdNumeric BcdNumeric::operator -(const BcdNumeric& subtrahend) const
/*******************************************************************/
{
	BcdNumeric difference;
	size_t leftLength = data.size(),
		rightLength = subtrahend.data.size(),
		i;
	DIGIT borrow;
	COMPARISON comparison;

	if (leftLength && rightLength && (comparison = Compare(subtrahend)) != COMPARISON::MINOR)
	{
		if (comparison == COMPARISON::EQUAL)
			return difference.ToZero();
		if (subtrahend.IsZero())
			return *this;
		borrow = 0;
		difference.data.resize(leftLength, 0);
		for (i = 0; i < rightLength; ++i)
		{
			borrow = DEC::BASE + data.at(i) - subtrahend.data.at(i) - borrow;
			difference.data.at(i) = borrow % DEC::BASE;
			borrow = borrow < DEC::BASE;
		}
		for (; i < leftLength; ++i)
		{
			borrow = DEC::BASE + data.at(i) - borrow;
			difference.data.at(i) = borrow % DEC::BASE;
			borrow = borrow < DEC::BASE;
		}
		General::Normalize(difference.data);
	}

	return difference;
}
/***********************************/
BcdNumeric& BcdNumeric::operator --()
/***********************************/
{
	size_t length = data.size(),
		i;

	if (length)
	{
		if (IsZero())
			ToNull();
		else
		{
			for (i = 0; i < length && !data.at(i); ++i)
				data.at(i) = DEC::MAXIMUM_VALUE;
			General::Normalize(data);
		}
	}

	return *this;
}
/**************************************************************/
BcdNumeric BcdNumeric::operator *(DOUBLE_DIGIT multiplier) const
/**************************************************************/
{
	size_t length = data.size();
	BcdNumeric product;
	DOUBLE_DIGIT carry;

	if (length && multiplier <= BIN::BASE)
	{
		if (IsZero() || !multiplier)
			return product.ToZero();
		if (IsOne())
			return BcdNumeric(multiplier);
		if (multiplier == 1)
			return *this;
		carry = 0;
		product.data.resize(length, 0);
		for (size_t i = 0; i < length; ++i)
		{
			carry += data.at(i) * multiplier;
			product.data.at(i) = DIGIT(carry % DEC::BASE);
			carry /= DEC::BASE;
		}
		if (carry)
			product.data.push_back(DIGIT(carry));
	}

	return product;
}
/*******************************************************************/
BcdNumeric BcdNumeric::operator *(const BcdNumeric& multiplier) const
/*******************************************************************/
{
	size_t leftLength = data.size(),
		rightLength = multiplier.data.size();
	BcdNumeric product;
	DOUBLE_DIGIT carry;

	if (leftLength && rightLength)
	{
		if (IsZero() || multiplier.IsZero())
			return product.ToZero();
		if (IsOne())
			return multiplier;
		if (multiplier.IsOne())
			return *this;
		carry = 0;
		product.data.resize(leftLength + rightLength, 0);
		for (size_t i = 0, k; i < leftLength; ++i)
		{
			k = i;
			carry = 0;
			for (size_t j = 0; j < rightLength; ++j)
			{
				carry = DOUBLE_DIGIT(data.at(i)) * multiplier.data.at(j) + product.data.at(k) + carry;
				product.data.at(k++) = carry % DEC::BASE;
				carry /= DEC::BASE;
			}
			while (carry)
			{
				carry += product.data.at(k);
				product.data.at(k++) = carry % DEC::BASE;
				carry /= DEC::BASE;
			}
		}
		General::Normalize(product.data);
	}

	return product;
}
/*******************************************************************************/
BcdNumeric BcdNumeric::Divide(const DOUBLE_DIGIT divisor, DIGIT& remainder) const
/*******************************************************************************/
{
	size_t length = data.size();
	DOUBLE_DIGIT partialDividend;
	BcdNumeric quotient,
		comparand;
	COMPARISON comparison;

	if (length && divisor && divisor <= BIN::BASE)
	{
		if (IsZero() || divisor == 1)
			return (remainder = 0, *this);
		if ((comparison = Compare(divisor)) == COMPARISON::MINOR)
			return (remainder = data.front(), quotient.ToZero());
		if (comparison == COMPARISON::EQUAL)
			return (remainder = 0, quotient.ToOne());
		partialDividend = 0;
		for (size_t i = length; i;)
		{
			partialDividend = partialDividend * DEC::BASE + data.at(--i);
			if (partialDividend < divisor)
			{
				if (!quotient.IsNull())
					quotient.data.insert(quotient.data.begin(), 0);
				remainder = DIGIT(partialDividend);
			}
			else
			{
				quotient.data.insert(quotient.data.begin(), DIGIT(partialDividend / divisor));
				remainder = DIGIT(partialDividend %= divisor);
			}
		}
		General::Normalize(quotient.data);
	}

	return quotient;
}
/*****************************************************************************************/
DIGIT BcdNumeric::BinarySearchDigit(const BcdNumeric& divisor, BcdNumeric& remainder) const
/*****************************************************************************************/
{
	COMPARISON comparison;
	BcdNumeric product;
	DIGIT digit,
		minimumDigit = 0,
		maximumDigit = DEC::MAXIMUM_VALUE;

	do
	{
		digit = (minimumDigit + maximumDigit) >> 0x1;
		product = divisor * digit;
		comparison = product.Compare(*this);
		if (comparison == COMPARISON::MINOR)
			minimumDigit = digit + 1;
		else if (comparison == COMPARISON::EQUAL)
		{
			maximumDigit = digit;
			break;
		}
		else
			maximumDigit = digit - 1;
	} while (minimumDigit <= maximumDigit);
	if (digit != maximumDigit)
	{
		digit = maximumDigit;
		product = divisor * digit;
	}
	remainder = *this - product;

	return digit;
}
/***********************************************************************************/
BcdNumeric BcdNumeric::Divide(const BcdNumeric& divisor, BcdNumeric& remainder) const
/***********************************************************************************/
{
	size_t length = data.size();
	BcdNumeric partialDividend,
		quotient;
	COMPARISON comparison;

	if (length && divisor.data.size() && !divisor.IsZero())
	{
		if (IsZero() || divisor.IsOne())
			return (remainder.ToZero(), *this);
		if ((comparison = Compare(divisor)) == COMPARISON::MINOR)
			return (remainder = *this, quotient.ToZero());
		if (comparison == COMPARISON::EQUAL)
			return (remainder.ToZero(), quotient.ToOne());
		for (size_t i = length; i;)
		{
			partialDividend.data.insert(partialDividend.data.begin(), data.at(--i));
			comparison = partialDividend.Compare(divisor);
			if (comparison == COMPARISON::MINOR)
			{
				if (quotient.data.size())
					quotient.data.insert(quotient.data.begin(), 0);
				remainder = partialDividend;
			}
			else if (comparison == COMPARISON::EQUAL)
			{
				partialDividend.ToNull();
				quotient.data.insert(quotient.data.begin(), 1);
				remainder.ToZero();
			}
			else
			{
				quotient.data.insert(quotient.data.begin(), partialDividend.BinarySearchDigit(divisor, remainder));
				partialDividend = remainder;
			}
		}
		General::Normalize(quotient.data);
	}

	return quotient;
}
/*******************************************************/
BcdNumeric BcdNumeric::Power(DOUBLE_DIGIT exponent) const
/*******************************************************/
{
	BcdNumeric base = *this,
		potency;

	if (base.data.size() && (exponent || !base.IsZero()))
	{
		if (!exponent)
			return potency.ToOne();
		if (exponent == 1 || base.IsZero() || base.IsOne())
			return base;
		potency.ToOne();
		for (;;)
		{
			if (exponent & 0x1)
			{
				potency *= base;
				if (exponent == 1)
					break;
			}
			base *= base;
			exponent >>= 1;
		}
	}

	return potency;
}
/****************************************************************************************************/
void BcdNumeric::BinarySearchDigit(DOUBLE_DIGIT degree, BcdNumeric& root, BcdNumeric& remainder) const
/****************************************************************************************************/
{
	BcdNumeric potency;
	DIGIT minimum = 0,
		maximum = DEC::BASE,
		digit;
	COMPARISON comparison;

	root.data.insert(root.data.begin(), DEC::BASE - 1);
	if (root.Power(degree) < *this)
		remainder.ToZero();
	else
	{
		root.data.front() = 0;
		do
		{
			digit = DIGIT((DOUBLE_DIGIT(minimum) + maximum) >> 1);
			root.data.front() = digit;
			potency = root.Power(degree);
			comparison = potency.Compare(*this);
			if (comparison == COMPARISON::MINOR)
				minimum = digit + 1;
			else if (comparison == COMPARISON::EQUAL)
			{
				maximum = digit;
				break;
			}
			else
				maximum = digit - 1;
		} while (minimum <= maximum);
		if (digit != maximum)
		{
			root.data.front() = maximum;
			potency = root.Power(degree);
		}
		remainder = *this - potency;
	}
}
/***************************************************************************/
BcdNumeric BcdNumeric::Root(DOUBLE_DIGIT degree, BcdNumeric& remainder) const
/***************************************************************************/
{
	BcdNumeric root,
		potency,
		partialRadicand;
	size_t length = data.size(),
		lengthGroup;

	if (degree > 0 && data.size())
	{
		if (degree == 1 || IsZero() || IsOne())
			return (remainder.ToZero(), *this);
		if (!(lengthGroup = length % degree))
			lengthGroup = degree;
		for (size_t i = length; i > 0;)
		{
			i -= lengthGroup;
			partialRadicand.data.insert(partialRadicand.data.begin(), data.begin() + i, data.begin() + i + lengthGroup);
			lengthGroup = degree;
			partialRadicand.BinarySearchDigit(degree, root, remainder);
		}
	}

	return root;
}
/******************************************************************************/
BcdNumeric BcdNumeric::CalculateGCD(BcdNumeric leftValue, BcdNumeric rightValue)
/******************************************************************************/
{
	BcdNumeric quotient,
		remainder;
	while (quotient = leftValue.Divide(rightValue, remainder), !remainder.IsZero())
	{
		leftValue = rightValue;
		rightValue = remainder;
	}

	return rightValue;
}
/************************************************************/
BcdNumeric BcdNumeric::GCD(const BcdNumeric& rightValue) const
/************************************************************/
{
	BcdNumeric result;
	COMPARISON comparison;

	if (data.size() && rightValue.data.size())
	{
		if (IsZero())
			return rightValue;
		if (rightValue.IsZero() || (comparison = Compare(rightValue)) == COMPARISON::EQUAL)
			return *this;
		if (comparison == COMPARISON::MINOR)
			result = CalculateGCD(rightValue, *this);
		else
			result = CalculateGCD(*this, rightValue);
	}

	return result;
}
/************************************************************/
BcdNumeric BcdNumeric::LCM(const BcdNumeric& rightValue) const
/************************************************************/
{
	BcdNumeric result,
		remainder,
		gcd;

	if (data.size() && rightValue.data.size())
	{
		if (IsZero() || rightValue.IsZero())
			return result.ToZero();
		if (Compare(rightValue) == COMPARISON::MINOR)
			gcd = CalculateGCD(rightValue, *this);
		else
			gcd = CalculateGCD(*this, rightValue);
		result = (*this * rightValue).Divide(gcd, remainder);
	}

	return result;
}
/**************************************/
BcdNumeric BcdNumeric::Factorial() const
/**************************************/
{
	BcdNumeric value = *this,
		result;

	if (value.data.size())
	{
		result.ToOne();
		if (!value.IsZero())
			while (!value.IsOne())
				result *= value--;
	}

	return result;
}

/***********/
/*** EOF ***/
/***********/