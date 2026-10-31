/******************************************************************************/
/*** File-Name: BcdNumeric.h                                                ***/
/*** Author...: João da Rocha Labrego                                       ***/
/*** E-Mail...: joaolabrego@gmail.com                                       ***/
/*** Purpose..: BcdNumeric header.                                          ***/
/******************************************************************************/

#ifndef BcdNumeric_H
#define BcdNumeric_H

#include "General.h"

struct BcdNumeric
{
public:
	BcdNumeric() {}
	BcdNumeric(const DOUBLE_DIGIT value) { ToValue(value); }
	BcdNumeric(const char value[]) { *this = string(value); }
	BcdNumeric(const string& value) { *this = value; }
	BcdNumeric(const VECTOR& value) { *this = value; }
	~BcdNumeric() {}

	BcdNumeric& AsNumeric() { return *this; }
	BcdNumeric AsNumeric() const { return *this; }
	VECTOR& AsVector() { return data; }
	VECTOR AsVector() const { return data; }

	BcdNumeric& ToNull() { return (data.clear(), *this); }
	BcdNumeric& ToZero() { return (General::ToZero(data), *this); }
	BcdNumeric& ToOne() { return (General::ToOne(data), *this); }
	BcdNumeric& ToValue(DOUBLE_DIGIT value);
	string ToString(const bool grouped = SHOW_GROUPED) const;

	BcdNumeric& operator =(DOUBLE_DIGIT value) { return (ToValue(value), *this); }
	BcdNumeric& operator =(const char value[]) { return *this = string(value); }
	BcdNumeric& operator =(const string& value);
	BcdNumeric& operator =(const VECTOR& value) { return (General::Assign(data, value), *this); }
	BcdNumeric& operator =(const BcdNumeric& value) { return (General::Assign(data, value.data), *this); }

	bool IsNull() const { return data.empty(); }
	bool IsZero() const { return General::IsZero(data); }
	bool IsNullOrZero() const { return General::IsNullOrZero(data); }
	bool IsOne() const { return General::IsOne(data); }
	bool IsOdd() const { return General::IsOdd(data); }
	bool IsEven() const { return General::IsEven(data); }
	COMPARISON Compare(const DOUBLE_DIGIT rightComparand) const;
	COMPARISON Compare(const BcdNumeric& rightComparand) const { return General::Compare(data, rightComparand.data); }

	BcdNumeric Divide(const DOUBLE_DIGIT divisor, DIGIT& remainder) const;
	DIGIT BinarySearchDigit(const BcdNumeric& divisor, BcdNumeric& remainder) const;
	BcdNumeric Divide(const BcdNumeric& divisor, BcdNumeric& remainder) const;
	BcdNumeric Power(DOUBLE_DIGIT exponent) const;
	void BinarySearchDigit(DOUBLE_DIGIT degree, BcdNumeric& root, BcdNumeric& remainder) const;
	BcdNumeric Root(const DOUBLE_DIGIT degree, BcdNumeric& remainder) const;
	BcdNumeric GCD(const BcdNumeric& rightValue) const;
	BcdNumeric LCM(const BcdNumeric& rightValue) const; 
	BcdNumeric Factorial() const;


	bool operator <(const DOUBLE_DIGIT rightComparand) const { return Compare(rightComparand) == COMPARISON::MINOR; }
	bool operator <=(const DOUBLE_DIGIT rightComparand) const { return Compare(rightComparand) != COMPARISON::MAJOR; }
	bool operator ==(const DOUBLE_DIGIT rightComparand) const { return Compare(rightComparand) == COMPARISON::EQUAL; }
	bool operator !=(const DOUBLE_DIGIT rightComparand) const { return Compare(rightComparand) != COMPARISON::EQUAL; }
	bool operator >(const DOUBLE_DIGIT rightComparand) const { return Compare(rightComparand) == COMPARISON::MAJOR; }
	bool operator >=(const DOUBLE_DIGIT rightComparand) const { return Compare(rightComparand) != COMPARISON::MINOR; }

	bool operator <(const BcdNumeric& rightComparand) const { return Compare(rightComparand) == COMPARISON::MINOR; }
	bool operator <=(const BcdNumeric& rightComparand) const { return Compare(rightComparand) != COMPARISON::MAJOR; }
	bool operator ==(const BcdNumeric& rightComparand) const { return Compare(rightComparand) == COMPARISON::EQUAL; }
	bool operator !=(const BcdNumeric& rightComparand) const { return Compare(rightComparand) != COMPARISON::EQUAL; }
	bool operator >(const BcdNumeric& rightComparand) const { return Compare(rightComparand) == COMPARISON::MAJOR; }
	bool operator >=(const BcdNumeric& rightComparand) const { return Compare(rightComparand) != COMPARISON::MINOR; }

	BcdNumeric operator +(const BcdNumeric& addend) const;
	BcdNumeric& operator ++();
	BcdNumeric operator ++(int) { BcdNumeric last = *this; ++ *this; return last; }
	BcdNumeric operator -(const BcdNumeric& subtrahend) const;
	BcdNumeric& operator --();
	BcdNumeric operator --(int) { BcdNumeric last = *this; -- *this; return last; }
	BcdNumeric operator *(DOUBLE_DIGIT multiplier) const;
	BcdNumeric operator *(const BcdNumeric& multiplier) const;
	BcdNumeric operator /(DOUBLE_DIGIT divisor) const { DIGIT remainder; return Divide(divisor, remainder); }
	BcdNumeric operator /(const BcdNumeric& divisor) const { BcdNumeric remainder; return Divide(divisor, remainder); }
	DIGIT operator %(DOUBLE_DIGIT divisor) const { DIGIT remainder; BcdNumeric quotient = Divide(divisor, remainder); return remainder; }
	BcdNumeric operator %(const BcdNumeric& divisor) const { BcdNumeric remainder, quotient = Divide(divisor, remainder); return remainder; }

	BcdNumeric& operator +=(const BcdNumeric& addend) { return *this = *this + addend; }
	BcdNumeric& operator -=(const BcdNumeric& subtrahend) { return *this = *this - subtrahend; }
	BcdNumeric& operator *=(const DOUBLE_DIGIT multiplier) { return *this = *this * multiplier; }
	BcdNumeric& operator *=(const BcdNumeric& multiplier) { return *this = *this * multiplier; }
	BcdNumeric& operator /=(const DOUBLE_DIGIT divisor) { return *this = *this / divisor; }
	BcdNumeric& operator /=(const BcdNumeric& divisor) { return *this = *this / divisor; }
	BcdNumeric& operator %=(const DOUBLE_DIGIT divisor) { return *this = *this % divisor; }
	BcdNumeric& operator %=(const BcdNumeric& divisor) { return *this = *this % divisor; }
protected:
	BcdNumeric Add(const BcdNumeric& addend) const;
	static BcdNumeric CalculateGCD(BcdNumeric leftValue, BcdNumeric rightValue);
private:
	VECTOR data;
};

#endif // BcdNumeric_H