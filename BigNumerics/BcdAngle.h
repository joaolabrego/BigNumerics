/******************************************************************************/
/*** File-Name: BcdAngle.h                                                   ***/
/*** Author...: João da Rocha Labrego                                       ***/
/*** E-Mail...: joaolabrego@gmail.com                                       ***/
/*** Purpose..: General header.                                             ***/
/******************************************************************************/

#ifndef BcdAngle_H
#define BcdAngle_H

#include "BcdDecimal.h"

struct BcdAngle
{
public:
	BcdAngle() { type = ANGLE_TYPE::ANGLE; ToNull(); }
	BcdAngle(BcdDecimal value, ANGLE_UNIT unit = ANGLE_UNIT::DEGREE) { ToValue(value, unit); }
	~BcdAngle() {}

	BcdAngle& ToNull() { data.ToNull(); type = ANGLE_TYPE::ANGLE;  quadrant = ANGLE_QUADRANT::FIRST; return *this; }
	BcdAngle& ToValue(BcdDecimal value, ANGLE_UNIT unit);
	BcdAngle& ToValue(const BcdDecimal& value, ANGLE_TYPE type, ANGLE_QUADRANT quadrant);

	BcdDecimal& AsDecimal() { return data; }
	BcdDecimal AsDecimal() const { return data; }

	BcdDecimal AsRadian() const { return data; }
	BcdDecimal AsGradian() const { return data * 200 / BcdDecimal::PI(); }
	BcdDecimal AsDegree() const { return data * 180ll / BcdDecimal::PI(); }

	BcdAngle Sin() const { return BcdAngle(Sin(data), ANGLE_TYPE::SIN, quadrant); }
	BcdAngle Cos() const { return BcdAngle(Cos(data), ANGLE_TYPE::COS, quadrant); }
	BcdAngle Tan() const { return BcdAngle(Tan(data), ANGLE_TYPE::TAN, quadrant); }
	BcdAngle Cot() const { return BcdAngle(Cot(data), ANGLE_TYPE::COT, quadrant); }
	BcdAngle Sec() const { return BcdAngle(Sec(data), ANGLE_TYPE::SEC, quadrant); }
	BcdAngle Csc() const { return BcdAngle(Csc(data), ANGLE_TYPE::CSC, quadrant); }

	BcdAngle Sinh() const { return BcdAngle(Sinh(data), ANGLE_TYPE::SINH, quadrant); }
	BcdAngle Cosh() const { return BcdAngle(Cosh(data), ANGLE_TYPE::COSH, quadrant); }
	BcdAngle Tanh() const { return BcdAngle(Tanh(data), ANGLE_TYPE::TANH, quadrant); }
	BcdAngle Coth() const { return BcdAngle(Coth(data), ANGLE_TYPE::COTH, quadrant); }
	BcdAngle Sech() const { return BcdAngle(Sech(data), ANGLE_TYPE::SECH, quadrant); }
	BcdAngle Csch() const { return BcdAngle(Csch(data), ANGLE_TYPE::CSCH, quadrant); }

	BcdAngle Arsinh() const { return BcdAngle(Arsinh(data), ANGLE_TYPE::SINH, quadrant); }
	BcdAngle Arcosh() const { return BcdAngle(Arcosh(data), ANGLE_TYPE::COSH, quadrant); }
	BcdAngle Artanh() const { return BcdAngle(Artanh(data), ANGLE_TYPE::TANH, quadrant); }
	BcdAngle Arcoth() const { return BcdAngle(Arcoth(data), ANGLE_TYPE::COTH, quadrant); }
	BcdAngle Arsech() const { return BcdAngle(Arsech(data), ANGLE_TYPE::SECH, quadrant); }
	BcdAngle Arcsch() const { return BcdAngle(Arcsch(data), ANGLE_TYPE::CSCH, quadrant); }

	BcdAngle Inverse() const;
	BcdAngle Complementary() const { return BcdAngle(HalfPI() - data, ANGLE_UNIT::RADIAN); }
	BcdAngle Supplementary() const { return BcdAngle(BcdDecimal::PI() - data, ANGLE_UNIT::RADIAN); }
	BcdAngle Explementary() const { return BcdAngle(BcdDecimal::DoublePI() - data, ANGLE_UNIT::RADIAN); }
protected:
	BcdAngle(const BcdDecimal& value, ANGLE_TYPE type, ANGLE_QUADRANT quadrant) { ToValue(value, type, quadrant); }
	static BcdDecimal Sin(BcdDecimal value);
	static BcdDecimal Cos(BcdDecimal value);
	static BcdDecimal Tan(const BcdDecimal& value);
	static BcdDecimal Cot(const BcdDecimal& value);
	static BcdDecimal Sec(const BcdDecimal& value);
	static BcdDecimal Csc(const BcdDecimal& value);
	static BcdDecimal Arcsin(const BcdDecimal& value, const ANGLE_QUADRANT quadrant);
	static BcdDecimal Arccos(BcdDecimal value, const ANGLE_QUADRANT quadrant);
	static BcdDecimal Arcsec(const BcdDecimal& value, const ANGLE_QUADRANT quadrant);
	static BcdDecimal Arccsc(const BcdDecimal& value, const ANGLE_QUADRANT quadrant);
	static BcdDecimal Arctan(const BcdDecimal& value, const ANGLE_QUADRANT quadrant);
	static BcdDecimal Arccot(const BcdDecimal& value, const ANGLE_QUADRANT quadrant);

	static BcdDecimal Sinh(const BcdDecimal& value);
	static BcdDecimal Cosh(const BcdDecimal& value);
	static BcdDecimal Tanh(const BcdDecimal& value);
	static BcdDecimal Coth(const BcdDecimal& value);
	static BcdDecimal Sech(const BcdDecimal& value);
	static BcdDecimal Csch(const BcdDecimal& value);
	static BcdDecimal Arsinh(const BcdDecimal& value);
	static BcdDecimal Arcosh(const BcdDecimal& value);
	static BcdDecimal Arsech(const BcdDecimal& value);
	static BcdDecimal Arcsch(const BcdDecimal& value);
	static BcdDecimal Artanh(const BcdDecimal& value);
	static BcdDecimal Arcoth(const BcdDecimal& value);

	static BcdDecimal FourthPI();
	static BcdDecimal HalfPI();
	static BcdDecimal OneAndHalfPI();
	static BcdDecimal Tan45Degree();
private:
	ANGLE_TYPE type;
	ANGLE_QUADRANT quadrant;
	BcdDecimal data;
};

#endif // BcdAngle_H
