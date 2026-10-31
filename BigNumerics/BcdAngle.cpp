/******************************************************************************/
/*** File-Name: BcdAngle.cpp                                                ***/
/*** Author...: João da Rocha Labrego                                       ***/
/*** E-Mail...: joaolabrego@gmail.com                                       ***/
/*** Purpose..: General implementation.                                     ***/
/******************************************************************************/

#include "BcdAngle.h"

/************************************************************/
BcdAngle& BcdAngle::ToValue(BcdDecimal value, ANGLE_UNIT unit)
/************************************************************/
{
    General::UpPrecision();
    if (value.IsNull())
        return ToNull();
    if (value.IsZero())
    {
        quadrant = ANGLE_QUADRANT::FIRST;
        value.ToZero();
    }
    else if (unit == ANGLE_UNIT::DEGREE)
    {
        value %= 360ll;
        if (value.IsZero() || value.IsNegative())
            value += 360ll;
        if (value < 90)
            quadrant = ANGLE_QUADRANT::FIRST;
        else if (value < 180)
            quadrant = ANGLE_QUADRANT::SECOND;
        else if (value < 270)
            quadrant = ANGLE_QUADRANT::THIRD;
        else
            quadrant = ANGLE_QUADRANT::FOURTH;
        value *= (BcdDecimal::PI() / 180ll);
    }
    else if (unit == ANGLE_UNIT::GRADIAN)
    {
        value %= 400ll;
        if (value.IsZero() || value.IsNegative())
            value += 400ll;
        if (value < 100)
            quadrant = ANGLE_QUADRANT::FIRST;
        else if (value < 200)
            quadrant = ANGLE_QUADRANT::SECOND;
        else if (value < 300)
            quadrant = ANGLE_QUADRANT::THIRD;
        else
            quadrant = ANGLE_QUADRANT::FOURTH;
        value *= (BcdDecimal::PI() / 200ll);
    }
    else
    {
        value %= BcdDecimal::DoublePI();
        if (value.IsZero() || value.IsNegative())
            value *= BcdDecimal::DoublePI();
        if (value < HalfPI())
            quadrant = ANGLE_QUADRANT::FIRST;
        else if (value < BcdDecimal::PI())
            quadrant = ANGLE_QUADRANT::SECOND;
        else if (value < OneAndHalfPI())
            quadrant = ANGLE_QUADRANT::THIRD;
        else
            quadrant = ANGLE_QUADRANT::FOURTH;
    }
    type = ANGLE_TYPE::ANGLE;
    data = value;
    General::DownPrecision();
    
    return *this;
}
/********************************************************************************************/
BcdAngle& BcdAngle::ToValue(const BcdDecimal& value, ANGLE_TYPE type, ANGLE_QUADRANT quadrant)
/********************************************************************************************/
{
    data = value;
    this->type = type;
    this->quadrant = quadrant;
    if (type == ANGLE_TYPE::SIN || type == ANGLE_TYPE::CSC)
    {
        if (quadrant == ANGLE_QUADRANT::FIRST || quadrant == ANGLE_QUADRANT::SECOND)
            data.ToSign(SIGN::POSITIVE);
        else
            data.ToSign(SIGN::NEGATIVE);
    }
    else if (type == ANGLE_TYPE::COS || type == ANGLE_TYPE::SEC)
    {
        if (quadrant == ANGLE_QUADRANT::FIRST || quadrant == ANGLE_QUADRANT::FOURTH)
            data.ToSign(SIGN::POSITIVE);
        else
            data.ToSign(SIGN::NEGATIVE);
    }
    else if (type == ANGLE_TYPE::TAN || type == ANGLE_TYPE::COT)
    {
        if (quadrant == ANGLE_QUADRANT::FIRST || quadrant == ANGLE_QUADRANT::THIRD)
            data.ToSign(SIGN::POSITIVE);
        else
            data.ToSign(SIGN::NEGATIVE);
    }

    return *this;
}
/****************************************/
BcdDecimal BcdAngle::Sin(BcdDecimal value)
/****************************************/
{
    BcdDecimal result,
        lastResult,
        parcel,
        potency,
        factorial;
    SIGNED_DOUBLE_DIGIT index;
    bool positive;

    if (value.IsNull())
        return result;
    if (value.IsZero() || value == BcdDecimal::PI() || value == BcdDecimal::DoublePI())
        return result.ToZero();
    if (value == HalfPI())
        return result.ToOne();
    if (value == OneAndHalfPI())
        return result.ToMinusOne();
    General::UpPrecision();
    value %= HalfPI();
    potency.ToOne();
    factorial.ToOne();
    result.ToZero();
    index = 1;
    positive = true;
    do
    {
        lastResult = result;
        potency *= value;
        factorial *= SIGNED_DOUBLE_DIGIT(index++);
        parcel = potency / factorial;
        if (positive)
            result += parcel;
        else
            result -= parcel;
        potency *= value;
        factorial *= index++;
        positive = !positive;
    } while (result != lastResult);
    General::DownPrecision();

    return result;
}
/****************************************/
BcdDecimal BcdAngle::Cos(BcdDecimal value)
/****************************************/
{
    BcdDecimal result,
        lastResult,
        parcel,
        potency,
        factorial,
        valuePower2;
    SIGNED_DOUBLE_DIGIT index;
    bool positive;

    if (value.IsNull())
        return result;
    if (value.IsZero() || value == BcdDecimal::DoublePI())
        return result.ToOne();
    if (value == BcdDecimal::PI())
        return result.ToMinusOne();
    if (value == HalfPI() || value == OneAndHalfPI())
        return result.ToZero();
    General::UpPrecision();
    value %= HalfPI();
    potency.ToOne();
    factorial.ToOne();
    index = 1;
    positive = true;
    valuePower2 = value.Power(2);
    result.ToZero();
    do
    {
        lastResult = result;
        parcel = potency / factorial;
        if (positive)
            result += parcel;
        else
            result -= parcel;
        potency *= valuePower2;
        factorial *= index++;
        factorial *= index++;
        positive = !positive;
    } while (result != lastResult);
    General::DownPrecision();

    return result;
}
/***********************************************/
BcdDecimal BcdAngle::Tan(const BcdDecimal& value)
/***********************************************/
{
    BcdDecimal result;

    if (value.IsNull() || value == HalfPI() || value == OneAndHalfPI())
        return result;
    if (value.IsZero() || value == BcdDecimal::PI() || value == BcdDecimal::DoublePI())
        return result.ToZero();

    General::UpPrecision();
    result = Sin(value) / Cos(value);
    General::DownPrecision();
 
    return result;
}
/***********************************************/
BcdDecimal BcdAngle::Cot(const BcdDecimal& value)
/***********************************************/
{
    BcdDecimal result;

    if (value.IsNull() || value.IsZero() || value == BcdDecimal::PI() || value == BcdDecimal::DoublePI())
        return result;
    if (value == HalfPI() || value == OneAndHalfPI())
        return result.ToZero();
    General::UpPrecision();
    result = Cos(value) / Sin(value);
    General::DownPrecision();

    return result;
}
/***********************************************/
BcdDecimal BcdAngle::Sec(const BcdDecimal& value)
/***********************************************/
{
    BcdDecimal result;

    if (value.IsNull() || value == HalfPI() || value == OneAndHalfPI())
        return result;
    General::UpPrecision();
    result = Cos(value).Inverse();
    General::DownPrecision();

    return result;
}
/***********************************************/
BcdDecimal BcdAngle::Csc(const BcdDecimal& value)
/***********************************************/
{
    BcdDecimal result;

    if (value.IsNull() || value.IsZero() || value == BcdDecimal::PI() || value == BcdDecimal::DoublePI())
        return result;
    General::UpPrecision();
    result = Sin(value).Inverse();
    General::DownPrecision();

    return result;
}
/********************************/
BcdAngle BcdAngle::Inverse() const
/********************************/
{
    BcdAngle result;

    if (type == ANGLE_TYPE::SIN)
        result.ToValue(Arcsin(data, quadrant), ANGLE_TYPE::ANGLE, quadrant);
    else if (type == ANGLE_TYPE::COS)
        result.ToValue(Arccos(data, quadrant), ANGLE_TYPE::ANGLE, quadrant);
    else if (type == ANGLE_TYPE::SEC)
        result.ToValue(Arcsec(data, quadrant), ANGLE_TYPE::ANGLE, quadrant);
    else if (type == ANGLE_TYPE::CSC)
        result.ToValue(Arccsc(data, quadrant), ANGLE_TYPE::ANGLE, quadrant);
    else if (type == ANGLE_TYPE::TAN)
        result.ToValue(Arctan(data, quadrant), ANGLE_TYPE::ANGLE, quadrant);
    else if (type == ANGLE_TYPE::COT)
        result.ToValue(Arccot(data, quadrant), ANGLE_TYPE::ANGLE, quadrant);
    else if (type == ANGLE_TYPE::SINH)
        result.ToValue(Arsinh(data), ANGLE_TYPE::ANGLE, quadrant);
    else if (type == ANGLE_TYPE::COSH)
        result.ToValue(Arcosh(data), ANGLE_TYPE::ANGLE, quadrant);
    else if (type == ANGLE_TYPE::SECH)
        result.ToValue(Arsech(data), ANGLE_TYPE::ANGLE, quadrant);
    else if (type == ANGLE_TYPE::CSCH)
        result.ToValue(Arcsch(data), ANGLE_TYPE::ANGLE, quadrant);
    else if (type == ANGLE_TYPE::TANH)
        result.ToValue(Artanh(data), ANGLE_TYPE::ANGLE, quadrant);
    else if (type == ANGLE_TYPE::COTH)
        result.ToValue(Arcoth(data), ANGLE_TYPE::ANGLE, quadrant);

    return result;
}
/*********************************************************************************/
BcdDecimal BcdAngle::Arcsin(const BcdDecimal& value, const ANGLE_QUADRANT quadrant)
/*********************************************************************************/
{
    BcdDecimal result;

    if (value.IsNullOrZero())
    {
        if (quadrant == ANGLE_QUADRANT::FIRST)
            return result.ToZero();
        if (quadrant == ANGLE_QUADRANT::SECOND || quadrant == ANGLE_QUADRANT::THIRD)
            BcdDecimal::PI();
        return BcdDecimal::DoublePI();
    }
    else if (value.IsMinusOne())
        return OneAndHalfPI();
    else if (value.IsOne())
        return HalfPI();
    General::UpPrecision();
    result = Arctan(value / ((BcdDecimal(1ll) + value) * (BcdDecimal(1ll) - value)).Root(2), quadrant);
    General::DownPrecision();

    return result;
}
/**************************************************************************/
BcdDecimal BcdAngle::Arccos(BcdDecimal value, const ANGLE_QUADRANT quadrant)
/**************************************************************************/
{
    BcdDecimal result;

    if (value.IsNullOrZero())
    {
        if (quadrant == ANGLE_QUADRANT::FIRST || quadrant == ANGLE_QUADRANT::SECOND)
            return HalfPI();
        return OneAndHalfPI();
    }
    else if (value.IsMinusOne())
        return BcdDecimal::PI();
    else if (value.IsOne())
    {
        if (quadrant == ANGLE_QUADRANT::FIRST)
            return result.ToZero();
        return BcdDecimal::DoublePI();
    }
    General::UpPrecision();
    result = Arctan(((BcdDecimal(1ll) + value) * (BcdDecimal(1ll) - value)).Root(2) / value, quadrant);
    General::DownPrecision();

    return result;
}
/*********************************************************************************/
BcdDecimal BcdAngle::Arcsec(const BcdDecimal& value, const ANGLE_QUADRANT quadrant)
/*********************************************************************************/
{
    BcdDecimal result;

    General::UpPrecision();
    result = Arccos(value.Inverse(), quadrant);
    General::DownPrecision();

    return result;
}
/*********************************************************************************/
BcdDecimal BcdAngle::Arccsc(const BcdDecimal& value, const ANGLE_QUADRANT quadrant)
/*********************************************************************************/
{
    BcdDecimal result;

    General::UpPrecision();
    result = Arcsin(value.Inverse(), quadrant);
    General::DownPrecision();

    return result;
}
/*********************************************************************************/
BcdDecimal BcdAngle::Arctan(const BcdDecimal& value, const ANGLE_QUADRANT quadrant)
/*********************************************************************************/
{
    bool positive;
    BcdDecimal result,
        parcel,
        potency,
        lastResult,
        valuePower2;
    SIGNED_DOUBLE_DIGIT index;

    if (value.IsNull())
    {
        if (quadrant == ANGLE_QUADRANT::FIRST || quadrant == ANGLE_QUADRANT::SECOND)
            return HalfPI();
        return OneAndHalfPI();
    }
    if (value.IsZero())
    {
        if (quadrant == ANGLE_QUADRANT::FIRST)
            return result.ToZero();
        if (quadrant == ANGLE_QUADRANT::SECOND || quadrant == ANGLE_QUADRANT::THIRD)
            return BcdDecimal::PI();
        return BcdDecimal::DoublePI();
    }
    if (value.IsMinusOne() || value == -Tan45Degree())
    {
        if (quadrant == ANGLE_QUADRANT::SECOND)
            return HalfPI() + FourthPI();
        return OneAndHalfPI() + FourthPI();
    }
    if (value.IsOne() || value == Tan45Degree())
    {
        if (quadrant == ANGLE_QUADRANT::FIRST)
            return FourthPI();
        return BcdDecimal::PI() + FourthPI();
    }
    General::UpPrecision();
    valuePower2 = value.Power(2);
    index = 1;
    if (value > -1 && value < 1)
    {
        potency = result = value;
        positive = true;
        do
        {
            lastResult = result;
            index += 2;
            potency *= valuePower2;
            parcel = potency / index;
            if (positive = !positive)
                result += parcel;
            else
                result -= parcel;
        } while (result != lastResult);
    }
    else
    {
        potency = value;
        if (value.Sign() == SIGN::NEGATIVE)
            result = -HalfPI() - potency.Inverse();
        else
            result = HalfPI() - potency.Inverse();
        positive = false;
        do
        {
            lastResult = result;
            index += 2;
            potency *= valuePower2;
            parcel = (potency * index).Inverse();
            if (positive = !positive)
                result += parcel;
            else
                result -= parcel;
        } while (result != lastResult);
    }
    result = +result;
    if (quadrant == ANGLE_QUADRANT::SECOND)
        result += HalfPI();
    else if (quadrant == ANGLE_QUADRANT::THIRD)
        result += BcdDecimal::PI();
    else if (quadrant == ANGLE_QUADRANT::FOURTH)
        result += OneAndHalfPI();
    General::DownPrecision();

    return result;
}
/*********************************************************************************/
BcdDecimal BcdAngle::Arccot(const BcdDecimal& value, const ANGLE_QUADRANT quadrant)
/*********************************************************************************/
{
    BcdDecimal result;

    General::UpPrecision();
    result = Arctan(value.Inverse(), quadrant);
    General::DownPrecision();

    return result;
}
/************************************************/
BcdDecimal BcdAngle::Sinh(const BcdDecimal& value)
/************************************************/
{
    BcdDecimal result;

    General::UpPrecision();
    result = value.Exp();
    result = (result - result.Inverse()) / 2;
    General::DownPrecision();

    return result;
}
/************************************************/
BcdDecimal BcdAngle::Cosh(const BcdDecimal& value)
/************************************************/
{
    BcdDecimal result;

    General::UpPrecision();
    result = value.Exp();
    result = (result + result.Inverse()) / 2;
    General::DownPrecision();

    return result;
}
/************************************************/
BcdDecimal BcdAngle::Sech(const BcdDecimal& value)
/************************************************/
{
    BcdDecimal result;

    General::UpPrecision();
    result = value.Exp();
    result = BcdDecimal(2ll) / (result + result.Inverse());
    General::DownPrecision();

    return result;
}
/************************************************/
BcdDecimal BcdAngle::Csch(const BcdDecimal& value)
/************************************************/
{
    BcdDecimal result;

    General::UpPrecision();
    result = value.Exp();
    result = BcdDecimal(2ll) / (result - result.Inverse());
    General::DownPrecision();

    return result;
}
/************************************************/
BcdDecimal BcdAngle::Tanh(const BcdDecimal& value)
/************************************************/
{
    BcdDecimal result,
        inverseResult;

    General::UpPrecision();
    result = value.Exp();
    inverseResult = result.Inverse();
    result = (result - inverseResult) / (result + inverseResult);
    General::DownPrecision();

    return result;
}
/************************************************/
BcdDecimal BcdAngle::Coth(const BcdDecimal& value)
/************************************************/
{
    BcdDecimal result,
        inverseResult;

    General::UpPrecision();
    result = value.Exp();
    inverseResult = result.Inverse();
    result = (result + inverseResult) / (result - inverseResult);
    General::DownPrecision();

    return result;
}
/**************************************************/
BcdDecimal BcdAngle::Arsinh(const BcdDecimal& value)
/**************************************************/
{
    BcdDecimal result;

    General::UpPrecision();
    result = (value + (value.Power(2) + 1ll).Root(2)).Ln();
    General::DownPrecision();

    return result;
}
/**************************************************/
BcdDecimal BcdAngle::Arcosh(const BcdDecimal& value)
/**************************************************/
{
    BcdDecimal result;

    General::UpPrecision();
    result = (value + (value.Power(2) - 1ll).Root(2)).Ln();
    General::DownPrecision();

    return result;
}
/**************************************************/
BcdDecimal BcdAngle::Arsech(const BcdDecimal& value)
/**************************************************/
{
    BcdDecimal result;

    General::UpPrecision();
    result = ((BcdDecimal(1ll) + (BcdDecimal(1ll) - value.Power(2)).Root(2)) / value).Ln();
    General::DownPrecision();

    return result;
}
/**************************************************/
BcdDecimal BcdAngle::Arcsch(const BcdDecimal& value)
/**************************************************/
{
    BcdDecimal result;

    General::UpPrecision();
    result = ((BcdDecimal(1ll) + (BcdDecimal(1ll) + value.Power(2)).Root(2)) / value).Ln();
    General::DownPrecision();

    return result;
}
/**************************************************/
BcdDecimal BcdAngle::Artanh(const BcdDecimal& value)
/**************************************************/
{
    BcdDecimal result;

    General::UpPrecision();
    result = ((BcdDecimal(1ll) + value) / (BcdDecimal(1ll) - value)).Ln() / 2;
    General::DownPrecision();

    return result;
}
/**************************************************/
BcdDecimal BcdAngle::Arcoth(const BcdDecimal& value)
/**************************************************/
{
    BcdDecimal result;

    General::UpPrecision();
    result = ((BcdDecimal(1ll) - value) / (BcdDecimal(1ll) + value)).Ln() / 2;
    General::DownPrecision();

    return result;
}
/*****************************/
BcdDecimal BcdAngle::FourthPI()
/*****************************/
{
    static BcdDecimal value;

    if (value.IsNull())
    {
        General::UpPrecision();
        value = BcdDecimal::PI() / 4;
        General::DownPrecision();
    }

    return value;
}
/***************************/
BcdDecimal BcdAngle::HalfPI()
/***************************/
{
    static BcdDecimal value;

    if (value.IsNull())
    {
        General::UpPrecision();
        value = BcdDecimal::PI() / 2;
        General::DownPrecision();
    }

    return value;
}
/*********************************/
BcdDecimal BcdAngle::OneAndHalfPI()
/*********************************/
{
    static BcdDecimal value;

    if (value.IsNull())
    {
        General::UpPrecision();
        value = BcdDecimal::PI() + HalfPI();
        General::DownPrecision();
    }

    return value;
}
/********************************/
BcdDecimal BcdAngle::Tan45Degree()
/********************************/
{
    static BcdDecimal value;

    if (value.IsNull())
    {
        General::UpPrecision();
        value = Tan(FourthPI());
        General::DownPrecision();
    }

    return value;
}
/***********/
/*** EOF ***/
/***********/