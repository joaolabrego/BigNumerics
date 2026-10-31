/******************************************************************************/
/*** File-Name: BigNumeric.cpp	                                            ***/
/*** Author...: João da Rocha Labrego                                       ***/
/*** E-Mail...: joaolabrego@gmail.com                                       ***/
/*** Purpose..: BigNumeric implementation.                                  ***/
/******************************************************************************/

#include "BigNumeric.h"

/*************************************************/
BigNumeric& BigNumeric::ToValue(DOUBLE_DIGIT value)
/*************************************************/
{
	data.clear();
	data.push_back(DIGIT(value));
	if (value >>= BIN::DIGIT_LENGTH)
		data.push_back(DIGIT(value));

	return *this;
}
/*****************************************************/
BigNumeric& BigNumeric::operator =(const string& value)
/*****************************************************/
{
	DIGIT digit;
	BcdNumeric whole = value;

	data.clear();
	if (whole.AsVector().size())
	{
		do
		{
			whole = whole.Divide(BIN::BASE, digit);
			data.push_back(digit);
		} while (!whole.IsZero());
	}

	return *this;
}
/***************************************************/
string BigNumeric::ToString(const bool grouped) const
/***************************************************/
{
	BigNumeric value = *this;
	string result;
	DIGIT remainder;

	if (data.empty())
		return UNDEFINED;
	if (IsZero())
		return "0";
	do
	{
		value = value.Divide(DEC::BASE, remainder);
		result.insert(0, General::PadLeft(to_string(remainder)));
	} while (!value.IsZero());
	result = General::LeftTrim(result);
	if (grouped)
		result = General::Agroup(result);

	return result;
}
/***************************************************************/
COMPARISON BigNumeric::Compare(DOUBLE_DIGIT rightComparand) const
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
		value += (DOUBLE_DIGIT(data.back()) << BIN::DIGIT_LENGTH);
	if (value < rightComparand)
		return COMPARISON::MINOR;
	if (value > rightComparand)
		return COMPARISON::MAJOR;

	return COMPARISON::EQUAL;
}
/********************************************************/
BigNumeric BigNumeric::Add(const BigNumeric& addend) const
/********************************************************/
{
	BigNumeric sum;
	size_t leftLength = data.size(),
		rightLength = addend.data.size(),
		i;
	DOUBLE_DIGIT carry = 0;

	sum.data.resize(leftLength, 0);
	for (i = 0; i < rightLength; i++)
	{
		carry += (DOUBLE_DIGIT(data.at(i)) + addend.data.at(i));
		sum.data.at(i) = (DIGIT)carry;
		carry >>= BIN::DIGIT_LENGTH;
	}
	for (; i < leftLength; i++)
	{
		carry += DOUBLE_DIGIT(data.at(i));
		sum.data.at(i) = (DIGIT)carry;
		carry >>= BIN::DIGIT_LENGTH;
	}
	if (carry)
		sum.data.push_back(DIGIT(carry));

	return sum;
}
/***************************************************************/
BigNumeric BigNumeric::operator +(const BigNumeric& addend) const
/***************************************************************/
{
	BigNumeric augend = *this,
		sum;
	size_t leftLength,
		rightLength;
	DOUBLE_DIGIT carry = 0;

	if ((leftLength = augend.data.size()) && (rightLength = addend.data.size()))
	{
		if (augend.IsZero())
			return addend;
		if (addend.IsZero())
			return augend;
		sum = leftLength < rightLength ? addend.Add(augend) : augend.Add(addend);
	}

	return sum;
}
/***********************************/
BigNumeric& BigNumeric::operator ++()
/***********************************/
{
	size_t length = data.size(),
		i;

	if (length)
	{
		for (i = 0; i < length && data.at(i) == BIN::MAXIMUM_VALUE; i++)
			data.at(i) = 0;
		if (i == length)
			data.push_back(1);
		else
			++data.at(i);
	}

	return *this;
}
/*******************************************************************/
BigNumeric BigNumeric::operator -(const BigNumeric& subtrahend) const
/*******************************************************************/
{
	BigNumeric minuend = *this,
		difference;
	size_t leftLength = minuend.data.size(),
		rightLength = subtrahend.data.size(),
		i;
	COMPARISON comparison;
	DOUBLE_DIGIT borrow = 0;

	if (leftLength && rightLength && (comparison = minuend.Compare(subtrahend)) != COMPARISON::MINOR)
	{
		if (comparison == COMPARISON::EQUAL)
			return difference.ToZero();
		if (subtrahend.IsZero())
			return minuend;
		difference.data.resize(leftLength, 0);
		for (i = 0; i < rightLength; i++)
		{
			borrow = DOUBLE_DIGIT(minuend.data.at(i)) - subtrahend.data.at(i) - borrow;
			difference.data.at(i) = DIGIT(borrow);
			borrow = (borrow >> BIN::DIGIT_LENGTH) & 0x1;
		}
		for (; i < leftLength; i++)
		{
			borrow = DOUBLE_DIGIT(minuend.data.at(i)) - borrow;
			difference.data.at(i) = DIGIT(borrow);
			borrow = (borrow >> BIN::DIGIT_LENGTH) & 0x1;
		}
		General::Normalize(difference.data);
	}

	return difference;
}
/***********************************/
BigNumeric& BigNumeric::operator --()
/***********************************/
{
	size_t length = data.size(),
		i;

	if (length)
	{
		if (IsZero())
			data.clear();
		else
		{
			for (i = 0; i < length && !data.at(i); i++)
				data.at(i) = BIN::MAXIMUM_VALUE;
			if (!(--data.at(i)))
				General::Normalize(data);
		}
	}

	return *this;
}
/**************************************************************/
BigNumeric BigNumeric::operator *(DOUBLE_DIGIT multiplier) const
/**************************************************************/
{
	BigNumeric product;
	size_t length = data.size();
	DOUBLE_DIGIT carry;

	if (length && multiplier <= BIN::BASE)
	{
		if (!multiplier || IsZero())
			return product.ToZero();
		if (IsOne())
			return product = multiplier;
		if (multiplier == 1)
			return *this;
		carry = 0;
		product.data.resize(length, 0);
		for (size_t i = 0; i < length; i++)
		{
			carry += DOUBLE_DIGIT(data.at(i)) * multiplier;
			product.data.at(i) = DIGIT(carry);
			carry >>= BIN::DIGIT_LENGTH;
		}
		if (carry)
			product.data.push_back(DIGIT(carry));
	}

	return product;
}
/*******************************************************************/
BigNumeric BigNumeric::operator *(const BigNumeric& multiplier) const
/*******************************************************************/
{
	BigNumeric multiplicand = *this,
		product;
	size_t leftLength = multiplicand.data.size(),
		rightLength = multiplier.data.size();
	DOUBLE_DIGIT carry;

	if (leftLength && rightLength)
	{
		if (multiplicand.IsZero() || multiplier.BigNumeric::IsZero())
			return product.ToZero();
		if (multiplicand.IsOne())
			return multiplier;
		if (multiplier.IsOne())
			return multiplicand;
		product.data.resize(leftLength + rightLength, 0);
		for (size_t i = 0, k; i < rightLength; i++)
		{
			k = i;
			carry = 0;
			for (size_t j = 0; j < leftLength; j++)
			{
				carry = DOUBLE_DIGIT(multiplier.data.at(i)) * DOUBLE_DIGIT(multiplicand.data.at(j)) + DOUBLE_DIGIT(product.data.at(k)) + carry;
				product.data.at(k++) = DIGIT(carry);
				carry >>= BIN::DIGIT_LENGTH;
			}
			while (carry)
			{
				carry += DOUBLE_DIGIT(product.data.at(k));
				product.data.at(k++) = DIGIT(carry);
				carry >>= BIN::DIGIT_LENGTH;
			}
		}
		General::Normalize(product.data);
	}

	return product;
}
/*******************************************************************************/
BigNumeric BigNumeric::Divide(const DOUBLE_DIGIT divisor, DIGIT& remainder) const
/*******************************************************************************/
{
	BigNumeric dividend = *this,
		quotient;
	size_t length = dividend.data.size();
	DOUBLE_DIGIT partialDividend = 0;

	remainder = 0;
	if (length && divisor && divisor <= BIN::BASE)
	{
		if (divisor == 1 || dividend.IsZero())
			return dividend;
		if (dividend.Compare(divisor) == COMPARISON::EQUAL)
			return quotient.ToOne();
		quotient.data.resize(length, 0);
		for (size_t i = length; i;)
		{
			i--;
			partialDividend = (partialDividend << BIN::DIGIT_LENGTH) | dividend.data.at(i);
			if (partialDividend >= divisor)
			{
				quotient.data.at(i) = DIGIT(partialDividend / divisor);
				partialDividend %= DOUBLE_DIGIT(divisor);
			}
		}
		remainder = DIGIT(partialDividend);
		General::Normalize(quotient.data);
	}

	return quotient;
}
/*****************************************************************************************/
DIGIT BigNumeric::BinarySearchDigit(const BigNumeric& divisor, BigNumeric& remainder) const
/*****************************************************************************************/
{
	BigNumeric product;
	DIGIT minimum = 0,
		maximum = BIN::MAXIMUM_VALUE,
		digit;
	COMPARISON comparison;

	if (Compare(product = divisor * maximum) != COMPARISON::MINOR)
		return (remainder = *this - product, maximum);
	do
	{
		digit = DIGIT((DOUBLE_DIGIT(minimum) + maximum) >> 1);
		product = divisor * digit;
		comparison = product.Compare(*this);
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
		product = divisor * maximum;
	remainder = *this - product;

	return maximum;
}
/***********************************************************************************/
BigNumeric BigNumeric::Divide(const BigNumeric& divisor, BigNumeric& remainder) const
/***********************************************************************************/
{
	BigNumeric dividend = *this,
		quotient;
	size_t leftLength = dividend.data.size(),
		rightLength = divisor.data.size();
	COMPARISON comparison;

	if (leftLength && rightLength && !divisor.IsZero())
	{
		if (dividend.IsZero() || divisor.IsOne())
			return (remainder.ToZero(), dividend);
		if ((comparison = dividend.Compare(divisor)) == COMPARISON::EQUAL)
			return (remainder.ToZero(), quotient.ToOne());
		if (comparison == COMPARISON::MINOR)
			return (remainder = dividend, quotient.ToZero());
		remainder.data.clear();
		quotient.data.resize(leftLength, 0);
		for (size_t i = leftLength; i;)
		{
			i--;
			remainder.data.insert(remainder.data.begin(), dividend.data.at(i));
			comparison = remainder.Compare(divisor);
			if (comparison == COMPARISON::EQUAL)
			{
				quotient.data.at(i) = 1;
				remainder.data.clear();
			}
			else if (comparison == COMPARISON::MAJOR)
				quotient.data.at(i) = remainder.BinarySearchDigit(divisor, remainder);
		}
		General::Normalize(quotient.data);
	}
	else
		remainder.data.clear();

	return quotient;
}
/*******************************************************/
BigNumeric BigNumeric::Power(DOUBLE_DIGIT exponent) const
/*******************************************************/
{
	BigNumeric base = *this,
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
/**********************************************************************************************************/
void BigNumeric::BinarySearchDigit(const DOUBLE_DIGIT degree, BigNumeric& root, BigNumeric& remainder) const
/**********************************************************************************************************/
{
	BigNumeric radicand,
		potency,
		difference;
	DOUBLE_DIGIT minimum = 0,
		maximum = BIN::MAXIMUM_VALUE,
		digit;
	COMPARISON comparison;

	root.data.insert(root.data.begin(), 0);
	radicand = root.Power(degree);
	if (remainder.IsNullOrZero())
		remainder.data.assign(data.begin(), data.end());
	else
		remainder.data.insert(remainder.data.begin(), data.begin(), data.end());
	do
	{
		digit = root.data.front() = DIGIT((minimum + maximum) >> 1);
		potency = root.Power(degree);
		difference = potency - radicand;
		comparison = difference.Compare(remainder);
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
		root.data.front() = DIGIT(maximum);
		difference = root.Power(degree) - radicand;
	}
	remainder -= difference;
}
// https://en.wikipedia.org/wiki/Shifting_nth_root_algorithm#Algorithm
/*********************************************************************************/
BigNumeric BigNumeric::Root(const DOUBLE_DIGIT degree, BigNumeric& remainder) const
/*********************************************************************************/
{
	BigNumeric rooting = *this,
		root;
	size_t length = rooting.data.size(),
		lengthGroup;

	if (degree && length)
	{
		if (degree == 1 || rooting.IsZero() || rooting.IsOne())
			return (remainder.ToZero(), rooting);
		if (!(lengthGroup = length % degree))
			lengthGroup = size_t(degree);
		remainder.data.clear();
		for (size_t i = length; i > 0;)
		{
			i -= lengthGroup;
			BigNumeric(General::SubVector(rooting.data, i, lengthGroup)).BinarySearchDigit(degree, root, remainder);
			lengthGroup = size_t(degree);
		}
	}
	else
		remainder.data.clear();

	return root;
}
/******************************************************************************/
BigNumeric BigNumeric::CalculateGCD(BigNumeric leftValue, BigNumeric rightValue)
/******************************************************************************/
{
	BigNumeric quotient,
		remainder;
	while (quotient = leftValue.Divide(rightValue, remainder), !remainder.IsZero())
	{
		leftValue = rightValue;
		rightValue = remainder;
	}

	return rightValue;
}
/************************************************************/
BigNumeric BigNumeric::GCD(const BigNumeric& rightValue) const
/************************************************************/
{
	BigNumeric leftValue = *this,
		quotient,
		remainder,
		result;
	COMPARISON comparison;

	if (leftValue.data.size() && rightValue.data.size())
	{
		if (leftValue.IsZero())
			return rightValue;
		if (rightValue.IsZero())
			return leftValue;
		if ((comparison = leftValue.Compare(rightValue)) == COMPARISON::EQUAL)
			return leftValue;
		result = comparison == COMPARISON::MINOR ? CalculateGCD(rightValue, leftValue) : CalculateGCD(leftValue, rightValue);
	}

	return result;
}
/************************************************************/
BigNumeric BigNumeric::LCM(const BigNumeric& rightValue) const
/************************************************************/
{
	BigNumeric leftValue = *this,
		remainder,
		gcd,
		result;

	if (leftValue.data.size() && rightValue.data.size())
	{
		if (leftValue.IsZero() || rightValue.IsZero())
			return result.ToZero();
		gcd = leftValue < rightValue ? CalculateGCD(rightValue, leftValue) : CalculateGCD(leftValue, rightValue);
		return gcd.IsZero() ? result.ToZero() : leftValue * rightValue / gcd;
	}

	return result;
}
/**************************************/
BigNumeric BigNumeric::Factorial() const
/**************************************/
{
	BigNumeric value = *this,
		result;

	if (value.data.size())
	{
		result.ToOne();
		while (value > 1ul)
			result *= value--;
	}

	return result;
}

/***********/
/*** EOF ***/
/***********/