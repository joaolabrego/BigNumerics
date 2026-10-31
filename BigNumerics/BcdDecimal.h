/******************************************************************************/
/*** File-Name: BcdDecimal.h  	                                            ***/
/*** Author...: João da Rocha Labrego                                       ***/
/*** E-Mail...: joaolabrego@gmail.com                                       ***/
/*** Purpose..: BcdDecimal header.                                          ***/
/******************************************************************************/

#ifndef BcdDecimal_H
#define BcdDecimal_H

#include "BcdInteger.h"

struct BcdDecimal
{
public:
	BcdDecimal() { ToNull(); }
	BcdDecimal(const SIGNED_DOUBLE_DIGIT value) { *this = value; }
	BcdDecimal(const float value) { *this = value; }
	BcdDecimal(const double value) { *this = value; }
	BcdDecimal(const DECIMAL value) { *this = value; }
	BcdDecimal(const char value[]) { *this = string(value); }
	BcdDecimal(const string& value) { *this = value; }
	BcdDecimal(const VECTOR& value) { *this = value; }
	BcdDecimal(const BcdNumeric& value) { *this = value; }
	BcdDecimal(const BcdInteger& data, const size_t decimalPlaces) { ToValue(data, decimalPlaces); }
	~BcdDecimal() {}

	static BcdDecimal PI();
	static BcdDecimal DoublePI();
	static BcdDecimal E();
	static vector <BcdDecimal> GammaCoefficients();
	BcdDecimal Log(BcdDecimal base) const;
	BcdDecimal Antilog(BcdDecimal base = SIGNED_DOUBLE_DIGIT(10)) const;
	BcdDecimal Ln() const;
	BcdDecimal Log10() const { return Log(SIGNED_DOUBLE_DIGIT(10)); }
	BcdDecimal Exp() const;
	BcdDecimal Add(const BcdDecimal& addend) const;
	BcdDecimal Subtract(const BcdDecimal& subtrahend) const;
	BcdDecimal Multiply(const BcdDecimal& multiplier) const;
	BcdDecimal Divide(const BcdDecimal& divisor) const;
	BcdDecimal Power(const BcdDecimal& exponent) const;
	BcdDecimal Root(const BcdDecimal& degree) const;
	BcdDecimal Gamma() const;

	BcdDecimal& ToNull() { data.ToNull(); decimalPlaces = 0; return *this; }
	BcdDecimal& ToMinusOne() { data.ToOne(); decimalPlaces = 0; return *this; }
	BcdDecimal& ToZero() { data.ToZero(); decimalPlaces = 0; return *this; }
	BcdDecimal& ToOne() { data.ToOne(); decimalPlaces = 0; return *this; }
	BcdDecimal& ToValue(const BcdInteger& data, const size_t decimalPlaces);
	void ToSign(SIGN sign) { data.ToSign(sign); }
	string ToString(const bool grouped = SHOW_GROUPED) const;

	BcdDecimal& AsDecimal() { return *this; }
	BcdDecimal AsDecimal() const { return *this; }
	BcdInteger& AsInteger() { return data; }
	BcdInteger AsInteger() const { return data; }
	BcdNumeric& AsNumeric() { return data.AsNumeric(); }
	BcdNumeric AsNumeric() const { return data.AsNumeric(); }
	VECTOR& AsVector() { return data.AsVector(); }
	VECTOR AsVector() const { return data.AsVector(); }

	BcdDecimal& operator =(SIGNED_DOUBLE_DIGIT value) { data = value; decimalPlaces = 0; return *this; }
	BcdDecimal& operator =(const float value) { DECIMAL decimal; VarDecFromR4(value, &decimal); return *this = decimal; }
	BcdDecimal& operator =(const double value) { DECIMAL decimal; VarDecFromR8(value, &decimal); return *this = decimal; }
	BcdDecimal& operator =(const DECIMAL& value);
	BcdDecimal& operator =(const char value[]) { return *this = string(value); }
	BcdDecimal& operator =(const string& value);
	BcdDecimal& operator =(const VECTOR& value) { data = value; decimalPlaces = 0; return *this; }
	BcdDecimal& operator =(const BcdNumeric& value) { data = value; decimalPlaces = 0; return *this; }
	BcdDecimal& operator =(const BcdInteger& value) { data = value; decimalPlaces = 0; return *this; }
	BcdDecimal& operator =(const BcdDecimal& value) { data = value.data; decimalPlaces = value.decimalPlaces; return *this; }

	COMPARISON Compare(SIGNED_DOUBLE_DIGIT rightComparand) const { return Compare(BcdDecimal(rightComparand)); }
	COMPARISON Compare(BcdDecimal rightComparand) const;
	
