#pragma once

#include "gui.h"
#include "framebuffer.h"
#include "v3.h"
#include "ppc.h"
#include "tmesh.h"

#include <iostream>

using namespace std;

class Scene 
{
  public:

  //SW framebuffer
  FrameBuffer *fb;
  
  //Planar Pinhole Camera
  PPC *ppc; 

  //Graphical User Interface
  GUI * gui; 

  //Geometry
  TMesh *tmeshes;
  int tmeshesN;

  Scene();
  void DBG();
  void Render();
};

extern Scene *scene;