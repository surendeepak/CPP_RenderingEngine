#include "framebuffer.h"
#include "V3.h"
#include <iostream>
#include "scene.h"
#include <math.h>

using namespace std;

// makes an OpenGL window that supports SW, HW rendering, that can be displayed on screen
//        and that receives UI events, i.e. keyboard, mouse, etc.

// rendering callback; see header file comment
void FrameBuffer::draw() 
{
	// SW window, just transfer computed pixels from pix to HW for display
	glDrawPixels(w, h, GL_RGBA, GL_UNSIGNED_BYTE, pix);
}

// function called automatically on event within window (callback)
int FrameBuffer::handle(int event)  
{  
  switch(event) 
  {   
  case FL_KEYBOARD: {
	KeyboardHandle();
	return 0;
  }
  default:
	break;
  }
  return 0;

}

void FrameBuffer::KeyboardHandle() 
{
  int key = Fl::event_key();
  switch (key) {
	case FL_Left: {
	  cerr << "pressed left arrow" << endl;
	  break;
	}
	case 'a': {
	  cerr << "pressed a" << endl;
	  break;
	}
	default:
	cerr << "INFO: do not understand keypress" << endl;
  }
}

//a. Construction from input resolution
FrameBuffer::FrameBuffer(int u0, int v0, int _w, int _h): Fl_Gl_Window(u0, v0, _w, _h, 0) 
{
  w = _w;
  h = _h;
  pix = new unsigned int[w*h];
  zb = new float[w*h];
}

//b. Set all pixels to given color
void FrameBuffer::Set(unsigned int bgr) 
{
  for (int uv = 0; uv < w*h; uv++) {
	pix[uv] = bgr;
  }
}

//c. Set one pixel to given color
void FrameBuffer::Set(int u, int v, unsigned int clr) 
{
  pix[(h-1-v)*w+u] = clr;
}

void FrameBuffer::SetGuarded(int u, int v, unsigned int clr) 
{
  if (u < 0 || u > w-1 || v < 0 || v > h-1)
	return;
  Set(u, v, clr);

}

void FrameBuffer::SetZ(float z0) 
{
  for (int uv = 0; uv < w*h; uv++) {
	zb[uv] = z0;
  }

}

//d. Draw 2-D segment with color interpolation
void FrameBuffer::Draw2DSegment(V3 p0, V3 c0, V3 p1, V3 c1) 
{
  float dx = fabsf(p0[0]-p1[0]);
  float dy = fabsf(p0[1]-p1[1]);
  int n;
  if (dx < dy)
	n = 1+(int)dy;
  else
	n = 1+(int)dx;

  for (int i = 0; i < n; i++) {
	float frac = (float) i / (float) (n-1);
	V3 curr = p0 + (p1-p0)*frac;
	V3 currc = c0 + (c1-c0)*frac;
	int u = (int) curr[0];
	int v = (int) curr[1];
	SetGuarded(u, v, currc.GetColor());
  }
}

void FrameBuffer::Draw2DBigPoint(int u0, int v0, int psize, unsigned int color) 
{
  for (int v = v0-psize/2; v <= v0+psize/2; v++) {
	for (int u = u0-psize/2; u <= u0+psize/2; u++) {
	  SetGuarded(u, v, color);
	}
  }
}

void FrameBuffer::Draw2DBigPoint(V3 pp, int psize, V3 color) 
{
  int u0 = (int) pp[0];
  int v0 = (int) pp[1];
  for (int v = v0-psize/2; v <= v0+psize/2; v++) {
	if (v < 0 || v > h-1)
	  continue;
	for (int u = u0-psize/2; u <= u0+psize/2; u++) {
	  if (u < 0 || u > w-1)
		continue;
	  int uv = (h-1-v)*w+u;
	  if (zb[uv] > pp[2])
		continue;
	  zb[uv] = pp[2];
	  Set(u, v, color.GetColor());
	}
  }

}

void FrameBuffer::Draw3DSegment(V3 p0, V3 c0, V3 p1, V3 c1, PPC *ppc) 
{
  V3 pp0;
  if (!ppc->Project(p0, pp0))
	return;
  V3 pp1;
  if (!ppc->Project(p1, pp1))
	return;

  Draw2DSegment(pp0, c0, pp1, c1);
}

void FrameBuffer::Draw3DBigPoint(V3 pt, PPC *ppc, int psize, V3 color) 
{
  V3 pp;
  if (!ppc->Project(pt, pp))
	return;

  Draw2DBigPoint(pp, psize, color);
}

