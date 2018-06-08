//
//  params.hpp
//  Proj6
//
//  Created by Matthew Meyn on 11/13/16.
//  Copyright © 2016 Matthew Meyn. All rights reserved.
//

#ifndef PARAMS_H
#define PARAMS_H

#include <stdio.h>


// title of these windows:

const char *WINDOWTITLE = { "Jeremy Fischer" };
const char *GLUITITLE   = { "User Interface Window" };


// initial window size:

const int INIT_WINDOW_SIZE = { 800 };


// minimum allowable scale factor:

const float MINSCALE = { 0.0025f };


// window background color (rgba):

const GLfloat BACKCOLOR[ ] = { 0., 0., 0., 1. };


// fog parameters:

const GLfloat FOGCOLOR[4] = { .0, .0, .0, 1. };
const GLenum  FOGMODE     = { GL_LINEAR };
const GLfloat FOGDENSITY  = { 0.30f };
const GLfloat FOGSTART    = { 1.5 };
const GLfloat FOGEND      = { 4. };


// line width for the axes:

const GLfloat AXES_WIDTH   = { 3. };


// particle parameters:					

const float XCENTER = { 35000. };
const float YCENTER = { -1000. };
const float ZCENTER = { 3000. };

const GLfloat AXES_COLOR[] = { 1., .5, 0. };
const float XMIN = { 100. };
const float XMAX = { 100.0 };
const float YMIN = { -800.0 };
const float YMAX = { -800.0 };
const float ZMIN = { 0. };
const float ZMAX = { 0. };
const float VMIN = { -100. };
const float VMAX = { 100. };
const float RMAX = { 2000. };
const float LOCAL_RMAX = { 500 };

const int MAXLOCALPARTICLES = { 200 * 1024 };


//OpenCL stuff

const int NUM_PARTICLES = (1024*1024);

const int LOCAL_SIZE    = 128;
const char *CL_FILE_NAME   = { "fischjer.cl" };
const char *CL_BINARY_NAME = { "particles.nv" };
const char *PERFORMANCE_FILE_NAME = { "particlePerformance.txt" };

//OpenGL stuff


#define ESCAPE		0x1b

const float ANGFACT = { 1. };
const float SCLFACT = { 0.00125f };


// structs we will need later:

struct xyzw
{
	float x, y, z, w;
};

struct rgba
{
	float r, g, b, a;
};



#endif
