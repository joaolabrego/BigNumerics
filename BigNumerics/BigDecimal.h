/******************************************************************************/
/*** File-Name: BigDecimal.h                                                ***/
/*** Author...: João da Rocha Labrego                                       ***/
/*** E-Mail...: joaolabrego@gmail.com                                       ***/
/*** Purpose..: BigDecimal header.                                          ***/
/******************************************************************************/
#ifndef BigDecimal_H
#define BigDecimal_H

#include "BigInteger.h"

struct BigDecimal
{
public:
	BigDecimal() { ToNull(); }
	BigDecimal(const SIGNED_DOUBLE_DIGIT value) { *this = value; }
	BigDecimal(const float value) { *this = value; }
	BigDecimal(const double value) { *this = value; }
	BigDecimal(const DECIMAL value) { *this = value; }
	BigDecimal(const char value[]) { *this = value; }
	BigDecimal(const string& value) { *this = value; }
	BigDecimal(const BigNumeric& value) { *this = value; }
	BigDecimal(const BigInteger& value) { *this = value; }
	BigDecimal(BigInteger value, size_t decimalPlaces) { ToValue(value, decimalPlaces); };
	~BigDecimal() {}

	void ToNull() { data.ToNull(); decimalPlaces = 0; }
	void ToMinusOne() { data.ToMinusOne(); decimalPlaces = 0; }
	void ToZero() { data.ToZero(); decimalPlaces = 0; }
	void ToOne() { data.ToOne(); decimalPlaces = 0; }
	BigDecimal& ToValue(BigInteger value, size_t decimalPlaces);

	BigDecimal Power(SIGNED_DOUBLE_DIGIT exponent) const;
	BigDecimal Root(SIGNED_DOUBLE_DIGIT degree) const;

	bool IsNull() const { return data.IsNull(); }
	bool IsMinusOne() const { return !decimalPlaces && data.IsMinusOne(); }
	bool IsZero() const { return data.IsZero(); }
	bool IsNullOrZero() const { return !decimalPlaces && data.IsNullOrZero(); }
	bool IsOne() const { return !decimalPlaces && data.IsOne(); }
	bool IsNegative() const { return data.IsNegative(); }
	bool IsPositive() const { return data.IsPositive(); }

	BigDecimal& operator=(SIGNED_DOUBLE_DIGIT value) { data = value; decimalPlaces = 0; return *this; }
	BigDecimal& operator =(const float value) { DECIMAL decimalValue; VarDecFromR4(value, &decimalValue); *this = decimalValue; return *this; }
	BigDecimal& operator =(const double value) { DECIMAL decimalValue; VarDecFromR8(value, &decimalValue); *this = decimalValue; return *this; }
	BigDecimal& operator =(const DECIMAL& value);
	BigDecimal& operator =(const char value[]) { *this = string(value); }
	BigDecimal& operator =(const string& value);
	BigDecimal& operator=(const BigNumeric& value) { data = value; decimalPlaces = 0; }
	BigDecimal& operator=(const BigInteger& value) { return (data = value, decimalPlaces = 0, *this); }
	BigDecimal& operator=(const BigDecimal& value) { return (data = value.data, decimalPlaces = value.decimalPlaces, *this); }

	BigDecimal WholePart() const { return BigDecimal(data / BigInteger(SIGNED_DOUBLE_DIGIT(DEC::RADIX)).Power(DOUBLE_DIGIT(decimalPlaces)), 0); }
	BigDecimal DecimalPart() const { return BigDecimal(data % BigInteger(SIGNED_DOUBLE_DIGIT(DEC::RADIX)).Power(DOUBLE_DIGIT(decimalPlaces)), decimalPlaces); }
	BigDecimal Inverse() const { return BigDecimal(1ll) / *this; }
	COMPARISON Compare(BigDecimal rightComparand) const;

	bool operator <(BigDecimal rightComparand) const { return Compare(rightComparand) == COMPARISON::MINOR; }
	bool operator <=(BigDecimal rightComparand) const { return Compare(rightComparand) != COMPARISON::MAJOR; }
	bool operator ==(BigDecimal rightComparand) const { return Compare(rightComparand) == COMPARISON::EQUAL; }
	bool operator !=(BigDecimal rightComparand) const { return Compare(rightComparand) != COMPARISON::EQUAL; }
	bool operator >(BigDecimal rightComparand) const { return Compare(rightComparand) == COMPARISON::MAJOR; }
	bool operator >=(BigDecimal rightComparand) const { return Compare(rightComparand) != COMPARISON::MINOR; }

	BigDecimal operator +() const { return BigDecimal(+data, decimalPlaces); }
	BigDecimal operator -() const { return BigDecimal(-data, decimalPlaces); }
	BigDecimal operator +(BigDecimal addend) const;
	BigDecimal operator -(BigDecimal subtrahend) const;
	BigDecimal operator *(SIGNED_DOUBLE_DIGIT multiplier) const { return BigDecimal(data * multiplier, decimalPlaces); }
	BigDecimal operator *(BigDecimal multiplier) const { return BigDecimal(data * multiplier.data, decimalPlaces + multiplier.decimalPlaces); }
	BigDecimal operator /(SIGNED_DOUBLE_DIGIT divisor) const;
	BigDecimal operator /(BigDecimal divisor) const;
	BigDecimal operator %(SIGNED_DOUBLE_DIGIT divisor) const { return *this - ((*this / divisor).WholePart() * divisor); }
	BigDecimal operator %(const BigDecimal divisor) const { return *this - ((*this / divisor).WholePart() * divisor); }

	BigDecimal& operator +=(const BigDecimal& addend) { return *this = *this + addend; }
	BigDecimal& operator -=(const BigDecimal& subtrahend) { return *this = *this - subtrahend; }
	BigDecimal& operator *=(SIGNED_DOUBLE_DIGIT multiplier) { return *this = *this * multiplier; }
	BigDecimal& operator *=(const BigDecimal& multiplier) { return *this = *this * multiplier; }
	BigDecimal& operator /=(SIGNED_DOUBLE_DIGIT divisor) { return *this = *this / divisor; }
	BigDecimal& operator /=(const BigDecimal& divisor) { return *this = *this / divisor; }
	BigDecimal& operator %=(SIGNED_DOUBLE_DIGIT divisor) { return *this = *this % divisor; }
	BigDecimal& operator %=(const BigDecimal& divisor) { return *this = *this % divisor; }
protected:
	void ToDecimalPlaces(size_t decimalPlaces) { this->decimalPlaces = IsNullOrZero() ? 0 : decimalPlaces; }
private:
	void Unnormalize(const size_t decimalPlaces);
	void Normalize();

	// Properties
	BigInteger data;
	size_t decimalPlaces;
};

#endif // BigDecimal_H
