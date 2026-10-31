/******************************************************************************/
/*** File-Name: BigFloat.h                                                  ***/
/*** Author...: João da Rocha Labrego                                       ***/
/*** E-Mail...: joaolabrego@gmail.com                                       ***/
/*** Purpose..: BigFloat header.                                            ***/
/******************************************************************************/
#ifndef BigFloat_H
#define BigFloat_H

#include "BigInteger.h"
#include "BcdDecimal.h"

struct BigFloat
{
public:
	BigFloat() { ToNull(); }
	BigFloat(SIGNED_DOUBLE_DIGIT value) { *this = value; }
	BigFloat(const float value) { *this = value; }
	BigFloat(const double value) { *this = value; }
	BigFloat(const DECIMAL& value) { *this = value; }
	BigFloat(const char value[]) { *this = value; }
	BigFloat(string value) { *this = value; }
	BigFloat(const BigNumeric& value) { *this = value; }
	BigFloat(const BigInteger& value) { *this = value; }
	BigFloat(const BigFloat& value) { *this = value; }
	~BigFloat() {}

	void ToNull() { data.ToNull(); exponent = 0; }
	void ToMinusOne() { data.ToMinusOne(); exponent = 0; }
	void ToZero() { data.ToZero(); exponent = 0; }
	void ToOne() { data.ToOne(); exponent = 0; }

	bool IsNull() const { return data.IsNull(); }
	bool IsMinusOne() const { return !exponent && data.IsMinusOne(); }
	bool IsZero() const { return data.IsZero(); }
	bool IsOne() const { return !exponent && data.IsOne(); }
	bool IsNegative() const { return data.IsNegative(); }
	bool IsPositive() const { return data.IsPositive(); }

	//bool operator <(const DOUBLE_DIGIT rightComparand) const { return Compare(rightComparand) == COMPARISON::MINOR; }
	//bool operator <=(const DOUBLE_DIGIT rightComparand) const { return Compare(rightComparand) != COMPARISON::MAJOR; }
	//bool operator ==(const DOUBLE_DIGIT rightComparand) const { return Compare(rightComparand) == COMPARISON::EQUAL; }
	//bool operator !=(const DOUBLE_DIGIT rightComparand) const { return Compare(rightComparand) != COMPARISON::EQUAL; }
	//bool operator >(const DOUBLE_DIGIT rightComparand) const { return Compare(rightComparand) == COMPARISON::MAJOR; }
	//bool operator >=(const DOUBLE_DIGIT rightComparand) const { return Compare(rightComparand) != COMPARISON::MINOR; }

	//bool operator <(const BigFloat& rightComparand) const { return Compare(rightComparand) == COMPARISON::MINOR; }
	//bool operator <=(const BigFloat& rightComparand) const { return Compare(rightComparand) != COMPARISON::MAJOR; }
	//bool operator ==(const BigFloat& rightComparand) const { return Compare(rightComparand) == COMPARISON::EQUAL; }
	//bool operator !=(const BigFloat& rightComparand) const { return Compare(rightComparand) != COMPARISON::EQUAL; }
	//bool operator >(const BigFloat& rightComparand) const { return Compare(rightComparand) == COMPARISON::MAJOR; }
	//bool operator >=(const BigFloat& rightComparand) const { return Compare(rightComparand) != COMPARISON::MINOR; }

	BigFloat& operator=(SIGNED_DOUBLE_DIGIT value) { data = value; exponent = 0; return *this; }
	BigFloat& operator =(const float value) { DECIMAL decimal; VarDecFromR4(value, &decimal); return *this = decimal; }
	BigFloat& operator =(const double value) { DECIMAL decimal; VarDecFromR8(value, &decimal); return *this = decimal; }
	BigFloat& operator =(const DECIMAL& value) { return *this; }
	BigFloat& operator =(const char value[]) { return *this = string(value); }
	BigFloat& operator =(string value);
	BigFloat& operator=(const BigNumeric& value) { return (data = value, exponent = 0, *this); }
	BigFloat& operator=(const BigInteger& value) { return (data = value, exponent = 0, *this); }
	BigFloat& operator=(const BigFloat& value) { return (data = value.data, exponent = value.exponent, *this); }

protected:
private:
	SIGNED_DOUBLE_DIGIT exponent;
	BigInteger data;
};

#endif // BigFloat_H