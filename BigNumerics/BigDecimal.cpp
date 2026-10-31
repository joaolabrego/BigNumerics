/******************************************************************************/
/*** File-Name: BigDecimal.cpp	                                            ***/
/*** Author...: João da Rocha Labrego                                       ***/
/*** E-Mail...: joaolabrego@gmail.com                                       ***/
/*** Purpose..: BigDecimal implementation.                                  ***/
/******************************************************************************/

#include "BigDecimal.h"

/*********************************************************************/
BigDecimal& BigDecimal::ToValue(BigInteger value, size_t decimalPlaces)
/*********************************************************************/
{
	this->data = data;
	ToDecimalPlaces(decimalPlaces);

	return *this;
}
/******************************************************/
BigDecimal& BigDecimal::operator =(const DECIMAL& value)
/******************************************************/
{
	size_t missingZeroes;
	BigNumeric data;

	ToNull();
	this->data = BigInteger(value.sign == 0 ? SIGN::POSITIVE : SIGN::NEGATIVE, data = value.Lo64);
	if ((this->decimalPlaces = value.scale) < DEC::DIGIT_LENGTH)
	{
		this->data *= DOUBLE_DIGIT(pow(DEC::RADIX, double(missingZeroes = DEC::DIGIT_LENGTH - value.scale % DEC::DIGIT_LENGTH)));
		this->decimalPlaces += missingZeroes;
	}

	return *this;
}
/*****************************************************/
BigDecimal& BigDecimal::operator =(const string& value)
/*****************************************************/
{
	char signal;
	string wholePart,
		decimalPart;

	ToNull();
	if (General::Parse(value, signal, wholePart, decimalPart))
	{
		data = BigInteger(signal + wholePart + decimalPart);
		decimalPlaces = decimalPart.size();
	}

	return *this;
}
/******************************************************/
void BigDecimal::Unnormalize(const size_t decimalPlaces)
/******************************************************/
{
	if (this->decimalPlaces < decimalPlaces && !data.IsNullOrZero())
	{
		data *= BigInteger(DEC::RADIX).Power(DOUBLE_DIGIT(decimalPlaces - this->decimalPlaces));
		this->decimalPlaces = decimalPlaces;
	}
}
/**************************/
void BigDecimal::Normalize()
/**************************/
{
	BigInteger quotient;
	SIGNED_DOUBLE_DIGIT remainder;
	size_t maximumDecimalPlaces = General::GetPrecision();

	if (decimalPlaces > maximumDecimalPlaces)
		data /= BigInteger(DEC::RADIX).Power(DOUBLE_DIGIT(decimalPlaces - maximumDecimalPlaces));
	while (decimalPlaces && (quotient = data.Divide(SIGNED_DOUBLE_DIGIT(DEC::BASE), remainder), !remainder))
	{
		data = quotient;
		decimalPlaces -= DEC::DIGIT_LENGTH;
	}
}
/*************************************************************/
COMPARISON BigDecimal::Compare(BigDecimal rightComparand) const
/*************************************************************/
{
	BigDecimal leftComparand = *this;

	leftComparand.Unnormalize(rightComparand.decimalPlaces);
	rightComparand.Unnormalize(leftComparand.decimalPlaces);

	return leftComparand.data.Compare(rightComparand.data);
}
/********************************************************/
BigDecimal BigDecimal::operator +(BigDecimal addend) const
/********************************************************/
{
	BigDecimal augend = *this;

	augend.Unnormalize(addend.decimalPlaces);
	addend.Unnormalize(augend.decimalPlaces);

	return BigDecimal(augend.data + addend.data, max(augend.decimalPlaces, addend.decimalPlaces));
}
/************************************************************/
BigDecimal BigDecimal::operator -(BigDecimal subtrahend) const
/************************************************************/
{
	BigDecimal minuend = *this;

	minuend.Unnormalize(subtrahend.decimalPlaces);
	subtrahend.Unnormalize(minuend.decimalPlaces);

	return BigDecimal(minuend.data - subtrahend.data, max(minuend.decimalPlaces, subtrahend.decimalPlaces));
}
/******************************************************************/
BigDecimal BigDecimal::operator /(SIGNED_DOUBLE_DIGIT divisor) const
/******************************************************************/
{
	BigDecimal dividend = *this,
		quotient; 
	size_t precision;
	SIGNED_DOUBLE_DIGIT remainder = 0;

	quotient = BigDecimal(dividend.data.Divide(divisor, remainder), dividend.decimalPlaces);
	if (remainder && !quotient.IsNull())
	{
		precision = General::GetPrecision();
		while (decimalPlaces < precision)
		{
			dividend = BigDecimal(remainder, 0) * DEC::BASE;
			quotient.data = quotient.data * DEC::BASE + dividend.data.Divide(divisor, remainder);
			quotient.decimalPlaces += DEC::DIGIT_LENGTH;
		} while (remainder);
	}
	quotient.Normalize();

	return quotient;
}
/*********************************************************/
BigDecimal BigDecimal::operator /(BigDecimal divisor) const
/*********************************************************/
{
	BigDecimal dividend = *this,
		quotient;
	BigInteger remainder;
	size_t maximumDecimalPlaces;

	dividend.Unnormalize(divisor.decimalPlaces);
	divisor.Unnormalize(dividend.decimalPlaces);

	quotient = BigDecimal(dividend.data.Divide(divisor.data, remainder), decimalPlaces);
	if (!(quotient.IsNull() || remainder.IsZero()))
	{
		maximumDecimalPlaces = General::GetPrecision();
		while (decimalPlaces < maximumDecimalPlaces)
		{
			dividend = BigDecimal(remainder * DEC::BASE);
			quotient.data = quotient.data * DEC::BASE + dividend.data.Divide(divisor.data, remainder);
			quotient.decimalPlaces += DEC::DIGIT_LENGTH;
		} while (!remainder.IsZero());
	}
	quotient.Normalize();

	return quotient;
}
/**************************************************************/
BigDecimal BigDecimal::Power(SIGNED_DOUBLE_DIGIT exponent) const
/**************************************************************/
{
	SIGN sign = ToAbsolute(exponent);
	BigDecimal potency;

	potency.data = data.Power(exponent);
	if (!potency.data.IsNull())
	{
		potency.decimalPlaces = decimalPlaces * size_t(exponent);
		if (sign == SIGN::NEGATIVE)
			return potency.Inverse();
	}
	potency.Normalize();

	return potency;
}
/***********************************************************/
BigDecimal BigDecimal::Root(SIGNED_DOUBLE_DIGIT degree) const
/***********************************************************/
{
	BigDecimal radicand = *this,
		root;
	BigInteger remainder;
	SIGN sign = ToAbsolute(degree);

	radicand.Unnormalize(General::GetPrecision() * size_t(degree));
	root = BigDecimal(radicand.data.Root(degree, remainder), radicand.decimalPlaces / size_t(degree));
	if (sign == SIGN::NEGATIVE)
		root = root.Inverse();
	root.Normalize();

	return root;
}

/***********/
/*** EOF ***/
/***********/