	bool IsNull() const { return data.IsNull(); }
	bool IsMinusOne() const { return data.IsMinusOne() && !decimalPlaces; }
	bool IsZero() const { return data.IsZero(); }
	bool IsNullOrZero() const { return data.IsNullOrZero(); }
	bool IsOne() const { return data.IsOne() && !decimalPlaces; }
	bool IsNegative() const { return data.IsNegative(); }
	bool IsPositive() const { return data.IsPositive(); }
	BcdDecimal WholePart() const;
	BcdDecimal DecimalPart() const;
	SIGN Sign() const { return data.Sign(); }
	VECTOR Repetend(const size_t times) const;
	size_t DecimalPlaces() const { return decimalPlaces; }
	BcdDecimal& Truncate(size_t decimalPlaces = General::GetPrecision());
	BcdDecimal& Round(size_t decimalPlaces = General::GetPrecision());
	
	BcdDecimal Inverse() const { return BcdDecimal(SIGNED_DOUBLE_DIGIT(1)) / *this; }
	BcdDecimal Power(SIGNED_DOUBLE_DIGIT exponent) const;
	BcdDecimal Root(SIGNED_DOUBLE_DIGIT degree) const;

	bool operator <(const DOUBLE_DIGIT rightComparand) const { return Compare(rightComparand) == COMPARISON::MINOR; }
	bool operator <=(const DOUBLE_DIGIT rightComparand) const { return Compare(rightComparand) != COMPARISON::MAJOR; }
	bool operator ==(const DOUBLE_DIGIT rightComparand) const { return Compare(rightComparand) == COMPARISON::EQUAL; }
	bool operator !=(const DOUBLE_DIGIT rightComparand) const { return Compare(rightComparand) != COMPARISON::EQUAL; }
	bool operator >(const DOUBLE_DIGIT rightComparand) const { return Compare(rightComparand) == COMPARISON::MAJOR; }
	bool operator >=(const DOUBLE_DIGIT rightComparand) const { return Compare(rightComparand) != COMPARISON::MINOR; }

	bool operator <(const BcdDecimal& rightComparand) const { return Compare(rightComparand, General::GetPrecision(true)) == COMPARISON::MINOR; }
	bool operator <=(const BcdDecimal& rightComparand) const { return Compare(rightComparand, General::GetPrecision(true)) != COMPARISON::MAJOR; }
	bool operator ==(const BcdDecimal& rightComparand) const { return Compare(rightComparand, General::GetPrecision(true)) == COMPARISON::EQUAL; }
	bool operator !=(const BcdDecimal& rightComparand) const { return Compare(rightComparand, General::GetPrecision(true)) != COMPARISON::EQUAL; }
	bool operator >(const BcdDecimal& rightComparand) const { return Compare(rightComparand, General::GetPrecision(true)) == COMPARISON::MAJOR; }
	bool operator >=(const BcdDecimal& rightComparand) const { return Compare(rightComparand, General::GetPrecision(true)) != COMPARISON::MINOR; }

	BcdDecimal operator +() const { return BcdDecimal(+data, decimalPlaces); }
	BcdDecimal operator +(BcdDecimal addend) const;
	BcdDecimal operator -() const { return BcdDecimal(-data, decimalPlaces); }
	BcdDecimal operator -(BcdDecimal subtrahend) const;
	BcdDecimal operator *(SIGNED_DOUBLE_DIGIT multiplier) const { return BcdDecimal(data * multiplier, decimalPlaces); }
	BcdDecimal operator *(const BcdDecimal& multiplier) const { return BcdDecimal(data * multiplier.data, this->decimalPlaces + multiplier.decimalPlaces); }
	BcdDecimal operator /(SIGNED_DOUBLE_DIGIT divisor) const;
	BcdDecimal operator /(BcdDecimal divisor) const;
	BcdDecimal operator %(SIGNED_DOUBLE_DIGIT divisor) const { return *this - (*this / divisor).WholePart() * divisor; }
	BcdDecimal operator %(const BcdDecimal divisor) const { return *this - (*this / divisor).WholePart() * divisor; }

	BcdDecimal& operator +=(const BcdDecimal& addend) { return *this = *this + addend; }
	BcdDecimal& operator -=(const BcdDecimal& subtrahend) { return *this = *this - subtrahend; }
	BcdDecimal& operator *=(const DOUBLE_DIGIT multiplier) { return *this = *this * multiplier; }
	BcdDecimal& operator *=(const BcdDecimal& multiplier) { return *this = *this * multiplier; }
	BcdDecimal& operator /=(const DOUBLE_DIGIT divisor) { return *this = *this / divisor; }
	BcdDecimal& operator /=(const BcdDecimal& divisor) { return *this = *this / divisor; }
	BcdDecimal& operator %=(const DOUBLE_DIGIT divisor) { return *this = *this % divisor; }
	BcdDecimal& operator %=(const BcdDecimal& divisor) { return *this = *this % divisor; }
protected:
	COMPARISON Compare(BcdDecimal rightComparand, size_t precision) const;
	void Denormalize(const size_t decimalPlaces);
	void Normalize();
	void ToDecimalPlaces(size_t decimalPlaces) { this->decimalPlaces = IsNullOrZero() ? 0 : decimalPlaces; }
private:
	BcdInteger data;
	size_t decimalPlaces;
};
#endif // BcdDecimal_H