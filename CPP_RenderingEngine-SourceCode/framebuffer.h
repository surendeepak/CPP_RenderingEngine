#pragma once

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <GL/glut.h>
#include "v3.h"
#include "ppc.h"

// framebuffer + window class

class FrameBuffer : public Fl_Gl_Window 
{
  public:
  unsigned int *pix; // SW color buffer
  int w, h; // image resolution
  float *zb; // SW Z buffer

  // function that is always called back by system and never called directly by programmer
  // programmer triggers framebuffer update by calling FrameBuffer::redraw(), which makes
  //            system call draw
  void draw();

  // function called back when event occurs (mouse, keyboard, etc)
  int handle(int event);
  void KeyboardHandle();

  //a. Construction from input resolution (Top & Left Co-Ordinates, Input Resolution)
  FrameBuffer(int u0, int v0, int _w, int _h); 

  //b. set all pixels to given color
  void Set(unsigned int bgr);

  //c. set one pixel to given color
  void Set(int u, int v, unsigned int bgr);
  void SetGuarded(int u, int v, unsigned int bgr);
  void SetZ(float z0);

  //d. Draw 2-D segments with color interpolation
  void Draw2DSegment(V3 p0, V3 c0, V3 p1, V3 c1);
  void Draw3DSegment(V3 p0, V3 c0, V3 p1, V3 c1, PPC *ppc);
  void Draw2DBigPoint(int u, int v, int psize, unsigned int color);
  void Draw2DBigPoint(V3 pp, int psize, V3 color);
  void Draw3DBigPoint(V3 pt, PPC *ppc, int psize, V3 color);

  //f. Draw axis aligned rectangle
  //g. Draw planar pinhole camera frustum
  //h. Load / save from tiff file
};


