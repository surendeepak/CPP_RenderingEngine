#include "ppc.h"
#include "m33.h"
#include "framebuffer.h"

#include <math.h>
#include <fstream>

//a. Constructor from resolution and horizontal field of view
PPC::PPC(float hfov,  int w, int h) : w(w), h(h) 
{
  C = V3(0.0f, 0.0f, 0.0f);
  a = V3(1.0f, 0.0f, 0.0f);
  b = V3(0.0f, -1.0f, 0.0f);
  float f = (float)w/2.0f/tan(hfov/180.0f*3.1415926f/2.0f);
  c = V3(-(float)w/2.0f, (float)h/2.0f, -f);
}

//b. Translation - Left
void PPC::TranslateLeft(float step) 
{
  C = C - a.UnitVector()*step;
}

//b. Translation - Right
void PPC::TranslateRight(float step) 
{
  C = C + a.UnitVector()*step;
}

//b. Translation - Up
void PPC::TranslateUp(float step) 
{
  C = C - b.UnitVector()*step;
}

//b. Translation - Down
void PPC::TranslateDown(float step) 
{
  C = C + b.UnitVector()*step;
}

//b. Translation - Forward-Backward
void PPC::TranslateFB(float step) 
{
  V3 vd = GetVD();
  C = C + vd*step;
}

//c. Pan about Center of Projection
void PPC::Pan(float rstep) 
{
  a = a.RotateDirectionAboutArbitraryAxis(b.UnitVector()*-1.0f, rstep);
  b = b.RotateDirectionAboutArbitraryAxis(b.UnitVector()*-1.0f, rstep);
  c = c.RotateDirectionAboutArbitraryAxis(b.UnitVector()*-1.0f, rstep);
  SetPMat();
}

//c. Tilt about Center of Projection
void PPC::Tilt(float rstep) 
{
  a = a.RotateDirectionAboutArbitraryAxis(a.UnitVector(), rstep);
  b = b.RotateDirectionAboutArbitraryAxis(a.UnitVector(), rstep);
  c = c.RotateDirectionAboutArbitraryAxis(a.UnitVector(), rstep);
  SetPMat();
}

//c. Roll about Center of Projection
void PPC::Roll(float rstep) 
{
  a = a.RotateDirectionAboutArbitraryAxis(GetVD(), rstep);
  b = b.RotateDirectionAboutArbitraryAxis(GetVD(), rstep);
  c = c.RotateDirectionAboutArbitraryAxis(GetVD(), rstep);
  SetPMat();
}

//d. Return Focal Length
float PPC::Getf() 
{
  return GetVD()*c;
}

//e. Projection of 3-D point
bool PPC::Project(V3 p, V3 &pp) 
{
  M33 m;
  m.SetColumn(0, a);
  m.SetColumn(1, b);
  m.SetColumn(2, c);
  V3 q = m.Inverted()*(p-C);

  if (q[2] < 0.0f)
    return false;

  pp[0] = q[0] / q[2];
  pp[1] = q[1] / q[2];
  pp[2] = 1.0f / q[2];

  return true;

}

//f. Interpolation between two given cameras
PPC PPC::Interpolate(PPC *ppc0, PPC *ppc1, float frac) 
{
  V3 newC = ppc0->C + (ppc1->C-ppc0->C)*frac;
  V3 anewb = ppc0->b + (ppc1->b-ppc0->b)*frac;
  V3 vinUP = anewb * -1.0f;
  V3 newVD = (ppc0->GetVD() + (ppc1->GetVD() - ppc0->GetVD())*frac).UnitVector();
  PPC ret;
  PositionAndOrient(newC, newVD, vinUP, ret);
  return ret;
}

//g. Save from text file
void PPC::Save(char *fname) 
{
  ofstream ofs(fname);
  ofs << a << endl;
  ofs << b << endl;
  ofs << c << endl;
  ofs << C << endl;
  ofs << "// a b c C" << endl;
  ofs.close();
}

////h. Save from text file
//void PPC::Load(char *fname) 
//{
//  ifstream ifs(fname);
//  ifs >> a ;
//  ifs >> b ;
//  ifs >> c ;
//  ifs >> C ;
//  ifs.close();
//  
//  SetPMat();
//}

//Returns Viewing Direction
V3 PPC::GetVD() 
{
  return (a^b).UnitVector();
}

//Set Projection Matrix
void PPC::SetPMat() 
{
  M33 cam;
  cam.SetColumn(0, a);
  cam.SetColumn(1, b);
  cam.SetColumn(2, c);
  pMat = cam.Inverted();
}

//Position and Orientation to a new position 
void PPC::PositionAndOrient(V3 newC, V3 newVD, V3 vinUP, PPC &ppc) 
{
  ppc.w = w;
  ppc.h = h;
  ppc.zFar = zFar;
  ppc.zNear = zNear;
  ppc.C = newC;
  ppc.a = (newVD ^ vinUP).UnitVector() * a.Length();
  V3 newUp = (ppc.a ^ newVD).UnitVector();
  ppc.b = newUp * -1.0f * b.Length();
  ppc.c = newVD*Getf() - ppc.b*(float)h/2.0f - ppc.a*(float)w/2.0f;

  ppc.SetPMat();
}
