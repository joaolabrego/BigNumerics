/******************************************************************************/
/*** File-Name: BcdMatrix.cpp  	                                            ***/
/*** Author...: João da Rocha Labrego                                       ***/
/*** E-Mail...: joaolabrego@gmail.com                                       ***/
/*** Purpose..: BcdMatrix implementation.                                   ***/
/******************************************************************************/

#include "BcdMatrix.h"

/**********************************************/
BcdMatrix& BcdMatrix::Create(const size_t order)
/**********************************************/
{
	data.clear();
	if (order)
	{
		data.resize(order);
		for (size_t i = 0; i < order; ++i)
		{
			data.at(i).Create(order);
			for (size_t j = 0; j < order; ++j)
				data[i][j].ToZero();
		}
	}

	return *this;
}
/*******************************************************/
BcdMatrix& BcdMatrix::operator +(const BcdMatrix& addend)
/*******************************************************/
{
	size_t order = data.size();

	if (addend.data.size() == order)
	{
		for (size_t i = 0; i < order; ++i)
			data[i] += addend.data[i];
	}

	return *this;
}
/***********************************************************/
BcdMatrix& BcdMatrix::operator -(const BcdMatrix& subtrahend)
/***********************************************************/
{
	size_t order = data.size();

	if (subtrahend.data.size() == order)
	{
		for (size_t i = 0; i < order; ++i)
			data[i] -= subtrahend.data[i];
	}

	return *this;
}
/************************************************************/
BcdMatrix& BcdMatrix::operator *(const BcdDecimal& multiplier)
/************************************************************/
{
	size_t order = data.size();

	for (size_t i = 0; i < order; ++i)
		data[i] *= multiplier;

	return *this;
}
/***********************************************************/
BcdMatrix& BcdMatrix::operator *(const BcdMatrix& multiplier)
/***********************************************************/
{
	size_t order = data.size();
	
	if (order == multiplier.data.size())
	{
		for (size_t i = 0; i < order; ++i)
		{
			for (size_t j = 0; i < order; ++i)
				data[i][j] += (data[i][j] * multiplier.data[j][i]);
		}
	}

	return *this;
}

/***********/
/*** EOF ***/
/***********/