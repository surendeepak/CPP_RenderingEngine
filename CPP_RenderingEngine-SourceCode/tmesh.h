#pragma once

#include "v3.h"
#include "ppc.h"
#include "framebuffer.h"

class PPC;
class FrameBuffer;

class TMesh 
{
public:
  V3 *verts;
  V3 *cols;
  V3 *normals;
  int vertsN;
  unsigned int *tris;
  int trisN;
  TMesh() : verts(0), vertsN(0), tris(0), trisN(0), cols(0), normals(0) {};

  //a. Load from bin file
  void LoadBin(char *fname);

  //b. Computation of 3-D axis aligned bounding box (AABB)
  AABB GetAABB();

  //c. Translation of vertices
  void Translate(V3 transv); 

  //d. Scaling of vertices
  void Scale(float scalef);

  //e. Placing the centroid at given position and scaling to given AABB size
  V3 GetCenter();
  void ScaleAboutCenter(float scalef); 

  //f. Rendering in Wireframe
  void RenderInWireFrame(PPC *ppc, FrameBuffer *fb);
  //New Function from Code 09.12
  void RenderWF(PPC *ppc, FrameBuffer *fb);
  //Rendering in Points
  void RenderPB(int psize, PPC *ppc, FrameBuffer *fb);
  //Rendering in Shaded Mode
  void Render(PPC *ppc, FrameBuffer *fb);

  void RotateAboutArbitraryAxis(V3 O, V3 a, float theta);
  void RotateAboutArbitraryAxis_2(V3 O, V3 a, float theta);
};