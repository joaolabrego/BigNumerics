/******************************************************************************/
/*** File-Name: BCDFloat.h  	                                            ***/
/*** Author...: João da Rocha Labrego                                       ***/
/*** E-Mail...: joaolabrego@gmail.com                                       ***/
/*** Purpose..: BCDFloat header.                                           ***/
/******************************************************************************/

#include "BCDDecimal.h"

struct BCDFloat
{
public:
	BCDFloat() { ToNull(); }
	BCDFloat(const SIGNED_DOUBLE_DIGIT value) { *this = value; }
	BCDFloat(const string& value) { *this = value; }
	~BCDFloat() {}

	BCDFloat& ToNull() { data.AsVector().clear(); exponent = 0; }
	void ToExponent(SIGNED_DOUBLE_DIGIT exponent) { this->exponent = data.AsVector().IsNullOrZero() ? 0 : exponent; }

	BCDFloat& operator =(const SIGNED_DOUBLE_DIGIT value);
	BCDFloat& operator =(const string& value);


protected:
	void Denormalize(SIGNED_DOUBLE_DIGIT exponent);
private:
	BCDInteger data;
	SIGNED_DOUBLE_DIGIT exponent;
};

