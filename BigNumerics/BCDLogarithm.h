/******************************************************************************/
/*** File-Name: BCDLogarithm.h	                                            ***/
/*** Author...: João da Rocha Labrego                                       ***/
/*** E-Mail...: joaolabrego@gmail.com                                       ***/
/*** Purpose..: BCDLogarithm header.                                          ***/
/******************************************************************************/

#ifndef BCDLogarithm_H
#define BCDLogarithm_H

#include "BCDDecimal.h"

struct BCDLogarithm
{
public:
	BCDLogarithm() {}
	BCDLogarithm(const BCDDecimal& value) { data = Ln(value); }
	~BCDLogarithm() {}

	BCDDecimal Log10() const { return Log(data, 10ll); }
	BCDLogarithm operator +(const BCDLogarithm& addend) const;
	BCDLogarithm operator -(const BCDLogarithm& subtrahend) const;
	BCDLogarithm operator *(const BCDLogarithm& multiplier) const;
	BCDLogarithm operator /(const BCDLogarithm& divisor) const;
	BCDLogarithm Power(const BCDDecimal& exponent) const;
	BCDLogarithm Root(const BCDDecimal& degree) const;
	static BCDDecimal Gamma(BCDDecimal value);
protected:
	static BCDDecimal Ln(BCDDecimal value);
	static BCDDecimal Exp(const BCDDecimal& logarithm);
	static BCDDecimal Log(const BCDDecimal& value, const BCDDecimal& base);
	static BCDDecimal Antilog(const BCDDecimal& logarithm, const BCDDecimal& base = 10ll);
	static BCDDecimal Baselog(const BCDDecimal& logarithming, const BCDDecimal& logarithm);
	static vector <BCDDecimal> GammaCoefficients();
private:
	BCDDecimal data;
};

#endif // BCDLogarithm_H

