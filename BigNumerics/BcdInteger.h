/******************************************************************************/
/*** File-Name: BcdInteger.h                                                ***/
/*** Author...: João da Rocha Labrego                                       ***/
/*** E-Mail...: joaolabrego@gmail.com                                       ***/
/*** Purpose..: BcdInteger header.                                          ***/
/******************************************************************************/
#ifndef BcdInteger_H
#define BcdInteger_H

#include "BcdNumeric.h"

struct BcdInteger
{
public:
	BcdInteger() { ToNull(); }
	BcdInteger(const SIGNED_DOUBLE_DIGIT value) { *this = value; }
	BcdInteger(const char value[]) { *this = string(value); }
	BcdInteger(const string& value) { *this = value; }
	BcdInteger(const VECTOR& value) { *this = value; }
	BcdInteger(const SIGN sign, const BcdNumeric& value) { ToValue(sign, value); }
	~BcdInteger() {}

	BcdInteger& AsInteger() { return *this; }
	BcdInteger AsInteger() const { return *this; }
	BcdNumeric& AsNumeric() { return data; }
	BcdNumeric AsNumeric() const { return data; }
	VECTOR& AsVector() { return data.AsVector(); }
	VECTOR AsVector() const { return data.AsVector(); }

	BcdInteger& ToNull() { sign = SIGN::NEGATIVE; data.ToNull(); return *this; }
	BcdInteger& ToMinusOne() { sign = SIGN::NEGATIVE; data.ToOne(); return *this; }
	BcdInteger& ToZero() { sign = SIGN::POSITIVE; data.ToZero(); return *this; }
	BcdInteger& ToOne() { sign = SIGN::POSITIVE; data.ToOne(); return *this; }
	BcdInteger& ToValue(const SIGN sign, const BcdNumeric& value);
	void ToSign(SIGN sign) { this->sign = IsNull() ? SIGN::NEGATIVE : IsZero() ? SIGN::POSITIVE : sign; }
	string ToString(const bool grouped = SHOW_GROUPED) const;

	COMPARISON Compare(SIGNED_DOUBLE_DIGIT rightComparand) const;
	COMPARISON Compare(const BcdInteger& rightComparand) const;
	bool IsNull() const { return data.IsNull(); }
	bool IsMinusOne() const { return sign == SIGN::NEGATIVE && data.IsOne(); }
	bool IsZero() const { return data.IsZero(); }
	bool IsNullOrZero() const { return data.IsNullOrZero(); }
	bool IsOne() const { return sign == SIGN::POSITIVE && data.IsOne(); }
	bool IsNegative() const { return sign == SIGN::NEGATIVE; }
	bool IsPositive() const { return sign == SIGN::POSITIVE; }
	SIGN Sign() const { return sign; }

	BcdInteger Divide(SIGNED_DOUBLE_DIGIT divisor, SIGNED_DOUBLE_DIGIT& remainder) const;
	BcdInteger Divide(const BcdInteger& divisor, BcdInteger& remainder) const;
	BcdInteger Power(DOUBLE_DIGIT exponent) const { return BcdInteger(exponent & 0x1 ? sign : SIGN::POSITIVE, data.Power(exponent)); }
	BcdInteger Root(const DOUBLE_DIGIT degree, BcdInteger& remainder) const;
	BcdInteger GCD(const BcdInteger& rightValue) const;
	BcdInteger LCM(const BcdInteger& rightValue) const;
	BcdInteger Factorial() const;

	BcdInteger& operator =(SIGNED_DOUBLE_DIGIT value) { this->sign = ToAbsolute(value); data = value; return *this; }
	BcdInteger& operator =(const char value[]) { return *this = string(value); }
	BcdInteger& operator =(const string& value);
	BcdInteger& operator =(const VECTOR& value) { sign = SIGN::POSITIVE; data = value; return *this; }
	BcdInteger& operator =(const BcdNumeric& value) { sign = SIGN::POSITIVE; data = value; return *this; }
	BcdInteger& operator =(BcdInteger value) { sign = value.sign; data = value.data; return *this; }

