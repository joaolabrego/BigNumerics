/******************************************************************************/
/*** File-Name: BcdMatrixRow.h  	                                        ***/
/*** Author...: João da Rocha Labrego                                       ***/
/*** E-Mail...: joaolabrego@gmail.com                                       ***/
/*** Purpose..: BcdMatrixRow header.                                        ***/
/******************************************************************************/

#include "BcdDecimal.h"

struct BcdMatrixRow : private vector<BcdDecimal>
{
public:
	BcdMatrixRow() {}
	BcdMatrixRow(const size_t dimension) { Create(dimension); }
	~BcdMatrixRow() {}

	BcdMatrixRow& Create(const size_t dimension);
	BcdDecimal& operator [](const size_t j) { return at(j); }
	BcdDecimal operator [](const size_t j) const { return at(j); }
	BcdMatrixRow& operator +(const BcdMatrixRow& addend);
	BcdMatrixRow& operator -(const BcdMatrixRow& subtrahend);
	BcdMatrixRow& operator *(const BcdDecimal& multiplier);

	BcdMatrixRow& operator +=(const BcdMatrixRow& addend) { return *this = *this + addend; }
	BcdMatrixRow& operator -=(const BcdMatrixRow& subtrahend) { return *this = *this - subtrahend; }
	BcdMatrixRow& operator *=(const BcdDecimal& multiplier) { return *this = *this * multiplier; }
protected:
private:
};

