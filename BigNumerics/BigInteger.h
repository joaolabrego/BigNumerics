/******************************************************************************/
/*** File-Name: BigInteger.h                                                ***/
/*** Author...: João da Rocha Labrego                                       ***/
/*** E-Mail...: joaolabrego@gmail.com                                       ***/
/*** Purpose..: BigInteger header.                                          ***/
/******************************************************************************/

#ifndef BigInteger_H
#define BigInteger_H

#include "BigNumeric.h"

struct BigInteger
{
public:
	BigInteger() { ToNull(); }
	BigInteger(const SIGNED_DOUBLE_DIGIT value) { *this = value; }
	BigInteger(const char value[]) { *this = string(value); }
	BigInteger(const string& value) { *this = value; }
	BigInteger(const BigNumeric& value) { *this = value; }
	BigInteger(const SIGN sign, const BigNumeric& value) { ToValue(sign, value); }
	~BigInteger() {}

	BigNumeric& AsNumeric() { return data; }
	BigNumeric AsNumeric() const { return data; }
	VECTOR& AsVector() { return data.AsVector(); }
	VECTOR AsVector() const { return data.AsVector(); }

	BigInteger& ToNull() { sign = SIGN::NEGATIVE; data.ToNull(); return *this; }
	BigInteger& ToMinusOne() { sign = SIGN::NEGATIVE; data.ToOne(); return *this; }
	BigInteger& ToZero() { sign = SIGN::POSITIVE; data.ToZero(); return *this; }
	BigInteger& ToOne() { sign = SIGN::POSITIVE; data.ToOne(); return *this; }
	BigInteger& ToValue(const SIGN sign, const BigNumeric& value);
	void ToSign(SIGN sign) { this->sign = IsNull() ? SIGN::NEGATIVE : IsZero() ? SIGN::POSITIVE : sign; }
	string ToString(const bool grouped = SHOW_GROUPED) const;

	COMPARISON Compare(SIGNED_DOUBLE_DIGIT rightComparand) const;
	COMPARISON Compare(const BigInteger& rightComparand) const;
	bool IsNull() const { return data.IsNull(); }
	bool IsMinusOne() const { return sign == SIGN::NEGATIVE && data.IsOne(); }
	bool IsZero() const { return data.IsZero(); }
	bool IsNullOrZero() const { return data.IsNullOrZero(); }
	bool IsOne() const { return sign == SIGN::POSITIVE && data.IsOne(); }
	bool IsNegative() const { return sign == SIGN::NEGATIVE; }
	bool IsPositive() const { return sign == SIGN::POSITIVE; }
	bool IsEven() const { return data.IsEven(); }
	bool IsOdd() const { return data.IsOdd(); }
	SIGN Sign() const { return sign; }

	BigInteger Divide(SIGNED_DOUBLE_DIGIT divisor, SIGNED_DOUBLE_DIGIT& remainder) const;
	BigInteger Divide(const BigInteger& divisor, BigInteger& remainder) const;
	BigInteger Power(DOUBLE_DIGIT exponent) const { return BigInteger(exponent & 0x1 ? sign : SIGN::POSITIVE, data.Power(exponent)); }
	BigInteger Root(const DOUBLE_DIGIT degree, BigInteger& remainder) const;
	BigInteger GCD(const BigInteger& rightValue) const;
	BigInteger LCM(const BigInteger& rightValue) const;
	BigInteger Factorial() const;

	bool operator <(const DOUBLE_DIGIT rightComparand) const { return Compare(rightComparand) == COMPARISON::MINOR; }
	bool operator <=(const DOUBLE_DIGIT rightComparand) const { return Compare(rightComparand) != COMPARISON::MAJOR; }
	bool operator ==(const DOUBLE_DIGIT rightComparand) const { return Compare(rightComparand) == COMPARISON::EQUAL; }
	bool operator !=(const DOUBLE_DIGIT rightComparand) const { return Compare(rightComparand) != COMPARISON::EQUAL; }
	bool operator >(const DOUBLE_DIGIT rightComparand) const { return Compare(rightComparand) == COMPARISON::MAJOR; }
	bool operator >=(const DOUBLE_DIGIT rightComparand) const { return Compare(rightComparand) != COMPARISON::MINOR; }

