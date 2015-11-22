#include "tmesh.h"
#include "framebuffer.h"
#include "ppc.h"

#include <fstream>
#include <iostream>


//a. Load from bin file
void TMesh::LoadBin(char *fname) {

  ifstream ifs(fname, ios::binary);
  if (ifs.fail()) {
	cerr << "INFO: cannot open file: " << fname << endl;
	return;
  }

  ifs.read((char*)&vertsN, sizeof(int));
  char yn;
  ifs.read(&yn, 1); // always xyz
  if (yn != 'y') {
	cerr << "INTERNAL ERROR: there should always be vertex xyz data" << endl;
	return;
  }
  if (verts)
	delete verts;
  verts = new V3[vertsN];

  ifs.read(&yn, 1); // cols 3 floats
  if (cols)
	delete cols;
  cols = 0;
  if (yn == 'y') {
	cols = new V3[vertsN];
  }

  ifs.read(&yn, 1); // normals 3 floats
  if (normals)
	delete normals;
  normals = 0;
  if (yn == 'y') {
	normals = new V3[vertsN];
  }

  ifs.read(&yn, 1); // texture coordinates 2 floats
  float *tcs = 0; // don't have texture coordinates for now
  if (tcs)
	delete tcs;
  tcs = 0;
  if (yn == 'y') {
	tcs = new float[vertsN*2];
  }

  ifs.read((char*)verts, vertsN*3*sizeof(float)); // load verts

  if (cols) {
	ifs.read((char*)cols, vertsN*3*sizeof(float)); // load cols
  }

  if (normals)
	ifs.read((char*)normals, vertsN*3*sizeof(float)); // load normals

  if (tcs)
	ifs.read((char*)tcs, vertsN*2*sizeof(float)); // load texture coordinates

  ifs.read((char*)&trisN, sizeof(int));
  if (tris)
	delete tris;
  tris = new unsigned int[trisN*3];
  ifs.read((char*)tris, trisN*3*sizeof(unsigned int)); // read tiangles

  ifs.close();

  cerr << "INFO: loaded " << vertsN << " verts, " << trisN << " tris from " << endl << "      " << fname << endl;
  cerr << "      xyz " << ((cols) ? "rgb " : "") << ((normals) ? "nxnynz " : "") << ((tcs) ? "tcstct " : "") << endl;

}

//b. Computation of 3-D axis aligned bounding box (AABB)
AABB TMesh::GetAABB() 
{
  AABB ret(verts[0]);
  for (int vi = 0; vi < vertsN; vi++) 
  {
	ret.AddPoint(verts[vi]);
  }
  return ret;
}

//c. Translation of vertices
void TMesh::Translate(V3 transv) 
{
  for (int vi = 0; vi < vertsN; vi++) 
  {
	verts[vi] = verts[vi] + transv;
  }
}

//d. Scaling of vertices
void TMesh::Scale(float scalef) 
{
  for (int vi = 0; vi < vertsN; vi++) 
  {
	verts[vi] = verts[vi] * scalef;
  }
}

//e. Placing the centroid at given position and scaling to given AABB size
V3 TMesh::GetCenter() 
{
  V3 ret(0.0f, 0.0f, 0.0f);
  for (int vi = 0; vi < vertsN; vi++) {
	ret = ret + verts[vi];
  }
  return ret/(float)vertsN;
}
void TMesh::ScaleAboutCenter(float scalef) 
{
  V3 center = GetCenter();
  Translate(center*-1.0f);

  Scale(scalef);

  Translate(center);
}

//f. Rendering in Wire Frame Mode
void TMesh::RenderInWireFrame(PPC *ppc, FrameBuffer *fb) 
{
  V3 *projectVertices = new V3[vertsN];
  for (int vi = 0; vi < vertsN; vi++) 
  {
	projectVertices[vi] = V3(FLT_MAX, FLT_MAX, FLT_MAX);
	ppc->Project(verts[vi], projectVertices[vi]);
  }

  for (int tri = 0; tri < trisN; tri++) 
  {
	unsigned int vinds[3];
	vinds[0] = tris[3*tri+0];
	vinds[1] = tris[3*tri+1];
	vinds[2] = tris[3*tri+2];
	for (int ei = 0; ei < 3; ei++) 
	{
	  fb->Draw2DSegment(projectVertices[vinds[ei]],
		projectVertices[vinds[(ei+1)%3]],
		cols[vinds[ei]], cols[vinds[(ei+1)%3]]);
	}
  }

  delete []projectVertices;
}

//New Function from Code 09.12
void TMesh::RenderWF(PPC *ppc, FrameBuffer *fb) 
{
  for (int tri = 0; tri < trisN; tri++) {
	for (int vi = 0; vi < 3; vi++) {
	  int _vi = (vi+1)%3;
	  unsigned int v0i = tris[3*tri+vi];
	  unsigned int v1i = tris[3*tri+_vi];
	  fb->Draw3DSegment(verts[v0i], cols[v0i], 
		verts[v1i], cols[v1i], ppc);
	}
  }

}

//Rendering in Points
void TMesh::RenderPB(int psize, PPC *ppc, FrameBuffer *fb) 
{
  for (int vi = 0; vi < vertsN; vi++) 
  {
	fb->Draw3DBigPoint(verts[vi], ppc, psize, cols[vi]);
  }
}

//Rendering in Shaded Mode
void TMesh::Render(PPC *ppc, FrameBuffer *fb) 
{
  // PROJECTION
  // For all vertices v
  // Project v
  // Shade v
  // END PROJECTION

  // For each triangle T

  // CLIPPING
  // Compute 2D AABB aabb of T' (projection of T)
  // Clip aabb to frame
  // END CLIPPING

  // RASTERIZATION SETUP
  // Compute screen space linear expression for z
  // Compute denominator linear expression for model space interpolation
  // Compute numerator linear expression for each rasterization parameter (i.e. red, green, blue, nx, ny, nz, ...)
  // Compute edge equation linear expressions for rasterization
  // END RASTERIZATION SETUP

  // RASTERIZATION
  // For every pixel p in aabb

  // CLIPPING
  // if p not inside triangle continue

  // ZBuffering
  // compute z at p
  // if z farther than what's already been seen at p continue
  // write z in zbuffer

  // SHADING
  // compute color c at p
  //      c = (Ac*u+Bc*v+Cc) / (D*u+E*v+F) (Gouraud shading)
  // for Phong shading evaluate lighting equation with pixel normal

  // write c in framebuffer
  // END RASTERIZATION
}

void TMesh::RotateAboutArbitraryAxis(V3 O, V3 a, float theta) 
{
  for (int vi = 0; vi < vertsN; vi++) 
  {
	verts[vi] = verts[vi].RotateThisPointAboutAxis(O, a, theta);
	//verts[vi] = verts[vi].RotateDirectionAboutArbitraryAxis(a, theta);
  }
}

void TMesh::RotateAboutArbitraryAxis_2(V3 O, V3 a, float theta) 
{
  for (int vi = 0; vi < vertsN; vi++) 
  {
	//verts[vi] = verts[vi].RotateThisPointAboutAxis(O, a, theta);
	verts[vi] = verts[vi].RotateDirectionAboutArbitraryAxis(a, theta);
  }
}