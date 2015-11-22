#include "M33.h"
#include <math.h>

//a. Constructor from 3 3-D vectors
M33::M33() 
{
  rows[0] = V3(1.0f, 0.0f, 0.0f);
  rows[1] = V3(0.0f, 1.0f, 0.0f);
  rows[2] = V3(0.0f, 0.0f, 1.0f);
}

//b. Read/write access to rows through square brackets operator “[]”
V3& M33::operator[](int i) 
{
  return rows[i];
}

//c. Function to get column
V3 M33::GetColumn(int i) 
{
  V3 ret(rows[0][i], rows[1][i], rows[2][i]);
  return ret;
}

//d. Function to set column
void M33::SetColumn(int i, V3 col) 
{
  rows[0][i] = col[0];
  rows[1][i] = col[1];
  rows[2][i] = col[2];
}

//e. Multiplication with 3-D Vector
V3 M33::operator*(V3 v) 
{
  return V3(rows[0]*v, rows[1]*v, rows[2]*v);
}

//f. Multiplication with another matrix
M33 M33::operator*(M33 m1) 
{
  M33 &m0 = *this;
  M33 ret;
  ret.SetColumn(0, m0*m1.GetColumn(0));
  ret.SetColumn(1, m0*m1.GetColumn(1));
  ret.SetColumn(2, m0*m1.GetColumn(2));
  return ret;
}

//g. Inversion
M33 M33::Inverted() 
{
  M33 ret;
  V3 a = GetColumn(0), b = GetColumn(1), c = GetColumn(2);
  V3 _a = b ^ c; _a = _a / (a * _a);
  V3 _b = c ^ a; _b = _b / (b * _b);
  V3 _c = a ^ b; _c = _c / (c * _c);
  ret[0] = _a;
  ret[1] = _b;
  ret[2] = _c;

  return ret;
}

//h. Transposition
M33 M33::Transpose() 
{
  M33 tmp(*this);
  rows[0] = tmp.GetColumn(0);
  rows[1] = tmp.GetColumn(1);
  rows[2] = tmp.GetColumn(2);
  return tmp;
}

//i. Function to set matrix as rotation about principal axis by theta degrees
void M33::SetRotationY(float thetadegrees) 
{
  float thetarotation = thetadegrees*3.1415f/180.0f;
  (*this)[0] = V3(cosf(thetarotation), 0.0f, sinf(thetarotation));
  (*this)[1] = V3(0.0f, 1.0f, 0.0f);
  (*this)[2] = V3(-sinf(thetarotation), 0.0f, cosf(thetarotation));
}

//j. Output to ostream, input from istream
ostream& operator<<(ostream &os, M33 m) 
{
  return os << m[0] << endl << m[1] << endl << m[2] << endl;
}

void M33::SetRotationByY(float theta) 
{
  float tr = theta / 180.0f * 3.1415926f;
  float ct = cosf(tr);
  float st = sinf(tr);
  rows[0] = V3(ct, 0.0f, st);
  rows[1] = V3(0.0f, 1.0f, 0.0f);
  rows[2] = V3(-st, 0.0, ct);
}

