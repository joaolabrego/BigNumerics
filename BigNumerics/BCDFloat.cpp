/******************************************************************************/
/*** File-Name: BCDFloat.cpp	                                            ***/
/*** Author...: João da Rocha Labrego                                       ***/
/*** E-Mail...: joaolabrego@gmail.com                                       ***/
/*** Purpose..: BCDFloat implementation.                                   ***/
/******************************************************************************/

#include "BCDFloat.h"

/***************************************************************/
BCDFloat& BCDFloat::operator =(const SIGNED_DOUBLE_DIGIT value)
/***************************************************************/
{
	data = value;
	if (!data.AsVector().IsNullOrZero())
		exponent = data.AsVector().size();

	return *this;
}
/***************************************************/
BCDFloat& BCDFloat::operator =(const string& value)
/***************************************************/
{
	BCDDecimal dec = value;

	if (!(data = dec.AsInteger()).AsVector().IsNullOrZero())
		ToExponent(SIGNED_DOUBLE_DIGIT(dec.WholePart().AsVector().size()));

	return *this;
}
/*******************************************************/
void BCDFloat::Denormalize(SIGNED_DOUBLE_DIGIT exponent)
/*******************************************************/
{
	if (!data.AsVector().IsNullOrZero())
	{
		if (exponent < this->exponent)
		{
			General::Append(data.AsVector(), size_t(abs(this->exponent - exponent)), 0);
			this->exponent = exponent;
		}
	}
}