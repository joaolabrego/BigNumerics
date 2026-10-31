/******************************************************************************/
/*** File-Name: BCDLogarithm.cpp                                            ***/
/*** Author...: João da Rocha Labrego                                       ***/
/*** E-Mail...: joaolabrego@gmail.com                                       ***/
/*** Purpose..: BCDLogarithm implementation.                                ***/
/******************************************************************************/

#include "BCDLogarithm.h"

/***************************************************************************/
BCDDecimal BCDLogarithm::Log(const BCDDecimal& value, const BCDDecimal& base)
/***************************************************************************/
{
	BCDDecimal result;

	if (value.AsVectorByVal().size())
	{
		General::UpPrecision();
		result = Ln(value) / Ln(base);
		General::DownPrecision();
	}

	return result;
}
/**************************************************/
BCDDecimal BCDLogarithm::Ln(BCDDecimal logarithming)
/**************************************************/
{
	SIGN sign;
	BCDDecimal value,
		result,
		lastResult,
		base;

	if (value > 0)
	{
		General::UpPrecision();
		base = BCDDecimal::E();
		sign = SIGN::POSITIVE;
		if (logarithming < 1)
		{
			sign = General::NotSign(sign);
			logarithming = logarithming.Inverse();
		}
		value = logarithming;
		result = 0ll;
		while (value >= base)
		{
			value /= base;
			result += 1ll;
		}
		result += (value / base);
		value = logarithming;
		do
		{
			lastResult = result;
			result = ((Exp(value / (result - 1ll))) + (result - 1ll) * base) / base;
		} while (result.Compare(lastResult, result.DecimalPlaces() - 1) != COMPARISON::EQUAL);
		result.ToSign(sign);
		General::DownPrecision();
	}

	return result;
}
/*******************************************************/
BCDDecimal BCDLogarithm::Exp(const BCDDecimal& logarithm)
/*******************************************************/
{
	BCDDecimal result = 0ll,
		lastResult,
		digit = 1ll;
	size_t index = 1;

	if (logarithm.AsVectorByVal().size())
	{
		if (logarithm.AsNumericByVal().IsZero())
			return BCDDecimal(1ll);
		General::UpPrecision();
		do
		{
			lastResult = result;
			result += digit;
			digit *= (logarithm / SIGNED_DOUBLE_DIGIT(index));
			++index;
		} while (result != lastResult);
		General::DownPrecision();
	}

	return result;
}
/***********************************************************************************/
BCDDecimal BCDLogarithm::Antilog(const BCDDecimal& logarithm, const BCDDecimal& base)
/***********************************************************************************/
{
	BCDDecimal result;

	if (logarithm.AsVectorByVal().size())
	{
		General::UpPrecision();
		result = Exp(logarithm * Ln(base));
		General::DownPrecision();
	}

	return result;
}
/*******************************************************************************************/
BCDDecimal BCDLogarithm::Baselog(const BCDDecimal& logarithming, const BCDDecimal& logarithm)
/*******************************************************************************************/
{
	BCDDecimal result;

	General::UpPrecision();
	result = logarithming.Power(logarithm.Inverse());
	General::DownPrecision();

	return result;
}
/*********************************************************************/
BCDLogarithm BCDLogarithm::operator +(const BCDLogarithm& addend) const
/*********************************************************************/
{
	BCDLogarithm result;

	if (data.AsVectorByVal().size() && addend.data.AsVectorByVal().size())
	{
		General::UpPrecision();
		if (data.AsNumericByVal().IsZero())
			result = addend;
		else
			result = data + (BCDDecimal(1ll) + addend.data / data);
		General::DownPrecision();
	}

	return result;
}
/*************************************************************************/
BCDLogarithm BCDLogarithm::operator -(const BCDLogarithm& subtrahend) const
/*************************************************************************/
{
	BCDLogarithm result;

	if (data.AsVectorByVal().size() && subtrahend.data.AsVectorByVal().size())
	{
		General::UpPrecision();
		if (data.AsNumericByVal().IsZero())
			result = subtrahend;
		else
			result = data + (BCDDecimal(1ll) - subtrahend.data / data);
		General::DownPrecision();
	}

	return result;
}
/*************************************************************************/
BCDLogarithm BCDLogarithm::operator *(const BCDLogarithm& multiplier) const
/*************************************************************************/
{
	BCDLogarithm result;

	if (data.AsVectorByVal().size() && multiplier.data.AsVectorByVal().size())
	{
		General::UpPrecision();
		if (data.AsNumericByVal().IsZero() || multiplier.data.AsNumericByVal().IsZero())
			result.data.ToZero();
		else if (data.IsOne())
			result = multiplier;
		else if (multiplier.data.IsOne())
			result = *this;
		else
			result = data + multiplier.data;
		General::DownPrecision();
	}

	return result;
}
/**********************************************************************/
BCDLogarithm BCDLogarithm::operator /(const BCDLogarithm& divisor) const
/**********************************************************************/
{
	BCDLogarithm result;

	if (data.AsVectorByVal().size() && divisor.data.AsVectorByVal().size() && !divisor.data.AsNumericByVal().IsZero())
	{
		General::UpPrecision();
		if (divisor.data.IsOne() || data.AsNumericByVal().IsZero())
			result = *this;
		else
			result = data - divisor.data;
		General::DownPrecision();
	}

	return result;
}
/****************************************************************/
BCDLogarithm BCDLogarithm::Power(const BCDDecimal& exponent) const
/****************************************************************/
{
	BCDLogarithm result;

	if (data.AsVectorByVal().size() && exponent.AsVectorByVal().size())
	{
		General::UpPrecision();
		if (exponent.AsNumericByVal().IsZero())
		{
			if (!data.AsNumericByVal().IsZero())
				result.data.ToOne();
		}
		else if (data.IsOne() || data.AsNumericByVal().IsZero() || exponent.IsOne())
			result = *this;
		else
			result = data * exponent;
		General::DownPrecision();
	}

	return result;
}
/*************************************************************/
BCDLogarithm BCDLogarithm::Root(const BCDDecimal& degree) const
/*************************************************************/
{
	BCDLogarithm result;

	if (data.AsVectorByVal().size() && degree.AsVectorByVal().size() && !degree.AsNumericByVal().IsZero())
	{
		General::UpPrecision();
		if (data.IsOne() || data.AsNumericByVal().IsZero() || degree.IsOne())
			result = *this;
		else
			result = data / degree;
		General::DownPrecision();
	}

	return result;
}
// https://rosettacode.org/wiki/Gamma_function#C
/**********************************************/
BCDDecimal BCDLogarithm::Gamma(BCDDecimal value)
/**********************************************/

