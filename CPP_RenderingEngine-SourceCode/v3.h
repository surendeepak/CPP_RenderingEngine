#pragma once

#include <iostream>
#include <ostream>

using namespace std;


class V3 
{
  float xyz[3];
public:
  V3() {};

  //a. Constructor from 3 Floats
  V3(float x, float y, float z);

  //b. Read/write access through square brackets operator “[]”
  float& operator[](int i);

  //c. Addition of two vectors
  V3 operator+(V3 v1);

  //d. Subtraction of two vectors
  V3 operator-(V3 v1);

  //e. Dot Product
  float operator*(V3 v1);

  //f. Cross Product
  V3 operator^(V3 op1);

  //g. Multiplication by a Scalar
  V3 operator*(float scf);

  //h. Division by a Scalar
  V3 operator/(float scf);

  //i. Normalization - Returns Normalized Unit Vector
  V3 UnitVector();

  //j. Length Computation
  float Length();

  //k. Rotation of “this” point about arbitrary axis
  V3 RotatePointAboutArbitraryAxis(V3 aO, V3 aD, float thetadegrees);

  //l. Rotation of “this” vector about arbitrary direction
  V3 RotateDirectionAboutArbitraryAxis(V3 aD,  float thetadegrees);

  //m. Output to ostream
  friend ostream& operator<<(ostream& ostr, V3 v);

  V3 RotateThisPointAboutAxis(V3 O, V3 a, float theta);

  void SetColor(unsigned int color);
  unsigned int GetColor();
};

//Axis Aligned Bounding Box
class AABB 
{
public:
  V3 corners[2];
  AABB(V3 pt0);
  void AddPoint(V3 pt);
};