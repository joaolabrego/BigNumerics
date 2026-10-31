/******************************************************************************/
/*** File-Name: BcdDecimal.cpp	                                            ***/
/*** Author...: João da Rocha Labrego                                       ***/
/*** E-Mail...: joaolabrego@gmail.com                                       ***/
/*** Purpose..: BcdDecimal implementation.                                 ***/
/******************************************************************************/

#include "BcdDecimal.h"

/**********************************************************************************/
BcdDecimal& BcdDecimal::ToValue(const BcdInteger& value, const size_t decimalPlaces)
/**********************************************************************************/
{
	data = value;
	ToDecimalPlaces(decimalPlaces);
	Normalize();

	return *this;
}
/**************************************/
BcdDecimal BcdDecimal::WholePart() const
/**************************************/
{
	BcdDecimal result;

	if (decimalPlaces)
	{
		result.data.ToValue(data.Sign(), General::SubVector(AsVector(), decimalPlaces));
		if (result.IsNull())
			result.ToZero();
	}
	else
		result.ToValue(data, decimalPlaces);
	
	return result;
}
/****************************************/
BcdDecimal BcdDecimal::DecimalPart() const
/****************************************/
{
	BcdDecimal result;

	if (decimalPlaces)
		result.ToValue(BcdInteger(Sign(), General::SubVector(AsVector(), 0, decimalPlaces)), decimalPlaces);
	
	return result;
}
/***************************************************/
VECTOR BcdDecimal::Repetend(const size_t times) const
/***************************************************/
{
	string decimalPart;

	decimalPart = DecimalPart().AsNumeric().ToString(false);
	if (decimalPart == UNDEFINED)
		decimalPart = "0";

	return General::Repetend(General::PadLeft(decimalPart), times);
}
/**********************************************************/
BcdDecimal& BcdDecimal::Truncate(const size_t decimalPlaces)
/**********************************************************/
{
	size_t start;

	if (decimalPlaces < this->decimalPlaces)
	{
		if (AsVector().size() > (start = this->decimalPlaces - decimalPlaces))
		{
			General::Assign(AsVector(), General::SubVector(AsVector(), start));
			ToDecimalPlaces(decimalPlaces);
		}
		else
			ToZero();
	}

	return *this;
}
/*******************************************************/
BcdDecimal& BcdDecimal::Round(const size_t decimalPlaces)
/*******************************************************/
{
	size_t start;
	VECTOR	value;
	DIGIT lastDigit;

	if (decimalPlaces < this->decimalPlaces)
	{
		if (AsVector().size() > (start = this->decimalPlaces - decimalPlaces))
		{
			lastDigit = AsVector().at(start - 1);
			General::Assign(AsVector(), General::SubVector(AsVector(), start));
			if (lastDigit >= (DEC::BASE >> 1) && IsPositive()) // to round.
				++data;
			this->decimalPlaces = decimalPlaces;
		}
		else
			ToZero();
	}

	return *this;
}
/***************************************************/
string BcdDecimal::ToString(const bool grouped) const
/***************************************************/
{
	string result,
		decimalPart;
	size_t length;

	if (IsNullOrZero())
		result = AsNumeric().ToString(grouped);
	else
	{
		result = WholePart().data.ToString(grouped);
		if (decimalPlaces)
		{
			length = AsVector().size();
			for (size_t i = decimalPlaces; i;)
				decimalPart.append(General::PadLeft(to_string(--i < length ? AsVector().at(i) : 0)));
			if (decimalPlaces > DECIMAL_PLACES)
			{
				decimalPart = General::PadRight(decimalPart, General::GetPrecision(), General::Repetend(decimalPart));
				if ((decimalPart = General::RightTrim(decimalPart.substr(0, DECIMAL_PLACES * DEC::DIGIT_LENGTH))).size())
				{
					result.push_back(DECIMAL_SEPARATOR);
					result.append(decimalPart);
				}
				if (decimalPart.size() >= THOUSANDS_LENGTH)
					result.append(ELLIPSIS);
			}
			else
			{
				result.push_back(DECIMAL_SEPARATOR);
				result.append(General::RightTrim(decimalPart));
			}
			if (!General::IsSignal(result.front()) && (IsNegative() || SHOW_POSITIVE))
				result.insert(result.begin(), char(General::ToSignal(data.Sign())));
		}
	}
		
	return result;
}
/*****************************************************/
BcdDecimal& BcdDecimal::operator =(const string& value)
/*****************************************************/
{
	char signal;
	string wholePart,
		decimalPart;

	if (General::Parse(value, signal, wholePart, decimalPart))
	{
		data = string(1, signal).append(wholePart.append(decimalPart));
		decimalPlaces = decimalPart.size() / DEC::DIGIT_LENGTH;
	}

	return *this;
}
/******************************************************/
BcdDecimal& BcdDecimal::operator =(const DECIMAL& value)
/******************************************************/
{
	size_t missingZeroes;
	DOUBLE_DIGIT base = DOUBLE_DIGIT(pow(DEC::RADIX, value.scale)),
		wholePart = value.Lo64 / base,
		decimalPart = value.Lo64 % base;
	BcdNumeric data;

	ToNull();
	if (decimalPart)
	{
		General::Append(AsVector(), data.ToValue(decimalPart).AsVector());
		if ((missingZeroes = DEC::DIGIT_LENGTH - value.scale % DEC::DIGIT_LENGTH) < DEC::DIGIT_LENGTH)
			AsNumeric() *= DOUBLE_DIGIT(pow(DEC::RADIX, missingZeroes));
		decimalPlaces = (value.scale + missingZeroes) / DEC::DIGIT_LENGTH;
	}
	if (wholePart)
		General::Append(AsVector(), data.ToValue(wholePart).AsVector());
	ToSign(value.sign == 0 ? SIGN::POSITIVE : SIGN::NEGATIVE);

	return *this;
}
/******************************************************/
void BcdDecimal::Denormalize(const size_t decimalPlaces)
/******************************************************/
{
	if (decimalPlaces > this->decimalPlaces && !IsNullOrZero())
	{
		General::Insert(AsVector(), decimalPlaces - this->decimalPlaces, 0);
		this->decimalPlaces = decimalPlaces;
	}
}
/***************************/
void BcdDecimal::Normalize()
/***************************/
{
	size_t i;

	Truncate();
	for (i = 0; i < decimalPlaces && !AsVector().at(i); i++);
	if (i)
	{
		General::Assign(AsVector(), General::SubVector(AsVector(), i));
		decimalPlaces -= i;
	}
}
/*************************************************************/
COMPARISON BcdDecimal::Compare(BcdDecimal rightComparand) const
/*************************************************************/
{
	BcdDecimal leftComparand = *this;

	leftComparand.Denormalize(rightComparand.decimalPlaces);
	rightComparand.Denormalize(leftComparand.decimalPlaces);

	return leftComparand.data.Compare(rightComparand.data);
}
/*******************************************************************************/
COMPARISON BcdDecimal::Compare(BcdDecimal rightComparand, size_t precision) const
/*******************************************************************************/
{
	BcdDecimal leftComparand = *this;

	leftComparand.Round(precision);
	rightComparand.Round(precision);

	return leftComparand.Compare(rightComparand);
}
/********************************************************/
BcdDecimal BcdDecimal::operator +(BcdDecimal addend) const
/********************************************************/
{
	BcdDecimal augend = *this;

	augend.Denormalize(addend.decimalPlaces);
	addend.Denormalize(augend.decimalPlaces);

	return BcdDecimal(augend.data + addend.data, max(augend.decimalPlaces, addend.decimalPlaces));
}
/************************************************************/
BcdDecimal BcdDecimal::operator -(BcdDecimal subtrahend) const
/************************************************************/
{
	BcdDecimal minuend = *this;

	minuend.Denormalize(subtrahend.decimalPlaces);
	subtrahend.Denormalize(minuend.decimalPlaces);

	return BcdDecimal(minuend.data - subtrahend.data, max(minuend.decimalPlaces, subtrahend.decimalPlaces));
}
/******************************************************************/
BcdDecimal BcdDecimal::operator /(SIGNED_DOUBLE_DIGIT divisor) const
/******************************************************************/
{
	SIGN sign = ToAbsolute(divisor);
	BcdDecimal dividend = *this,
		quotient;
	SIGNED_DOUBLE_DIGIT remainder;
	size_t precision,
		decimalPlaces;

	if (dividend.IsZero())
		return dividend;
	if (!(quotient = BcdDecimal(dividend.data.Divide(divisor, remainder), dividend.decimalPlaces)).IsNull())
	{
		if (remainder && (decimalPlaces = dividend.decimalPlaces) < (precision = General::GetPrecision()))
		{
			if (quotient.IsZero())
				quotient.ToNull();
			do
			{
				General::Insert((dividend = remainder).AsVector(), 0);
				General::Insert(quotient.AsVector(), dividend.data.Divide(divisor, remainder).AsVector().front());
				++decimalPlaces;
			} while (remainder && decimalPlaces < precision);
			quotient.ToSign(sign == this->Sign() ? SIGN::POSITIVE : SIGN::NEGATIVE);
			quotient.ToDecimalPlaces(decimalPlaces);
		}
	}

	return quotient;
}
/*********************************************************/
BcdDecimal BcdDecimal::operator /(BcdDecimal divisor) const
/*********************************************************/
{
	BcdDecimal dividend = *this,
		quotient,
		remainder;
	size_t precision = General::GetPrecision(),
		i;
	VECTOR repetend;

	dividend.Denormalize(divisor.decimalPlaces);
	divisor.Denormalize(dividend.decimalPlaces);
	if ((quotient.ToValue(dividend.data.Divide(divisor.data, remainder.data), 0)).AsVector().size())
	{
		repetend = Repetend(1);
		i = 0;
		while (quotient.decimalPlaces < precision && !remainder.IsZero())
		{
			General::Insert((dividend = remainder).AsVector(), repetend.at(i++));
			General::Insert(quotient.AsVector(), dividend.AsNumeric().BinarySearchDigit(divisor.AsNumeric(), remainder.AsNumeric()));
			++quotient.decimalPlaces;
		}
		quotient.ToSign(dividend.Sign() == divisor.Sign() ? SIGN::POSITIVE : SIGN::NEGATIVE);
	}
	General::Normalize(quotient.AsVector());

	return quotient;
}
/**************************************************************/
BcdDecimal BcdDecimal::Power(SIGNED_DOUBLE_DIGIT exponent) const
/**************************************************************/
{
	SIGN sign = ToAbsolute(exponent);
	BcdDecimal result = BcdDecimal(data.Power(exponent), size_t(decimalPlaces * exponent));

	if (sign == SIGN::NEGATIVE && result.AsVector().size())
		result = result.Inverse();

	return result;
}
/***********************************************************/
BcdDecimal BcdDecimal::Root(SIGNED_DOUBLE_DIGIT degree) const
/***********************************************************/
{
	SIGN sign = ToAbsolute(degree);
	BcdDecimal rooting = *this,
		root,
		remainder;
	size_t missingDigits,
		precision,
		i;
	VECTOR repetend;

	if (AsVector().size() && (IsPositive() || degree % 2) && (sign == SIGN::POSITIVE || !rooting.IsZero()))
	{
		General::UpPrecision();
		i = 0;
		precision = General::GetPrecision();
		missingDigits = size_t(degree) - rooting.decimalPlaces % size_t(degree);
		repetend = Repetend(degree + missingDigits);
		if (missingDigits < size_t(degree))
		{
			General::Insert(rooting.AsVector(), General::SubVector(repetend, i, missingDigits));
			rooting.decimalPlaces += missingDigits;
			i += missingDigits;
		}
		root = BcdDecimal(rooting.data.Root(degree, remainder.data), size_t(rooting.decimalPlaces / degree));
		if (!root.IsNull())
		{
			
			while (!remainder.IsZero() && root.decimalPlaces < precision)
			{
				General::Insert(rooting.AsVector(), General::SubVector(repetend, i, degree));
				i += degree;
				rooting.AsNumeric().BinarySearchDigit(degree, root.AsNumeric(), remainder.AsNumeric());
				root.decimalPlaces++;
			}
			root.Normalize();
			if (sign == SIGN::NEGATIVE)
				root = root.Inverse();
		}
		General::DownPrecision();
	}

	return root;
}
/***********************************************/
BcdDecimal BcdDecimal::Log(BcdDecimal base) const
/***********************************************/
{
	BcdDecimal result;

	if (AsVector().size())
	{
		General::UpPrecision();
		result = Ln() / base.Ln();
		General::DownPrecision();
	}

	return result;
}
/*******************************/
BcdDecimal BcdDecimal::Ln() const
/*******************************/
{
	SIGN sign;
	BcdDecimal logarithmand,
		value,
		result,
		lastResult,
		base;
		
	if (*this > 0)
	{
		General::UpPrecision();
		logarithmand = *this;
		base = E();
		sign = SIGN::POSITIVE;
		if (logarithmand < 1)
		{
			sign = General::NotSign(sign);
			logarithmand = logarithmand.Inverse();
		}
		value = logarithmand;
		result.ToZero();
		while (value >= base)
		{
			value /= base;
			result += SIGNED_DOUBLE_DIGIT(1);
		}
		result += (value / base);
		value = logarithmand;
		do
		{
			lastResult = result;
			result = ((value / (result - SIGNED_DOUBLE_DIGIT(1)).Exp()) + (result - SIGNED_DOUBLE_DIGIT(1)) * base) / base;
		} while (result != lastResult);
		result.ToSign(sign);
		General::DownPrecision();
	}

	return result;
}
/********************************/
BcdDecimal BcdDecimal::Exp() const
/********************************/
{
	BcdDecimal result = SIGNED_DOUBLE_DIGIT(0),
		lastResult,
		lastDigit,
		digit = SIGNED_DOUBLE_DIGIT(1);
	size_t index = 1;

	if (AsVector().size())
	{
		if (IsZero())
			return BcdDecimal(SIGNED_DOUBLE_DIGIT(1));
		General::UpPrecision();
		do
		{
			lastResult = result;
			result += digit;
			digit *= (*this / SIGNED_DOUBLE_DIGIT(index));
			++index;
		} while (result != lastResult);
		General::DownPrecision();
	}

	return result;
}
/***************************************************/
BcdDecimal BcdDecimal::Antilog(BcdDecimal base) const
/***************************************************/
{
	BcdDecimal result;

	if (AsVector().size())
	{
		General::UpPrecision();
		result = (*this * base.Ln()).Exp();
		General::DownPrecision();
	}

	return result;
}
/********************************************************/
BcdDecimal BcdDecimal::Add(const BcdDecimal& addend) const
/********************************************************/
{
	BcdDecimal result;
	
	if (AsVector().size() && addend.AsVector().size())
	{
		General::UpPrecision();
		if (IsZero())
			result = addend;
		else
			result = (Ln() + (BcdDecimal(SIGNED_DOUBLE_DIGIT(1)) + addend / *this).Ln()).Exp();
		General::DownPrecision();
	}

	return result;
}
/*****************************************************************/
BcdDecimal BcdDecimal::Subtract(const BcdDecimal& subtrahend) const
/*****************************************************************/
{
	BcdDecimal result;

	if (AsVector().size() && subtrahend.AsVector().size())
	{
		General::UpPrecision();
		if (IsZero())
			result = subtrahend;
		else
			result = (Ln() + (BcdDecimal(SIGNED_DOUBLE_DIGIT(1)) - subtrahend / *this).Ln()).Exp();
		General::DownPrecision();
	}

	return result;
}
/*****************************************************************/
BcdDecimal BcdDecimal::Multiply(const BcdDecimal& multiplier) const
/*****************************************************************/
{
	BcdDecimal result;

	if (AsVector().size() && multiplier.AsVector().size())
	{
		General::UpPrecision();
		if (IsZero() || multiplier.IsZero())
			result.ToZero();
		else if (IsOne())
			result = multiplier;
		else if (multiplier.IsOne())
			result = *this;
		else
			result = (Ln() + multiplier.Ln()).Exp();
		General::DownPrecision();
	}

	return result;
}
/************************************************************/
BcdDecimal BcdDecimal::Divide(const BcdDecimal& divisor) const
/************************************************************/
{
	BcdDecimal result;

	if (AsVector().size() && divisor.AsVector().size() && !divisor.IsZero())
	{
		General::UpPrecision();
		if (divisor.IsOne() || IsZero())
			result = *this;
		else
		{
			result = (Ln() - divisor.Ln()).Exp();
		}
		General::DownPrecision();
	}

	return result;
}
/************************************************************/
BcdDecimal BcdDecimal::Power(const BcdDecimal& exponent) const
/************************************************************/
{
	BcdDecimal result;

	if (AsVector().size() && exponent.AsVector().size())
	{
		General::UpPrecision();
		if (exponent.IsZero())
		{
			if (!IsZero())
				result.ToOne();
		}
		else if (IsOne() || IsZero() || exponent.IsOne())
			result = *this;
		else
			result = (Ln() * exponent).Exp();
		General::DownPrecision();
	}

	return result;
}
/*********************************************************/
BcdDecimal BcdDecimal::Root(const BcdDecimal& degree) const
/*********************************************************/
{
	BcdDecimal result;

	if (AsVector().size() && degree.AsVector().size() && !degree.IsZero())
	{
		General::UpPrecision();
		if (IsOne() || IsZero() || degree.IsOne())
			result = *this;
		else
			result = (Ln() / degree).Exp();
		General::DownPrecision();
	}

	return result;
}
// https://pt.wikipedia.org/wiki/Algoritmo_de_Gauss-Legendre
/*************************/
BcdDecimal BcdDecimal::PI()
/*************************/
{
	static BcdDecimal value;

	if (value.IsNull())
	{
		General::UpPrecision();
		BcdDecimal lastArithmeticMean = SIGNED_DOUBLE_DIGIT(1),
			geometricMean = BcdDecimal(SIGNED_DOUBLE_DIGIT(2)).Root(2).Inverse(),
			arthimectMean,
			sum = SIGNED_DOUBLE_DIGIT(0),
			potencyTwo = SIGNED_DOUBLE_DIGIT(2);
		
		do
		{
			arthimectMean = (lastArithmeticMean + geometricMean) / 2;
			geometricMean = (lastArithmeticMean * geometricMean).Root(2);
			lastArithmeticMean = arthimectMean;
			potencyTwo *= SIGNED_DOUBLE_DIGIT(2);
			sum = sum + (lastArithmeticMean.Power(2) - geometricMean.Power(2)) * potencyTwo;
		} while (lastArithmeticMean != geometricMean);
		value = lastArithmeticMean.Power(2) * SIGNED_DOUBLE_DIGIT(4) / (BcdDecimal(SIGNED_DOUBLE_DIGIT(1)) - sum);
		General::DownPrecision();
	}

	return value;
}
/*******************************/
BcdDecimal BcdDecimal::DoublePI()
/*******************************/
{
	static BcdDecimal value;

	if (value.IsNull())
	{
		General::UpPrecision();
		value = BcdDecimal::PI() * 2;
		General::DownPrecision();
	}

	return value;
}
/************************/
BcdDecimal BcdDecimal::E()
/************************/
{
	static BcdDecimal value;

	if (value.IsNull())
	{
		General::UpPrecision();
		BcdDecimal lastResult,
			result = SIGNED_DOUBLE_DIGIT(2),
			factorial = SIGNED_DOUBLE_DIGIT(2);
		SIGNED_DOUBLE_DIGIT index = SIGNED_DOUBLE_DIGIT(2);
		
		do
		{
			lastResult = result;
			result += factorial.Inverse();
			++index;
			factorial *= index;
		} while (result != lastResult);
		value = result;
		General::DownPrecision();
	}

	return value;
}
// https://rosettacode.org/wiki/Gamma_function#C
/**********************************/
BcdDecimal BcdDecimal::Gamma() const
/**********************************/
{
	BcdDecimal value = *this,
		result, 
		parcel, 
		half = BcdDecimal(0.5);
	vector<BcdDecimal> coeficients = GammaCoefficients();
	size_t length = coeficients.size();

	General::UpPrecision();
	value += BcdDecimal(SIGNED_DOUBLE_DIGIT(1));
	result = coeficients.front();
	for (size_t i = 1; i < length; i++)
		result += coeficients[i] / (value + BcdDecimal(SIGNED_DOUBLE_DIGIT(i)));
	parcel = value + BcdDecimal(SIGNED_DOUBLE_DIGIT(length));
	result *= ((-parcel).Exp() * parcel.Power(value + half) / value);
	General::DownPrecision();
	result.Normalize();

	return result;
}
// https://rosettacode.org/wiki/Gamma_function#C
/*************************************************/
vector <BcdDecimal> BcdDecimal::GammaCoefficients()
/*************************************************/
{
	static vector <BcdDecimal> value;

	if (value.empty())
	{
		General::UpPrecision();

		BcdDecimal factorial = SIGNED_DOUBLE_DIGIT(1),
			index,
			half = BcdDecimal(0.5),
			parcel,
			coefficient;
		size_t precision = DECIMAL_PLACES * DEC::DIGIT_LENGTH;

		value.insert(value.end(), coefficient = DoublePI().Root(2));
		for (size_t i = 1; i < precision; i++)
		{
			string s = coefficient.ToString();
			index = BcdDecimal(SIGNED_DOUBLE_DIGIT(i));
			parcel = BcdDecimal(SIGNED_DOUBLE_DIGIT(precision - i));
			string a = parcel.Exp().ToString();
			string b = parcel.Power(index - half).ToString();
			string c = factorial.ToString();
			coefficient = parcel.Exp() * parcel.Power(index - half) / factorial;
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