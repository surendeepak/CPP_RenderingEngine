#include "v3.h"
#include "m33.h"
#include <math.h>

//a. Constructor from 3 Floats
V3::V3(float x, float y, float z) 
{
  xyz[0] = x;
  xyz[1] = y;
  xyz[2] = z;

}

//b. Read/write access through square brackets operator “[]”
float& V3::operator[](int i) 
{
  return xyz[i];
}

//c. Addition of two vectors
V3 V3::operator+(V3 v1) 
{
  V3 &v0 = (*this);
  V3 ret(v0[0]+v1[0], v0[1]+v1[1], v0[2]+v1[2]);
  return ret;
}

//d. Subtraction of two vectors
V3 V3::operator-(V3 v1) 
{
  V3 &v0 = (*this);
  V3 ret(v0[0]-v1[0], v0[1]-v1[1], v0[2]-v1[2]);
  return ret;
}

//e. Dot Product
float V3::operator*(V3 v1) 
{
  V3 &v0 = (*this);
  float ret = v0[0]*v1[0] + v0[1]*v1[1] + v0[2]*v1[2];
  return ret;
}

//f. Cross product
V3 V3::operator^(V3 op1) 
{
  V3 ret;
  ret.xyz[0] = xyz[1]*op1.xyz[2]-xyz[2]*op1.xyz[1];
  ret.xyz[1] = xyz[2]*op1.xyz[0]-xyz[0]*op1.xyz[2];
  ret.xyz[2] = xyz[0]*op1.xyz[1]-xyz[1]*op1.xyz[0];
  return ret;
}

//g. Multiplication by a Scalar
V3 V3::operator*(float scf) 
{
  V3 ret = *this;
  ret[0] *= scf;
  ret[1] *= scf;
  ret[2] *= scf;
  return ret;
}

//h. Division by a Scalar
V3 V3::operator/(float scf) 
{
  V3 ret = *this;
  ret[0] /= scf;
  ret[1] /= scf;
  ret[2] /= scf;
  return ret;
}

//i. Normalization - Returns Normalized Unit Vector
V3 V3::UnitVector() 
{
  V3 ret = (*this) / Length();
  return ret;
}

//j. Length Computation
float V3::Length() 
{
  return sqrtf(*this * (*this));
}

//k. Rotation of “this” point about arbitrary axis
// axis origin is aO
// axis direction is aD
// thetad degrees
V3 V3::RotatePointAboutArbitraryAxis(V3 aO, V3 aD, 
                                     float thetadegrees) {

  V3 xaxis(1.0f, 0.0f, 0.0f);
  V3 yaxis(0.0f, 1.0f, 0.0f);

  float adx = fabsf(xaxis*aD);
  float ady = fabsf(yaxis*aD);
  V3 aux;
  if (adx < ady) {
    aux = xaxis;
  }
  else {
    aux = yaxis;
  }

  M33 lm;
  lm[0] = (aux^aD).UnitVector();
  lm[1] = aD.UnitVector();
  lm[2] = (lm[0] ^ lm[1]).UnitVector();

  M33 ilm = lm.Inverted();

  M33 rotY; rotY.SetRotationY(thetadegrees);

  V3 pt(*this);
  V3 lpt = lm*(pt-aO);
  V3 rlpt = rotY*lpt;
  V3 ret = aO + ilm*rlpt;
  return ret;

}

//l. Rotation of “this” vector about arbitrary direction
// axis direction is aD
// thetad degrees
V3 V3::RotateDirectionAboutArbitraryAxis(V3 aD,  float thetadegrees) 
{
  V3 xaxis(1.0f, 0.0f, 0.0f);
  V3 yaxis(0.0f, 1.0f, 0.0f);

  float adx = fabsf(xaxis*aD);
  float ady = fabsf(yaxis*aD);
  V3 aux;
  if (adx < ady) 
  {
    aux = xaxis;
  }
  else 
  {
    aux = yaxis;
  }

  M33 lm;
  lm[0] = (aux^aD).UnitVector();
  lm[1] = aD.UnitVector();
  lm[2] = (lm[0] ^ lm[1]).UnitVector();

  M33 ilm = lm.Inverted();

  M33 rotY; rotY.SetRotationY(thetadegrees);

  V3 pt(*this);
  V3 lpt = lm*pt;
  V3 rlpt = rotY*lpt;
  V3 ret = ilm*rlpt;
  return ret;
}

//m. Output to ostream
ostream& operator<<(ostream& ostr, V3 v) 
{
  ostr << v[0] << " " << v[1] << " "  << v[2]; 
  return ostr;
}

//m. Input to istream
istream& operator>>(istream &istr, V3 &v) 
{
  istr >> v[0] >> v[1] >> v[2];
  return istr;
}

void V3::SetColor(unsigned int color) 
{
  xyz[0] = ((float) (((unsigned char*)&color)[0])) / 255.0f;
  xyz[1] = ((float) (((unsigned char*)&color)[1])) / 255.0f;
  xyz[2] = ((float) (((unsigned char*)&color)[2])) / 255.0f;
}

unsigned int V3::GetColor() 
{
  unsigned int ret = 0xFF000000;
  int red = (int) (xyz[0] * 256.0f);
  if (red == 256)
    red = 255;
  ret += red;

  int green = (int) (xyz[1] * 256.0f);
  if (green == 256)
    green = 255;
  ret += green << 8;

  int blue = (int) (xyz[2] * 256.0f);
  if (blue == 256)
    blue = 255;
  ret += blue << 16;

  return ret;
}

AABB::AABB(V3 pt0) 
{

  corners[0] = corners[1] = pt0;

}

void AABB::AddPoint(V3 pt) 
{
  if (pt[0] < corners[0][0])
    corners[0][0] = pt[0];
  if (pt[1] < corners[0][1])
    corners[0][1] = pt[1];
  if (pt[2] < corners[0][2])
    corners[0][2] = pt[2];

  if (pt[0] > corners[1][0])
    corners[1][0] = pt[0];
  if (pt[1] > corners[1][1])
    corners[1][1] = pt[1];
  if (pt[2] > corners[1][2])
    corners[1][2] = pt[2];
}

V3 V3::RotateThisPointAboutAxis(V3 O, V3 a, float theta) {

  V3 ret;

  // construct local coordinate system with a as second axis
  V3 aux;
  if (fabsf(a[0]) < fabsf(a[1])) {
    aux = V3(1.0f, 0.0f, 0.0f);
  }
  else {
    aux = V3(0.0f, 1.0f, 0.0f);
  }

  V3 ax = (aux ^ a).UnitVector();
  V3 az = (ax ^ a).UnitVector();

  M33 lcs;
  lcs[0] = ax;
  lcs[1] = a;
  lcs[2] = az;

  // transform to local coordinate system
  V3 &P = (*this);
  V3 lP = lcs*(P-O);

  // rotate in local coordinate system
  M33 rm;
  rm.SetRotationByY(theta);
  V3 rlP = rm*lP;

  // transform back to world
  ret = lcs.Inverted()*rlP+O;

  return ret;

}