{
	BCDDecimal result,
		parcel,
		half = BCDDecimal(0.5);
	vector<BCDDecimal> coeficients = GammaCoefficients();
	size_t length = coeficients.size();

	General::UpPrecision();
	value += BCDDecimal(1ll);
	result = coeficients.front();
	for (size_t i = 1; i < length; i++)
		result += coeficients[i] / (value + BCDDecimal(SIGNED_DOUBLE_DIGIT(i)));
	parcel = value + BCDDecimal(SIGNED_DOUBLE_DIGIT(length));
	result *= (Exp(-parcel) * Power(parcel, value + half) / value);
	General::DownPrecision();

	return result;
}
// https://rosettacode.org/wiki/Gamma_function#C
/*************************************************/
vector <BCDDecimal> BCDLogarithm::GammaCoefficients()
/*************************************************/
{
	static vector <BCDDecimal> value;

	if (value.empty())
	{
		General::UpPrecision();

		BCDDecimal factorial = 1ll,
			index,
			half = BCDDecimal(0.5),
			parcel,
			coefficient;
		size_t precision = DECIMAL_PLACES * DEC::DIGIT_LENGTH;

		value.insert(value.end(), BCDDecimal::DoublePI().Root(2));
		for (size_t i = 1; i < precision; i++)
		{
			index = BCDDecimal(SIGNED_DOUBLE_DIGIT(i));
			parcel = BCDDecimal(SIGNED_DOUBLE_DIGIT(precision - i));
			coefficient = Exp(parcel) * parcel.Power(index - half) / factorial;
			value.insert(value.end(), coefficient);
			factorial *= -index;
		}
		General::DownPrecision();
	}

	return value;
}

/***********/
/*** EOF ***/
/***********/
