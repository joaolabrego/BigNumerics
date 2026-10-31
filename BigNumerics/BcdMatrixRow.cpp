/******************************************************************************/
/*** File-Name: BcdMatrixRow.cpp  	                                        ***/
/*** Author...: João da Rocha Labrego                                       ***/
/*** E-Mail...: joaolabrego@gmail.com                                       ***/
/*** Purpose..: BcdMatrixRow implementation.                                ***/
/******************************************************************************/

#include "BcdMatrixRow.h"

/********************************************************/
BcdMatrixRow& BcdMatrixRow::Create(const size_t dimension)
/********************************************************/
{
	clear();
	resize(dimension);
	for (size_t i = 0; i < dimension; ++i)
		at(i).ToZero();

	return *this;
}
/****************************************************************/
BcdMatrixRow& BcdMatrixRow::operator +(const BcdMatrixRow& addend)
/****************************************************************/
{
	size_t length = size();

	if (addend.size() == length)
	{
		for (size_t i = 0; i < length; ++i)
			at(i) += addend.at(i);
	}

	return *this;
}
/********************************************************************/
BcdMatrixRow& BcdMatrixRow::operator -(const BcdMatrixRow& subtrahend)
/********************************************************************/
{
	size_t length = size();

	if (subtrahend.size() == length)
	{
		for (size_t i = 0; i < length; ++i)
			at(i) -= subtrahend.at(i);
	}

	return *this;
}
/******************************************************************/
BcdMatrixRow& BcdMatrixRow::operator *(const BcdDecimal& multiplier)
/******************************************************************/
{
	size_t length = size();

	for (size_t i = 0; i < length; ++i)
		at(i) *= multiplier;

	return *this;
}

/***********/
/*** EOF ***/
/***********/