#pragma once

#include "v3.h"
#include "m33.h"

class PPC 
{
public:
  V3 a, b, c, C;
  int w, h;
  //Near and Far Planes
  float zNear, zFar; 
  PPC() {};

  //a. Constructor from resolution and horizontal field of view
  PPC(float hfov,  int w, int h);

  //b. Left-right, up-down, and forward-backward translations
  void TranslateLeft(float step);
  void TranslateRight(float step); 
  void TranslateUp(float step);
  void TranslateDown(float step);
  void TranslateFB(float step);

  //c. Pan, tilt, roll rotations about center of projection
  void Pan(float rstep);
  void Tilt(float rstep);
  void Roll(float rstep);

  //d. Change of focal length
  float Getf(); 

  //e. Projection of 3-D point
  bool Project(V3 p, V3 &pp);

  //f. Interpolation between two given cameras
  PPC Interpolate(PPC *ppc0, PPC *ppc1, float frac);

  //g. Save / load from text file
  void Save(char *fname); 
  void Load(char *fname); 

  //Returns Viewing Direction
  V3 GetVD();
  
  //Projection Matrix
  M33 pMat;

  //Set Projection Matrix
  void SetPMat();
    
  //Position and Orientation to a new position 
  void PositionAndOrient(V3 newC, V3 newVD, V3 vinUP, PPC &ppc);
};