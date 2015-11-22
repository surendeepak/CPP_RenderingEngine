#include "scene.h"
#include <float.h>

#include <iostream>
#include <fstream>

using namespace std;

Scene *scene;

Scene::Scene() {

  // create user interface
  gui = new GUI();
  gui->show();

  // create SW framebuffer
  int u0 = 20;
  int v0 = 50;
  int sci = 2;
  //int w = sci*240;
  //int h = sci*180;
  int w = sci*1280;
  int h = sci*720;
  fb = new FrameBuffer(u0, v0, w, h);
  fb->label("SW Framebuffer");
  fb->show();
  
  // position UI window
  gui->uiw->position(fb->w+u0 + 2*20, v0);

  // create camera
  float hfov = 50.0f;
  ppc = new PPC(hfov, fb->w, fb->h);
  //ppc->C = ppc->C + V3(-10.0f, +20.0f, +1540.0f);

  tmeshesN = 5;

  tmeshes = new TMesh[tmeshesN];

  //Loading the 1st geometry
  tmeshes[0].LoadBin("geometry/teapot1K.bin");
  V3 center(0.0f, 0.0f, -175.0f);
  V3 tcenter = tmeshes[0].GetCenter();
  tmeshes[0].Translate(tcenter*-1.0f+center);
  
  //Loading the 2nd geometry
  tmeshes[1].LoadBin("geometry/teapot1K.bin");
  V3 tcenter2 = tmeshes[1].GetCenter();
  tmeshes[1].Translate(tcenter2*-3.0f+center);

  //Loading the 3rd geometry
  tmeshes[2].LoadBin("geometry/teapot1K.bin");
  V3 tcenter3 = tmeshes[2].GetCenter();
  tmeshes[2].Translate(tcenter3*-6.0f+center);

  //Loading the 4th geometry
  tmeshes[3].LoadBin("geometry/teapot1K.bin");
  V3 tcenter4 = tmeshes[3].GetCenter();
  tmeshes[3].Translate(tcenter4*-9.0f+center);

  //Loading the 5th geometry
  tmeshes[4].LoadBin("geometry/teapot1K.bin");
  V3 tcenter5 = tmeshes[4].GetCenter();
  tmeshes[4].Translate(tcenter5*-12.0f+center);

  float size1 = 150.0f;
  AABB aabb = tmeshes[0].GetAABB();
  float size0 = (aabb.corners[1]-aabb.corners[0]).Length();
  tmeshes[0].ScaleAboutCenter(size1/size0);

  Render();
  return;
}

void Scene::Render() 
{
  fb->Set(0xFFFFFFFF);
  fb->SetZ(0.0f);

  for (int tmi = 0; tmi < tmeshesN; tmi++) 
  {
	//tmeshes[tmi].RenderPB(11, ppc, fb);
	//tmeshes[tmi].RenderInWireFrame(ppc, fb);
	tmeshes[tmi].RenderWF(ppc, fb);
  }

	//Rotating point about the axis
  V3 P(1.0f, 0.0f, 0.0f);
  V3 a(0.0f, 1.0f, 0.0f);
  V3 O(0.0f, 0.0f, 0.0f);
  float step = 15.0f;
  for (int i = 0; i <= 24; i++) 
  {
	float theta = step*i;
	V3 rP = P.RotateThisPointAboutAxis(O, a, theta);
	cerr << rP << endl;
  }

  fb->redraw();
}

// function linked to the DBG GUI button for testing new features
void Scene::DBG() 
{

  //Camera Movement
  {
	  ppc->C = ppc->C + V3(-10.0f, +20.0f, +240.0f);
  }
	
  //Rotation About Arbitrary Axis
  {
	V3 a(0.2f, 1.0f, 0.0f);
	a = a.UnitVector();
	V3 O = tmeshes[0].GetCenter();
	//O = O + V3(20.0f, 0.0f, 0.0f);
	//int stepsN = 80;
	int stepsN = 80;
	//float step = 360.0f / stepsN;
	float step = 360.0f / stepsN;
	int revsN = 10;
	for (int i = 1; i <= stepsN*revsN; i++) 
	{
	  tmeshes[0].RotateAboutArbitraryAxis(O, a, step);
	  tmeshes[1].RotateAboutArbitraryAxis_2(O, a, step);
	  tmeshes[2].RotateAboutArbitraryAxis(O, a, step);
	  tmeshes[3].RotateAboutArbitraryAxis_2(O, a, step);
	  tmeshes[4].RotateAboutArbitraryAxis(O, a, step);
	  Render();
	  Fl::check();
	}
	return;
  }

  //Rotating point about the axis
  V3 P(1.0f, 0.0f, 0.0f);
  V3 a(0.0f, 1.0f, 0.0f);
  V3 O(0.0f, 0.0f, 0.0f);
  float step = 15.0f;
  for (int i = 0; i <= 24; i++) 
  {
	float theta = step*i;
	V3 rP = P.RotateThisPointAboutAxis(O, a, theta);
	cerr << rP << endl;
  }
  return;

  Render();
  return;

  V3 p = ppc->C + ppc->c + ppc->a*(float)ppc->w;
  V3 pp;
  bool projOK = ppc->Project(p, pp);
  cerr << projOK << endl << " " << pp << endl;
  return;

  V3 p0(5.2f, 10.7f, 0.0f);
  V3 p1(235.2f, 180.7f, 0.0f);

  unsigned int red = 0xFF0000FF;
  unsigned int green = 0xFF00FF00;

  V3 c0, c1;
  c0.SetColor(red);
  c1.SetColor(green);
  fb->Set(0xFFFFFFFF);
  fb->Draw2DSegment(p0, c0, p1, c1);
  fb->redraw();
  return;


  V3 v0(0.0f, 1.0f, 0.0f);
  V3 v1(10.0f, 11.0f, -30.0f);
  cerr << v0 << endl << v1 << endl << v0+v1 << endl << v0*v1 << endl;
  return;

  fb->Set(0xFFFFFFFF);
  int v = fb->h/2;
  for (int u = 0; u < fb->w; u++) {
	fb->Set(u, v, 0xFF000000);
  }
  fb->redraw();
  return;

  fb->Set(0xFF0000FF);
  fb->redraw();

}

