/******************************************************************************/
/*** File-Name: BigInteger.cpp	                                            ***/
/*** Author...: João da Rocha Labrego                                       ***/
/*** E-Mail...: joaolabrego@gmail.com                                       ***/
/*** Purpose..: BigInteger implementation.                                  ***/
/******************************************************************************/

#include "BigInteger.h"

/**********************************************************************/
BigInteger& BigInteger::ToValue(const SIGN sign, const BigNumeric& data)
/**********************************************************************/
{
	this->data = data;
	ToSign(sign);

	return *this;
}
/***************************************************/
string BigInteger::ToString(const bool grouped) const
/***************************************************/
{
	string result = data.ToString(grouped);

	if (IsNullOrZero())
		return result;
	if (sign == SIGN::NEGATIVE)
		return string(1, char(General::ToSignal(sign))).append(result);
	else if (SHOW_POSITIVE)
		return string(1, char(SIGN::POSITIVE)).append(result);
	
	return result;
}
/*****************************************************/
BigInteger& BigInteger::operator =(const string& value)
/*****************************************************/
{
	char signal;
	string wholePart,
		decimalPart;

	if (General::Parse(value, signal, wholePart, decimalPart))
	{
		if (wholePart == "0")
			this->ToZero();
		else
		{
			this->data = wholePart;
			this->sign = General::ToSign(signal);
		}
	}
	else
		ToNull();

	return *this;
}
/**********************************************************************/
COMPARISON BigInteger::Compare(SIGNED_DOUBLE_DIGIT rightComparand) const
/**********************************************************************/
{
	COMPARISON comparison;

	if (sign == ToAbsolute(rightComparand))
	{
		comparison = data.Compare(rightComparand);
		if (sign == SIGN::NEGATIVE)
			return General::NotComparison(comparison);
		return comparison;
	}
	if (sign == SIGN::NEGATIVE)
		return COMPARISON::MINOR;

	return COMPARISON::MAJOR;
}
/********************************************************************/
COMPARISON BigInteger::Compare(const BigInteger& rightComparand) const
/********************************************************************/
{
	COMPARISON comparison;

	if (sign == rightComparand.sign)
	{
		comparison = data.Compare(rightComparand.data);
		if (sign == SIGN::NEGATIVE)
			return General::NotComparison(comparison);
		return comparison;
	}
	if (sign == SIGN::NEGATIVE)
		return COMPARISON::MINOR;

	return COMPARISON::MAJOR;
}
/***************************************************************/
BigInteger BigInteger::operator +(const BigInteger& addend) const
/***************************************************************/
{
	BigInteger sum;
	COMPARISON comparison;

	if (sign == addend.sign)
		return BigInteger(sign, data + addend.data);
	if ((comparison = data.Compare(addend.data)) == COMPARISON::MINOR)
		return BigInteger(addend.sign, addend.data - data);
	if (comparison == COMPARISON::EQUAL)
		return sum.ToZero();

	return BigInteger(sign, data - addend.data);
}
/***********************************/
BigInteger& BigInteger::operator ++()
/***********************************/
{
	if (sign == SIGN::POSITIVE)
		++data;
	else if (data.IsOne())
		ToZero();
	else
		--data;

	return *this;
}
/*******************************************************************/
BigInteger BigInteger::operator -(const BigInteger& subtrahend) const
/*******************************************************************/
{
	COMPARISON comparison;

	if (sign == subtrahend.sign)
	{
		if ((comparison = data.Compare(subtrahend.data)) == COMPARISON::MINOR)
			return BigInteger(General::NotSign(subtrahend.sign), subtrahend.data - data);
		return BigInteger(sign, data - subtrahend.data);
	}

	return BigInteger(sign, data + subtrahend.data);
}
/***********************************/
BigInteger& BigInteger::operator --()
/***********************************/
{
	if (sign == SIGN::NEGATIVE)
		++data;
	else if (data.IsZero())
		ToMinusOne();
	else
		--data;

	return *this;
}
/**********************************************************************************************/
BigInteger BigInteger::Divide(SIGNED_DOUBLE_DIGIT divisor, SIGNED_DOUBLE_DIGIT& remainder) const
/**********************************************************************************************/
{
	DIGIT remain;
	BigInteger quotient = BigInteger(sign == ToAbsolute(divisor) ? SIGN::POSITIVE : SIGN::NEGATIVE, data.Divide(divisor, remain));
		
	remainder = sign == SIGN::NEGATIVE ? remainder = -SIGNED_DOUBLE_DIGIT(remain) : remain;
	
	return quotient;
}
/***********************************************************************************/
BigInteger BigInteger::Divide(const BigInteger& divisor, BigInteger& remainder) const
/***********************************************************************************/
{
	BigNumeric quotient = data.Divide(divisor.data, remainder.data);
	
	remainder.sign = sign;

	return BigInteger(sign == divisor.sign ? SIGN::POSITIVE : SIGN::NEGATIVE, quotient);
}
/*********************************************************************************/
BigInteger BigInteger::Root(const DOUBLE_DIGIT degree, BigInteger& remainder) const
/*********************************************************************************/
{
	remainder.sign = sign;

	return BigInteger(sign, data.Root(degree, remainder.data));
}
/************************************************************/
BigInteger BigInteger::GCD(const BigInteger& rightValue) const
/************************************************************/
{
	BigInteger result;

	if (sign == SIGN::POSITIVE || rightValue.sign == SIGN::POSITIVE)
		result = BigInteger(SIGN::POSITIVE, data.GCD(rightValue.data));

	return result;
}
/************************************************************/
BigInteger BigInteger::LCM(const BigInteger& rightValue) const
/************************************************************/
{
	BigInteger result;

	if (sign == SIGN::POSITIVE && rightValue.sign == SIGN::POSITIVE)
		result = BigInteger(SIGN::POSITIVE, data.LCM(rightValue.data));

	return result;
}
/**************************************/
BigInteger BigInteger::Factorial() const
/**************************************/
{
	BigInteger result;

	if (sign == SIGN::POSITIVE)
		result = BigInteger(SIGN::POSITIVE, data.Factorial());

	return result;
}

/***********/
/*** EOF ***/
/***********/