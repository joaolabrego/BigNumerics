/******************************************************************************/
/*** File-Name: BigNumeric.h                                              ***/
/*** Author...: João da Rocha Labrego                                       ***/
/*** E-Mail...: joaolabrego@gmail.com                                       ***/
/*** Purpose..: BigNumeric header.                                        ***/
/******************************************************************************/

#ifndef BigNumeric_H
#define BigNumeric_H

#include "BcdNumeric.h"

struct BigNumeric
{
public:
	BigNumeric() {}
	BigNumeric(const DOUBLE_DIGIT value) { *this = value; }
	BigNumeric(const char value[]) { *this = value; }
	BigNumeric(const string& value) { *this = value; }
	BigNumeric(const VECTOR& value) { *this = value; }
	~BigNumeric() {}

	BigNumeric& AsNumeric() { return *this; }
	BigNumeric AsNumeric() const { return *this; }
	VECTOR& AsVector() { return data; }
	VECTOR AsVector() const { return data; }

	BigNumeric& ToNull() { return (data.clear(), *this); }
	BigNumeric& ToZero() { return (data.assign(ZERO.begin(), ZERO.end()), *this); }
	BigNumeric& ToOne() { return (data.assign(ONE.begin(), ONE.end()), *this); }
	BigNumeric& ToValue(DOUBLE_DIGIT value);
	string ToString(const bool grouped = SHOW_GROUPED) const;

	BigNumeric& operator =(DOUBLE_DIGIT value) { return (ToValue(value), *this); }
	BigNumeric& operator =(const char value[]) { return *this = string(value); }
	BigNumeric& operator =(const string& value);
	BigNumeric& operator =(const VECTOR& value) { return (data.assign(value.begin(), value.end()), *this); }
	BigNumeric& operator =(const BigNumeric& value) { return (data.assign(value.data.begin(), value.data.end()), *this); }

	bool IsNull() const { return data.empty(); }
	bool IsZero() const { return General::IsZero(data); }
	bool IsNullOrZero() const { return General::IsNullOrZero(data); }
	bool IsOne() const { return General::IsOne(data); }
	bool IsOdd() const { return General::IsOdd(data); }
	bool IsEven() const { return General::IsEven(data); }
	COMPARISON Compare(DOUBLE_DIGIT rightComparand) const;
	COMPARISON Compare(const BigNumeric& rightComparand) const { return General::Compare(data, rightComparand.data); }

	BigNumeric Divide(const DOUBLE_DIGIT divisor, DIGIT& remainder) const;
	DIGIT BinarySearchDigit(const BigNumeric& divisor, BigNumeric& remainder) const;
	BigNumeric Divide(const BigNumeric& divisor, BigNumeric& remainder) const;
	BigNumeric Power(DOUBLE_DIGIT exponent) const;
	void BinarySearchDigit(const DOUBLE_DIGIT degree, BigNumeric& root, BigNumeric& remainder) const;
	BigNumeric Root(const DOUBLE_DIGIT degree, BigNumeric& remainder) const;
	BigNumeric GCD(const BigNumeric& rightValue) const;
	BigNumeric LCM(const BigNumeric& rightValue) const;
	BigNumeric Factorial() const;

	bool operator <(DOUBLE_DIGIT rightComparand) const { return Compare(rightComparand) == COMPARISON::MINOR; }
	bool operator <=(DOUBLE_DIGIT rightComparand) const { return Compare(rightComparand) != COMPARISON::MAJOR; }
	bool operator ==(DOUBLE_DIGIT rightComparand) const { return Compare(rightComparand) == COMPARISON::EQUAL; }
	bool operator !=(DOUBLE_DIGIT rightComparand) const { return Compare(rightComparand) != COMPARISON::EQUAL; }
	bool operator >(DOUBLE_DIGIT rightComparand) const { return Compare(rightComparand) == COMPARISON::MAJOR; }
	bool operator >=(DOUBLE_DIGIT rightComparand) const { return Compare(rightComparand) != COMPARISON::MINOR; }

	bool operator <(BigNumeric rightComparand) const { return Compare(rightComparand) == COMPARISON::MINOR; }
	bool operator <=(BigNumeric rightComparand) const { return Compare(rightComparand) != COMPARISON::MAJOR; }
	bool operator ==(BigNumeric rightComparand) const { return Compare(rightComparand) == COMPARISON::EQUAL; }
	bool operator !=(BigNumeric rightComparand) const { return Compare(rightComparand) != COMPARISON::EQUAL; }
	bool operator >(BigNumeric rightComparand) const { return Compare(rightComparand) == COMPARISON::MAJOR; }
	bool operator >=(BigNumeric rightComparand) const { return Compare(rightComparand) != COMPARISON::MINOR; }

	BigNumeric operator +(const BigNumeric& addend) const;
	BigNumeric& operator ++();
	BigNumeric operator ++(int) { BigNumeric last = *this; ++* this; return last; }
	BigNumeric operator -(const BigNumeric& subtrahend) const;
	BigNumeric& operator --();
	BigNumeric operator --(int) { BigNumeric last = *this; --* this; return last; }
	BigNumeric operator *(DOUBLE_DIGIT multiplier) const;
	BigNumeric operator *(const BigNumeric& multiplier) const;
	BigNumeric operator /(DOUBLE_DIGIT divisor) const { DIGIT remainder; return Divide(divisor, remainder); }
	BigNumeric operator /(const BigNumeric& divisor) const { BigNumeric remainder; return Divide(divisor, remainder); }
	DIGIT operator %(DOUBLE_DIGIT divisor) const { DIGIT remainder; BigNumeric quotient = Divide(divisor, remainder); return remainder; }
	BigNumeric operator %(const BigNumeric& divisor) const { BigNumeric remainder, quotient = Divide(divisor, remainder); return remainder; }

	BigNumeric& operator +=(const BigNumeric& addend) { return *this = *this + addend; }
	BigNumeric& operator -=(const BigNumeric& subtrahend) { return *this = *this - subtrahend; }
	BigNumeric& operator *=(const DOUBLE_DIGIT multiplier) { return *this = *this * multiplier; }
	BigNumeric& operator *=(const BigNumeric& multiplier) { return *this = *this * multiplier; }
	BigNumeric& operator /=(const DOUBLE_DIGIT divisor) { return *this = *this / divisor; }
	BigNumeric& operator /=(const BigNumeric& divisor) { return *this = *this / divisor; }
	BigNumeric& operator %=(const DOUBLE_DIGIT divisor) { return *this = *this % divisor; }
	BigNumeric& operator %=(const BigNumeric& divisor) { return *this = *this % divisor; }
protected:
	BigNumeric Add(const BigNumeric& addend) const;
	static BigNumeric CalculateGCD(BigNumeric leftValue, BigNumeric rightValue);
private:
	VECTOR data;
};

#endif // BigNumeric_H
