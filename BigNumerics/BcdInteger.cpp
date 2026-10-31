/******************************************************************************/
/*** File-Name: BcdInteger.cpp	                                            ***/
/*** Author...: João da Rocha Labrego                                       ***/
/*** E-Mail...: joaolabrego@gmail.com                                       ***/
/*** Purpose..: BcdInteger implementation.                                  ***/
/******************************************************************************/

#include "BcdInteger.h"

/***********************************************************************/
BcdInteger& BcdInteger::ToValue(const SIGN sign, const BcdNumeric& value)
/***********************************************************************/
{
	data = value;
	ToSign(sign);

	return *this;
}
/***************************************************/
string BcdInteger::ToString(const bool grouped) const
/***************************************************/
{
	string result = data.ToString(grouped);

	if (IsNullOrZero())
		return result;
	if (IsNegative())
		return string(1, NEGATIVE_SIGNAL).append(result);
	else if (SHOW_POSITIVE)
		return string(1, POSITIVE_SIGNAL).append(result);

	return result;
}
/*****************************************************/
BcdInteger& BcdInteger::operator =(const string& value)
/*****************************************************/
{
	char signal;
	string wholePart,
		decimalPart;

	if (General::Parse(value, signal, wholePart, decimalPart))
	{
		data = wholePart;
		if (signal == char(SIGNAL::POSITIVE) && decimalPart.size() && decimalPart.front() >= '5') // to round.
			++*this;
		ToSign(General::ToSign(signal));
	}
	else
		ToNull();

	return *this;
}
/**********************************************************************/
COMPARISON BcdInteger::Compare(SIGNED_DOUBLE_DIGIT rightComparand) const
/**********************************************************************/
{
	COMPARISON comparison;

	if (sign == ToAbsolute(rightComparand))
	{
		comparison = data.Compare(rightComparand);
		if (IsNegative())
			return General::NotComparison(comparison);
		return comparison;
	}
	if (IsNegative())
		return COMPARISON::MINOR;

	return COMPARISON::MAJOR;
}
/********************************************************************/
COMPARISON BcdInteger::Compare(const BcdInteger& rightComparand) const
/********************************************************************/
{
	COMPARISON comparison;

	if (sign == rightComparand.sign)
	{
		comparison = data.Compare(rightComparand.data);
		if (IsNegative())
			return General::NotComparison(comparison);
		return comparison;
	}
	if (IsNegative())
		return COMPARISON::MINOR;

	return COMPARISON::MAJOR;
}
/***************************************************************/
BcdInteger BcdInteger::operator +(const BcdInteger& addend) const
/***************************************************************/
{
	BcdInteger sum;
	COMPARISON comparison;

	if (sign == addend.sign)
		return BcdInteger(sign, data + addend.data);
	if ((comparison = data.Compare(addend.data)) == COMPARISON::MINOR)
		return BcdInteger(addend.sign, addend.data - data);
	if (comparison == COMPARISON::EQUAL)
		return sum.ToZero();

	return BcdInteger(sign, data - addend.data);
}
/***********************************/
BcdInteger& BcdInteger::operator ++()
/***********************************/
{
	if (IsPositive())
		++data;
	else if (data.IsOne())
		ToZero();
	else
		--data;

	return *this;
}
/*******************************************************************/
BcdInteger BcdInteger::operator -(const BcdInteger& subtrahend) const
/*******************************************************************/
{
	COMPARISON comparison;

	if (sign == subtrahend.sign)
	{
		if ((comparison = data.Compare(subtrahend.data)) == COMPARISON::MINOR)
			return BcdInteger(General::NotSign(subtrahend.sign), subtrahend.data - data);
		return BcdInteger(sign, data - subtrahend.data);
	}

	return BcdInteger(sign, data + subtrahend.data);
}
/***********************************/
BcdInteger& BcdInteger::operator --()
/***********************************/
{
	if (IsNegative())
		++data;
	else if (data.IsZero())
		ToMinusOne();
	else
		--data;

	return *this;
}
/**********************************************************************************************/
BcdInteger BcdInteger::Divide(SIGNED_DOUBLE_DIGIT divisor, SIGNED_DOUBLE_DIGIT& remainder) const
/**********************************************************************************************/
{
	DIGIT remain = 0;
	BcdInteger quotient = BcdInteger(sign == ToAbsolute(divisor) ? SIGN::POSITIVE : SIGN::NEGATIVE, data.Divide(divisor, remain));

	remainder = sign == SIGN::NEGATIVE ? remainder = -SIGNED_DOUBLE_DIGIT(remain) : remain;

	return quotient;
}
/***********************************************************************************/
BcdInteger BcdInteger::Divide(const BcdInteger& divisor, BcdInteger& remainder) const
/***********************************************************************************/
{
	BcdNumeric quotient = data.Divide(divisor.data, remainder.data);

	remainder.sign = sign;

	return BcdInteger(sign == divisor.sign ? SIGN::POSITIVE : SIGN::NEGATIVE, quotient);
}
/*********************************************************************************/
BcdInteger BcdInteger::Root(const DOUBLE_DIGIT degree, BcdInteger& remainder) const
/*********************************************************************************/
{
	BcdInteger root;

	if (degree && (degree & 0x1 || IsPositive()))
	{
		remainder.sign = sign;
		root = BcdInteger(sign, data.Root(degree, remainder.data));
	}

	return root;
}
/************************************************************/
BcdInteger BcdInteger::GCD(const BcdInteger& rightValue) const
/************************************************************/
{
	BcdInteger result;

	if (IsPositive() && rightValue.IsPositive())
		result.ToValue(SIGN::POSITIVE, data.GCD(rightValue.data));

	return result;
}
/************************************************************/
BcdInteger BcdInteger::LCM(const BcdInteger& rightValue) const
/************************************************************/
{
	BcdInteger result;

	if (IsPositive() || rightValue.IsPositive())
		result.ToValue(SIGN::POSITIVE, data.LCM(rightValue.data));

	return result;
}
/**************************************/
BcdInteger BcdInteger::Factorial() const
/**************************************/
{
	BcdInteger result;

	if (IsPositive())
		result.ToValue(SIGN::POSITIVE, data.Factorial());

	return result;
}

/***********/
/*** EOF ***/
/***********/