	bool operator <(const DOUBLE_DIGIT rightComparand) const { return Compare(rightComparand) == COMPARISON::MINOR; }
	bool operator <=(const DOUBLE_DIGIT rightComparand) const { return Compare(rightComparand) != COMPARISON::MAJOR; }
	bool operator ==(const DOUBLE_DIGIT rightComparand) const { return Compare(rightComparand) == COMPARISON::EQUAL; }
	bool operator !=(const DOUBLE_DIGIT rightComparand) const { return Compare(rightComparand) != COMPARISON::EQUAL; }
	bool operator >(const DOUBLE_DIGIT rightComparand) const { return Compare(rightComparand) == COMPARISON::MAJOR; }
	bool operator >=(const DOUBLE_DIGIT rightComparand) const { return Compare(rightComparand) != COMPARISON::MINOR; }

	bool operator <(const BcdInteger& rightComparand) const { return Compare(rightComparand) == COMPARISON::MINOR; }
	bool operator <=(const BcdInteger& rightComparand) const { return Compare(rightComparand) != COMPARISON::MAJOR; }
	bool operator ==(const BcdInteger& rightComparand) const { return Compare(rightComparand) == COMPARISON::EQUAL; }
	bool operator !=(const BcdInteger& rightComparand) const { return Compare(rightComparand) != COMPARISON::EQUAL; }
	bool operator >(const BcdInteger& rightComparand) const { return Compare(rightComparand) == COMPARISON::MAJOR; }
	bool operator >=(const BcdInteger& rightComparand) const { return Compare(rightComparand) != COMPARISON::MINOR; }

	BcdInteger operator +() const { return BcdInteger(SIGN::POSITIVE, data); }
	BcdInteger operator +(const BcdInteger& addend) const;
	BcdInteger& operator ++();
	BcdInteger operator ++(int) { BcdInteger last = *this; ++*this; return last; }
	BcdInteger operator -() const { return BcdInteger(General::NotSign(sign), data); }
	BcdInteger operator -(const BcdInteger& subtrahend) const;
	BcdInteger& operator --();
	BcdInteger operator --(int) { BcdInteger last = *this; --*this; return last; }
	BcdInteger operator *(SIGNED_DOUBLE_DIGIT multiplier) const { return BcdInteger(this->sign == ToAbsolute(multiplier) ? SIGN::POSITIVE : SIGN::NEGATIVE, data * multiplier); }
	BcdInteger operator *(const BcdInteger& multiplier) const { return BcdInteger(sign == multiplier.sign ? SIGN::POSITIVE : SIGN::NEGATIVE, data * multiplier.data); }
	BcdInteger operator /(SIGNED_DOUBLE_DIGIT divisor) const { SIGNED_DOUBLE_DIGIT remainder; return Divide(divisor, remainder); }
	BcdInteger operator /(const BcdInteger& divisor) const { BcdInteger remainder; return Divide(divisor, remainder); }
	SIGNED_DOUBLE_DIGIT operator %(SIGNED_DOUBLE_DIGIT divisor) const { SIGNED_DOUBLE_DIGIT remainder; BcdInteger quotient = Divide(divisor, remainder); return remainder; }
	BcdInteger operator %(const BcdInteger& divisor) const { BcdInteger remainder, quotient = Divide(divisor, remainder); return remainder; }

	BcdInteger& operator +=(const BcdInteger& addend) { return *this = *this + addend; }
	BcdInteger& operator -=(const BcdInteger& subtrahend) { return *this = *this - subtrahend; }
	BcdInteger& operator *=(const DOUBLE_DIGIT multiplier) { return *this = *this * multiplier; }
	BcdInteger& operator *=(const BcdInteger& multiplier) { return *this = *this * multiplier; }
	BcdInteger& operator /=(const DOUBLE_DIGIT divisor) { return *this = *this / divisor; }
	BcdInteger& operator /=(const BcdInteger& divisor) { return *this = *this / divisor; }
	BcdInteger& operator %=(const DOUBLE_DIGIT divisor) { return *this = *this % divisor; }
	BcdInteger& operator %=(const BcdInteger& divisor) { return *this = *this % divisor; }
protected:
private:
	SIGN sign;
	BcdNumeric data;
};
#endif // BcdInteger_H