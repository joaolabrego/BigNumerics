/******************************************************************************/
/*** File-Name: Config.h                                                    ***/
/*** Author...: João da Rocha Labrego                                       ***/
/*** E-Mail...: joaolabrego@gmail.com                                       ***/
/*** Purpose..: Config header.                                              ***/
/******************************************************************************/

#ifndef Config_H
#define Config_H

#include <windows.h>
#include <vector>
#include <string>
#include <locale>
#include <math.h>
#include <cstring>
#undef NULL

using namespace std;

// Changeable definitions.
static const size_t DECIMAL_PLACES = 6; // Change with even values ​​greater than zero.
static const string UNDEFINED = "NULL";
static const string ELLIPSIS = "...";

// Unchangeable definitions.
typedef INT32 SIGNED_DIGIT; // Change the type to INT64 if type INT128 is available.
typedef UINT32 DIGIT; // Change the type to UINT64 if type UINT128 is available.
typedef INT64 SIGNED_DOUBLE_DIGIT; // Change the type to INT128 if type INT128 is available.
typedef UINT64 DOUBLE_DIGIT; // Change the type to UINT128 if type UINT128 is available.
typedef vector<DIGIT> VECTOR;

namespace BIN
{
	const DIGIT RADIX = 2;
	const size_t DIGIT_LENGTH = sizeof(DIGIT) * CHAR_BIT;
	const DOUBLE_DIGIT BASE = DOUBLE_DIGIT(pow(RADIX, DIGIT_LENGTH));
	const DIGIT MAXIMUM_VALUE = DIGIT(BASE - 1);
}
namespace DEC
{
	const DIGIT RADIX = 10;
	const size_t DIGIT_LENGTH = size_t(log10(double(BIN::BASE)));
	const DIGIT BASE = DIGIT(pow(RADIX, double(DIGIT_LENGTH)));
	const DIGIT MAXIMUM_VALUE = DIGIT(BASE - 1);
}

enum class COMPARISON : INT8 { MINOR = -1, EQUAL = 0, MAJOR = 1 };
enum class SIGN : INT8 { NEGATIVE = -1, POSITIVE = 1 };
enum class SIGNAL : char { NEGATIVE = '-', POSITIVE = '+' };

enum class ANGLE_UNIT : char { DEGREE = 'D', GRADIAN = 'G', RADIAN = 'R' };
enum class ANGLE_QUADRANT : UINT8 { FIRST = 0, SECOND = 1, THIRD = 2, FOURTH = 3 };
enum class ANGLE_TYPE : UINT8 { ANGLE = 0, SIN = 1, COS = 2, TAN = 3, COT = 4, SEC = 5, CSC = 6, SINH = 7, COSH = 8, TANH = 9, COTH = 10, SECH = 11, CSCH = 12 };

#endif // Config_H