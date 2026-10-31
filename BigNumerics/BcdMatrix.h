/******************************************************************************/
/*** File-Name: BcdMatrix.h  	                                            ***/
/*** Author...: João da Rocha Labrego                                       ***/
/*** E-Mail...: joaolabrego@gmail.com                                       ***/
/*** Purpose..: BcdMatrix header.                                           ***/
/******************************************************************************/

#ifndef BcdMatrix_H
#define BcdMatrix_H

#include "BcdMatrixRow.h"

typedef vector<BcdMatrixRow> MATRIX;

struct BcdMatrix
{
public:
	BcdMatrix() {}
	BcdMatrix(const size_t order) { Create(order); }
	~BcdMatrix() {}

	BcdMatrix& Create(const size_t order);
	BcdMatrixRow& operator[](const size_t i) { return data.at(i); }
	BcdMatrixRow operator[](const size_t i) const { return data.at(i); }
	BcdMatrix& operator +(const BcdMatrix& addend);
	BcdMatrix& operator -(const BcdMatrix& subtrahend);
	BcdMatrix& operator *(const BcdDecimal& multiplier);
	BcdMatrix& operator *(const BcdMatrix& multiplier);


protected:
private:
	MATRIX data;
};

#endif // BcdMatrix_H