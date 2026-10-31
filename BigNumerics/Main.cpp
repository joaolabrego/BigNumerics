// BigNumerics.cpp : This file lcontains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "BcdAngle.h"
//#include "BigNumeric.h"

void BcdDecimalTest()
{

    BcdDecimal j = "-35";
    BcdDecimal logaritmo = j.Ln();
    BcdDecimal exponencial = j.Exp();
    string w = exponencial.ToString();

    BcdAngle rad = BcdAngle(j, ANGLE_UNIT::DEGREE);
    string pistr = rad.AsDegree().ToString();
    BcdDecimal inverso = j.Inverse();
    string inverso1 = inverso.ToString();
    BcdAngle sin = rad.Sin();
    BcdAngle cos = rad.Cos();
    BcdAngle sec = rad.Sec();
    BcdAngle csc = rad.Csc();
    BcdAngle tan = rad.Tan();
    BcdAngle cot = rad.Cot();


    w = sin.AsDecimal().ToString();
    w = cos.AsDecimal().ToString();
    w = sec.AsDecimal().ToString();
    w = csc.AsDecimal().ToString();
    w = tan.AsDecimal().ToString();
    w = cot.AsDecimal().ToString();

    w = sin.Inverse().AsDegree().ToString();
    w = cos.Inverse().AsDegree().ToString();
    w = sec.Inverse().AsDegree().ToString();
    w = csc.Inverse().AsDegree().ToString();
    w = tan.Inverse().AsDegree().ToString();
    w = cot.Inverse().AsDegree().ToString();

    BcdDecimal xy = BcdDecimal::E();
    BcdDecimal k = j.Ln();
    BcdDecimal l = j.Log(10ll);
    string m = k.ToString();
    string p = l.ToString();

    k = k.Exp();
    m = k.ToString();
    l = l.Antilog(10ll);
    p = l.ToString();

    BcdDecimal x = 2.12345678901234;
    BcdDecimal a = "15,121212";
    //string k = a.ToString();

    BcdAngle angle = BcdDecimal(30ll);
    //BcdAngle sin = angle.Tan();

    BcdDecimal b = "-6,6";
    BcdDecimal c = BcdDecimal("5").Gamma();
    SIGNED_DOUBLE_DIGIT n = 6;
    string s = c.ToString();
    string t;

    s = (c = a + b).ToString();
    s = (c = a - b).ToString();
    s = (c = a * b).ToString();
    s = (c = a * n).ToString();
    s = (c = a / b).ToString();
    s = (c = a / n).ToString();
    s = (c = a % b).ToString();
    s = (c = a % n).ToString();
    s = (c = a.Power(3)).ToString();
    s = (c = a.Root(2)).ToString();
    s = (c = a.Power(BcdDecimal(3.0))).ToString();
    s = (c = a.Root(BcdDecimal(2.0))).ToString();
    s = a.ToString();

    BcdDecimal pi = BcdDecimal::PI();
    BcdDecimal e = BcdDecimal::E();
    BcdDecimal log = pi.BcdDecimal::Log(10ll);
    BcdDecimal ln = pi.Ln();
    BcdDecimal ln1 = pi.Log(BcdDecimal::E());

    s = e.ToString();
    s = log.ToString();
    t = pi.ToString();
    s = log.Antilog().ToString();
    s = ln.ToString();
    t = ln1.ToString();
    s = ln.Exp().ToString();
    s = (c = a.Inverse()).ToString();
}
int main()
{
    BcdDecimalTest();

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
