#pragma once

#include "v3.h"

class M33 
{	
  V3 rows[3];
public:
  //a. Constructor from 3 3-D vectors
  M33();

  //b. Read/write access to rows through square brackets operator “[]”
  V3& operator[](int i);

  //c. Function to get column
  V3 GetColumn(int i);

  //d. Function to set column
  void SetColumn(int i, V3 col);

  //e. Multiplication with 3-D Vector
  V3 operator*(V3 v);

  //f. Multiplication with another matrix
  M33 operator*(M33 m);

  //g. Inversion
  M33 Inverted();

  //h. Transposition
  M33 Transpose();

  //i. Function to set matrix as rotation about principal axis by theta degrees
  void SetRotationY(float thetad);

  //j. Output to ostream, input from istream
  friend ostream& operator<<(ostream &os, M33 m);

  void SetRotationByY(float theta);
};