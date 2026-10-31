/******************************************************************************/
/*** File-Name: General.h                                                   ***/
/*** Author...: João da Rocha Labrego                                       ***/
/*** E-Mail...: joaolabrego@gmail.com                                       ***/
/*** Purpose..: General header.                                             ***/
/******************************************************************************/

#ifndef General_H
#define General_H

#include "Config.h"

static const VECTOR NULL = {};
static const VECTOR ZERO = { 0 };
static const VECTOR ONE = { 1 };

namespace General
{
	SIGN ToSign(const char signal);
	SIGN ToSign(const SIGNAL signal);
	SIGNAL ToSignal(const char signal);
	SIGNAL ToSignal(const SIGN sign);
	SIGN NotSign(const SIGN sign);
	SIGNAL NotSignal(const SIGNAL signal);
	bool IsSignal(const char signal);
	COMPARISON NotComparison(const COMPARISON comparison);

	size_t GetPrecision(bool ignoreExtra = false);
	void UpPrecision();
	void DownPrecision();

	string PadLeft(const string& value, size_t length = DEC::DIGIT_LENGTH, const string& filler = "0");
	string PadRight(const string& value, size_t length = DEC::DIGIT_LENGTH, const string& filler = "0");
	string LeftTrim(const string& value);
	string RightTrim(const string& value);
	string Agroup(const string& value);
	string Replace(const string& value, const string& oldValue, const string& newValue);
	string Replicate(const string& value, const size_t count);
	bool IsNumeric(const string& value);
	bool Parse(string value, char& signal, string& wholePart, string& decimalPart);
	string Repetend(const string& value);
	VECTOR Repetend(const string& value, const size_t times);
	static size_t CountDigits(const string& value, const string digit);
	static void Decrement(string& value);

	COMPARISON Compare(const VECTOR& leftComparand, const VECTOR& rightComparand);
	void Normalize(VECTOR& value);
	VECTOR SubVector(const VECTOR& value, const size_t start, size_t length = string::npos);
	VECTOR& Assign(VECTOR& target, const VECTOR& value);
	VECTOR& Insert(VECTOR& target, const DIGIT value);
	VECTOR& Insert(VECTOR& target, const size_t count, const DIGIT value);
	VECTOR& Insert(VECTOR& target, const VECTOR& value);
	VECTOR& Append(VECTOR& target, const DIGIT value);
	VECTOR& Append(VECTOR& target, const size_t count, const DIGIT value);
	VECTOR& Append(VECTOR& target, const VECTOR& value);
	VECTOR& ToZero(VECTOR& target);
	VECTOR& ToOne(VECTOR& target);
	bool IsZero(const VECTOR& value);
	bool IsNullOrZero(const VECTOR& value);
	bool IsOne(const VECTOR& value);
	bool IsOdd(const VECTOR& value);
	bool IsEven(const VECTOR& value);

	bool ShowPositiveSignal();
	bool ShowGrouped();
	string NegativeSignal();
	string PositiveSignal();
	string DecimalSeparator();
	string ThousandSeparator();
	size_t ThousandLength();
};

static const char DECIMAL_SEPARATOR = General::DecimalSeparator().front();
static const char THOUSANDS_SEPARATOR = General::ThousandSeparator().front();
static const size_t THOUSANDS_LENGTH = General::ThousandLength();
static const bool SHOW_POSITIVE = General::ShowPositiveSignal();
static const bool SHOW_GROUPED = General::ShowGrouped();
static const char NEGATIVE_SIGNAL = General::NegativeSignal().front();
static const char POSITIVE_SIGNAL = General::PositiveSignal().front();

template <class T> SIGN ToAbsolute(T& value)
{
	if (value < 0)
	{
		value = -value;
		return SIGN::NEGATIVE;
	}

	return SIGN::POSITIVE;
}
static size_t counterPrecision = 0;

#endif // General_H