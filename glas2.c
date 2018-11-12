#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>
#include <jerror.h>
#include "noise.h"
int WHAT_OBJECT;
int WHAT_OBJECT_NOW;
struct state_vector     ***  worm_ventral(struct state_vector     ***   next_state, int num ,int index, int what1, int what2);
#define		RESOLUTION 1
int TOTAL_OBJECTS = 0;
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
 
#define SIZE_OBJECT 7936 //126976
#define SIZE_OBJECTs 7936 //126976
float springlength =  0.004/4.0;
#define AIR_FRICTION 0.9959999
#define FIRST 1
#define WORK 0
float fovy = 45.0;
float dNear = 100;
float dFar = 2000;
//double **F_total ;
//double** F_totals ;

float ***  F_total_next ;
float *** F_totals_next;
int **totalneigbours ;
int totalneigbourss[SIZE_OBJECTs*104][10]; 
float     vertexpoint_g_ventral[SIZE_OBJECT][3];
struct state_vector * springVector;
struct state_vector * springVectors;
struct state_vector *** feather_result ; 
 
struct state_vector  *** state_result ;  
 
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

float frictionConstant =    0.422  ;

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
int findnearestpoint(int points,struct state_vector *** worm , int num,int howmany);
void display  (void);
 
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
  
void taskworm(int lllll, int **totalneigbours__ ,struct state_vector *** feather_result__ ,  struct state_vector  ***state_result__ , int WHAT_OBJECT,float ***  F_total_next_,float ***  F_totals_next_)
{
 llll = lllll;
int k;
 
for(k = 0; k <= totalneigbours__[lllll][WHAT_OBJECT_NOW]; k++)
{	// m*a = -k*s
 	//10*a= -k * 0.004 solve k
////printf("taskworm 1\n");
if(acc1once == 0)
	{
	if(fabs(acc1) > 0)
		{		
		springConstant =   14.0/1 *1;
//	printf("springConstant = %.10f \n", springConstant);
		 frictionConstant =      0.01*1;
	}
	}
acc1once = 0;	
 
	INDEX_NR =  (feather_result__[lllll][k][WHAT_OBJECT_NOW].INDEX_NR);
	//printf("taskworm 2\n"); 
//if(  INDEX_NR > 0 && INDEX_NR < 1000000  )
//{
	//if(state_result_worm_ventral[INDEX_NR]->force_sign != -1 && state_result_worm_ventral[INDEX_NR]->force_sign != 1 && INDEX_NR >0)
	//{
		 if(feather_once == FIRST  )
		{	
 //printf("taskworm 2.5\n");
						 if(fabs(F_total_next_[0][INDEX_NR][WHAT_OBJECT_NOW]) >  6  )
	 				 	  F_total_next_[0][INDEX_NR][WHAT_OBJECT_NOW] =0;
 
		 		 if(fabs(F_total_next_[1][INDEX_NR][WHAT_OBJECT_NOW])  >  6 )
						F_total_next_[1][INDEX_NR][WHAT_OBJECT_NOW] =0;

			 	 if(fabs(F_total_next_[2][INDEX_NR][WHAT_OBJECT_NOW])  >  6 )
						F_total_next_[2][INDEX_NR][WHAT_OBJECT_NOW] =0;
 
  //printf("2totalneigbours__[lllll]  %d :: number %d :: index_____ = %d\n",totalneigbours__[lllll],lllll,INDEX_NR);
		springVector->pos_new_x = state_result__[INDEX_NR][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_x - state_result__[lllll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_x;
//printf("taskworm 2.8\n");
		springVector->pos_new_y = state_result__[INDEX_NR][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_y - state_result__[lllll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_y;
		springVector->pos_new_z = state_result__[INDEX_NR][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_z - state_result__[lllll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_z;
 
//printf("taskworm 3\n");
		float r = length(state_result__[INDEX_NR][WHAT_OBJECT_NOW],state_result__[lllll][WHAT_OBJECT_NOW]);
 
		if ( r != 0  &&r < 1.1  && r > -1.1   )
		{
	 		
			F_total_next_[0][INDEX_NR][WHAT_OBJECT_NOW] +=( springVector->pos_new_x  /  r ) * ( r - springlength ) * ( -springConstant );
			F_total_next_[1][INDEX_NR][WHAT_OBJECT_NOW] += ( springVector->pos_new_y /  r ) * ( r - springlength ) * ( -springConstant );
			F_total_next_[2][INDEX_NR][WHAT_OBJECT_NOW] +=( springVector->pos_new_z  /  r ) * ( r - springlength ) * ( -springConstant );

			F_total_next_[0][lllll][WHAT_OBJECT_NOW] +=( springVector->pos_new_x  / r ) * ( r - springlength ) * ( -springConstant );
			F_total_next_[1][lllll][WHAT_OBJECT_NOW] += ( springVector->pos_new_y  / r ) * ( r - springlength) * ( -springConstant );
			F_total_next_[2][lllll][WHAT_OBJECT_NOW] +=( springVector->pos_new_z  / r ) * ( r - springlength ) * ( -springConstant );

		F_total_next_[0][INDEX_NR][WHAT_OBJECT_NOW] +=-(  state_result__[INDEX_NR][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_x - state_result__[lllll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_x ) * frictionConstant;
 		F_total_next_[1][INDEX_NR][WHAT_OBJECT_NOW] += -( state_result__[INDEX_NR][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_y- state_result__[lllll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_y ) * frictionConstant;
 		F_total_next_[2][INDEX_NR][WHAT_OBJECT_NOW] += -(  state_result__[INDEX_NR][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_z - state_result__[lllll ][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_z ) * frictionConstant;


		F_total_next_[0][lllll][WHAT_OBJECT_NOW] +=-(  state_result__[INDEX_NR][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_x - state_result__[lllll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_x ) * frictionConstant;
 		F_total_next_[1][lllll][WHAT_OBJECT_NOW] += -( state_result__[INDEX_NR][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_y- state_result__[lllll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_y ) * frictionConstant;
 		F_total_next_[2][lllll][WHAT_OBJECT_NOW] += -(  state_result__[INDEX_NR][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_z - state_result__[lllll ][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_z ) * frictionConstant;
		state_result__[lllll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_x = state_result__[lllll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_x*0.995 ;
		
		state_result__[lllll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_y = state_result__[lllll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_y*0.995 ;

		state_result__[lllll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_z = state_result__[lllll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_z*0.995 ;

		state_result__[INDEX_NR][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_x = state_result__[INDEX_NR][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_x*0.995 ;
		
		state_result__[INDEX_NR][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_y = state_result__[INDEX_NR][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_y*0.995 ;
		state_result__[INDEX_NR][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_z = state_result__[INDEX_NR][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_z*0.995 ;
BIGONE = 1;
		state_result__[INDEX_NR][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].force_sign =  -1;

		state_result__     =  worm_ventral(  state_result__,1,INDEX_NR,WHAT_OBJECT,WHAT_OBJECT_NOW   ) ;

		state_result__[lllll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].force_sign =   1; ; 
		state_result__     =  worm_ventral(  state_result__,1,lllll,WHAT_OBJECT,WHAT_OBJECT_NOW  );
		}
else
{
 
		state_result__[INDEX_NR][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].force_sign =  -1;
		state_result__     =  worm_ventral(  state_result__  ,1,INDEX_NR,WHAT_OBJECT,WHAT_OBJECT_NOW) ;

		state_result__[lllll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].force_sign =   1; ; 
		state_result__    =  worm_ventral(  state_result__,1 ,lllll,WHAT_OBJECT,WHAT_OBJECT_NOW   );

}
		

//printf("taskworm 4\n");
		
if(state_result__[INDEX_NR][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_y < -2)
			{
			 state_result__[INDEX_NR][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_y = -2;
			 state_result__[INDEX_NR][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_y = -state_result__[INDEX_NR][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_y*0.00995;
 
			}	
		if(state_result__[lllll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_y < -2)
			{
				 state_result__[lllll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_y = -2;
			 state_result__[lllll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_y = -state_result__[lllll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_y*0.00995;
 
			}
//printf("taskworm 5\n");
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//interaktioner mellan 2 objekt!

if(lllll < SIZE_OBJECTs)
{
int s;
int d;
//2 v 1, 1 v 0 , 2 v 0           1 v 2, 1 v 3 , 1 v 4           2 v 3, 3 v 1, 4 v 2
for(s = 0; s < TOTAL_OBJECTS; s++)
{
for(d =1 ; d < TOTAL_OBJECTS; d++)
{	
if(d != s)
{
 
 if( k <=totalneigbours__[lllll][s])
	{
	  INDEX_NRs =  (feather_result__[lllll][k][s].INDEX_NR);
 

 if(fabs(F_totals_next_[0][lllll][d]) >  6  )
  F_totals_next_[0][lllll][d] =0;
 
		 		 if(fabs(F_totals_next_[1][lllll][d])  >  6 )
						F_totals_next_[1][lllll][d] =0;

			 	 if(fabs(F_totals_next_[2][lllll][d])  >  6 )
						F_totals_next_[2][lllll][d] =0;


 if(fabs(F_total_next_[0][INDEX_NRs][s]) >  6  )
  F_total_next_[0][INDEX_NRs][s] =0;
 
		 		 if(fabs(F_total_next_[1][INDEX_NRs][s])  >  6 )
						F_total_next_[1][INDEX_NRs][s] =0;

			 	 if(fabs(F_total_next_[2][INDEX_NRs][s])  >  6 )
						F_total_next_[2][INDEX_NRs][s] =0;
springVector->pos_new_x = state_result__[INDEX_NRs][d][WHAT_OBJECT_NOW].pos_new_x - state_result__[lllll][s][WHAT_OBJECT_NOW].pos_new_x;
		springVector->pos_new_y = state_result__[INDEX_NRs][d][WHAT_OBJECT_NOW].pos_new_y - state_result__[lllll][s][WHAT_OBJECT_NOW].pos_new_y;
		springVector->pos_new_z = state_result__[INDEX_NRs][d][WHAT_OBJECT_NOW].pos_new_z - state_result__[lllll][s][WHAT_OBJECT_NOW].pos_new_z;
 //printf("taskworm 6\n");

		  r = length(state_result__[INDEX_NRs][d],state_result__[lllll][s]);

		if ( r != 0  &&   r < 0.2   && r > -0.2   )
		{			
			F_totals_next_[0][INDEX_NRs][d] +=( springVector->pos_new_x  /  r ) * ( r - springlength ) * ( -springConstant );
			F_totals_next_[1][INDEX_NRs][d] += ( springVector->pos_new_y /  r ) * ( r - springlength ) * ( -springConstant );
			F_totals_next_[2][INDEX_NRs][d] +=( springVector->pos_new_z  /  r ) * ( r - springlength ) * ( -springConstant );
	//  printf("r = %f\n",r);
			F_total_next_[0][lllll][s] +=( springVector->pos_new_x  / r ) * ( r - springlength ) * ( -springConstant );
			F_total_next_[1][lllll][s] += ( springVector->pos_new_y  / r ) * ( r - springlength) * ( -springConstant );
			F_total_next_[2][lllll][s] +=( springVector->pos_new_z  / r ) * ( r - springlength ) * ( -springConstant );

F_totals_next_[0][INDEX_NRs][d] +=-(  state_result__[INDEX_NRs][d][WHAT_OBJECT_NOW].vel_new_x - state_result__[lllll][s][WHAT_OBJECT_NOW].vel_new_x ) * frictionConstant;
 		F_totals_next_[1][INDEX_NRs][d] += -( state_result__[INDEX_NRs][d][WHAT_OBJECT_NOW].vel_new_y- state_result__[lllll][s][WHAT_OBJECT_NOW].vel_new_y ) * frictionConstant;
 		F_totals_next_[2][INDEX_NRs][d] += -(  state_result__[INDEX_NRs][d][WHAT_OBJECT_NOW].vel_new_z - state_result__[lllll ][s][WHAT_OBJECT_NOW].vel_new_z ) * frictionConstant;

//printf("taskworm 7\n");
		F_total_next_[0][lllll][s] +=-(  state_result__[INDEX_NRs][d][WHAT_OBJECT_NOW].vel_new_x - state_result__[lllll][s][WHAT_OBJECT_NOW].vel_new_x ) * frictionConstant;
 		F_total_next_[1][lllll][s] += -( state_result__[INDEX_NRs][d][WHAT_OBJECT_NOW].vel_new_y- state_result__[lllll][s][WHAT_OBJECT_NOW].vel_new_y ) * frictionConstant;
 		F_total_next_[2][lllll][s] += -(  state_result__[INDEX_NRs][d][WHAT_OBJECT_NOW].vel_new_z - state_result__[lllll ][s][WHAT_OBJECT_NOW].vel_new_z ) * frictionConstant;
	 	state_result__[lllll][s][WHAT_OBJECT_NOW].vel_new_x = state_result__[lllll][s][WHAT_OBJECT_NOW].vel_new_x*0.995 ;
		
		state_result__[lllll][s][WHAT_OBJECT_NOW].vel_new_y = state_result__[lllll][s][WHAT_OBJECT_NOW].vel_new_y*0.995 ;

		state_result__[lllll][s][WHAT_OBJECT_NOW].vel_new_z = state_result__[lllll][s][WHAT_OBJECT_NOW].vel_new_z*0.995 ;

		state_result__[INDEX_NRs][d][WHAT_OBJECT_NOW].vel_new_x = state_result__[INDEX_NRs][d][WHAT_OBJECT_NOW].vel_new_x*0.995 ;
		
		state_result__[INDEX_NRs][d][WHAT_OBJECT_NOW].vel_new_y = state_result__[INDEX_NRs][d][WHAT_OBJECT_NOW].vel_new_y*0.995 ;
		state_result__[INDEX_NRs][d][WHAT_OBJECT_NOW].vel_new_z = state_result__[INDEX_NRs][d][WHAT_OBJECT_NOW].vel_new_z*0.995 ; 
BIGONE = 0;
 WHAT_OBJECT_NOW = d;
//printf("taskworm 8\n");
		state_result__[INDEX_NRs][d][WHAT_OBJECT_NOW].force_sign =  -1;
		state_result__    =  worm_ventral(  state_result__,1,INDEX_NRs,d,WHAT_OBJECT_NOW  ) ;
BIGONE = 1;
 WHAT_OBJECT_NOW = s;
		state_result__[lllll][s][WHAT_OBJECT_NOW].force_sign =   1; ; 
		state_result__     =  worm_ventral(  state_result__,1,lllll,s,WHAT_OBJECT_NOW  );
 WHAT_OBJECT_NOW = WHAT_OBJECT;
//printf("taskworm 9\n");
		}
else
{
 int s;
int d;
//2 v 1, 1 v 0 , 2 v 0           1 v 2, 1 v 3 , 1 v 4           2 v 3, 3 v 1, 4 v 2
for(s = 0; s < TOTAL_OBJECTS; s++)
{
for(d =1 ; d < TOTAL_OBJECTS; d++)
{
if(d != s)
{
 //printf("taskworm 10\n");
 WHAT_OBJECT_NOW = d;
 BIGONE = 0;
		state_result__[INDEX_NRs][d][WHAT_OBJECT_NOW].force_sign =  -1;
		state_result__     =  worm_ventral(  state_result__,1,INDEX_NRs,d,WHAT_OBJECT_NOW  ) ;
BIGONE = 1;
 WHAT_OBJECT_NOW = s;
		state_result__[lllll][s][WHAT_OBJECT_NOW].force_sign =   1; ; 
		state_result__   =  worm_ventral(  state_result__ ,1,lllll,s  ,WHAT_OBJECT_NOW);
 WHAT_OBJECT_NOW = WHAT_OBJECT;
//printf("taskworm 11\n");

}
}
}	
}

if(state_result__[INDEX_NRs][d][WHAT_OBJECT_NOW].pos_new_y < -2)
			{
			 state_result__[INDEX_NRs][d][WHAT_OBJECT_NOW].pos_new_y = -2;
			 state_result__[INDEX_NRs][d][WHAT_OBJECT_NOW].vel_new_y = -state_result__[INDEX_NRs][d][WHAT_OBJECT_NOW].vel_new_y*0.00995;
 
			}	
		if(state_result__[lllll][s][WHAT_OBJECT_NOW].pos_new_y < -2)
			{
				 state_result__[lllll][s][WHAT_OBJECT_NOW].pos_new_y = -2;
			 state_result__[lllll][s][WHAT_OBJECT_NOW].vel_new_y = -state_result__[lllll][s][WHAT_OBJECT_NOW].vel_new_y*0.00995;
 
			}
}
 

}
}
}

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	for(int d = 0; d <= totalneigbours__[lllll][WHAT_OBJECT_NOW]; d++)
	{	 
		
	 

 
   // state_result_worm_ventral_feather[lllll][d].INDEX_NR = find_index_____(SIZE_OBJECT,state_result_worm_ventral_feather,lllll,d);
INDEX_NRmore = feather_result__[lllll][d][WHAT_OBJECT_NOW].INDEX_NR;
 

		 

 
 //printf(" d=%d\n", d);
				 if(fabs(F_total_next_[0][INDEX_NRmore][WHAT_OBJECT_NOW]) >  6  )
	 				  F_total_next_[0][INDEX_NRmore][WHAT_OBJECT_NOW] =0;
 
		 		 if(fabs(F_total_next_[1][INDEX_NRmore][WHAT_OBJECT_NOW])  >  6 )
					  F_total_next_[1][INDEX_NRmore][WHAT_OBJECT_NOW] =0;

			 	 if(fabs(F_total_next_[2][INDEX_NRmore][WHAT_OBJECT_NOW])  >  6 )
					  F_total_next_[2][INDEX_NRmore][WHAT_OBJECT_NOW] =0;
			

 				if(fabs(F_total_next_[0][INDEX_NR][WHAT_OBJECT_NOW]) >  6  )
	 				  F_total_next_[0][INDEX_NR][WHAT_OBJECT_NOW] =0;
 
		 		 if(fabs(F_total_next_[1][INDEX_NR][WHAT_OBJECT_NOW])  >  6 )
					  F_total_next_[1][INDEX_NR][WHAT_OBJECT_NOW] =0;

			 	 if(fabs(F_total_next_[2][INDEX_NR][WHAT_OBJECT_NOW])  >  6 )
					  F_total_next_[2][INDEX_NR][WHAT_OBJECT_NOW] =0;
			
		springVector->pos_new_x = state_result__[INDEX_NR][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_x - state_result__[INDEX_NRmore][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_x;
		springVector->pos_new_y = state_result__[INDEX_NR][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_y - state_result__[INDEX_NRmore][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_y;
		springVector->pos_new_z = state_result__[INDEX_NR][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_z - state_result__[INDEX_NRmore][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_z;
 			
//printf("taskworm 13\n");
		float r = length(state_result__[INDEX_NR][WHAT_OBJECT_NOW],state_result__[INDEX_NRmore][WHAT_OBJECT_NOW]);

		if ( r != 0 && r < 1.1  && r > -1.1    )
		{	
	//	 printf("1totalneigbours[lllll]  %d :: number %d :: index_____ = %d\n",totalneigbours[lllll],lllll,INDEX_NRmore);	
			F_total_next_[0][INDEX_NR][WHAT_OBJECT_NOW] +=( springVector->pos_new_x  /  r ) * ( r - springlength ) * ( -springConstant );
			F_total_next_[1][INDEX_NR][WHAT_OBJECT_NOW] += ( springVector->pos_new_y /  r ) * ( r - springlength ) * ( -springConstant );
			F_total_next_[2][INDEX_NR][WHAT_OBJECT_NOW] +=( springVector->pos_new_z  /  r ) * ( r - springlength ) * ( -springConstant );

			F_total_next_[0][INDEX_NRmore][WHAT_OBJECT_NOW] +=( springVector->pos_new_x  / r ) * ( r - springlength ) * ( -springConstant );
			F_total_next_[1][INDEX_NRmore][WHAT_OBJECT_NOW] += ( springVector->pos_new_y  / r ) * ( r - springlength) * ( -springConstant );
			F_total_next_[2][INDEX_NRmore][WHAT_OBJECT_NOW] +=( springVector->pos_new_z  / r ) * ( r - springlength ) * ( -springConstant );

		F_total_next_[0][INDEX_NR][WHAT_OBJECT_NOW] +=-(  state_result__[INDEX_NR][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_x - state_result__[INDEX_NRmore][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_x ) * frictionConstant;
 		F_total_next_[1][INDEX_NR][WHAT_OBJECT_NOW] += -( state_result__[INDEX_NR][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_y- state_result__[INDEX_NRmore][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_y ) * frictionConstant;
 		F_total_next_[2][INDEX_NR][WHAT_OBJECT_NOW] += -(  state_result__[INDEX_NR][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_z - state_result__[INDEX_NRmore ][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_z ) * frictionConstant;


		F_total_next_[0][INDEX_NRmore][WHAT_OBJECT_NOW] +=-(  state_result__[INDEX_NR][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_x - state_result__[INDEX_NRmore][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_x ) * frictionConstant;
 		F_total_next_[1][INDEX_NRmore][WHAT_OBJECT_NOW] += -( state_result__[INDEX_NR][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_y- state_result__[INDEX_NRmore][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_y ) * frictionConstant;
 		F_total_next_[2][INDEX_NRmore][WHAT_OBJECT_NOW] += -(  state_result__[INDEX_NR][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_z - state_result__[INDEX_NRmore ][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_z ) * frictionConstant;

		state_result__[INDEX_NRmore][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_x = state_result__[INDEX_NRmore][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_x*0.995 ;
		//printf("taskworm 14\n");
		state_result__[INDEX_NRmore][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_y = state_result__[INDEX_NRmore][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_y*0.995 ;

		state_result__[INDEX_NRmore][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_z = state_result__[INDEX_NRmore][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_z*0.995 ;

		state_result__[INDEX_NR][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_x = state_result__[INDEX_NR][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_x*0.995 ;
		
		state_result__[INDEX_NR][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_y = state_result__[INDEX_NR][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_y*0.995 ;
		state_result__[INDEX_NR][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_z = state_result__[INDEX_NR][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_z*0.995 ;
BIGONE = 1;

		state_result__[INDEX_NR][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].force_sign =  -1;
		state_result__     =  worm_ventral(  state_result__,1,INDEX_NR,WHAT_OBJECT,WHAT_OBJECT_NOW  ) ;
// printf("taskworm 15\n");
		state_result__[INDEX_NRmore][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].force_sign =   1; ; 
		state_result__    =  worm_ventral(  state_result__,1,INDEX_NRmore,WHAT_OBJECT,WHAT_OBJECT_NOW );

		}
else
{
 BIGONE = 1;

		state_result__[INDEX_NR][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].force_sign =  -1;
		state_result__     =  worm_ventral(  state_result__,1,INDEX_NR,WHAT_OBJECT,WHAT_OBJECT_NOW   ) ;
 //printf("taskworm 16\n");
		state_result__[INDEX_NRmore][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].force_sign =   1; ; 
		state_result__    =  worm_ventral(  state_result__,1,INDEX_NRmore,WHAT_OBJECT,WHAT_OBJECT_NOW );

}



//printf("taskworm 17\n");
if(state_result__[INDEX_NR][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_y < -2)
			{
			 state_result__[INDEX_NR][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_y = -2;
			 state_result__[INDEX_NR][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_y = -state_result__[INDEX_NR][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_y*0.00995;
 
			}	
		if(state_result__[INDEX_NRmore][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_y < -2)
			{
				 state_result__[INDEX_NRmore][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_y = -2;
			 state_result__[INDEX_NRmore][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_y = -state_result__[INDEX_NRmore][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_y*0.00995;
 
			}
	//	 printf("f[0] = %.10f:%.10f%.10f \n", state_result__[INDEX_NRmore][WHAT_OBJECT_NOW].pos_new_x,state_result__[INDEX_NRmore][WHAT_OBJECT_NOW].pos_new_y,state_result__[INDEX_NRmore][WHAT_OBJECT_NOW].pos_new_z);


	 ////////////////////////////////////////////////////////////////////////////////////////////////////
//interaktioner mellan 2 objekt!








//printf("taskworm 18\n");
if(lllll < SIZE_OBJECTs)
{
 int s;
int d;
//2 v 1, 1 v 0 , 2 v 0           1 v 2, 1 v 3 , 1 v 4           2 v 3, 3 v 1, 4 v 2
for(s = 0; s < TOTAL_OBJECTS ; s++)
{
for(d =1 ; d < TOTAL_OBJECTS; d++)
{
if(d != s)
{
 if( k <=totalneigbours__[lllll][s])
	{
INDEX_NRmores = feather_result__[lllll][k][d].INDEX_NR;


			if(fabs(F_total_next_[0][lllll][s]) >  6  )
	 				  F_total_next_[0][lllll][s] =0;
 
		 		 if(fabs(F_total_next_[1][lllll][s])  >  6 )
					  F_total_next_[1][lllll][s] =0;

			 	 if(fabs(F_total_next_[2][lllll][s])  >  6 )
					  F_total_next_[2][lllll][s] =0;


//printf("taskworm 19\n");
			if(fabs(F_totals_next_[0][INDEX_NRmores][d]) >  6  )
	 				  F_totals_next_[0][INDEX_NRmores][d] =0;
 
		 		 if(fabs(F_totals_next_[1][INDEX_NRmores][d])  >  6 )
					  F_totals_next_[1][INDEX_NRmores][d] =0;

			 	 if(fabs(F_totals_next_[2][INDEX_NRmores][d])  >  6 )
					  F_totals_next_[2][INDEX_NRmores][d] =0;


springVector->pos_new_x = state_result__[INDEX_NR][s][WHAT_OBJECT_NOW].pos_new_x - state_result__[INDEX_NRmores][d][WHAT_OBJECT_NOW].pos_new_x;
		springVector->pos_new_y = state_result__[INDEX_NR][s][WHAT_OBJECT_NOW].pos_new_y - state_result__[INDEX_NRmores][d][WHAT_OBJECT_NOW].pos_new_y;
		springVector->pos_new_z = state_result__[INDEX_NR][s][WHAT_OBJECT_NOW].pos_new_z - state_result__[INDEX_NRmores][d][WHAT_OBJECT_NOW].pos_new_z;
 	//printf("taskworm 20\n");		

		float r = length(state_result__[INDEX_NR][s],state_result__[INDEX_NRmores][d]);

		if ( r != 0  &&    r < 0.2  && r > -0.2    )
		{	
	//	 printf("1totalneigbours[lllll]  %d :: number %d :: index_____ = %d\n",totalneigbours[lllll],lllll,INDEX_NRmore);	
			F_total_next_[0][INDEX_NR][s] +=( springVector->pos_new_x  /  r ) * ( r - springlength ) * ( -springConstant );
			F_total_next_[1][INDEX_NR][s] += ( springVector->pos_new_y /  r ) * ( r - springlength ) * ( -springConstant );
			F_total_next_[2][INDEX_NR][s] +=( springVector->pos_new_z  /  r ) * ( r - springlength ) * ( -springConstant );
 
			F_totals_next_[0][INDEX_NRmores][d] +=( springVector->pos_new_x  / r ) * ( r - springlength ) * ( -springConstant );
			F_totals_next_[1][INDEX_NRmores][d] += ( springVector->pos_new_y  / r ) * ( r - springlength) * ( -springConstant );
			F_totals_next_[2][INDEX_NRmores][d] +=( springVector->pos_new_z  / r ) * ( r - springlength ) * ( -springConstant );

		F_total_next_[0][INDEX_NR][s] +=-(  state_result__[INDEX_NR][s][WHAT_OBJECT_NOW].vel_new_x - state_result__[INDEX_NRmores][d][WHAT_OBJECT_NOW].vel_new_x ) * frictionConstant;
 		F_total_next_[1][INDEX_NR][s] += -( state_result__[INDEX_NR][s][WHAT_OBJECT_NOW].vel_new_y- state_result__[INDEX_NRmores][d][WHAT_OBJECT_NOW].vel_new_y ) * frictionConstant;
 		F_total_next_[2][INDEX_NR][s] += -(  state_result__[INDEX_NR][s][WHAT_OBJECT_NOW].vel_new_z - state_result__[INDEX_NRmores ][d][WHAT_OBJECT_NOW].vel_new_z ) * frictionConstant;

//printf("taskworm 21\n");
		F_totals_next_[0][INDEX_NRmores][d] +=-(  state_result__[INDEX_NR][s][WHAT_OBJECT_NOW].vel_new_x - state_result__[INDEX_NRmores][d][WHAT_OBJECT_NOW].vel_new_x ) * frictionConstant;
 		F_totals_next_[1][INDEX_NRmores][d] += -( state_result__[INDEX_NR][s][WHAT_OBJECT_NOW].vel_new_y- state_result__[INDEX_NRmores][d][WHAT_OBJECT_NOW].vel_new_y ) * frictionConstant;
 		F_totals_next_[2][INDEX_NRmores][d] += -(  state_result__[INDEX_NR][s][WHAT_OBJECT_NOW].vel_new_z - state_result__[INDEX_NRmores ][d][WHAT_OBJECT_NOW].vel_new_z ) * frictionConstant;

	 	state_result__[INDEX_NRmores][d][WHAT_OBJECT_NOW].vel_new_x = state_result__[INDEX_NRmores][d][WHAT_OBJECT_NOW].vel_new_x*0.995 ;
		
		state_result__[INDEX_NRmores][d][WHAT_OBJECT_NOW].vel_new_y = state_result__[INDEX_NRmores][d][WHAT_OBJECT_NOW].vel_new_y*0.995 ;
//printf("taskworm 22\n");
		state_result__[INDEX_NRmores][d][WHAT_OBJECT_NOW].vel_new_z = state_result__[INDEX_NRmores][d][WHAT_OBJECT_NOW].vel_new_z*0.995 ;

		state_result__[INDEX_NR][s][WHAT_OBJECT_NOW].vel_new_x = state_result__[INDEX_NR][s][WHAT_OBJECT_NOW].vel_new_x*0.995 ;
		
		state_result__[INDEX_NR][s][WHAT_OBJECT_NOW].vel_new_y = state_result__[INDEX_NR][s][WHAT_OBJECT_NOW].vel_new_y*0.995 ;
		state_result__[INDEX_NR][s][WHAT_OBJECT_NOW].vel_new_z = state_result__[INDEX_NR][s][WHAT_OBJECT_NOW].vel_new_z*0.995 ; 
BIGONE = 1;

 WHAT_OBJECT_NOW = s;
		state_result__[INDEX_NR][s][WHAT_OBJECT_NOW].force_sign =  -1;
		state_result__   =  worm_ventral(  state_result__,1,INDEX_NR,s,WHAT_OBJECT_NOW  ) ;
BIGONE = 0;
 WHAT_OBJECT_NOW = d;
		state_result__[INDEX_NRmores][d][WHAT_OBJECT_NOW].force_sign =   1; ; 
		state_result__  =  worm_ventral(  state_result__ ,1,INDEX_NRmores,d,WHAT_OBJECT_NOW );
 WHAT_OBJECT_NOW =WHAT_OBJECT;
//printf("taskworm 23\n");
		}
}
else
{
 int s;
int d;
//2 v 1, 1 v 0 , 2 v 0           1 v 2, 1 v 3 , 1 v 4           2 v 3, 3 v 1, 4 v 2
for(s = 0; s < TOTAL_OBJECTS ; s++)
{
for(d =1 ; d < TOTAL_OBJECTS; d++)
{
if(d != s)
{
 BIGONE = 1;
 WHAT_OBJECT_NOW = WHAT_OBJECT;
		state_result__[INDEX_NR][s][WHAT_OBJECT_NOW].force_sign =  -1;
		state_result__    =  worm_ventral(  state_result__,1,INDEX_NR,s,WHAT_OBJECT_NOW) ;
BIGONE = 0;
 WHAT_OBJECT_NOW = d;
		state_result__[INDEX_NRmores][d][WHAT_OBJECT_NOW].force_sign =   1; ; 
		state_result__   =  worm_ventral(  state_result__,1,INDEX_NRmores,d ,WHAT_OBJECT_NOW);
 WHAT_OBJECT_NOW =WHAT_OBJECT;

}
}
}
}

//printf("taskworm 24\n");
if(state_result__[INDEX_NR][s][WHAT_OBJECT_NOW].pos_new_y < -2)
			{
			 state_result__[INDEX_NR][s][WHAT_OBJECT_NOW].pos_new_y = -2;
			 state_result__[INDEX_NR][s][WHAT_OBJECT_NOW].vel_new_y = -state_result__[INDEX_NR][s][WHAT_OBJECT_NOW].vel_new_y*0.00995;
 
			}	
		if(state_result__[INDEX_NRmores][d][WHAT_OBJECT_NOW].pos_new_y < -2)
			{
				 state_result__[INDEX_NRmores][d][WHAT_OBJECT_NOW].pos_new_y = -2;
			 state_result__[INDEX_NRmores][d][WHAT_OBJECT_NOW].vel_new_y = -state_result__[INDEX_NRmores][d][WHAT_OBJECT_NOW].vel_new_y*0.00995;
 
			}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	 
 	//} 
}
}
 }

}
 	
//}
		 
 }	
//}
		 
 }		
//printf("taskworm 25\n");
		 if(fabs(F_total_next_[0][lllll][WHAT_OBJECT_NOW]) >  6  )
	 				 F_total_next_[0][lllll][WHAT_OBJECT_NOW] =0;
 
		 		 if(fabs(F_total_next_[1][lllll][WHAT_OBJECT_NOW])  >  6 )
					 F_total_next_[1][lllll][WHAT_OBJECT_NOW] =0;

			 	 if(fabs(F_total_next_[2][lllll][WHAT_OBJECT_NOW])  >  6 )
					 F_total_next_[2][lllll][WHAT_OBJECT_NOW] =0;


				 if(fabs(F_totals_next_[0][lllll][WHAT_OBJECT_NOW]) >  6  )
	 				 F_totals_next_[0][lllll][WHAT_OBJECT_NOW] =0;
 
		 		 if(fabs(F_totals_next_[1][lllll][WHAT_OBJECT_NOW])  >  6 )
					 F_totals_next_[1][lllll][WHAT_OBJECT_NOW] =0;

			 	 if(fabs(F_totals_next_[2][lllll][WHAT_OBJECT_NOW])  >  6 )
					 F_totals_next_[2][lllll][WHAT_OBJECT_NOW] =0;
			// printf("FORCE(x,y,z) = %.50f:%.50f%.50f \n", fabs(F_totals_next_[0][INDEX_NRs][WHAT_OBJECT_NOW]),fabs(F_totals_next_[1][lllll][WHAT_OBJECT_NOW]),fabs(F_totals_next_[2][INDEX_NRmores][WHAT_OBJECT_NOW]));
//printf("taskworm 26\n");
 //printf("FORCE(x,y,z) = %.50f:%.50f%.50f \n", fabs(F_total_next_[0][INDEX_NR][WHAT_OBJECT_NOW]),fabs(F_total_next_[1][lllll][WHAT_OBJECT_NOW]),fabs(F_total_next_[2][INDEX_NRmore][WHAT_OBJECT_NOW]));
	
		//	if(lllll >= SIZE_OBJECT -1  )
			//{
				//feather_once= WORK;
	   		//	lllll =0;
			////	int var;
			//	for(var = 0; var <  3 -1 ; var++)
			//	{
				//	state_result__[var]->force_sign = 0;
 	
				//}
			//}
}

}

int find_index_____(int NUM, struct state_vector  ***real  ,int num,int k )
{
int ll = 0;
 	while(ll < NUM)
	{
		if(feather_result[num][k][WHAT_OBJECT_NOW].pos_new_x ==real[ll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_x && feather_result[num][k][WHAT_OBJECT_NOW].pos_new_y ==real[ll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_y && feather_result[num][k][WHAT_OBJECT_NOW].pos_new_z ==real[ll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_z)
		{
			//printf("found index_____ @ %d \n", ll);
			return ll;
		}
	ll++;
	}
}
/*int find_index_____s(int NUM, struct state_vector **real  ,int num,int k )
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
}*/
void *ptree;
 #define HOW_MANY_OBJECTS 2
int ptreeonce = 1;
int ptreeonce_[HOW_MANY_OBJECTS]  = {1} ;
int findnearestpoint(int points,struct state_vector *** worm , int num,int howmany) {
int i, num_pts = points;
int INDEX;
char *data, *pch;
struct kdres *presults;
double pos[3], dist;
double pt[3] = { worm[num][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_x,worm[num][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_y, worm[num][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_z };
double radius = 0.0002/1.0;

num_pts =points;

srand( time(0) );

if( ptreeonce_[howmany] < 2  )
{
printf("ptreeonce_[howmany] = %d \n", ptreeonce_[howmany]);
	ptree = kd_create( 3 );
  	for( i=0; i<points; i++ ) {   
    		assert( 0 == kd_insert3( ptree, worm[i][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_x, worm[i][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_y, worm[i][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_z, NULL ) );
 	 }
}
ptreeonce_[howmany]=ptreeonce_[howmany] +1;

 
presults = kd_nearest_range( ptree, pt, radius );
 
  /* print out all the points found in results */
  //printf( "found %d results:\n", kd_res_size(presults) );


 feather_result[num][0][WHAT_OBJECT_NOW].totaln = kd_res_size(presults) ;

//state_result_worm_dorsal_feather[num][0].totaln = kd_res_size(presults) ;
  printf( "found %d results:\n", kd_res_size(presults) );
int feather_count = 0;
//printf(" state_result_worm_ventral_feather[llll][0].totaln %d ::%d\n",  state_result_worm_ventral_feather[num][0].totaln,num);
while( !kd_res_end( presults ) ) {
   
	pch = (char*)kd_res_item( presults, pos ); 
    	dist = sqrt( dist_sq( pt, pos, 3 ) );

 // if(worm[num]->pos_new_x != pos[0])
 //	  printf( "node at (%.3f, %.3f, %.3f) is %.3f away   \n", 
 //	    pos[0], pos[1], pos[2], dist  );
	 

  	totalneigbours[num][WHAT_OBJECT_NOW] = feather_result[num][0][WHAT_OBJECT_NOW].totaln;
	// if(worm[num]->pos_new_x != pos[0])
   		feather_result[num][feather_count][WHAT_OBJECT_NOW].pos_new_x = pos[0];
	// if(worm[num]->pos_new_y != pos[1])
  		feather_result[num][feather_count][WHAT_OBJECT_NOW].pos_new_y = pos[1];
	//  if(worm[num]->pos_new_z != pos[2])
   		feather_result[num][feather_count][WHAT_OBJECT_NOW].pos_new_z = pos[2];
	int k;
 
   
  INDEX = find_index_____(SIZE_OBJECT,state_result ,num,feather_count);
 // printf("INDEX = %d \n", INDEX);
  feather_result[num][feather_count][WHAT_OBJECT_NOW].INDEX_NR = INDEX;
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
 




#if 0
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
#endif
int first_run = 1;
int initonce =1;
extern int KvvVENTAL ;
extern int KvvDORSAL ;
struct state_vector*** arr3dAlloc(const int ind1, const int ind2, const int ind3)
{
  int i;
  int j;
  struct state_vector*** array = (struct state_vector***) malloc( (ind1 * sizeof(struct state_vector*)) + (ind1*ind2 * sizeof(struct state_vector**)) + (ind1*ind2*ind3 * sizeof(struct state_vector)) );
  for(i = 0; i < ind1; ++i) {
    array[i] = (struct state_vector**)(array + ind1) + i * ind2;
    for(j = 0; j < ind2; ++j) {
      array[i][j] = (struct state_vector*)(array + ind1 + ind1*ind2) + i*ind2*ind3 + j*ind3;
    }
  }
  return array;
}


float*** arr3dAlloc_(const int ind1, const int ind2, const int ind3)
{
  int i;
  int j;
  float*** array = (float***) malloc( (ind1 * sizeof(float*)) + (ind1*ind2 * sizeof(float**)) + (ind1*ind2*ind3 * sizeof(float)) );
  for(i = 0; i < ind1; ++i) {
    array[i] = (float**)(array + ind1) + i * ind2;
    for(j = 0; j < ind2; ++j) {
      array[i][j] = (float*)(array + ind1 + ind1*ind2) + i*ind2*ind3 + j*ind3;
    }
  }
  return array;
}
extern float* V; 
extern float* V2;
int main (int argc, char **argv)
{ 

loadOBJ__("sphere.obj"); //61856 //61856 //13108_Eastern_Hognose_Snake_v1_L3  61856
 totalneigbours =(int*)malloc(sizeof(int*)*SIZE_OBJECT*32);

F_total_next = arr3dAlloc_(SIZE_OBJECT*1,44,10);
F_totals_next  = arr3dAlloc_(SIZE_OBJECT*1,44,10);

for(int i=0; i<SIZE_OBJECT*1; i++)
totalneigbours[i] = (int*)malloc(sizeof(int)*32);


springVector = malloc(SIZE_OBJECT*11);
springVectors = malloc(SIZE_OBJECTs*11);
//*feather_result  = (struct state_vector*)malloc(sizeof(struct state_vector*)*SIZE_OBJECT*4);
//state_result_worm_ventral_feathers = (struct state_vector*)malloc(sizeof(struct state_vector*)*SIZE_OBJECTs*4);
//*state_result   =(struct state_vector*)malloc(sizeof(struct state_vector*)*SIZE_OBJECT*32);
feather_result = arr3dAlloc(SIZE_OBJECT ,44,10);
state_result = arr3dAlloc(SIZE_OBJECT ,44,10);
//state_result_worm_ventral_small =(struct state_vector*)malloc(sizeof(struct state_vector*)*SIZE_OBJECTs*32);

 int i,j,k;

/*
F_totals = (double*)malloc(SIZE_OBJECTs*11);
for(int i=0; i<SIZE_OBJECTs*1; i++)
	F_totals[i] = (double*)malloc(4*32);

F_total  = (double*)malloc(SIZE_OBJECT *11);
for(int i=0; i<SIZE_OBJECT*1; i++)
	F_total [i] = (double*)malloc(4*32);


F_totals_next = (double*)malloc(SIZE_OBJECTs*11);
for(int i=0; i<SIZE_OBJECTs*1; i++)
	F_totals_next[i] = (double*)malloc(4*32);

F_total_next  = (double*)malloc(SIZE_OBJECT *11);
for(int i=0; i<SIZE_OBJECT*1; i++)
	F_total_next [i] = (double*)malloc(4*32);
*/
/*
for(int i=0; i<SIZE_OBJECT*1; i++)
	 state_result[i]  = (struct state_vector*)malloc(sizeof(struct state_vector)*32);
       
printf("done ! \n");
 
for(int i=0; i<SIZE_OBJECT*1; i++)
	 feather_result[i]  = (struct state_vector*)malloc(sizeof(struct state_vector)*32);

*/
//for(int i=0; i<SIZE_OBJECTs*1; i++)
//	state_result_worm_ventral_feathers[i] = (struct state_vector*)malloc(sizeof(struct state_vector)*32); 
printf("done ! \n");
  
 
//for(int i=0; i<SIZE_OBJECTs*1; i++)
//	state_result_worm_ventral_small[i] = (struct state_vector*)malloc(sizeof(struct state_vector)*32);

  
printf("done ! \n");

if(initonce==1)
{
	Vec3 *v ;
 TOTAL_OBJECTS = 1;
	int ll;
WHAT_OBJECT_NOW = 0;
	for(ll = 0; ll < SIZE_OBJECT-1  ; ll++)
	{
		  v= (Vec3 *)(V + 3*(KvvVENTALA[ll]-1));
         	  state_result[ll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_x = v->x ;
 
 	          state_result[ll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_y = v->y ;
 		  state_result[ll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_z = v->z ;
 
 	  	  state_result[ll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_x =  0 ;
 		  state_result[ll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_y = 0;
 		  state_result[ll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_z =  0 ;
		  state_result[ll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].mass = ll;
  		  state_result[ll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].have_bounce = 0;

 


	}
ptreeonce_[0] = 1;
	for(ll = 0; ll < SIZE_OBJECT-1      ; ll++)
	{
		
 		  findnearestpoint(SIZE_OBJECT-1  ,state_result,ll,0 );
 
	}

TOTAL_OBJECTS++;
WHAT_OBJECT_NOW = 1;
printf("done ! \n");
loadOBJ__("sphere.obj"); //61856 //61856 //13108_Eastern_Hognose_Snake_v1_L3  61856	 
	for(ll = 0; ll < SIZE_OBJECTs-1  ; ll++)
	{
		  v= (Vec3 *)(V + 3*(KvvVENTALA[ll]-1));
 
    		  state_result[ll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_x = v->x ;
 
 	          state_result[ll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_y = v->y +3.5;
 		  state_result[ll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_z = v->z ;
 
 	  	  state_result[ll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_x =  0 ;
 		  state_result[ll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_y =  -0.0;
 		  state_result[ll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].vel_new_z =  0 ;
		  state_result[ll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].mass = ll;
  		  state_result[ll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].have_bounce = 1;

	}
 
ptreeonce = 1;
ptreeonce_[1] = 1;	
	for(ll = 0; ll < SIZE_OBJECTs-1     ; ll++)
	{ 
		
 	 	  findnearestpoint(SIZE_OBJECTs-1  ,state_result ,ll,1 );
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
dirLight->setSpecular_light(0.995f, 0.2f, 0.88f);
 
material =(Material*)malloc(sizeof(Material));
material->setAmbient_material =setAmbient_material;
material->setDifusse_material = setDifusse_material;
material->setSpecular_material =setSpecular_material;
material->setShininess_material = setShininess_material;
material->setAmbient_material(0, 0, 1, 1);
material->setDifusse_material(0.5f, 0.2f, 0.995f, 1.0f/50.0f);
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

struct state_vector     ***  worm_ventral(struct state_vector     ***   next_state, int num ,int index, int what1, int what2)
{
	double f0[6];
	double f0_result[6];

	double z[6];
 	double t0[6] = {0,0,0,0,0,0};
 	double tburn = 1.0;
	double tf[6] = {tburn,tburn,tburn,tburn,tburn,tburn};

	z[0] =  next_state[index][what1][what2].pos_new_x;
	z[1] =  next_state[index][what1][what2].pos_new_y;
	z[2] = next_state[index][what1][what2].pos_new_z;
	z[3] =  next_state[index][what1][what2].vel_new_x;
	z[4] =  next_state[index][what1][what2].vel_new_y;
	z[5] = next_state[index][what1][what2].vel_new_z;

	f0[0] = next_state[index][what1][what2].pos_new_x;
	f0[1] =next_state[index][what1][what2].pos_new_y;
	f0[2] =next_state[index][what1][what2].pos_new_z;
	f0[3] =next_state[index][what1][what2].vel_new_x;
	f0[4] =next_state[index][what1][what2].vel_new_y;
	f0[5] = next_state[index][what1][what2].vel_new_z;
 
	rk45(rates_dorsal, t0, f0,f0_result, tf,z,6, 1  );

	next_state[index][what1][what2].pos_new_x = f0[0];
	next_state[index][what1][what2].pos_new_y = f0[1];
	next_state[index][what1][what2].pos_new_z = f0[2];

	next_state[index][what1][what2].vel_new_x = f0[3];
	next_state[index][what1][what2].vel_new_y = f0[4];
	next_state[index][what1][what2].vel_new_z = f0[5];

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
double rotnow = 0;
void display  (void){
 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1,1,1,1);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_COLOR_MATERIAL);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glMultMatrixf(lightRotation);
 

    glLoadIdentity();
    gluLookAt(cameraEye[0]+453, cameraEye[1]-222, cameraEye[2]-600, cameraLookAt[0], cameraLookAt[1], cameraLookAt[2], cameraUp[0], cameraUp[1], cameraUp[2]);

    glMultMatrixf(viewRotation);

    glRotatef(rotationX,1,0,0);
    glRotatef(rotationY+320,0,1,0);

    glMultMatrixf(lightRotation);
 //glPushMatrix();
  // glRotatef(rotnow ,rotnow,rotnow,0);
	 
rotnow += 0.01;
   for(int ll = 0; ll <   SIZE_OBJECTs ; ll++)  
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


	/* if(fabs(F_total[0][ll]) >  6  )
	 				 	  F_total[0][ll] =0;
 
		 		 if(fabs(F_total[1][ll])  >  6 )
						F_total[1][ll] =0;

			 	 if(fabs(F_total[2][ll])  >  6 )
						F_total[2][ll] =0;

	 if(fabs(F_totals[0][ll]) >  6  )
	 				 	  F_totals[0][ll] =0;
 
		 		 if(fabs(F_totals[1][ll])  >  6 )
						F_totals[1][ll] =0;

			 	 if(fabs(F_totals[2][ll])  >  6 )
						F_totals[2][ll] =0;*/
BIGONE = 1;
WHAT_OBJECT = 0;
WHAT_OBJECT_NOW = 0;
  taskworm(ll, totalneigbours,  feather_result  ,  state_result  ,0,( (F_total_next )),F_totals_next  );
 

//BIGONE = 1;
//WHAT_OBJECT = 2;
//WHAT_OBJECT_NOW = 2;
  //taskworm(ll, totalneigbours[2] ,feather_result[2] ,  state_result[2] ,2 );



/*	 if(fabs(F_total_next[0][ll]) >  6  )
	 				 	  F_total_next[0][ll] =0;
 
		 		 if(fabs(F_total_next[1][ll])  >  6 )
						F_total_next[1][ll] =0;

			 	 if(fabs(F_total_next[2][ll])  >  6 )
						F_total_next[2][ll] =0;

	 if(fabs(F_totals_next[0][ll]) >  6  )
	 				 	  F_totals_next[0][ll] =0;
 
		 		 if(fabs(F_totals_next[1][ll])  >  6 )
						F_totals_next[1][ll] =0;

			 	 if(fabs(F_totals_next[2][ll])  >  6 )
						F_totals_next[2][ll] =0;*/

//taskworm();

 //taskwormsmall();

 

  /* Vertices */
  //
for (j = 0; j < RESOLUTION; j++)
    {
   //   y = (j + 1) * delta - 1;
      for (i = 0; i <= RESOLUTION; i++)
	{
	  indice = 6 * (i + j * (RESOLUTION + 1));

	//  x = i * delta - 1;
	  surface[indice + 3] = state_result[ll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_x;
	  surface[indice + 4] = state_result[ll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_y;//z (state_result_worm_ventral[ll]->pos_new_x, state_result_worm_ventral[ll][WHAT_OBJECT_NOW].pos_new_y, t); 
	  surface[indice + 5] = state_result[ll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_z;
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
	      surface[indice] = state_result[ll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_x;
	      surface[indice + 1] = state_result[ll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_y;// z (state_result_worm_ventral[ll][WHAT_OBJECT_NOW].pos_new_x, -1, t);
	      surface[indice + 2] = state_result[ll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_z;
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
 
 
 

if(ll <  SIZE_OBJECT/4 )
{
 glPushMatrix();
 
glScalef(100,100,100);
   glTranslatef(state_result[ll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_x, state_result[ll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_y,state_result[ll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_z);
 
  glColor3f(state_result[ll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_x, state_result[ll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_y, state_result[ll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_z);  
 //glColor3f(1,1,1);
  glutSolidSphere(0.02,42,42);
  glPopMatrix(); 
 
}

BIGONE = 1;
WHAT_OBJECT = 1;
WHAT_OBJECT_NOW = 1;
  taskworm(ll, totalneigbours,  feather_result  ,  state_result  ,1,F_totals_next  ,( (F_total_next )) );
  /* Vertices */
  //
for (j = 0; j < RESOLUTION; j++)
    {
   //   y = (j + 1) * delta - 1;
      for (i = 0; i <= RESOLUTION; i++)
	{
	  indice = 6 * (i + j * (RESOLUTION + 1));

	//  x = i * delta - 1;
	  surface[indice + 3] = state_result[ll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_x;
	  surface[indice + 4] = state_result[ll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_y;//z (state_result_worm_ventral[ll][WHAT_OBJECT_NOW].pos_new_x, state_result_worm_ventral[ll][WHAT_OBJECT_NOW].pos_new_y, t); 
	  surface[indice + 5] = state_result[ll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_z;
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
	      surface[indice] = state_result[ll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_x;
	      surface[indice + 1] = state_result[ll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_y;// z (state_result_worm_ventral[ll][WHAT_OBJECT_NOW].pos_new_x, -1, t);
	      surface[indice + 2] = state_result[ll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_z;
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
if(ll <  SIZE_OBJECT/4 )
{
glPushMatrix();
glScalef(100,100,100);
 glTranslatef(state_result[ll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_x, state_result[ll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_y,state_result[ll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_z);
 
 glColor3f(state_result[ll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_x, state_result[ll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_y, state_result[ll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].pos_new_z);  
// glColor3f(1,1,1);
   glutSolidSphere(0.06,42,42);
 
  glPopMatrix(); 
}
 glutPostRedisplay();
}
//  glPopMatrix();
display_mpeg();
 glFlush ();
glutSwapBuffers();

}


 

void rates_dorsal ( double *t, double *f, double result[]   )
{

// printf("f[0] = %.10f:%.10f%.10f \n", f[0],f[1],f[2]);
if(BIGONE == 1)
{
if(state_result[llll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].force_sign == 1)
{
    	result[0] =             f[3]/100.0;
    	result[1] =             f[4]/100.0;
    	result[2] =             f[5]/100.0;
  
	result[3] = (F_total_next[0][llll][WHAT_OBJECT_NOW])/(42222.0   );
	result[4] = (F_total_next[1][llll][WHAT_OBJECT_NOW]   )/(42222.0   );
	result[5] = (F_total_next[2][llll][WHAT_OBJECT_NOW] )/(42222.0); 

 
}
else if(state_result[INDEX_NR][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].force_sign == -1)
{
    result[0] =             f[3]/100.0;
    result[1] =             f[4]/100.0;
    result[2] =             f[5]/100.0;
  
    result[3] = -(F_total_next[0][INDEX_NR][WHAT_OBJECT_NOW])/(42222.0);
    result[4] = -(F_total_next[1][INDEX_NR][WHAT_OBJECT_NOW]  ) /(42222.0) ;
    result[5] = -(F_total_next[2][INDEX_NR][WHAT_OBJECT_NOW] )/(42222.0); //522
 

}
else if(state_result[INDEX_NRmore][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].force_sign == 1)
{
    	result[0] =             f[3]/100.0;
    	result[1] =             f[4]/100.0;
    	result[2] =             f[5]/100.0;
  
	result[3] = (F_total_next[0][INDEX_NRmore][WHAT_OBJECT_NOW])/(42222.0   );
	result[4] = (F_total_next[1][INDEX_NRmore][WHAT_OBJECT_NOW]  )/(42222.0  );
	result[5] = (F_total_next[2][INDEX_NRmore][WHAT_OBJECT_NOW] )/(42222.0); 

 
}

}
else if(BIGONE == 0)
{
if(state_result[llll][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].force_sign == 1)
{
    	result[0] =             f[3]/100.0;
    	result[1] =             f[4]/100.0;
    	result[2] =             f[5]/100.0;
  
	result[3] = (F_totals_next[0][llll][WHAT_OBJECT_NOW])/(42222.0   );
	result[4] = (F_totals_next[1][llll][WHAT_OBJECT_NOW]  -9.8  )/(42222.0   );
	result[5] = (F_totals_next[2][llll][WHAT_OBJECT_NOW] )/(42222.0); 

 
}
 else if(state_result[INDEX_NRs][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].force_sign == -1)
{
    result[0] =             f[3]/100.0;
    result[1] =             f[4]/100.0;
    result[2] =             f[5]/100.0;
  
    result[3] = -(F_totals_next[0][INDEX_NRs][WHAT_OBJECT_NOW])/(42222.0);
    result[4] = -(F_totals_next[1][INDEX_NRs][WHAT_OBJECT_NOW]  -9.8 ) /(42222.0) ;
    result[5] = -(F_totals_next[2][INDEX_NRs][WHAT_OBJECT_NOW] )/(42222.0); //522
 

}
else if(state_result[INDEX_NRmores][WHAT_OBJECT_NOW][WHAT_OBJECT_NOW].force_sign == 1)
{
    	result[0] =             f[3]/100.0;
    	result[1] =             f[4]/100.0;
    	result[2] =             f[5]/100.0;

	result[3] = (F_totals_next[0][INDEX_NRmores][WHAT_OBJECT_NOW])/(42222.0   );
	result[4] = (F_totals_next[1][INDEX_NRmores][WHAT_OBJECT_NOW]   -9.8 )/(42222.0  );
	result[5] = (F_totals_next[2][INDEX_NRmores][WHAT_OBJECT_NOW] )/(42222.0); 

 
}
}
  acc1 = sqrtf(pow(result[3],2.0) + pow(result[4],2.0)+pow(result[5],2.0));
//printf("acc1 = %.10f \n", acc1);
}

 
