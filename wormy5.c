/* ============================================================================
**
** Demonstration of water rendering
** Copyright (C) 2005  Julien Guertault
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
**
** ========================================================================= */

#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>
#include <jerror.h>
#include "noise.h"

#define		RESOLUTION 64

static GLuint	texture;

static int	left_click = GLUT_UP;
static int	right_click = GLUT_UP;
static int	wire_frame = 0;
static int	normals = 0;
static int	xold = 0;
static int	yold = 0;
static float	rotate_x = 30;
static float	rotate_y = 15;
static float	translate_z = 4;

static float	surface[6 * RESOLUTION * (RESOLUTION + 1)];
static float	normal[6 * RESOLUTION * (RESOLUTION + 1)];


static float	z (const float x, const float y, const float t)
{
  const float x2 = x - 3;
  const float y2 = y + 1;
  const float xx = x2 * x2;
  const float yy = y2 * y2;
  return ((2 * sinf (20 * sqrtf (xx + yy) - 4 * t) +
	   Noise (10 * x, 10 * y, t, 0)) / 200);
}


/*
** Function to load a Jpeg file.
*/
int		load_texture (const char * filename,
			      unsigned char * dest,
			      const int format,
			      const unsigned int size)
{
  FILE *fd;
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  unsigned char * line;

  cinfo.err = jpeg_std_error (&jerr);
  jpeg_create_decompress (&cinfo);

  if (0 == (fd = fopen(filename, "rb")))
    return 1;

  jpeg_stdio_src (&cinfo, fd);
  jpeg_read_header (&cinfo, TRUE);
  if ((cinfo.image_width != size) || (cinfo.image_height != size))
    return 1;

  if (GL_RGB == format)
    {
      if (cinfo.out_color_space == JCS_GRAYSCALE)
	return 1;
    }
  else
    if (cinfo.out_color_space != JCS_GRAYSCALE)
      return 1;

  jpeg_start_decompress (&cinfo);

  while (cinfo.output_scanline < cinfo.output_height)
    {
      line = dest +
	(GL_RGB == format ? 3 * size : size) * cinfo.output_scanline;
      jpeg_read_scanlines (&cinfo, &line, 1);
    }
  jpeg_finish_decompress (&cinfo);
  jpeg_destroy_decompress (&cinfo);
  return 0;
}

/*
** Function called to update rendering
*/
void		DisplayFunc (void)
{
  const float t = glutGet (GLUT_ELAPSED_TIME) / 1000.;
  const float delta = 2. / RESOLUTION;
  const unsigned int length = 2 * (RESOLUTION + 1);
  const float xn = (RESOLUTION + 1) * delta + 1;
  unsigned int i;
  unsigned int j;
  float x;
  float y;
  unsigned int indice;
  unsigned int preindice;

  /* Yes, I know, this is quite ugly... */
  float v1x;
  float v1y;
  float v1z;

  float v2x;
  float v2y;
  float v2z;

  float v3x;
  float v3y;
  float v3z;

  float vax;
  float vay;
  float vaz;

  float vbx;
  float vby;
  float vbz;

  float nx;
  float ny;
  float nz;

  float l;


  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity ();
  glTranslatef (0, 0, -translate_z);
  glRotatef (rotate_y, 1, 0, 0);
  glRotatef (rotate_x, 0, 1, 0);

  /* Vertices */
  for (j = 0; j < RESOLUTION; j++)
    {
      y = (j + 1) * delta - 1;
      for (i = 0; i <= RESOLUTION; i++)
	{
	  indice = 6 * (i + j * (RESOLUTION + 1));

	  x = i * delta - 1;
	  surface[indice + 3] = x;
	  surface[indice + 4] = z (x, y, t);
	  surface[indice + 5] = y;
	  if (j != 0)
	    {
	      /* Values were computed during the previous loop */
	      preindice = 6 * (i + (j - 1) * (RESOLUTION + 1));
	      surface[indice] = surface[preindice + 3];
	      surface[indice + 1] = surface[preindice + 4];
	      surface[indice + 2] = surface[preindice + 5];
	    }
	  else
	    {
	      surface[indice] = x;
	      surface[indice + 1] = z (x, -1, t);
	      surface[indice + 2] = -1;
	    }
	}
    }

  /* Normals */
  for (j = 0; j < RESOLUTION; j++)
    for (i = 0; i <= RESOLUTION; i++)
      {
	indice = 6 * (i + j * (RESOLUTION + 1));

	v1x = surface[indice + 3];
	v1y = surface[indice + 4];
	v1z = surface[indice + 5];

	v2x = v1x;
	v2y = surface[indice + 1];
	v2z = surface[indice + 2];

	if (i < RESOLUTION)
	  {
	    v3x = surface[indice + 9];
	    v3y = surface[indice + 10];
	    v3z = v1z;
	  }
	else
	  {
	    v3x = xn;
	    v3y = z (xn, v1z, t);
	    v3z = v1z;
	  }

	vax =  v2x - v1x;
	vay =  v2y - v1y;
	vaz =  v2z - v1z;

	vbx = v3x - v1x;
	vby = v3y - v1y;
	vbz = v3z - v1z;

	nx = (vby * vaz) - (vbz * vay);
	ny = (vbz * vax) - (vbx * vaz);
	nz = (vbx * vay) - (vby * vax);

	l = sqrtf (nx * nx + ny * ny + nz * nz);
	if (l != 0)
	  {
	    l = 1 / l;
	    normal[indice + 3] = nx * l;
	    normal[indice + 4] = ny * l;
	    normal[indice + 5] = nz * l;
	  }
	else
	  {
	    normal[indice + 3] = 0;
	    normal[indice + 4] = 1;
	    normal[indice + 5] = 0;
	  }


	if (j != 0)
	  {
	    /* Values were computed during the previous loop */
	    preindice = 6 * (i + (j - 1) * (RESOLUTION + 1));
	    normal[indice] = normal[preindice + 3];
	    normal[indice + 1] = normal[preindice + 4];
	    normal[indice + 2] = normal[preindice + 5];
	  }
	else
	  {
/* 	    v1x = v1x; */
	    v1y = z (v1x, (j - 1) * delta - 1, t);
	    v1z = (j - 1) * delta - 1;

/* 	    v3x = v3x; */
	    v3y = z (v3x, v2z, t);
	    v3z = v2z;

	    vax = v1x - v2x;
	    vay = v1y - v2y;
	    vaz = v1z - v2z;

	    vbx = v3x - v2x;
	    vby = v3y - v2y;
	    vbz = v3z - v2z;

	    nx = (vby * vaz) - (vbz * vay);
	    ny = (vbz * vax) - (vbx * vaz);
	    nz = (vbx * vay) - (vby * vax);

	    l = sqrtf (nx * nx + ny * ny + nz * nz);
	    if (l != 0)
	      {
		l = 1 / l;
		normal[indice] = nx * l;
		normal[indice + 1] = ny * l;
		normal[indice + 2] = nz * l;
	      }
	    else
	      {
		normal[indice] = 0;
		normal[indice + 1] = 1;
		normal[indice + 2] = 0;
	      }
	  }
      }

  /* The ground */
  glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
  glDisable (GL_TEXTURE_2D);
  glColor3f (1, 0.9, 0.7);
  glBegin (GL_TRIANGLE_FAN);
  glVertex3f (-1, 0, -1);
  glVertex3f (-1, 0,  1);
  glVertex3f ( 1, 0,  1);
  glVertex3f ( 1, 0, -1);
  glEnd ();

  glTranslatef (0, 0.2, 0);

  /* Render wireframe? */
  if (wire_frame != 0)
    glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);

  /* The water */
  glEnable (GL_TEXTURE_2D);
  glColor3f (1, 1, 1);
  glEnableClientState (GL_NORMAL_ARRAY);
  glEnableClientState (GL_VERTEX_ARRAY);
  glNormalPointer (GL_FLOAT, 0, normal);
  glVertexPointer (3, GL_FLOAT, 0, surface);
  for (i = 0; i < RESOLUTION; i++)
    glDrawArrays (GL_TRIANGLE_STRIP, i * length, length);

  /* Draw normals? */
  if (normals != 0)
    {
      glDisable (GL_TEXTURE_2D);
      glColor3f (1, 0, 0);
      glBegin (GL_LINES);
      for (j = 0; j < RESOLUTION; j++)
	for (i = 0; i <= RESOLUTION; i++)
	  {
	    indice = 6 * (i + j * (RESOLUTION + 1));
	    glVertex3fv (&(surface[indice]));
	    glVertex3f (surface[indice] + normal[indice] / 50,
			surface[indice + 1] + normal[indice + 1] / 50,
			surface[indice + 2] + normal[indice + 2] / 50);
	  }

      glEnd ();
    }

  /* End */
  glFlush ();
  glutSwapBuffers ();
  glutPostRedisplay ();
}

/*
** Function called when the window is created or resized
*/
void		ReshapeFunc (int width, int height)
{
  glMatrixMode(GL_PROJECTION);

  glLoadIdentity ();
  gluPerspective (20, width / (float) height, 0.1, 15);
  glViewport (0, 0, width, height);

  glMatrixMode(GL_MODELVIEW);
  glutPostRedisplay();
}

/*
** Function called when a key is hit
*/
void		KeyboardFunc (unsigned char key, int x, int y)
{
  xold = x; /* Has no effect: just to avoid a warning */
  yold = y;
  switch (key)
    {
    case 'q':
    case 'Q':
    case 27:
      exit (0);
      break;
    case 'f':
    case 'F':
    case 'p':
    case 'P':
      wire_frame = 0;
      break;
    case 'l':
    case 'L':
      wire_frame = 1;
      break;
    case 'n':
    case 'N':
      if (0 == normals)
	normals = 1;
      else
	normals = 0;
      break;
    }
}

/*
** Function called when a mouse button is hit
*/
void		MouseFunc (int button, int state, int x, int y)
{
  if (GLUT_LEFT_BUTTON == button)
    left_click = state;
  if (GLUT_RIGHT_BUTTON == button)
    right_click = state;
  xold = x;
  yold = y;
}

/*
** Function called when the mouse is moved
*/
void		MotionFunc (int x, int y)
{
  if (GLUT_DOWN == left_click)
    {
      rotate_y = rotate_y + (y - yold) / 5.0;
      rotate_x = rotate_x + (x - xold) / 5.0;
      if (rotate_y > 90)
	rotate_y = 90;
      if (rotate_y < -90)
	rotate_y = -90;
      glutPostRedisplay ();
    }
  if (GLUT_DOWN == right_click)
    {
      rotate_x = rotate_x + (x - xold) / 5.;
      translate_z = translate_z +
	(yold - y) / 50.;
      if (translate_z < 0.5)
	translate_z = 0.5;
      if (translate_z > 10)
	translate_z = 10;
      glutPostRedisplay ();
    }
  xold = x;
  yold = y;
}