	bool operator <(const BigInteger& rightComparand) const { return Compare(rightComparand) == COMPARISON::MINOR; }
	bool operator <=(const BigInteger& rightComparand) const { return Compare(rightComparand) != COMPARISON::MAJOR; }
	bool operator ==(const BigInteger& rightComparand) const { return Compare(rightComparand) == COMPARISON::EQUAL; }
	bool operator !=(const BigInteger& rightComparand) const { return Compare(rightComparand) != COMPARISON::EQUAL; }
	bool operator >(const BigInteger& rightComparand) const { return Compare(rightComparand) == COMPARISON::MAJOR; }
	bool operator >=(const BigInteger& rightComparand) const { return Compare(rightComparand) != COMPARISON::MINOR; }

	BigInteger& operator =(SIGNED_DOUBLE_DIGIT value) { this->sign = ToAbsolute(value); data = value; return *this; }
	BigInteger& operator =(const char value[]) { return *this = string(value); }
	BigInteger& operator =(const string& value);
	BigInteger& operator =(const VECTOR& value) { sign = SIGN::POSITIVE; data = value; return *this; }
	BigInteger& operator =(const BigNumeric& value) { sign = SIGN::POSITIVE; data = value; return *this; }
	BigInteger& operator =(BigInteger value) { sign = value.sign; data = value.data; return *this; }

	BigInteger operator +() const { return BigInteger(SIGN::POSITIVE, data); }
	BigInteger operator +(const BigInteger& addend) const;
	BigInteger& operator ++();
	BigInteger operator ++(int) { BigInteger last = *this; ++* this; return last; }
	BigInteger operator -() const { return BigInteger(General::NotSign(sign), data); }
	BigInteger operator -(const BigInteger& subtrahend) const;
	BigInteger& operator --();
	BigInteger operator --(int) { BigInteger last = *this; --* this; return last; }
	BigInteger operator *(SIGNED_DOUBLE_DIGIT multiplier) const { return BigInteger(this->sign == ToAbsolute(multiplier) ? SIGN::POSITIVE : SIGN::NEGATIVE, data * multiplier); }
	BigInteger operator *(const BigInteger& multiplier) const { return BigInteger(sign == multiplier.sign ? SIGN::POSITIVE : SIGN::NEGATIVE, this->data * multiplier.data); }
	BigInteger operator /(SIGNED_DOUBLE_DIGIT divisor) const { SIGNED_DOUBLE_DIGIT remainder; return Divide(divisor, remainder); }
	BigInteger operator /(const BigInteger& divisor) const { BigInteger remainder; return Divide(divisor, remainder); }
	SIGNED_DOUBLE_DIGIT operator %(SIGNED_DOUBLE_DIGIT divisor) const { SIGNED_DOUBLE_DIGIT remainder; BigInteger quotient = Divide(divisor, remainder); return remainder; }
	BigInteger operator %(const BigInteger& divisor) const { BigInteger remainder, quotient = Divide(divisor, remainder); return remainder; }

	BigInteger& operator +=(const BigInteger& addend) { return *this = *this + addend; }
	BigInteger& operator -=(const BigInteger& subtrahend) { return *this = *this - subtrahend; }
	BigInteger& operator *=(const DOUBLE_DIGIT multiplier) { return *this = *this * multiplier; }
	BigInteger& operator *=(const BigInteger& multiplier) { return *this = *this * multiplier; }
	BigInteger& operator /=(const DOUBLE_DIGIT divisor) { return *this = *this / divisor; }
	BigInteger& operator /=(const BigInteger& divisor) { return *this = *this / divisor; }
	BigInteger& operator %=(const DOUBLE_DIGIT divisor) { return *this = *this % divisor; }
	BigInteger& operator %=(const BigInteger& divisor) { return *this = *this % divisor; }
protected:
private:
	SIGN sign;
	BigNumeric data;
};

#endif // BigInteger_H