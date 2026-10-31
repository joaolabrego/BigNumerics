/******************************************************************************/
/*** File-Name: BigFloat.cpp	                                            ***/
/*** Author...: João da Rocha Labrego                                       ***/
/*** E-Mail...: joaolabrego@gmail.com                                       ***/
/*** Purpose..: BigFloat implementation.                                    ***/
/******************************************************************************/

#include "BigFloat.h"

/******************************************/
BigFloat& BigFloat::operator =(string value)
/******************************************/
{
	DIGIT digit;
	BcdDecimal decimal = value;


	ToNull();
	if (!decimal.AsNumeric().IsNull())
	{
		exponent = 0;
		while (decimal.WholePart() >= BIN::BASE)
		{
			decimal /= BIN::BASE;
			++exponent;
		}
		while (decimal.WholePart().AsNumeric().IsZero())
		{
			decimal *= BIN::BASE;
			--exponent;
		}
	}

	return *this;
}