int		water (int narg, char ** args)
{
  unsigned char total_texture[4 * 256 * 256];
  unsigned char alpha_texture[256 * 256];
  unsigned char caustic_texture[3 * 256 * 256];
  unsigned int i;

  InitNoise ();

  /* Creation of the window */
  glutInit (&narg, args);
  glutInitDisplayMode (GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize (500, 500);
  glutCreateWindow ("Water");

  /* OpenGL settings */
  glClearColor (0, 0, 0, 0);
  glEnable (GL_DEPTH_TEST);
  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  /* Texture loading  */
  glGenTextures (1, &texture);
  if (load_texture ("alpha.jpg", alpha_texture, GL_ALPHA, 256) != 0 ||
      load_texture ("reflection.jpg", caustic_texture, GL_RGB, 256) != 0)
    return 1;
  for (i = 0; i < 256 * 256; i++)
    {
      total_texture[4 * i] = caustic_texture[3 * i];
      total_texture[4 * i + 1] = caustic_texture[3 * i + 1];
      total_texture[4 * i + 2] = caustic_texture[3 * i + 2];
      total_texture[4 * i + 3] = alpha_texture[i];
    }
  glBindTexture (GL_TEXTURE_2D, texture);
  gluBuild2DMipmaps (GL_TEXTURE_2D, GL_RGBA, 256, 256, GL_RGBA,
		     GL_UNSIGNED_BYTE,  total_texture);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glEnable (GL_TEXTURE_GEN_S);
  glEnable (GL_TEXTURE_GEN_T);
  glTexGeni (GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
  glTexGeni (GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

  /* Declaration of the callbacks */
  glutDisplayFunc (&DisplayFunc);
  glutReshapeFunc (&ReshapeFunc);
  glutKeyboardFunc (&KeyboardFunc);
  glutMouseFunc (&MouseFunc);
  glutMotionFunc (&MotionFunc);

  /* Loop */
  glutMainLoop ();

  /* Never reached */
  return 0;
}

/* ========================================================================= */













#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <stdio.h>
#include <GL/glut.h>
#include <float.h>
#include <limits.h>
#include <stdio.h>
#include <pthread.h>
#include "system.h"
#include "kdtree.h"
#define NUM_THREADS 5
#include <stdbool.h>
float acc1;
int BIGONE = 1;
float acc2;
int INDEX_NRmore;
int INDEX_NRmores;
void taskwormsmall(void );
#define SIZE_OBJECT 7936 //126976
#define SIZE_OBJECTs 7936 //126976
float springlength =  0.004/4.0;
#define AIR_FRICTION 0.9959999
#define FIRST 1
#define WORK 0
float fovy = 45.0;
float dNear = 100;
float dFar = 2000;
double F_total[3][SIZE_OBJECT*104] ;
double F_totals[3][SIZE_OBJECTs*104] ;
int totalneigbours[SIZE_OBJECT*104];
int totalneigbourss[SIZE_OBJECTs*104];
float     vertexpoint_g_ventral[SIZE_OBJECT][3];
struct state_vector * springVector;
struct state_vector * springVectors;
struct state_vector ** state_result_worm_ventral_feather;
struct state_vector ** state_result_worm_ventral_feathers;
struct state_vector  ** state_result_worm_ventral ;
struct state_vector  ** state_result_worm_ventral_small ;
extern int KvvVENTALA[SIZE_OBJECT*8];
int llll = 0;
int lllls = 0;
int i; 
int feather_once = 1;
int INDEX_NR = 0;
int INDEX_NRs = 0;
Light *dirLight;
Material *material;


float springConstant =   200.202f;

float frictionConstant =    0.4200000f;

float cameraEye[3] = {0.0, 0.0, 1000.0};
float lightRotation[16] = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
float cameraLookAt[4] = {0.0, 0.0, 0.0, 1.0};
float cameraUp[4] = {0.0, 1.0, 0.0};

float viewRotation[16] = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};

float rotationX=0.0;
float rotationY=0.0;

 typedef struct {
	float x, y, z;
} Vec3;

void rates_dorsal ( double *t, double *f, double result[]   );
void key(unsigned char key, int x, int y);
int findnearestpoint(int points,struct state_vector ** worm , int num,int howmany);
void display  (void);
struct state_vector   *   worm_ventral(struct state_vector     *  next_state, int num );
double length (struct state_vector *a,struct state_vector *b ) { return sqrtf ( b->pos_new_x*a->pos_new_x + b->pos_new_y*a->pos_new_y + b->pos_new_z*a->pos_new_z); }
static double dist_sq( double *a1, double *a2, int dims ) {
  double dist_sq = 0, diff;
  while( --dims >= 0 ) {
    diff = (a1[dims] - a2[dims]);
    dist_sq += diff*diff;
  }
  return dist_sq;
}
float global_ambient[4] = {0.0, 0.0, 0.0, 0.0};
void  setPosition_light(float x, float y, float z)
{
////////////////////////////printf("light1\n");
    dirLight->pos[0] = x;
    dirLight->pos[1] = y;
    dirLight->pos[2] = z;
    dirLight->pos[3] = 0.0f;
}

void  setAmbient_light(float x, float y, float z)
{
////////////////////////////printf("light2\n");
    dirLight->ambient[0] = x;
    dirLight->ambient[1] = y;
    dirLight->ambient[2] = z;
}

void  setDifusse_light(float x, float y, float z)
{
////////////////////////////printf("light3\n");
    dirLight->difusse[0] = x;
    dirLight->difusse[1] = y;
    dirLight->difusse[2] = z;
}

void  setSpecular_light(float x, float y, float z)
{
 
    dirLight->specular[0] = x;
    dirLight->specular[1] = y;
    dirLight->specular[2] = z;
}

 

void setAmbient_material(float x, float y, float z, float e)
{
    material->ambient[0] = x;
    material->ambient[1] = y;
    material->ambient[2] = z;
    material->ambient[3] = e;
}

void  setDifusse_material(float x, float y, float z, float e)
{
    material->difusse[0] = x;
    material->difusse[1] = y;
    material->difusse[2] = z;
    material->difusse[3] = e;
}

void  setSpecular_material(float x, float y, float z, float e)
{
    material->specular[0] = x;
    material->specular[1] = y;
    material->specular[2] = z;
    material->specular[3] = e;
}

void  setShininess_material(float shine)
{
    material->shininess = shine;
}
float prevX=0.0;
float prevY=0.0;
bool mouseDown=false;
void mouse(int button, int state, int x, int y){
    if(button == GLUT_LEFT_BUTTON && state==GLUT_DOWN){
        mouseDown = true;
        prevX = x - rotationY;
        prevY = y - rotationX;
    }else{
        mouseDown = false;
    }
}

void mouseMotion(int x, int y){
    if(mouseDown){
        rotationX = y - prevY;
        rotationY = x - prevX;
        glutPostRedisplay();
    }
}
 
 
void reshape(int w, int h){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fovy, (GLdouble)w/(GLdouble)h, dNear, dFar);
    glViewport(0, 0, w, h);
} 

int acc1once= 1;
void taskworm(void )
{

int k;
for(k = 0; k <= totalneigbours[llll]; k++)
{	// m*a = -k*s
 	//10*a= -k * 0.004 solve k
if(acc1once == 0)
	{
	if(fabs(acc1) > 0)
		{		
		springConstant =    14.0/1 *1;
//	printf("springConstant = %.10f \n", springConstant);
		 frictionConstant =   .001*1;
	}
	}
acc1once = 0;	
	// printf("1totalneigbours[llll]  %d :: number %d :: index_____ = %d\n",totalneigbours[llll],llll,INDEX_NR);
	INDEX_NR =  (state_result_worm_ventral_feather[llll][k].INDEX_NR);
	
//if(  INDEX_NR > 0 && INDEX_NR < 1000000  )
//{
	//if(state_result_worm_ventral[INDEX_NR]->force_sign != -1 && state_result_worm_ventral[INDEX_NR]->force_sign != 1 && INDEX_NR >0)
	//{
		 if(feather_once == FIRST  )
		{	

						 if(fabs(F_total[0][INDEX_NR]) > 6  )
	 				 	  F_total[0][INDEX_NR] =0;
 
		 		 if(fabs(F_total[1][INDEX_NR])  > 6 )
						F_total[1][INDEX_NR] =0;

			 	 if(fabs(F_total[2][INDEX_NR])  > 6 )
						F_total[2][INDEX_NR] =0;
  //printf("2totalneigbours[llll]  %d :: number %d :: index_____ = %d\n",totalneigbours[llll],llll,INDEX_NR);
		springVector->pos_new_x = state_result_worm_ventral[INDEX_NR]->pos_new_x - state_result_worm_ventral[llll]->pos_new_x;
		springVector->pos_new_y = state_result_worm_ventral[INDEX_NR]->pos_new_y - state_result_worm_ventral[llll]->pos_new_y;
		springVector->pos_new_z = state_result_worm_ventral[INDEX_NR]->pos_new_z - state_result_worm_ventral[llll]->pos_new_z;
 

		float r = length(state_result_worm_ventral[INDEX_NR],state_result_worm_ventral[llll]);

		if ( r != 0  &&r < 1.1  && r > -1.1   )
		{			
			F_total[0][INDEX_NR] +=( springVector->pos_new_x  /  r ) * ( r - springlength ) * ( -springConstant );
			F_total[1][INDEX_NR] += ( springVector->pos_new_y /  r ) * ( r - springlength ) * ( -springConstant );
			F_total[2][INDEX_NR] +=( springVector->pos_new_z  /  r ) * ( r - springlength ) * ( -springConstant );

			F_total[0][llll] +=( springVector->pos_new_x  / r ) * ( r - springlength ) * ( -springConstant );
			F_total[1][llll] += ( springVector->pos_new_y  / r ) * ( r - springlength) * ( -springConstant );
			F_total[2][llll] +=( springVector->pos_new_z  / r ) * ( r - springlength ) * ( -springConstant );

F_total[0][INDEX_NR] +=-(  state_result_worm_ventral[INDEX_NR]->vel_new_x - state_result_worm_ventral[llll]->vel_new_x ) * frictionConstant;
 		F_total[1][INDEX_NR] += -( state_result_worm_ventral[INDEX_NR]->vel_new_y- state_result_worm_ventral[llll]->vel_new_y ) * frictionConstant;
 		F_total[2][INDEX_NR] += -(  state_result_worm_ventral[INDEX_NR]->vel_new_z - state_result_worm_ventral[llll ]->vel_new_z ) * frictionConstant;


		F_total[0][llll] +=-(  state_result_worm_ventral[INDEX_NR]->vel_new_x - state_result_worm_ventral[llll]->vel_new_x ) * frictionConstant;
 		F_total[1][llll] += -( state_result_worm_ventral[INDEX_NR]->vel_new_y- state_result_worm_ventral[llll]->vel_new_y ) * frictionConstant;
 		F_total[2][llll] += -(  state_result_worm_ventral[INDEX_NR]->vel_new_z - state_result_worm_ventral[llll ]->vel_new_z ) * frictionConstant;
		state_result_worm_ventral[llll]->vel_new_x = state_result_worm_ventral[llll]->vel_new_x*0.995 ;
		
		state_result_worm_ventral[llll]->vel_new_y = state_result_worm_ventral[llll]->vel_new_y*0.995 ;

		state_result_worm_ventral[llll]->vel_new_z = state_result_worm_ventral[llll]->vel_new_z*0.995 ;

		state_result_worm_ventral[INDEX_NR]->vel_new_x = state_result_worm_ventral[INDEX_NR]->vel_new_x*0.995 ;
		
		state_result_worm_ventral[INDEX_NR]->vel_new_y = state_result_worm_ventral[INDEX_NR]->vel_new_y*0.995 ;
		state_result_worm_ventral[INDEX_NR]->vel_new_z = state_result_worm_ventral[INDEX_NR]->vel_new_z*0.995 ;

		state_result_worm_ventral[INDEX_NR]->force_sign =  -1;
		state_result_worm_ventral[INDEX_NR]    =  worm_ventral(  state_result_worm_ventral[INDEX_NR] ,1) ;

		state_result_worm_ventral[llll]->force_sign =   1; ; 
		state_result_worm_ventral[llll]    =  worm_ventral(  state_result_worm_ventral[llll] ,1);
		}
else
{
		state_result_worm_ventral[INDEX_NR]->force_sign =  -1;
		state_result_worm_ventral[INDEX_NR]    =  worm_ventral(  state_result_worm_ventral[INDEX_NR] ,1) ;

		state_result_worm_ventral[llll]->force_sign =   1; ; 
		state_result_worm_ventral[llll]    =  worm_ventral(  state_result_worm_ventral[llll] ,1);


}
		


		
if(state_result_worm_ventral[INDEX_NR]->pos_new_y < -10)
			{
			 state_result_worm_ventral[INDEX_NR]->pos_new_y = -10;
			 state_result_worm_ventral[INDEX_NR]->vel_new_y = -state_result_worm_ventral[INDEX_NR]->vel_new_y*0.00995;
 
			}	
		if(state_result_worm_ventral[llll]->pos_new_y < -10)
			{
				 state_result_worm_ventral[llll]->pos_new_y = -10;
			 state_result_worm_ventral[llll]->vel_new_y = -state_result_worm_ventral[llll]->vel_new_y*0.00995;
 
			}

 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//interaktioner mellan 2 objekt!

if(llll < SIZE_OBJECTs)
{	 
 if( k <=totalneigbourss[llll])
	{
	  INDEX_NRs =  (state_result_worm_ventral_feathers[llll][k].INDEX_NR);
 if(fabs(F_totals[0][INDEX_NRs]) > 6  )
	 				 	  F_totals[0][INDEX_NRs] =0;
 
		 		 if(fabs(F_totals[1][INDEX_NRs])  > 6 )
						F_totals[1][INDEX_NRs] =0;

			 	 if(fabs(F_totals[2][INDEX_NRs])  > 6 )
						F_totals[2][INDEX_NRs] =0;
springVector->pos_new_x = state_result_worm_ventral_small[INDEX_NRs]->pos_new_x - state_result_worm_ventral[llll]->pos_new_x;
		springVector->pos_new_y = state_result_worm_ventral_small[INDEX_NRs]->pos_new_y - state_result_worm_ventral[llll]->pos_new_y;
		springVector->pos_new_z = state_result_worm_ventral_small[INDEX_NRs]->pos_new_z - state_result_worm_ventral[llll]->pos_new_z;
 

		  r = length(state_result_worm_ventral_small[INDEX_NRs],state_result_worm_ventral[llll]);

		if ( r != 0  &&   r < 0.1   && r > -0.1   )
		{			
			F_totals[0][INDEX_NRs] +=( springVector->pos_new_x  /  r ) * ( r - springlength ) * ( -springConstant );
			F_totals[1][INDEX_NRs] += ( springVector->pos_new_y /  r ) * ( r - springlength ) * ( -springConstant );
			F_totals[2][INDEX_NRs] +=( springVector->pos_new_z  /  r ) * ( r - springlength ) * ( -springConstant );
	 
			F_total[0][llll] +=( springVector->pos_new_x  / r ) * ( r - springlength ) * ( -springConstant );
			F_total[1][llll] += ( springVector->pos_new_y  / r ) * ( r - springlength) * ( -springConstant );
			F_total[2][llll] +=( springVector->pos_new_z  / r ) * ( r - springlength ) * ( -springConstant );

F_totals[0][INDEX_NRs] +=-(  state_result_worm_ventral_small[INDEX_NRs]->vel_new_x - state_result_worm_ventral[llll]->vel_new_x ) * frictionConstant;
 		F_totals[1][INDEX_NRs] += -( state_result_worm_ventral_small[INDEX_NRs]->vel_new_y- state_result_worm_ventral[llll]->vel_new_y ) * frictionConstant;
 		F_totals[2][INDEX_NRs] += -(  state_result_worm_ventral_small[INDEX_NRs]->vel_new_z - state_result_worm_ventral[llll ]->vel_new_z ) * frictionConstant;


		F_total[0][llll] +=-(  state_result_worm_ventral_small[INDEX_NRs]->vel_new_x - state_result_worm_ventral[llll]->vel_new_x ) * frictionConstant;
 		F_total[1][llll] += -( state_result_worm_ventral_small[INDEX_NRs]->vel_new_y- state_result_worm_ventral[llll]->vel_new_y ) * frictionConstant;
 		F_total[2][llll] += -(  state_result_worm_ventral_small[INDEX_NRs]->vel_new_z - state_result_worm_ventral[llll ]->vel_new_z ) * frictionConstant;
		state_result_worm_ventral[llll]->vel_new_x = state_result_worm_ventral[llll]->vel_new_x*0.999 ;
		
		state_result_worm_ventral[llll]->vel_new_y = state_result_worm_ventral[llll]->vel_new_y*0.999 ;

		state_result_worm_ventral[llll]->vel_new_z = state_result_worm_ventral[llll]->vel_new_z*0.999 ;

		state_result_worm_ventral_small[INDEX_NRs]->vel_new_x = state_result_worm_ventral_small[INDEX_NRs]->vel_new_x*0.999 ;
		
		state_result_worm_ventral_small[INDEX_NRs]->vel_new_y = state_result_worm_ventral_small[INDEX_NRs]->vel_new_y*0.999 ;
		state_result_worm_ventral_small[INDEX_NRs]->vel_new_z = state_result_worm_ventral_small[INDEX_NRs]->vel_new_z*0.999 ;

		state_result_worm_ventral_small[INDEX_NRs]->force_sign =  -1;
		state_result_worm_ventral_small[INDEX_NRs]    =  worm_ventral(  state_result_worm_ventral_small[INDEX_NRs] ,1) ;

		state_result_worm_ventral[llll]->force_sign =   1; ; 
		state_result_worm_ventral[llll]    =  worm_ventral(  state_result_worm_ventral[llll] ,1);
		}
else
{

		state_result_worm_ventral_small[INDEX_NRs]->force_sign =  -1;
		state_result_worm_ventral_small[INDEX_NRs]    =  worm_ventral(  state_result_worm_ventral_small[INDEX_NRs] ,1) ;

		state_result_worm_ventral[llll]->force_sign =   1; ; 
		state_result_worm_ventral[llll]    =  worm_ventral(  state_result_worm_ventral[llll] ,1);

}

		



if(state_result_worm_ventral_small[INDEX_NRs]->pos_new_y < -10)
			{
			 state_result_worm_ventral_small[INDEX_NRs]->pos_new_y = -10;
			 state_result_worm_ventral_small[INDEX_NRs]->vel_new_y = -state_result_worm_ventral_small[INDEX_NRs]->vel_new_y*0.00995;
 
			}	
		if(state_result_worm_ventral[llll]->pos_new_y < -10)
			{
				 state_result_worm_ventral[llll]->pos_new_y = -10;
			 state_result_worm_ventral[llll]->vel_new_y = -state_result_worm_ventral[llll]->vel_new_y*0.00995;
 
			}
}
 

}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	for(int d = 0; d <= totalneigbours[llll]; d++)
	{	 
		
		if(d == k  )
		  continue;

 
   // state_result_worm_ventral_feather[llll][d].INDEX_NR = find_index_____(SIZE_OBJECT,state_result_worm_ventral_feather,llll,d);
INDEX_NRmore = state_result_worm_ventral_feather[llll][d].INDEX_NR;
 

		 

 

				 if(fabs(F_total[0][INDEX_NRmore]) > 6  )
	 				  F_total[0][INDEX_NRmore] =0;
 
		 		 if(fabs(F_total[1][INDEX_NRmore])  > 6 )
					  F_total[1][INDEX_NRmore] =0;

			 	 if(fabs(F_total[2][INDEX_NRmore])  > 6 )
					  F_total[2][INDEX_NRmore] =0;
			

 				if(fabs(F_total[0][INDEX_NR]) > 6  )
	 				  F_total[0][INDEX_NR] =0;
 
		 		 if(fabs(F_total[1][INDEX_NR])  > 6 )
					  F_total[1][INDEX_NR] =0;

			 	 if(fabs(F_total[2][INDEX_NR])  > 6 )
					  F_total[2][INDEX_NR] =0;
			
		springVector->pos_new_x = state_result_worm_ventral[INDEX_NR]->pos_new_x - state_result_worm_ventral[INDEX_NRmore]->pos_new_x;
		springVector->pos_new_y = state_result_worm_ventral[INDEX_NR]->pos_new_y - state_result_worm_ventral[INDEX_NRmore]->pos_new_y;
		springVector->pos_new_z = state_result_worm_ventral[INDEX_NR]->pos_new_z - state_result_worm_ventral[INDEX_NRmore]->pos_new_z;
 			

		float r = length(state_result_worm_ventral[INDEX_NR],state_result_worm_ventral[INDEX_NRmore]);

		if ( r != 0 && r < 1.1  && r > -1.1    )
		{	
	//	 printf("1totalneigbours[llll]  %d :: number %d :: index_____ = %d\n",totalneigbours[llll],llll,INDEX_NRmore);	
			F_total[0][INDEX_NR] +=( springVector->pos_new_x  /  r ) * ( r - springlength ) * ( -springConstant );
			F_total[1][INDEX_NR] += ( springVector->pos_new_y /  r ) * ( r - springlength ) * ( -springConstant );
			F_total[2][INDEX_NR] +=( springVector->pos_new_z  /  r ) * ( r - springlength ) * ( -springConstant );

			F_total[0][INDEX_NRmore] +=( springVector->pos_new_x  / r ) * ( r - springlength ) * ( -springConstant );
			F_total[1][INDEX_NRmore] += ( springVector->pos_new_y  / r ) * ( r - springlength) * ( -springConstant );
			F_total[2][INDEX_NRmore] +=( springVector->pos_new_z  / r ) * ( r - springlength ) * ( -springConstant );

		F_total[0][INDEX_NR] +=-(  state_result_worm_ventral[INDEX_NR]->vel_new_x - state_result_worm_ventral[INDEX_NRmore]->vel_new_x ) * frictionConstant;
 		F_total[1][INDEX_NR] += -( state_result_worm_ventral[INDEX_NR]->vel_new_y- state_result_worm_ventral[INDEX_NRmore]->vel_new_y ) * frictionConstant;
 		F_total[2][INDEX_NR] += -(  state_result_worm_ventral[INDEX_NR]->vel_new_z - state_result_worm_ventral[INDEX_NRmore ]->vel_new_z ) * frictionConstant;


		F_total[0][INDEX_NRmore] +=-(  state_result_worm_ventral[INDEX_NR]->vel_new_x - state_result_worm_ventral[INDEX_NRmore]->vel_new_x ) * frictionConstant;
 		F_total[1][INDEX_NRmore] += -( state_result_worm_ventral[INDEX_NR]->vel_new_y- state_result_worm_ventral[INDEX_NRmore]->vel_new_y ) * frictionConstant;
 		F_total[2][INDEX_NRmore] += -(  state_result_worm_ventral[INDEX_NR]->vel_new_z - state_result_worm_ventral[INDEX_NRmore ]->vel_new_z ) * frictionConstant;

		state_result_worm_ventral[INDEX_NRmore]->vel_new_x = state_result_worm_ventral[INDEX_NRmore]->vel_new_x*0.995 ;
		
		state_result_worm_ventral[INDEX_NRmore]->vel_new_y = state_result_worm_ventral[INDEX_NRmore]->vel_new_y*0.995 ;

		state_result_worm_ventral[INDEX_NRmore]->vel_new_z = state_result_worm_ventral[INDEX_NRmore]->vel_new_z*0.995 ;

		state_result_worm_ventral[INDEX_NR]->vel_new_x = state_result_worm_ventral[INDEX_NR]->vel_new_x*0.995 ;
		
		state_result_worm_ventral[INDEX_NR]->vel_new_y = state_result_worm_ventral[INDEX_NR]->vel_new_y*0.995 ;
		state_result_worm_ventral[INDEX_NR]->vel_new_z = state_result_worm_ventral[INDEX_NR]->vel_new_z*0.995 ;

		state_result_worm_ventral[INDEX_NR]->force_sign =  -1;
		state_result_worm_ventral[INDEX_NR]    =  worm_ventral(  state_result_worm_ventral[INDEX_NR] ,1) ;

		state_result_worm_ventral[INDEX_NRmore]->force_sign =   1; ; 
		state_result_worm_ventral[INDEX_NRmore]    =  worm_ventral(  state_result_worm_ventral[INDEX_NRmore] ,1);
		}
else
{
		state_result_worm_ventral[INDEX_NR]->force_sign =  -1;
		state_result_worm_ventral[INDEX_NR]    =  worm_ventral(  state_result_worm_ventral[INDEX_NR] ,1) ;

		state_result_worm_ventral[INDEX_NRmore]->force_sign =   1; ; 
		state_result_worm_ventral[INDEX_NRmore]    =  worm_ventral(  state_result_worm_ventral[INDEX_NRmore] ,1);

}




if(state_result_worm_ventral[INDEX_NR]->pos_new_y < -10)
			{
			 state_result_worm_ventral[INDEX_NR]->pos_new_y = -10;
			 state_result_worm_ventral[INDEX_NR]->vel_new_y = -state_result_worm_ventral[INDEX_NR]->vel_new_y*0.00995;
 
			}	
		if(state_result_worm_ventral[INDEX_NRmore]->pos_new_y < -10)
			{
				 state_result_worm_ventral[INDEX_NRmore]->pos_new_y = -10;
			 state_result_worm_ventral[INDEX_NRmore]->vel_new_y = -state_result_worm_ventral[INDEX_NRmore]->vel_new_y*0.00995;
 
			}
	//	 printf("f[0] = %.10f:%.10f%.10f \n", state_result_worm_ventral[INDEX_NRmore]->pos_new_x,state_result_worm_ventral[INDEX_NRmore]->pos_new_y,state_result_worm_ventral[INDEX_NRmore]->pos_new_z);


	 ////////////////////////////////////////////////////////////////////////////////////////////////////
//interaktioner mellan 2 objekt!









if(llll < SIZE_OBJECTs)
{
 
 if( k <=totalneigbourss[llll])
	{
INDEX_NRmores = state_result_worm_ventral_feathers[llll][d].INDEX_NR;


			if(fabs(F_totals[0][INDEX_NRmores]) > 6  )
	 				  F_totals[0][INDEX_NRmores] =0;
 
		 		 if(fabs(F_totals[1][INDEX_NRmores])  > 6 )
					  F_totals[1][INDEX_NRmores] =0;

			 	 if(fabs(F_totals[2][INDEX_NRmores])  > 6 )
					  F_totals[2][INDEX_NRmores] =0;





springVector->pos_new_x = state_result_worm_ventral[INDEX_NR]->pos_new_x - state_result_worm_ventral_small[INDEX_NRmores]->pos_new_x;
		springVector->pos_new_y = state_result_worm_ventral[INDEX_NR]->pos_new_y - state_result_worm_ventral_small[INDEX_NRmores]->pos_new_y;
		springVector->pos_new_z = state_result_worm_ventral[INDEX_NR]->pos_new_z - state_result_worm_ventral_small[INDEX_NRmores]->pos_new_z;
 			

		float r = length(state_result_worm_ventral[INDEX_NR],state_result_worm_ventral_small[INDEX_NRmores]);

		if ( r != 0  &&    r < 0.1  && r > -0.1    )
		{	
	//	 printf("1totalneigbours[llll]  %d :: number %d :: index_____ = %d\n",totalneigbours[llll],llll,INDEX_NRmore);	
			F_total[0][INDEX_NR] +=( springVector->pos_new_x  /  r ) * ( r - springlength ) * ( -springConstant );
			F_total[1][INDEX_NR] += ( springVector->pos_new_y /  r ) * ( r - springlength ) * ( -springConstant );
			F_total[2][INDEX_NR] +=( springVector->pos_new_z  /  r ) * ( r - springlength ) * ( -springConstant );
 
			F_totals[0][INDEX_NRmores] +=( springVector->pos_new_x  / r ) * ( r - springlength ) * ( -springConstant );
			F_totals[1][INDEX_NRmores] += ( springVector->pos_new_y  / r ) * ( r - springlength) * ( -springConstant );
			F_totals[2][INDEX_NRmores] +=( springVector->pos_new_z  / r ) * ( r - springlength ) * ( -springConstant );

		F_total[0][INDEX_NR] +=-(  state_result_worm_ventral[INDEX_NR]->vel_new_x - state_result_worm_ventral_small[INDEX_NRmores]->vel_new_x ) * frictionConstant;
 		F_total[1][INDEX_NR] += -( state_result_worm_ventral[INDEX_NR]->vel_new_y- state_result_worm_ventral_small[INDEX_NRmores]->vel_new_y ) * frictionConstant;
 		F_total[2][INDEX_NR] += -(  state_result_worm_ventral[INDEX_NR]->vel_new_z - state_result_worm_ventral_small[INDEX_NRmores ]->vel_new_z ) * frictionConstant;


		F_totals[0][INDEX_NRmores] +=-(  state_result_worm_ventral[INDEX_NR]->vel_new_x - state_result_worm_ventral_small[INDEX_NRmores]->vel_new_x ) * frictionConstant;
 		F_totals[1][INDEX_NRmores] += -( state_result_worm_ventral[INDEX_NR]->vel_new_y- state_result_worm_ventral_small[INDEX_NRmores]->vel_new_y ) * frictionConstant;
 		F_totals[2][INDEX_NRmores] += -(  state_result_worm_ventral[INDEX_NR]->vel_new_z - state_result_worm_ventral_small[INDEX_NRmores ]->vel_new_z ) * frictionConstant;

		state_result_worm_ventral_small[INDEX_NRmores]->vel_new_x = state_result_worm_ventral_small[INDEX_NRmores]->vel_new_x*0.999 ;
		
		state_result_worm_ventral_small[INDEX_NRmores]->vel_new_y = state_result_worm_ventral_small[INDEX_NRmores]->vel_new_y*0.999 ;

		state_result_worm_ventral_small[INDEX_NRmores]->vel_new_z = state_result_worm_ventral_small[INDEX_NRmores]->vel_new_z*0.999 ;

		state_result_worm_ventral[INDEX_NR]->vel_new_x = state_result_worm_ventral[INDEX_NR]->vel_new_x*0.999 ;
		
		state_result_worm_ventral[INDEX_NR]->vel_new_y = state_result_worm_ventral[INDEX_NR]->vel_new_y*0.999 ;
		state_result_worm_ventral[INDEX_NR]->vel_new_z = state_result_worm_ventral[INDEX_NR]->vel_new_z*0.999 ;

		state_result_worm_ventral[INDEX_NR]->force_sign =  -1;
		state_result_worm_ventral[INDEX_NR]    =  worm_ventral(  state_result_worm_ventral[INDEX_NR] ,1) ;

		state_result_worm_ventral_small[INDEX_NRmores]->force_sign =   1; ; 
		state_result_worm_ventral_small[INDEX_NRmores]    =  worm_ventral(  state_result_worm_ventral_small[INDEX_NRmores] ,1);

		}
else
{


		state_result_worm_ventral[INDEX_NR]->force_sign =  -1;
		state_result_worm_ventral[INDEX_NR]    =  worm_ventral(  state_result_worm_ventral[INDEX_NR] ,1) ;

		state_result_worm_ventral_small[INDEX_NRmores]->force_sign =   1; ; 
		state_result_worm_ventral_small[INDEX_NRmores]    =  worm_ventral(  state_result_worm_ventral_small[INDEX_NRmores] ,1);

}



if(state_result_worm_ventral[INDEX_NR]->pos_new_y < -10)
			{
			 state_result_worm_ventral[INDEX_NR]->pos_new_y = -10;
			 state_result_worm_ventral[INDEX_NR]->vel_new_y = -state_result_worm_ventral[INDEX_NR]->vel_new_y*0.00995;
 
			}	
		if(state_result_worm_ventral_small[INDEX_NRmores]->pos_new_y < -10)
			{
				 state_result_worm_ventral_small[INDEX_NRmores]->pos_new_y = -10;
			 state_result_worm_ventral_small[INDEX_NRmores]->vel_new_y = -state_result_worm_ventral_small[INDEX_NRmores]->vel_new_y*0.00995;
 
			}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	 
 	//} 
}
}
 
 	
//}
		 
 }	
//}
		 
 }		

		 if(fabs(F_total[0][lllls]) > 6  )
	 				 F_total[0][lllls] =0;
 
		 		 if(fabs(F_total[1][lllls])  > 6 )
					 F_total[1][lllls] =0;

			 	 if(fabs(F_total[2][lllls])  > 6 )
					 F_total[2][lllls] =0;


				 if(fabs(F_total[0][llll]) > 6  )
	 				 F_total[0][llll] =0;
 
		 		 if(fabs(F_total[1][llll])  > 6 )
					 F_total[1][llll] =0;

			 	 if(fabs(F_total[2][llll])  > 6 )
					 F_total[2][llll] =0;
			// printf("FORCE(x,y,z) = %.50f:%.50f%.50f \n", fabs(F_total[0][INDEX_NR]),fabs(F_total[1][llll]),fabs(F_total[2][INDEX_NRmore]));
			
			if(llll >= SIZE_OBJECT -1  )
			{
				//feather_once= WORK;
	   			llll =0;
				int var;
			//	for(var = 0; var <  3 -1 ; var++)
			//	{
				//	state_result_worm_ventral[var]->force_sign = 0;
 	
				//}
			}
}
llll++;
}

int find_index_____(int NUM, struct state_vector **real  ,int num,int k )
{
int ll = 0;
 	while(ll < NUM)
	{
		if(state_result_worm_ventral_feather[num][k].pos_new_x ==real[ll]->pos_new_x && state_result_worm_ventral_feather[num][k].pos_new_y ==real[ll]->pos_new_y && 		state_result_worm_ventral_feather[num][k].pos_new_z ==real[ll]->pos_new_z)
		{
			//printf("found index_____ @ %d \n", ll);
			return ll;
		}
	ll++;
	}
}
int find_index_____s(int NUM, struct state_vector **real  ,int num,int k )
{
int ll = 0;
 	while(ll < NUM)
	{
		if(state_result_worm_ventral_feathers[num][k].pos_new_x ==real[ll]->pos_new_x && state_result_worm_ventral_feathers[num][k].pos_new_y ==real[ll]->pos_new_y && 		state_result_worm_ventral_feathers[num][k].pos_new_z ==real[ll]->pos_new_z)
		{
			//printf("found index_____ @ %d \n", ll);
			return ll;
		}
	ll++;
	}
}
void *ptree;
  
int ptreeonce = 1;
int findnearestpoint(int points,struct state_vector ** worm , int num,int howmany) {
int i, num_pts = points;
int INDEX;
char *data, *pch;
struct kdres *presults;
double pos[3], dist;
double pt[3] = { worm[num]->pos_new_x,worm[num]->pos_new_y, worm[num]->pos_new_z };
double radius = 0.002/1.0;

num_pts =points;

srand( time(0) );

if(ptreeonce < 2  )
{
	ptree = kd_create( 3 );
  	for( i=0; i<points; i++ ) {   
    		assert( 0 == kd_insert3( ptree, worm[i]->pos_new_x, worm[i]->pos_new_y, worm[i]->pos_new_z, NULL ) );
 	 }
}
ptreeonce++;

 
presults = kd_nearest_range( ptree, pt, radius );
 
  /* print out all the points found in results */
  //printf( "found %d results:\n", kd_res_size(presults) );


 state_result_worm_ventral_feather[num][0].totaln = kd_res_size(presults) ;

//state_result_worm_dorsal_feather[num][0].totaln = kd_res_size(presults) ;
 //printf( "found %d results:\n", kd_res_size(presults) );
int feather_count = 0;
//printf(" state_result_worm_ventral_feather[llll][0].totaln %d ::%d\n",  state_result_worm_ventral_feather[num][0].totaln,num);
while( !kd_res_end( presults ) ) {
   
	pch = (char*)kd_res_item( presults, pos ); 
    	dist = sqrt( dist_sq( pt, pos, 3 ) );

 // if(worm[num]->pos_new_x != pos[0])
 //	  printf( "node at (%.3f, %.3f, %.3f) is %.3f away   \n", 
 //	    pos[0], pos[1], pos[2], dist  );
	 

  	totalneigbours[num] = state_result_worm_ventral_feather[num][0].totaln;
	// if(worm[num]->pos_new_x != pos[0])
   		state_result_worm_ventral_feather[num][feather_count].pos_new_x = pos[0];
	// if(worm[num]->pos_new_y != pos[1])
  		state_result_worm_ventral_feather[num][feather_count].pos_new_y = pos[1];
	//  if(worm[num]->pos_new_z != pos[2])
   		state_result_worm_ventral_feather[num][feather_count].pos_new_z = pos[2];
	int k;
 
   
  INDEX = find_index_____(SIZE_OBJECT,state_result_worm_ventral,num,feather_count);
 // printf("INDEX = %d \n", INDEX);
  state_result_worm_ventral_feather[num][feather_count].INDEX_NR = INDEX;
  feather_count++;
    kd_res_next( presults );
 if(num % 1000 ==0)
 	printf("num @ %d \n", num);
}
    /* go to the next entry */

 
  feather_count = 0;
  /* free our tree, results set, and other allocated memory */
  //free( data );
  kd_res_free( presults );
  //kd_free( ptree );

  return 0;
}
 





void *ptrees;
  
int ptreeonces = 1;
int findnearestpoints(int points,struct state_vector ** worm , int num,int howmany) {
int i, num_pts = points;
int INDEX;
char *data, *pch;
struct kdres *presults;
double pos[3], dist;
double pt[3] = { worm[num]->pos_new_x,worm[num]->pos_new_y, worm[num]->pos_new_z };
double radius = 0.002/1.0;

num_pts =points;

srand( time(0) );

if(ptreeonces < 2  )
{
	ptrees = kd_create( 3 );
  	for( i=0; i<points; i++ ) {   
    		assert( 0 == kd_insert3( ptrees, worm[i]->pos_new_x, worm[i]->pos_new_y, worm[i]->pos_new_z, NULL ) );
 	 }
}
ptreeonces++;

 
presults = kd_nearest_range( ptrees, pt, radius );
 
  /* print out all the points found in results */
  //printf( "found %d results:\n", kd_res_size(presults) );


 state_result_worm_ventral_feathers[num][0].totaln = kd_res_size(presults) ;

//state_result_worm_dorsal_feather[num][0].totaln = kd_res_size(presults) ;
 //printf( "found %d results:\n", kd_res_size(presults) );
int feather_count = 0;
//printf(" state_result_worm_ventral_feather[llll][0].totaln %d ::%d\n",  state_result_worm_ventral_feather[num][0].totaln,num);
while( !kd_res_end( presults ) ) {
   
	pch = (char*)kd_res_item( presults, pos ); 
    	dist = sqrt( dist_sq( pt, pos, 3 ) );

 // if(worm[num]->pos_new_x != pos[0])
 //	  printf( "node at (%.3f, %.3f, %.3f) is %.3f away   \n", 
 //	    pos[0], pos[1], pos[2], dist  );
	 

  	totalneigbourss[num] = state_result_worm_ventral_feathers[num][0].totaln;
	// if(worm[num]->pos_new_x != pos[0])
   		state_result_worm_ventral_feathers[num][feather_count].pos_new_x = pos[0];
	// if(worm[num]->pos_new_y != pos[1])
  		state_result_worm_ventral_feathers[num][feather_count].pos_new_y = pos[1];
	//  if(worm[num]->pos_new_z != pos[2])
   		state_result_worm_ventral_feathers[num][feather_count].pos_new_z = pos[2];
	int k;
 
   
  INDEX = find_index_____s(SIZE_OBJECTs,state_result_worm_ventral_small,num,feather_count);
 // printf("INDEX = %d \n", INDEX);
  state_result_worm_ventral_feathers[num][feather_count].INDEX_NR = INDEX;
  feather_count++;
    kd_res_next( presults );
 if(num % 1000 ==0)
 	printf("num @ %d \n", num);
}
    /* go to the next entry */

 
  feather_count = 0;
  /* free our tree, results set, and other allocated memory */
  //free( data );
  kd_res_free( presults );
  //kd_free( ptree );

  return 0;
}
int first_run = 1;
int initonce =1;
extern int KvvVENTAL ;
extern int KvvDORSAL ;

extern float* V; 
extern float* V2;
int main (int argc, char **argv)
{ 

loadOBJ__("sphere.obj"); //61856 //61856 //13108_Eastern_Hognose_Snake_v1_L3  61856
 
springVector = malloc(SIZE_OBJECT*1);
springVectors = malloc(SIZE_OBJECTs*1);
state_result_worm_ventral_feather = (struct state_vector*)malloc(sizeof(struct state_vector*)*SIZE_OBJECT*4);
state_result_worm_ventral_feathers = (struct state_vector*)malloc(sizeof(struct state_vector*)*SIZE_OBJECTs*4);
state_result_worm_ventral =(struct state_vector*)malloc(sizeof(struct state_vector*)*SIZE_OBJECT*32);
state_result_worm_ventral_small =(struct state_vector*)malloc(sizeof(struct state_vector*)*SIZE_OBJECTs*32);

 int i,j,k;

for(int i=0; i<SIZE_OBJECT*1; i++)
	state_result_worm_ventral[i] = (struct state_vector*)malloc(sizeof(struct state_vector)*32);
       
printf("done ! \n");
 
for(int i=0; i<SIZE_OBJECT*1; i++)
	state_result_worm_ventral_feather[i] = (struct state_vector*)malloc(sizeof(struct state_vector)*32);


for(int i=0; i<SIZE_OBJECTs*1; i++)
	state_result_worm_ventral_feathers[i] = (struct state_vector*)malloc(sizeof(struct state_vector)*32); 
printf("done ! \n");
  
 
for(int i=0; i<SIZE_OBJECTs*1; i++)
	state_result_worm_ventral_small[i] = (struct state_vector*)malloc(sizeof(struct state_vector)*32);

  
printf("done ! \n");
if(initonce==1)
{
	Vec3 *v ;
 
	int ll;
	for(ll = 0; ll < SIZE_OBJECT  ; ll++)
	{
		  v= (Vec3 *)(V + 3*(KvvVENTALA[ll]-1));
         	  state_result_worm_ventral[ll]->pos_new_x = v->x ;
 
 	          state_result_worm_ventral[ll]->pos_new_y = v->y ;
 		  state_result_worm_ventral[ll]->pos_new_z = v->z ;
 
 	  	  state_result_worm_ventral[ll]->vel_new_x =  0 ;
 		  state_result_worm_ventral[ll]->vel_new_y = 0;
 		  state_result_worm_ventral[ll]->vel_new_z =  0 ;
		  state_result_worm_ventral[ll]->mass = ll;
 

 


	}
	for(ll = 0; ll < SIZE_OBJECT      ; ll++)
	{
 		 findnearestpoint(SIZE_OBJECT  ,state_result_worm_ventral,ll,1 );
	}

printf("done ! \n");
loadOBJ__("sphere.obj"); //61856 //61856 //13108_Eastern_Hognose_Snake_v1_L3  61856	 
	for(ll = 0; ll < SIZE_OBJECTs  ; ll++)
	{
		  v= (Vec3 *)(V + 3*(KvvVENTALA[ll]-1));
 
    		  state_result_worm_ventral_small[ll]->pos_new_x = v->x ;
 
 	          state_result_worm_ventral_small[ll]->pos_new_y = v->y +2.5;
 		  state_result_worm_ventral_small[ll]->pos_new_z = v->z ;
 
 	  	  state_result_worm_ventral_small[ll]->vel_new_x =  0 ;
 		  state_result_worm_ventral_small[ll]->vel_new_y =  0.0;
 		  state_result_worm_ventral_small[ll]->vel_new_z =  0 ;
		  state_result_worm_ventral_small[ll]->mass = ll;


	}
 

	for(ll = 0; ll < SIZE_OBJECTs     ; ll++)
	{
 	 	findnearestpoints(SIZE_OBJECTs  ,state_result_worm_ventral_small,ll,1 );
	}
 
	initonce = 0;
}
unsigned char total_texture[4 * 256 * 256];
  unsigned char alpha_texture[256 * 256];
  unsigned char caustic_texture[3 * 256 * 256];
 init_mpgeg();

  InitNoise ();

  /* Creation of the window */
  glutInit (&argc, argv);
  glutInitDisplayMode (GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize (1920, 1080);
  glutCreateWindow ("Water");

  /* OpenGL settings */
  glClearColor (0, 0, 0, 0);
  glEnable (GL_DEPTH_TEST);
  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  /* Texture loading  */
  glGenTextures (1, &texture);
  if (load_texture ("alpha.jpg", alpha_texture, GL_ALPHA, 256) != 0 ||
      load_texture ("reflection.jpg", caustic_texture, GL_RGB, 256) != 0)
    return 1;
  for (i = 0; i < 256 * 256; i++)
    {
      total_texture[4 * i] = caustic_texture[3 * i];
      total_texture[4 * i + 1] = caustic_texture[3 * i + 1];
      total_texture[4 * i + 2] = caustic_texture[3 * i + 2];
      total_texture[4 * i + 3] = alpha_texture[i];
    }
  glBindTexture (GL_TEXTURE_2D, texture);
  gluBuild2DMipmaps (GL_TEXTURE_2D, GL_RGBA, 256, 256, GL_RGBA,
		     GL_UNSIGNED_BYTE,  total_texture);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glEnable (GL_TEXTURE_GEN_S);
  glEnable (GL_TEXTURE_GEN_T);
  glTexGeni (GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
  glTexGeni (GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
/*
glutInit(&argc, argv);
init_mpgeg();

GLint glut_display;

glutInitWindowSize (1920,1080);
glutCreateWindow ("wormy");

int mainMenu;
const GLubyte *str;
str = glGetString (GL_EXTENSIONS);

glEnable(GL_NORMALIZE);
glEnable(GL_DEPTH_TEST);
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
glMatrixMode(GL_MODELVIEW);

glEnable(GL_LIGHTING);
glEnable(GL_LIGHT0);
glEnable(GL_NORMALIZE);
glEnable(GL_COLOR_MATERIAL);

glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
dirLight = (Light*)malloc(sizeof(Light));
dirLight->setPosition_light = setPosition_light;
dirLight->setAmbient_light = setAmbient_light;
dirLight->setDifusse_light = setDifusse_light;
dirLight->setSpecular_light = setSpecular_light;
 
dirLight->setPosition_light(100,100, 800);
dirLight->setAmbient_light(0, 0, 0);
dirLight->setDifusse_light(1, 1, 1);
dirLight->setSpecular_light(0.995f, 0.1f, 0.88f);
 
material =(Material*)malloc(sizeof(Material));
material->setAmbient_material =setAmbient_material;
material->setDifusse_material = setDifusse_material;
material->setSpecular_material =setSpecular_material;
material->setShininess_material = setShininess_material;
material->setAmbient_material(0, 0, 1, 1);
material->setDifusse_material(0.5f, 0.1f, 0.995f, 1.0f/50.0f);
material->setSpecular_material(0.002f, 0.002f, 0.002f, 1.0f/50.0f);
material->setShininess_material(128);
glLightfv(GL_LIGHT0, GL_POSITION, dirLight->pos);
glLightfv(GL_LIGHT0, GL_AMBIENT,   dirLight->ambient);
glLightfv(GL_LIGHT0, GL_SPECULAR,  dirLight->specular);
glLightfv(GL_LIGHT0, GL_DIFFUSE,   dirLight->difusse);  


glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient); */ 
glutDisplayFunc (display );
 
glutReshapeFunc(reshape);
 
glutMouseFunc(mouse);
glutKeyboardFunc(key);
  	
 
glutMotionFunc(mouseMotion);
  
	
glutMainLoop();

}

struct state_vector   *   worm_ventral(struct state_vector     *  next_state, int num )
{
	double f0[6];
	double f0_result[6];

	double z[6];
 	double t0[6] = {0,0,0,0,0,0};
 	double tburn = 1.0;
	double tf[6] = {tburn,tburn,tburn,tburn,tburn,tburn};

	z[0] =  next_state->pos_new_x;
	z[1] =  next_state->pos_new_y;
	z[2] = next_state->pos_new_z;
	z[3] =  next_state->vel_new_x;
	z[4] =  next_state->vel_new_y;
	z[5] = next_state->vel_new_z;

	f0[0] = next_state->pos_new_x;
	f0[1] =next_state->pos_new_y;
	f0[2] =next_state->pos_new_z;
	f0[3] =next_state->vel_new_x;
	f0[4] =next_state->vel_new_y;
	f0[5] = next_state->vel_new_z;
 
	rk45(rates_dorsal, t0, f0,f0_result, tf,z,6, 1  );

	next_state->pos_new_x = f0[0];
	next_state->pos_new_y = f0[1];
	next_state->pos_new_z = f0[2];

	next_state->vel_new_x = f0[3];
	next_state->vel_new_y = f0[4];
	next_state->vel_new_z = f0[5];

	return   next_state ;
}
 
void key(unsigned char key, int x, int y)
{
 
   if(key == 'X') cameraEye[0]-= 50;
   if(key == 'x') cameraEye[2]+= 50;
   if(key == 'y') cameraEye[1]-= 50;
   if(key == 'Y') cameraEye[1]+= 50;
   if(key == 'z') cameraEye[2]-= 50;
   if(key == 'Z') cameraEye[2]+= 50;
 
   if(key == '+')
   {
      
           
        
   }
   if(key == '-')
   {        
   }

   glutPostRedisplay();
}

void display  (void){
 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1,1,1,1);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_COLOR_MATERIAL);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glMultMatrixf(lightRotation);
 

    glLoadIdentity();
    gluLookAt(cameraEye[0]+353, cameraEye[1]-0, cameraEye[2]-600, cameraLookAt[0], cameraLookAt[1], cameraLookAt[2], cameraUp[0], cameraUp[1], cameraUp[2]);

    glMultMatrixf(viewRotation);

    glRotatef(rotationX,1,0,0);
    glRotatef(rotationY+320,0,1,0);

    glMultMatrixf(lightRotation);

   for(int ll = 0; ll <   SIZE_OBJECTs/4 ; ll++)  
		  {



  const float t = glutGet (GLUT_ELAPSED_TIME) / 1000.;
  const float delta = 2. / RESOLUTION;
  const unsigned int length = 2 * (RESOLUTION + 1);
  const float xn = (RESOLUTION + 1) * delta + 1;
  unsigned int i;
  unsigned int j;
  float x;
  float y;
  unsigned int indice;
  unsigned int preindice;

  /* Yes, I know, this is quite ugly... */
  float v1x;
  float v1y;
  float v1z;

  float v2x;
  float v2y;
  float v2z;

  float v3x;
  float v3y;
  float v3z;

  float vax;
  float vay;
  float vaz;

  float vbx;
  float vby;
  float vbz;

  float nx;
  float ny;
  float nz;

  float l;

BIGONE = 1;
taskworm();
BIGONE = 0;

taskwormsmall();

 

  /* Vertices */
  //
for (j = 0; j < RESOLUTION; j++)
    {
   //   y = (j + 1) * delta - 1;
      for (i = 0; i <= RESOLUTION; i++)
	{
	  indice = 6 * (i + j * (RESOLUTION + 1));

	//  x = i * delta - 1;
	  surface[indice + 3] = state_result_worm_ventral[ll]->pos_new_x;
	  surface[indice + 4] = state_result_worm_ventral[ll]->pos_new_y;//z (state_result_worm_ventral[ll]->pos_new_x, state_result_worm_ventral[ll]->pos_new_y, t); 
	  surface[indice + 5] = state_result_worm_ventral[ll]->pos_new_z;
	  if (j != 0)
	    {
	      /* Values were computed during the previous loop */
	      preindice = 6 * (i + (j - 1) * (RESOLUTION + 1));
	      surface[indice] = surface[preindice + 3];
	      surface[indice + 1] = surface[preindice + 4];
	      surface[indice + 2] = surface[preindice + 5];
	    }
	  else
	    {
	      surface[indice] = state_result_worm_ventral[ll]->pos_new_x;
	      surface[indice + 1] = state_result_worm_ventral[ll]->pos_new_y;// z (state_result_worm_ventral[ll]->pos_new_x, -1, t);
	      surface[indice + 2] = state_result_worm_ventral[ll]->pos_new_z;
	    }
	}
    }

  /* Normals */
  for (j = 0; j < RESOLUTION; j++)
    for (i = 0; i <= RESOLUTION; i++)
      {
	indice = 6 * (i + j * (RESOLUTION + 1));

	v1x = surface[indice + 3];
	v1y = surface[indice + 4];
	v1z = surface[indice + 5];

	v2x = v1x;
	v2y = surface[indice + 1];
	v2z = surface[indice + 2];

	if (i < RESOLUTION)
	  {
	    v3x = surface[indice + 9];
	    v3y = surface[indice + 10];
	    v3z = v1z;
	  }
	else
	  {
	    v3x = xn;
	    v3y = z (xn, v1z, t);
	    v3z = v1z;
	  }

	vax =  v2x - v1x;
	vay =  v2y - v1y;
	vaz =  v2z - v1z;

	vbx = v3x - v1x;
	vby = v3y - v1y;
	vbz = v3z - v1z;

	nx = (vby * vaz) - (vbz * vay);
	ny = (vbz * vax) - (vbx * vaz);
	nz = (vbx * vay) - (vby * vax);

	l = sqrtf (nx * nx + ny * ny + nz * nz);
	if (l != 0)
	  {
	    l = 1 / l;
	    normal[indice + 3] = nx * l;
	    normal[indice + 4] = ny * l;
	    normal[indice + 5] = nz * l;
	  }
	else
	  {
	    normal[indice + 3] = 0;
	    normal[indice + 4] = 1;
	    normal[indice + 5] = 0;
	  }


	if (j != 0)
	  {
	    /* Values were computed during the previous loop */
	    preindice = 6 * (i + (j - 1) * (RESOLUTION + 1));
	    normal[indice] = normal[preindice + 3];
	    normal[indice + 1] = normal[preindice + 4];
	    normal[indice + 2] = normal[preindice + 5];
	  }
	else
	  {
/* 	    v1x = v1x; */
	    v1y = z (v1x, (j - 1) * delta - 1, t);
	    v1z = (j - 1) * delta - 1;

/* 	    v3x = v3x; */
	    v3y = z (v3x, v2z, t);
	    v3z = v2z;

	    vax = v1x - v2x;
	    vay = v1y - v2y;
	    vaz = v1z - v2z;

	    vbx = v3x - v2x;
	    vby = v3y - v2y;
	    vbz = v3z - v2z;

	    nx = (vby * vaz) - (vbz * vay);
	    ny = (vbz * vax) - (vbx * vaz);
	    nz = (vbx * vay) - (vby * vax);

	    l = sqrtf (nx * nx + ny * ny + nz * nz);
	    if (l != 0)
	      {
		l = 1 / l;
		normal[indice] = nx * l;
		normal[indice + 1] = ny * l;
		normal[indice + 2] = nz * l;
	      }
	    else
	      {
		normal[indice] = 0;
		normal[indice + 1] = 1;
		normal[indice + 2] = 0;
	      }
	  }
      }

  /* The ground */
 /* glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
  glDisable (GL_TEXTURE_2D);
  glColor3f (1, 0.9, 0.7);
  glBegin (GL_TRIANGLE_FAN);
  glVertex3f (-1, 0, -1);
  glVertex3f (-1, 0,  1);
  glVertex3f ( 1, 0,  1);
  glVertex3f ( 1, 0, -1);
  glEnd ();

  glTranslatef (0, 0.2, 0);*/

  /* Render wireframe? */
  //if (wire_frame != 0)
  //  glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);

  /* The water */
  glEnable (GL_TEXTURE_2D);
  glColor3f (1, 1, 1);
  glEnableClientState (GL_NORMAL_ARRAY);
  glEnableClientState (GL_VERTEX_ARRAY);
  glNormalPointer (GL_FLOAT, 0, normal);
  glVertexPointer (3, GL_FLOAT, 0, surface);
  for (i = 0; i < RESOLUTION; i++)
    glDrawArrays (GL_TRIANGLES, i * length, length);

  /* Draw normals? */
  if (normals != 0)
    {
      glDisable (GL_TEXTURE_2D);
      glColor3f (1, 0, 0);
      glBegin (GL_LINES);
      for (j = 0; j < RESOLUTION; j++)
	for (i = 0; i <= RESOLUTION; i++)
	  {
	    indice = 6 * (i + j * (RESOLUTION + 1));
	    glVertex3fv (&(surface[indice]));
	    glVertex3f (surface[indice] + normal[indice] / 50,
			surface[indice + 1] + normal[indice + 1] / 50,
			surface[indice + 2] + normal[indice + 2] / 50);
	  }

      glEnd ();
    }

  /* End */
 
 



if(ll <  SIZE_OBJECT/4)
{
 glPushMatrix();
glScalef(210,210,210);
   glTranslatef(state_result_worm_ventral[ll]->pos_new_x, state_result_worm_ventral[ll]->pos_new_y,state_result_worm_ventral[ll]->pos_new_z);
 
  glColor3f(state_result_worm_ventral[ll]->pos_new_x, state_result_worm_ventral[ll]->pos_new_y, state_result_worm_ventral[ll]->pos_new_z);  

  glutSolidSphere(0.06,12,12);
  glPopMatrix(); 
 
}


 
glPushMatrix();
glScalef(210,210,210);
  glTranslatef(state_result_worm_ventral_small[ll]->pos_new_x, state_result_worm_ventral_small[ll]->pos_new_y,state_result_worm_ventral_small[ll]->pos_new_z);
 
 glColor3f(state_result_worm_ventral_small[ll]->pos_new_x, state_result_worm_ventral_small[ll]->pos_new_y, state_result_worm_ventral_small[ll]->pos_new_z);  

  glutSolidSphere(0.06,12,12);
 
  glPopMatrix(); 

 glutPostRedisplay();
}

display_mpeg();
 glFlush ();
glutSwapBuffers();

}




void rates_dorsal ( double *t, double *f, double result[]   )
{
// printf("f[0] = %.10f:%.10f%.10f \n", f[0],f[1],f[2]);
if(BIGONE == 1)
{
if(state_result_worm_ventral[llll]->force_sign == 1)
{
    	result[0] =             f[3]/100.0;
    	result[1] =             f[4]/100.0;
    	result[2] =             f[5]/100.0;
  
	result[3] = (F_total[0][llll])/(42000   );
	result[4] = (F_total[1][llll]   )/(42000   );
	result[5] = (F_total[2][llll] )/(42000); 

 
}
else if(state_result_worm_ventral[INDEX_NR]->force_sign == -1)
{
    result[0] =             f[3]/100.0;
    result[1] =             f[4]/100.0;
    result[2] =             f[5]/100.0;
  
    result[3] = -(F_total[0][INDEX_NR])/(42000);
    result[4] = -(F_total[1][INDEX_NR]  ) /(42000) ;
    result[5] = -(F_total[2][INDEX_NR] )/(42000); //522
 

}
else if(state_result_worm_ventral[INDEX_NRmore]->force_sign == 1)
{
    	result[0] =             f[3]/100.0;
    	result[1] =             f[4]/100.0;
    	result[2] =             f[5]/100.0;
  
	result[3] = (F_total[0][INDEX_NRmore])/(42000   );
	result[4] = (F_total[1][INDEX_NRmore]  )/(42000  );
	result[5] = (F_total[2][INDEX_NRmore] )/(42000); 

 
}

}else
 if(state_result_worm_ventral_small[lllls]->force_sign == 1)
{
    	result[0] =             f[3]/100.0;
    	result[1] =             f[4]/100.0;
    	result[2] =             f[5]/100.0;
  
	result[3] = (F_totals[0][lllls])/(42000  );
	result[4] = (F_totals[1][lllls]   -9.8  )/(42000   );
	result[5] = (F_totals[2][lllls] )/(42000); 

 
}
else if(state_result_worm_ventral_small[INDEX_NRs]->force_sign == -1)
{
    result[0] =             f[3]/100.0;
    result[1] =             f[4]/100.0;
    result[2] =             f[5]/100.0;
  
    result[3] = -(F_totals[0][INDEX_NRs])/(42000);
    result[4] = -(F_totals[1][INDEX_NRs]  -9.8  ) /(42000) ;
    result[5] = -(F_totals[2][INDEX_NRs] )/(42000); //522
 

}
else if(state_result_worm_ventral_small[INDEX_NRmores]->force_sign == 1)
{
    	result[0] =             f[3]/100.0;
    	result[1] =             f[4]/100.0;
    	result[2] =             f[5]/100.0;
  
	result[3] = (F_totals[0][INDEX_NRmores])/(42000   );
	result[4] = (F_totals[1][INDEX_NRmores] -9.8  )/(42000  );
	result[5] = (F_totals[2][INDEX_NRmores] )/(42000); 

 
}
  acc1 = sqrtf(pow(result[3],2.0) + pow(result[4],2.0)+pow(result[5],2.0));
//printf("acc1 = %.10f \n", acc1);
}



int feather_onces = 1;

int acc1onces = 0;
void taskwormsmall(void )
{

int k;
for(k = 0; k <= totalneigbourss[lllls]; k++)
{	// m*a = -k*s
 	//10*a= -k * 0.004 solve k
if(acc1onces == 0)
	{
	if(fabs(acc1) > 0)
		{		
		springConstant =    14.0/1 *1;
//	printf("springConstant = %.10f \n", springConstant);
		 frictionConstant =   .001 *1;
	}
	}
acc1onces = 0;	
	// printf("1totalneigbours[llll]  %d :: number %d :: index_____ = %d\n",totalneigbours[llll],llll,INDEX_NR);
	INDEX_NRs =  (state_result_worm_ventral_feathers[lllls][k].INDEX_NR);
	
if(  INDEX_NRs > 0 && INDEX_NRs < 1000000  )
{
	//if(state_result_worm_ventral[INDEX_NR]->force_sign != -1 && state_result_worm_ventral[INDEX_NR]->force_sign != 1 && INDEX_NR >0)
	//{
		 if(feather_onces == FIRST && INDEX_NRs > 0)
		{	

						 if(fabs(F_totals[0][INDEX_NRs]) > 6  )
	 				 	  F_totals[0][INDEX_NRs] =0;
 
		 		 if(fabs(F_totals[1][INDEX_NRs])  > 6 )
						F_totals[1][INDEX_NRs] =0;

			 	 if(fabs(F_totals[2][INDEX_NRs])  > 6 )
						F_totals[2][INDEX_NRs] =0;
  //printf("2totalneigbours[llll]  %d :: number %d :: index_____ = %d\n",totalneigbours[llll],llll,INDEX_NR);
		springVectors->pos_new_x = state_result_worm_ventral_small[INDEX_NRs]->pos_new_x - state_result_worm_ventral_small[lllls]->pos_new_x;
		springVectors->pos_new_y = state_result_worm_ventral_small[INDEX_NRs]->pos_new_y - state_result_worm_ventral_small[lllls]->pos_new_y;
		springVectors->pos_new_z = state_result_worm_ventral_small[INDEX_NRs]->pos_new_z - state_result_worm_ventral_small[lllls]->pos_new_z;
 

		float r = length(state_result_worm_ventral_small[INDEX_NRs],state_result_worm_ventral_small[lllls]);

		if ( r != 0  &&   r < 1.1   && r > -1.1     )
		{			
			F_totals[0][INDEX_NRs] +=( springVectors->pos_new_x  /  r ) * ( r - springlength ) * ( -springConstant );
			F_totals[1][INDEX_NRs] += ( springVectors->pos_new_y /  r ) * ( r - springlength ) * ( -springConstant );
			F_totals[2][INDEX_NRs] +=( springVectors->pos_new_z  /  r ) * ( r - springlength ) * ( -springConstant );

			F_totals[0][lllls] +=( springVectors->pos_new_x  / r ) * ( r - springlength ) * ( -springConstant );
			F_totals[1][lllls] += ( springVectors->pos_new_y  / r ) * ( r - springlength) * ( -springConstant );
			F_totals[2][lllls] +=( springVectors->pos_new_z  / r ) * ( r - springlength ) * ( -springConstant );

F_totals[0][INDEX_NRs] +=-(  state_result_worm_ventral_small[INDEX_NRs]->vel_new_x - state_result_worm_ventral_small[lllls]->vel_new_x ) * frictionConstant;
 		F_totals[1][INDEX_NRs] += -( state_result_worm_ventral_small[INDEX_NRs]->vel_new_y- state_result_worm_ventral_small[lllls]->vel_new_y ) * frictionConstant;
 		F_totals[2][INDEX_NRs] += -(  state_result_worm_ventral_small[INDEX_NRs]->vel_new_z - state_result_worm_ventral_small[lllls ]->vel_new_z ) * frictionConstant;


		F_totals[0][lllls] +=-(  state_result_worm_ventral_small[INDEX_NRs]->vel_new_x - state_result_worm_ventral_small[lllls]->vel_new_x ) * frictionConstant;
 		F_totals[1][lllls] += -( state_result_worm_ventral_small[INDEX_NRs]->vel_new_y- state_result_worm_ventral_small[lllls]->vel_new_y ) * frictionConstant;
 		F_totals[2][lllls] += -(  state_result_worm_ventral_small[INDEX_NRs]->vel_new_z - state_result_worm_ventral_small[lllls ]->vel_new_z ) * frictionConstant;
		state_result_worm_ventral_small[lllls]->vel_new_x = state_result_worm_ventral_small[lllls]->vel_new_x*0.995 ;
		
		state_result_worm_ventral_small[lllls]->vel_new_y = state_result_worm_ventral_small[lllls]->vel_new_y*0.995 ;

		state_result_worm_ventral_small[lllls]->vel_new_z = state_result_worm_ventral_small[lllls]->vel_new_z*0.995 ;

		state_result_worm_ventral_small[INDEX_NRs]->vel_new_x = state_result_worm_ventral_small[INDEX_NRs]->vel_new_x*0.995 ;
		
		state_result_worm_ventral_small[INDEX_NRs]->vel_new_y = state_result_worm_ventral_small[INDEX_NRs]->vel_new_y*0.995 ;
		state_result_worm_ventral_small[INDEX_NRs]->vel_new_z = state_result_worm_ventral_small[INDEX_NRs]->vel_new_z*0.995 ;


		state_result_worm_ventral_small[INDEX_NRs]->force_sign =  -1;
		state_result_worm_ventral_small[INDEX_NRs]    =  worm_ventral(  state_result_worm_ventral_small[INDEX_NRs] ,1) ;

		state_result_worm_ventral_small[lllls]->force_sign =   1; ; 
		state_result_worm_ventral_small[lllls]    =  worm_ventral(  state_result_worm_ventral_small[lllls] ,1);
		}
else
{

		state_result_worm_ventral_small[INDEX_NRs]->force_sign =  -1;
		state_result_worm_ventral_small[INDEX_NRs]    =  worm_ventral(  state_result_worm_ventral_small[INDEX_NRs] ,1) ;

		state_result_worm_ventral_small[lllls]->force_sign =   1; ; 
		state_result_worm_ventral_small[lllls]    =  worm_ventral(  state_result_worm_ventral_small[lllls] ,1);


}
		

		

if(state_result_worm_ventral_small[INDEX_NRs]->pos_new_y < -10)
			{
			 state_result_worm_ventral_small[INDEX_NRs]->pos_new_y = -10;
			 state_result_worm_ventral_small[INDEX_NRs]->vel_new_y = -state_result_worm_ventral_small[INDEX_NRs]->vel_new_y*0.0995;
 
			}	
		if(state_result_worm_ventral_small[lllls]->pos_new_y < -10)
			{
				 state_result_worm_ventral_small[lllls]->pos_new_y = -10;
			 state_result_worm_ventral_small[lllls]->vel_new_y = -state_result_worm_ventral_small[lllls]->vel_new_y*0.00995;
 
			}
 		





//interaktioner mellan 2 objekt!

if(lllls < SIZE_OBJECT)
{
	 
 if( k <=totalneigbours[lllls])
	{
	  INDEX_NR =  (state_result_worm_ventral_feather[lllls][k].INDEX_NR);
 if(fabs(F_total[0][INDEX_NR]) > 6  )
	 				 	  F_total[0][INDEX_NR] =0;
 
		 		 if(fabs(F_total[1][INDEX_NR])  > 6 )
						F_total[1][INDEX_NR] =0;

			 	 if(fabs(F_total[2][INDEX_NR])  > 6 )
						F_total[2][INDEX_NR] =0;
springVectors->pos_new_x = state_result_worm_ventral[INDEX_NR]->pos_new_x - state_result_worm_ventral_small[lllls]->pos_new_x;
		springVectors->pos_new_y = state_result_worm_ventral[INDEX_NR]->pos_new_y - state_result_worm_ventral_small[lllls]->pos_new_y;
		springVectors->pos_new_z = state_result_worm_ventral[INDEX_NR]->pos_new_z - state_result_worm_ventral_small[lllls]->pos_new_z;
 

		  r = length(state_result_worm_ventral[INDEX_NR],state_result_worm_ventral_small[lllls]);

		if ( r != 0  &&   r <0.1 && r > -0.1   )
		{			
			F_total[0][INDEX_NR] +=( springVectors->pos_new_x  /  r ) * ( r - springlength ) * ( -springConstant );
			F_total[1][INDEX_NR] += ( springVectors->pos_new_y /  r ) * ( r - springlength ) * ( -springConstant );
			F_total[2][INDEX_NR] +=( springVectors->pos_new_z  /  r ) * ( r - springlength ) * ( -springConstant );
	 
			F_totals[0][lllls] +=( springVectors->pos_new_x  / r ) * ( r - springlength ) * ( -springConstant );
			F_totals[1][lllls] += ( springVectors->pos_new_y  / r ) * ( r - springlength) * ( -springConstant );
			F_totals[2][lllls] +=( springVectors->pos_new_z  / r ) * ( r - springlength ) * ( -springConstant );

F_total[0][INDEX_NR] +=-(  state_result_worm_ventral[INDEX_NR]->vel_new_x - state_result_worm_ventral_small[lllls]->vel_new_x ) * frictionConstant;
 		F_total[1][INDEX_NR] += -( state_result_worm_ventral[INDEX_NR]->vel_new_y- state_result_worm_ventral_small[lllls]->vel_new_y ) * frictionConstant;
 		F_total[2][INDEX_NR] += -(  state_result_worm_ventral[INDEX_NR]->vel_new_z - state_result_worm_ventral_small[lllls ]->vel_new_z ) * frictionConstant;


		F_totals[0][lllls] +=-(  state_result_worm_ventral[INDEX_NR]->vel_new_x - state_result_worm_ventral_small[lllls]->vel_new_x ) * frictionConstant;
 		F_totals[1][lllls] += -( state_result_worm_ventral[INDEX_NR]->vel_new_y- state_result_worm_ventral_small[lllls]->vel_new_y ) * frictionConstant;
 		F_totals[2][lllls] += -(  state_result_worm_ventral[INDEX_NR]->vel_new_z - state_result_worm_ventral_small[lllls ]->vel_new_z ) * frictionConstant;
		state_result_worm_ventral_small[lllls]->vel_new_x = state_result_worm_ventral_small[lllls]->vel_new_x*0.999 ;
		
		state_result_worm_ventral_small[lllls]->vel_new_y = state_result_worm_ventral_small[lllls]->vel_new_y*0.999 ;

		state_result_worm_ventral_small[lllls]->vel_new_z = state_result_worm_ventral_small[lllls]->vel_new_z*0.999 ;

		state_result_worm_ventral[INDEX_NR]->vel_new_x = state_result_worm_ventral[INDEX_NR]->vel_new_x*0.999 ;
		
		state_result_worm_ventral[INDEX_NR]->vel_new_y = state_result_worm_ventral[INDEX_NR]->vel_new_y*0.999 ;
		state_result_worm_ventral[INDEX_NR]->vel_new_z = state_result_worm_ventral[INDEX_NR]->vel_new_z*0.999 ;

	state_result_worm_ventral[INDEX_NR]->force_sign =  -1;
		state_result_worm_ventral[INDEX_NR]    =  worm_ventral(  state_result_worm_ventral[INDEX_NR] ,1) ;

		state_result_worm_ventral_small[lllls]->force_sign =   1; ; 
		state_result_worm_ventral_small[lllls]    =  worm_ventral(  state_result_worm_ventral_small[lllls] ,1);
		}
else
{

state_result_worm_ventral[INDEX_NR]->force_sign =  -1;
		state_result_worm_ventral[INDEX_NR]    =  worm_ventral(  state_result_worm_ventral[INDEX_NR] ,1) ;

		state_result_worm_ventral_small[lllls]->force_sign =   1; ; 
		state_result_worm_ventral_small[lllls]    =  worm_ventral(  state_result_worm_ventral_small[lllls] ,1);

}

		

	

if(state_result_worm_ventral[INDEX_NR]->pos_new_y < -10)
			{
			 state_result_worm_ventral[INDEX_NR]->pos_new_y = -10;
			 state_result_worm_ventral[INDEX_NR]->vel_new_y = -state_result_worm_ventral[INDEX_NR]->vel_new_y*0.00995;
 
			}	
		if(state_result_worm_ventral_small[lllls]->pos_new_y < -10)
			{
				 state_result_worm_ventral_small[lllls]->pos_new_y = -10;
			 state_result_worm_ventral_small[lllls]->vel_new_y = -state_result_worm_ventral_small[lllls]->vel_new_y*0.00995;
 
			}
}
 

}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////










	for(int d = 0; d <= totalneigbourss[lllls]; d++)
	{	 
		
		if(d == k  )
		  continue;

 
   // state_result_worm_ventral_small_feather[llll][d].INDEX_NR = find_index_____(SIZE_OBJECT,state_result_worm_ventral_feather,llll,d);
INDEX_NRmores = state_result_worm_ventral_feathers[lllls][d].INDEX_NR;
 

		 

 

				 if(fabs(F_totals[0][INDEX_NRmores]) > 6  )
	 				  F_totals[0][INDEX_NRmores] =0;
 
		 		 if(fabs(F_totals[1][INDEX_NRmores])  > 6 )
					  F_totals[1][INDEX_NRmores] =0;

			 	 if(fabs(F_totals[2][INDEX_NRmores])  > 6 )
					  F_totals[2][INDEX_NRmores] =0;
			

 				if(fabs(F_totals[0][INDEX_NRs]) > 6  )
	 				  F_totals[0][INDEX_NRs] =0;
 
		 		 if(fabs(F_totals[1][INDEX_NRs])  > 6 )
					  F_totals[1][INDEX_NRs] =0;

			 	 if(fabs(F_totals[2][INDEX_NRs])  > 6 )
					  F_totals[2][INDEX_NRs] =0;
			
		springVectors->pos_new_x = state_result_worm_ventral_small[INDEX_NRs]->pos_new_x - state_result_worm_ventral_small[INDEX_NRmores]->pos_new_x;
		springVectors->pos_new_y = state_result_worm_ventral_small[INDEX_NRs]->pos_new_y - state_result_worm_ventral_small[INDEX_NRmores]->pos_new_y;
		springVectors->pos_new_z = state_result_worm_ventral_small[INDEX_NRs]->pos_new_z - state_result_worm_ventral_small[INDEX_NRmores]->pos_new_z;
 			

		float r = length(state_result_worm_ventral_small[INDEX_NRs],state_result_worm_ventral_small[INDEX_NRmores]);

		if ( r != 0  &&  r < 1.1  && r > -1.1   )
		{	
	//	 printf("1totalneigbours[llll]  %d :: number %d :: index_____ = %d\n",totalneigbours[llll],llll,INDEX_NRmore);	
			F_totals[0][INDEX_NRs] +=( springVectors->pos_new_x  /  r ) * ( r - springlength ) * ( -springConstant );
			F_totals[1][INDEX_NRs] += ( springVectors->pos_new_y /  r ) * ( r - springlength ) * ( -springConstant );
			F_totals[2][INDEX_NRs] +=( springVectors->pos_new_z  /  r ) * ( r - springlength ) * ( -springConstant );

			F_totals[0][INDEX_NRmores] +=( springVectors->pos_new_x  / r ) * ( r - springlength ) * ( -springConstant );
			F_totals[1][INDEX_NRmores] += ( springVectors->pos_new_y  / r ) * ( r - springlength) * ( -springConstant );
			F_totals[2][INDEX_NRmores] +=( springVectors->pos_new_z  / r ) * ( r - springlength ) * ( -springConstant );

		F_totals[0][INDEX_NRs] +=-(  state_result_worm_ventral_small[INDEX_NRs]->vel_new_x - state_result_worm_ventral_small[INDEX_NRmores]->vel_new_x ) * frictionConstant;
 		F_totals[1][INDEX_NRs] += -( state_result_worm_ventral_small[INDEX_NRs]->vel_new_y- state_result_worm_ventral_small[INDEX_NRmores]->vel_new_y ) * frictionConstant;
 		F_totals[2][INDEX_NRs] += -(  state_result_worm_ventral_small[INDEX_NRs]->vel_new_z - state_result_worm_ventral_small[INDEX_NRmores ]->vel_new_z ) * frictionConstant;


		F_totals[0][INDEX_NRmores] +=-(  state_result_worm_ventral_small[INDEX_NRs]->vel_new_x - state_result_worm_ventral_small[INDEX_NRmores]->vel_new_x ) * frictionConstant;
 		F_totals[1][INDEX_NRmores] += -( state_result_worm_ventral_small[INDEX_NRs]->vel_new_y- state_result_worm_ventral_small[INDEX_NRmores]->vel_new_y ) * frictionConstant;
 		F_totals[2][INDEX_NRmores] += -(  state_result_worm_ventral_small[INDEX_NRs]->vel_new_z - state_result_worm_ventral_small[INDEX_NRmores ]->vel_new_z ) * frictionConstant;

		state_result_worm_ventral_small[INDEX_NRmores]->vel_new_x = state_result_worm_ventral_small[INDEX_NRmores]->vel_new_x*0.995 ;
		
		state_result_worm_ventral_small[INDEX_NRmores]->vel_new_y = state_result_worm_ventral_small[INDEX_NRmores]->vel_new_y*0.995 ;

		state_result_worm_ventral_small[INDEX_NRmores]->vel_new_z = state_result_worm_ventral_small[INDEX_NRmores]->vel_new_z*0.995 ;

		state_result_worm_ventral_small[INDEX_NRs]->vel_new_x = state_result_worm_ventral_small[INDEX_NRs]->vel_new_x*0.995 ;
		
		state_result_worm_ventral_small[INDEX_NRs]->vel_new_y = state_result_worm_ventral_small[INDEX_NRs]->vel_new_y*0.995 ;
		state_result_worm_ventral_small[INDEX_NRs]->vel_new_z = state_result_worm_ventral_small[INDEX_NRs]->vel_new_z*0.995 ;

		state_result_worm_ventral_small[INDEX_NRs]->force_sign =  -1;
		state_result_worm_ventral_small[INDEX_NRs]    =  worm_ventral(  state_result_worm_ventral_small[INDEX_NRs] ,1) ;

		state_result_worm_ventral_small[INDEX_NRmores]->force_sign =   1; ; 
		state_result_worm_ventral_small[INDEX_NRmores]    =  worm_ventral(  state_result_worm_ventral_small[INDEX_NRmores] ,1);
		}
else
{

		state_result_worm_ventral_small[INDEX_NRs]->force_sign =  -1;
		state_result_worm_ventral_small[INDEX_NRs]    =  worm_ventral(  state_result_worm_ventral_small[INDEX_NRs] ,1) ;

		state_result_worm_ventral_small[INDEX_NRmores]->force_sign =   1; ; 
		state_result_worm_ventral_small[INDEX_NRmores]    =  worm_ventral(  state_result_worm_ventral_small[INDEX_NRmores] ,1);

}







if(state_result_worm_ventral_small[INDEX_NRs]->pos_new_y < -10)
			{
			 state_result_worm_ventral_small[INDEX_NRs]->pos_new_y = -10;
			 state_result_worm_ventral_small[INDEX_NRs]->vel_new_y = -state_result_worm_ventral_small[INDEX_NRs]->vel_new_y*0.00995;
 
			}	
		if(state_result_worm_ventral_small[INDEX_NRmores]->pos_new_y < -10)
			{
				 state_result_worm_ventral_small[INDEX_NRmores]->pos_new_y = -10;
			 state_result_worm_ventral_small[INDEX_NRmores]->vel_new_y = -state_result_worm_ventral_small[INDEX_NRmores]->vel_new_y*0.00995;
 
			}






//interaktioner mellan 2 objekt!

if(lllls < SIZE_OBJECT)
{	 
 if( k <=totalneigbours[lllls])
	{
	  INDEX_NRmore =  (state_result_worm_ventral_feather[lllls][k].INDEX_NR);
 if(fabs(F_total[0][INDEX_NRmore]) > 6  )
	 				 	  F_total[0][INDEX_NRmore] =0;
 
		 		 if(fabs(F_total[1][INDEX_NRmore])  > 6 )
						F_total[1][INDEX_NRmore] =0;

			 	 if(fabs(F_total[2][INDEX_NRmore])  > 6 )
						F_total[2][INDEX_NRmore] =0;
springVectors->pos_new_x = state_result_worm_ventral[INDEX_NRmore]->pos_new_x - state_result_worm_ventral_small[INDEX_NRs]->pos_new_x;
		springVectors->pos_new_y = state_result_worm_ventral[INDEX_NRmore]->pos_new_y - state_result_worm_ventral_small[INDEX_NRs]->pos_new_y;
		springVectors->pos_new_z = state_result_worm_ventral[INDEX_NRmore]->pos_new_z - state_result_worm_ventral_small[INDEX_NRs]->pos_new_z;
 

		  r = length(state_result_worm_ventral[INDEX_NRmore],state_result_worm_ventral_small[INDEX_NRs]);

		if ( r != 0  &&   r < 0.1 && r > -0.1    )
		{			
			F_total[0][INDEX_NRmore] +=( springVectors->pos_new_x  /  r ) * ( r - springlength ) * ( -springConstant );
			F_total[1][INDEX_NRmore] += ( springVectors->pos_new_y /  r ) * ( r - springlength ) * ( -springConstant );
			F_total[2][INDEX_NRmore] +=( springVectors->pos_new_z  /  r ) * ( r - springlength ) * ( -springConstant );
	 
			F_totals[0][INDEX_NRs] +=( springVectors->pos_new_x  / r ) * ( r - springlength  ) * ( -springConstant );
			F_totals[1][INDEX_NRs] += ( springVectors->pos_new_y  / r ) * ( r - springlength ) * ( -springConstant );
			F_totals[2][INDEX_NRs] +=( springVectors->pos_new_z  / r ) * ( r - springlength ) * ( -springConstant );

F_total[0][INDEX_NRmore] +=-(  state_result_worm_ventral[INDEX_NRmore]->vel_new_x - state_result_worm_ventral_small[INDEX_NRs]->vel_new_x ) * frictionConstant;
 		F_total[1][INDEX_NRmore] += -( state_result_worm_ventral[INDEX_NRmore]->vel_new_y- state_result_worm_ventral_small[INDEX_NRs]->vel_new_y ) * frictionConstant;
 		F_total[2][INDEX_NRmore] += -(  state_result_worm_ventral[INDEX_NRmore]->vel_new_z - state_result_worm_ventral_small[INDEX_NRs ]->vel_new_z ) * frictionConstant;


		F_totals[0][INDEX_NRs] +=-(  state_result_worm_ventral[INDEX_NRmore]->vel_new_x - state_result_worm_ventral_small[INDEX_NRs]->vel_new_x ) * frictionConstant;
 		F_totals[1][INDEX_NRs] += -( state_result_worm_ventral[INDEX_NRmore]->vel_new_y- state_result_worm_ventral_small[INDEX_NRs]->vel_new_y ) * frictionConstant;
 		F_totals[2][INDEX_NRs] += -(  state_result_worm_ventral[INDEX_NRmore]->vel_new_z - state_result_worm_ventral_small[INDEX_NRs ]->vel_new_z ) * frictionConstant;
		state_result_worm_ventral_small[INDEX_NRs]->vel_new_x = state_result_worm_ventral_small[INDEX_NRs]->vel_new_x*0.999 ;
		
		state_result_worm_ventral_small[INDEX_NRs]->vel_new_y = state_result_worm_ventral_small[INDEX_NRs]->vel_new_y*0.999 ;

		state_result_worm_ventral_small[INDEX_NRs]->vel_new_z = state_result_worm_ventral_small[INDEX_NRs]->vel_new_z*0.999 ;

		state_result_worm_ventral[INDEX_NRmore]->vel_new_x = state_result_worm_ventral[INDEX_NRmore]->vel_new_x*0.999 ;
		
		state_result_worm_ventral[INDEX_NRmore]->vel_new_y = state_result_worm_ventral[INDEX_NRmore]->vel_new_y*0.999 ;
		state_result_worm_ventral[INDEX_NRmore]->vel_new_z = state_result_worm_ventral[INDEX_NRmore]->vel_new_z*0.999 ;
	state_result_worm_ventral[INDEX_NRmore]->force_sign =  -1;
		state_result_worm_ventral[INDEX_NRmore]    =  worm_ventral(  state_result_worm_ventral[INDEX_NRmore] ,1) ;

		state_result_worm_ventral_small[INDEX_NRs]->force_sign =   1; ; 
		state_result_worm_ventral_small[INDEX_NRs]    =  worm_ventral(  state_result_worm_ventral_small[INDEX_NRs] ,1);

		}
else
{

	state_result_worm_ventral[INDEX_NRmore]->force_sign =  -1;
		state_result_worm_ventral[INDEX_NRmore]    =  worm_ventral(  state_result_worm_ventral[INDEX_NRmore] ,1) ;

		state_result_worm_ventral_small[INDEX_NRs]->force_sign =   1; ; 
		state_result_worm_ventral_small[INDEX_NRs]    =  worm_ventral(  state_result_worm_ventral_small[INDEX_NRs] ,1);

}

		

	

if(state_result_worm_ventral[INDEX_NRmore]->pos_new_y < -10)
			{
			 state_result_worm_ventral[INDEX_NRmore]->pos_new_y = -10;
			 state_result_worm_ventral[INDEX_NRmore]->vel_new_y = -state_result_worm_ventral[INDEX_NRmore]->vel_new_y*0.00995;
 
			}	
		if(state_result_worm_ventral_small[INDEX_NRs]->pos_new_y < -10)
			{
				 state_result_worm_ventral_small[INDEX_NRs]->pos_new_y = -10;
			 state_result_worm_ventral_small[INDEX_NRs]->vel_new_y = -state_result_worm_ventral_small[INDEX_NRs]->vel_new_y*0.00995;
 
			}
}
 
}



	//	 printf("f[0] = %.10f:%.10f%.10f \n", state_result_worm_ventral_small[INDEX_NRmore]->pos_new_x,state_result_worm_ventral_small[INDEX_NRmore]->pos_new_y,state_result_worm_ventral_small[INDEX_NRmore]->pos_new_z);


	 
	 
 	} 
 	
//}
		 
 }	
//}
		 
 }		
				 if(fabs(F_totals[0][lllls]) > 6  )
	 				 F_totals[0][lllls] =0;
 
		 		 if(fabs(F_totals[1][lllls])  > 6 )
					 F_totals[1][lllls] =0;

			 	 if(fabs(F_totals[2][lllls])  > 6 )
					 F_totals[2][lllls] =0;
			// printf("FORCE(x,y,z) = %.50f:%.50f%.50f \n", fabs(F_total[0][INDEX_NR]),fabs(F_total[1][llll]),fabs(F_total[2][INDEX_NRmore]));
			
			if(lllls >= SIZE_OBJECTs -1  )
			{
				//feather_once= WORK;
	   			lllls =0;
				int var;
			//	for(var = 0; var <  3 -1 ; var++)
			//	{
				//	state_result_worm_ventral_small[var]->force_sign = 0;
 	
				//}
			}
}
lllls++;
}
