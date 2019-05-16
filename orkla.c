 
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
#include <jpeglib.h>
#include <complex.h>
#include <jerror.h>
#include <stdlib.h>
#include <jpeglib.h>
#include <jerror.h>
#include "noise.h"
int totaln[10000] ;

int c_m = 0;
int cm2 = 0;
double pos_[3];
double oldpos[3];
int once_pos =1;
int k_flappers = 0;
int INDEX_p[100000];
double time_particle[100000];
double dist__first[100000];
double posdiff[2][10000];
double veldiff[2][10000];
double dist__next[100000];
int time_activate[100000]  ;
int total_molekules_nearby  = 1;
double Area_tube;
	double mass_fluid_element;
static GLuint texture;
int totalneigbours_m = 0;
void DrawGL(void) ;
#define NUM_THREADS 5
extern int* KvvVENTALA2 ;
int pipe = 0;
#include <stdbool.h>
extern int KvvVENTAL2;
  struct state_vector  ** wall_info ; 
double  neighbourV_[600000*2][11] ;
double  neighbourV_m[1000][100] ;
double  neighbourV_water[1000][100] ;
double neigbours_wall_xyz[10000][100][4];
int neigbours_wall[60000][400];
 double acc2_fluid[3][600000 ][12][10] ;
double water_radius =0.00005;
int alloc_once = 1;
double density =   1000.0;
double viscous_constant = 1.6735;
float acc1;
float acc2;
int INDEX_NRmore;
#define SIZE_OBJECT 1984 //126976
#define HOW_MANY_FLUIDS 1
#define HOWMANY HOW_MANY_FLUIDS
float springlength =  9.4/1.0;
#define AIR_FRICTION 0.999999999
#define FIRST 1
#define WORK 0
float fovy = 45.0;
float dNear = 100;
float dFar = 2000;
double F_total[3][SIZE_OBJECT*2][HOW_MANY_FLUIDS][10] ;
int totalneigbours[SIZE_OBJECT*2][HOW_MANY_FLUIDS] ;
float     vertexpoint_g_ventral[SIZE_OBJECT][3];
struct state_vector * springVector;
struct state_vector *** state_result_worm_ventral_feather;
struct state_vector *** reall;
struct state_vector  *** state_result_worm_ventral ;
extern int KvvVENTALA[SIZE_OBJECT*8];
int llll = 0;
int i; 
int feather_once = 1;
int INDEX_NR = 0;
Light *dirLight;
Material *material;


float springConstant =   200.202f;

float frictionConstant =    0.2200000f;

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
int findnearestpoint(int points,struct state_vector   *   worm , int num,int howmany,int kk, int xxx);
void display  (void);
struct state_vector       worm_ventral(struct state_vector       next_state, int num );
double length (struct state_vector  a,struct state_vector  b ) { return sqrtf ( b.pos_new_x*a.pos_new_x + b.pos_new_y*a.pos_new_y + b.pos_new_z*a.pos_new_z); }
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
 
    dirLight->pos[0] = x;
    dirLight->pos[1] = y;
    dirLight->pos[2] = z;
    dirLight->pos[3] = 0.0f;
}

void  setAmbient_light(float x, float y, float z)
{
 
    dirLight->ambient[0] = x;
    dirLight->ambient[1] = y;
    dirLight->ambient[2] = z;
}

void  setDifusse_light(float x, float y, float z)
{
 
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

void reshape(int w, int h){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fovy, (GLdouble)w/(GLdouble)h, dNear, dFar);
    glViewport(0, 0, w, h);
} 
int howmanyfluids_g;
int acc1once= 1;
void taskworm(int howmanyfluids, int kkkkkkkk, int xxx )
{
	llll = kkkkkkkk;
	howmanyfluids_g =howmanyfluids;
	k_flappers = xxx;
	int k = xxx;

	findnearestpoint_3_points_water_pressure(c_m      ,   llll, .065 , NULL ,llll,howmanyfluids,xxx);
 
if(acc1once == 0)
	{
		if(fabs(acc1) > 0)
		{		
			springConstant =   0.205;
 
			frictionConstant =  1.0202;
		}
	}
	acc1once = 0;	
 
	INDEX_NR =   state_result_worm_ventral_feather[llll][k][howmanyfluids].INDEX_NR ;
	
if(  INDEX_NR > 0  )
{
		 if(feather_once == FIRST && INDEX_NR > 0)
		{	

				 if(fabs(F_total[0][INDEX_NR][howmanyfluids][k]) > 21 )
	 				 	  F_total[0][INDEX_NR][howmanyfluids][k] =0;
 
		 		 if(fabs(F_total[1][INDEX_NR][howmanyfluids][k])  > 21)
						F_total[1][INDEX_NR][howmanyfluids][k] =0;

			 	 if(fabs(F_total[2][INDEX_NR][howmanyfluids][k])  > 21)
						F_total[2][INDEX_NR][howmanyfluids][k] =0;
 
				springVector->pos_new_x = state_result_worm_ventral[INDEX_NR][k][howmanyfluids].pos_new_x - state_result_worm_ventral[llll][k][howmanyfluids].pos_new_x;
				springVector->pos_new_y = state_result_worm_ventral[INDEX_NR][k][howmanyfluids].pos_new_y - state_result_worm_ventral[llll][k][howmanyfluids].pos_new_y;
				springVector->pos_new_z = state_result_worm_ventral[INDEX_NR][k][howmanyfluids].pos_new_z - state_result_worm_ventral[llll][k][howmanyfluids].pos_new_z;
 

				float r = length(state_result_worm_ventral[INDEX_NR][k][howmanyfluids],state_result_worm_ventral[llll][k][howmanyfluids]);

				if ( r != 0  &&    (r) <10.1  )
				{			
					F_total[0][INDEX_NR][howmanyfluids][k] +=( springVector->pos_new_x  /  r ) * ( r - springlength ) * ( -springConstant );
					F_total[1][INDEX_NR][howmanyfluids][k] += ( springVector->pos_new_y /  r ) * ( r - springlength ) * ( -springConstant );
					F_total[2][INDEX_NR][howmanyfluids][k] +=( springVector->pos_new_z  /  r ) * ( r - springlength ) * ( -springConstant );

					F_total[0][llll][howmanyfluids][k] +=( springVector->pos_new_x  / r ) * ( r - springlength ) * ( -springConstant );
					F_total[1][llll][howmanyfluids][k] += ( springVector->pos_new_y  / r ) * ( r - springlength) * ( -springConstant );
					F_total[2][llll][howmanyfluids][k] +=( springVector->pos_new_z  / r ) * ( r - springlength ) * ( -springConstant );

					F_total[0][INDEX_NR][howmanyfluids][k] +=-(  state_result_worm_ventral[INDEX_NR][k][howmanyfluids].vel_new_x - state_result_worm_ventral[llll][k][howmanyfluids].vel_new_x ) * frictionConstant;
 					F_total[1][INDEX_NR][howmanyfluids][k] += -( state_result_worm_ventral[INDEX_NR][k][howmanyfluids].vel_new_y- state_result_worm_ventral[llll][k][howmanyfluids].vel_new_y ) * frictionConstant;
 					F_total[2][INDEX_NR][howmanyfluids][k] += -(  state_result_worm_ventral[INDEX_NR][k][howmanyfluids].vel_new_z - state_result_worm_ventral[llll ][k][howmanyfluids].vel_new_z ) * frictionConstant;


					F_total[0][llll][howmanyfluids][k] +=-(  state_result_worm_ventral[INDEX_NR][k][howmanyfluids].vel_new_x - state_result_worm_ventral[llll][k][howmanyfluids].vel_new_x ) * frictionConstant;
 					F_total[1][llll][howmanyfluids][k] += -( state_result_worm_ventral[INDEX_NR][k][howmanyfluids].vel_new_y- state_result_worm_ventral[llll][k][howmanyfluids].vel_new_y ) * frictionConstant;
 					F_total[2][llll][howmanyfluids][k] += -(  state_result_worm_ventral[INDEX_NR][k][howmanyfluids].vel_new_z - state_result_worm_ventral[llll ][k][howmanyfluids].vel_new_z ) * frictionConstant;
	 	/*state_result_worm_ventral[llll][k][howmanyfluids].vel_new_x = state_result_worm_ventral[llll][k][howmanyfluids].vel_new_x*0.9999 ;
		
		state_result_worm_ventral[llll][k][howmanyfluids].vel_new_y = state_result_worm_ventral[llll][k][howmanyfluids].vel_new_y*0.9999 ;

		state_result_worm_ventral[llll][k][howmanyfluids].vel_new_z = state_result_worm_ventral[llll][k][howmanyfluids].vel_new_z*0.9999 ;

		state_result_worm_ventral[INDEX_NR][k][howmanyfluids].vel_new_x = state_result_worm_ventral[INDEX_NR][k][howmanyfluids].vel_new_x*0.9999 ;
		
		state_result_worm_ventral[INDEX_NR][k][howmanyfluids].vel_new_y = state_result_worm_ventral[INDEX_NR][k][howmanyfluids].vel_new_y*0.9999 ;
		state_result_worm_ventral[INDEX_NR][k][howmanyfluids].vel_new_z = state_result_worm_ventral[INDEX_NR][k][howmanyfluids].vel_new_z*0.9999 ; */
		}

		

		state_result_worm_ventral[INDEX_NR][k][howmanyfluids].force_sign =  -1;
		state_result_worm_ventral[INDEX_NR][k][howmanyfluids]    =  worm_ventral(   state_result_worm_ventral[INDEX_NR][k][howmanyfluids] ,1) ;

		state_result_worm_ventral[llll][k][howmanyfluids].force_sign =   1; ; 
		state_result_worm_ventral[llll][k][howmanyfluids]    =  worm_ventral(   state_result_worm_ventral[llll][k][howmanyfluids] ,1);
 
	for(int d = 0; d <= totalneigbours[llll][howmanyfluids]; d++)
	{	 
		
		if(d == k  )
		  continue;
 
		INDEX_NRmore = state_result_worm_ventral_feather[llll][d][howmanyfluids].INDEX_NR;
  
		if(fabs(F_total[0][INDEX_NRmore][howmanyfluids][k]) > 21 )
	 				  F_total[0][INDEX_NRmore][howmanyfluids][k] =0;
 
		if(fabs(F_total[1][INDEX_NRmore][howmanyfluids][k])  > 21)
					  F_total[1][INDEX_NRmore][howmanyfluids][k] =0;

		if(fabs(F_total[2][INDEX_NRmore][howmanyfluids][k])  > 21)
					  F_total[2][INDEX_NRmore][howmanyfluids][k] =0;
			

 		if(fabs(F_total[0][INDEX_NR][howmanyfluids][k]) > 21 )
	 				  F_total[0][INDEX_NR][howmanyfluids][k] =0;
 
		if(fabs(F_total[1][INDEX_NR][howmanyfluids][k])  > 21)
					  F_total[1][INDEX_NR][howmanyfluids][k] =0;

		if(fabs(F_total[2][INDEX_NR][howmanyfluids][k])  > 21)
					  F_total[2][INDEX_NR][howmanyfluids][k] =0;
			
		springVector->pos_new_x = state_result_worm_ventral[INDEX_NR][k][howmanyfluids].pos_new_x - state_result_worm_ventral[INDEX_NRmore][k][howmanyfluids].pos_new_x;
		springVector->pos_new_y = state_result_worm_ventral[INDEX_NR][k][howmanyfluids].pos_new_y - state_result_worm_ventral[INDEX_NRmore][k][howmanyfluids].pos_new_y;
		springVector->pos_new_z = state_result_worm_ventral[INDEX_NR][k][howmanyfluids].pos_new_z - state_result_worm_ventral[INDEX_NRmore][k][howmanyfluids].pos_new_z;
 			

		float r = length(state_result_worm_ventral[INDEX_NR][k][howmanyfluids],state_result_worm_ventral[INDEX_NRmore][k][howmanyfluids]);

		if ( r != 0  &&   (r) < 10.1    )
		{	
 
			F_total[0][INDEX_NR][howmanyfluids][k] +=( springVector->pos_new_x  /  r ) * ( r - springlength ) * ( -springConstant );
			F_total[1][INDEX_NR][howmanyfluids][k] += ( springVector->pos_new_y /  r ) * ( r - springlength ) * ( -springConstant );
			F_total[2][INDEX_NR][howmanyfluids][k] +=( springVector->pos_new_z  /  r ) * ( r - springlength ) * ( -springConstant );

			F_total[0][INDEX_NRmore][howmanyfluids][k] +=( springVector->pos_new_x  / r ) * ( r - springlength ) * ( -springConstant );
			F_total[1][INDEX_NRmore][howmanyfluids][k] += ( springVector->pos_new_y  / r ) * ( r - springlength) * ( -springConstant );
			F_total[2][INDEX_NRmore][howmanyfluids][k] +=( springVector->pos_new_z  / r ) * ( r - springlength ) * ( -springConstant );

			F_total[0][INDEX_NR][howmanyfluids][k] +=-(  state_result_worm_ventral[INDEX_NR][k][howmanyfluids].vel_new_x - state_result_worm_ventral[INDEX_NRmore][k][howmanyfluids].vel_new_x ) * frictionConstant;
 			F_total[1][INDEX_NR][howmanyfluids][k] += -( state_result_worm_ventral[INDEX_NR][k][howmanyfluids].vel_new_y- state_result_worm_ventral[INDEX_NRmore][k][howmanyfluids].vel_new_y ) * frictionConstant;
 			F_total[2][INDEX_NR][howmanyfluids][k] += -(  state_result_worm_ventral[INDEX_NR][k][howmanyfluids].vel_new_z - state_result_worm_ventral[INDEX_NRmore ][k][howmanyfluids].vel_new_z ) * frictionConstant;


			F_total[0][INDEX_NRmore][howmanyfluids][k] +=-(  state_result_worm_ventral[INDEX_NR][k][howmanyfluids].vel_new_x - state_result_worm_ventral[INDEX_NRmore][k][howmanyfluids].vel_new_x ) * frictionConstant;
 			F_total[1][INDEX_NRmore][howmanyfluids][k] += -( state_result_worm_ventral[INDEX_NR][k][howmanyfluids].vel_new_y- state_result_worm_ventral[INDEX_NRmore][k][howmanyfluids].vel_new_y ) * frictionConstant;
 			F_total[2][INDEX_NRmore][howmanyfluids][k] += -(  state_result_worm_ventral[INDEX_NR][k][howmanyfluids].vel_new_z - state_result_worm_ventral[INDEX_NRmore ][k][howmanyfluids].vel_new_z ) * frictionConstant;

		/* state_result_worm_ventral[INDEX_NRmore][k][howmanyfluids].vel_new_x = state_result_worm_ventral[INDEX_NRmore][k][howmanyfluids].vel_new_x*0.9999 ;
		
		state_result_worm_ventral[INDEX_NRmore][k][howmanyfluids].vel_new_y = state_result_worm_ventral[INDEX_NRmore][k][howmanyfluids].vel_new_y*0.9999 ;

		state_result_worm_ventral[INDEX_NRmore][k][howmanyfluids].vel_new_z = state_result_worm_ventral[INDEX_NRmore][k][howmanyfluids].vel_new_z*0.9999 ;

		state_result_worm_ventral[INDEX_NR][k][howmanyfluids].vel_new_x = state_result_worm_ventral[INDEX_NR][k][howmanyfluids].vel_new_x*0.9999 ;
		
		state_result_worm_ventral[INDEX_NR][k][howmanyfluids].vel_new_y = state_result_worm_ventral[INDEX_NR][k][howmanyfluids].vel_new_y*0.9999 ;
		state_result_worm_ventral[INDEX_NR][k][howmanyfluids].vel_new_z = state_result_worm_ventral[INDEX_NR][k][howmanyfluids].vel_new_z*0.9999 ; */ 
		}



		state_result_worm_ventral[INDEX_NR][k][howmanyfluids].force_sign =  -1;
		state_result_worm_ventral[INDEX_NR][k][howmanyfluids]    =  worm_ventral(   state_result_worm_ventral[INDEX_NR][k][howmanyfluids] ,1) ;

		state_result_worm_ventral[INDEX_NRmore][k][howmanyfluids].force_sign =   1; ; 
		state_result_worm_ventral[INDEX_NRmore][k][howmanyfluids]    =  worm_ventral(   state_result_worm_ventral[INDEX_NRmore][k][howmanyfluids] ,1);
  
 	} 		 
 }	
 
		 
 }		
				 if(fabs(F_total[0][llll][howmanyfluids][k]) > 21 )
	 				 F_total[0][llll][howmanyfluids][k] =0;
 
		 		 if(fabs(F_total[1][llll][howmanyfluids][k])  > 21)
					 F_total[1][llll][howmanyfluids][k] =0;

			 	 if(fabs(F_total[2][llll][howmanyfluids][k])  > 21)
					 F_total[2][llll][howmanyfluids][k] =0;
 
}

int find_index_____(int NUM, double *pos   ,int num,int k, int howm )
{
int ll = 0;
 	while(ll < NUM)
	{

		if(wall_info[ll]->pos_new_x ==pos[0] && wall_info[ll]->pos_new_y ==pos[1]  && wall_info[ll]->pos_new_z ==pos[2] )
		{

			return ll;
		}

	ll++;
	}
}



int find_index (int NUM, struct state_vector ***real   ,int num,int k, int howm , int j)
{
int ll = 0;
 	while(ll < NUM)
	{
		if(state_result_worm_ventral[num][howm][j].pos_new_x ==real[ll][howm][j].pos_new_x && state_result_worm_ventral[num][howm][j].pos_new_y ==real[ll][howm][j].pos_new_y && 		state_result_worm_ventral[num][howm][j].pos_new_z ==real[ll][howm][j].pos_new_z)
		{
			//  printf("found index_____ @ %d \n", ll);
			return ll;
		}
	ll++;
	}
}


 void *ptree;
int TOTALFLAPPERS[HOW_MANY_FLUIDS] = {0,0};
  
	int ptreeonce = 1;
int findnearestpoint(int points,struct state_vector     *worm , int num,int howmany,int kk, int xxx) {
	int i, num_pts = points;
	int INDEX;
	char *data, *pch;
	struct kdres *presults;
	double pos[3], dist;
	double pt[3];

	pt[0] = wall_info[num]->pos_new_x;
	pt[1] = wall_info[num]->pos_new_y;
	pt[2] = wall_info[num]->pos_new_z;
	double radius = 2.0/1.0;
 
	num_pts =points;
	
	srand( time(0) );
if(ptreeonce == 1)
{
 	ptree = kd_create( 3 );
  	for( i=0; i<points; i++ ) {   
    		assert( 0 == kd_insert3( ptree,  wall_info[i]->pos_new_x,  wall_info[i]->pos_new_y,  wall_info[i]->pos_new_z, NULL ) );
 	 }
 
ptreeonce = 0;
}

 
	presults = kd_nearest_range( ptree, pt, radius );
  
 
	totaln[num]= kd_res_size(presults);
 
	int feather_count = 0;
 
while( !kd_res_end( presults ) ) {
   
	 pch = (char*)kd_res_item( presults, pos ); 
    	 dist = sqrt( dist_sq( pt, pos, 3 ) );

  
	 int k;
  
  
 	 INDEX = find_index_____(points,pos,num,feather_count,kk);
 
  	neigbours_wall[num][feather_count] = INDEX;
 
 	feather_count++;
        kd_res_next( presults );
 if(num % 1000 ==0)
 	printf("num @ %d \n", num);
}
 

 
  feather_count = 0;
 
 
  kd_res_free( presults );
 // kd_free( ptree );

  return 0;
}
 static void GlutIdle(void)			{glutPostRedisplay();}
void GlutNormalKeys(unsigned char key, int x, int y);
void GlutSpecialKeys(int key,int x,int y);
void GlutMouse(int a,int b,int c,int d);
int first_run = 1;
int initonce =1;
extern int KvvVENTAL ;
extern int KvvDORSAL ;

extern float* V; 
extern float* V2;

int main (int argc, char **argv)
{ 

 int i,j,k;  
loadOBJ__("s.obj");  
     
springVector = malloc(SIZE_OBJECT*1);
 
state_result_worm_ventral= (struct state_vector ***) malloc(sizeof(struct state_vector**) * SIZE_OBJECT); //FIX 1
for(i = 0; i < SIZE_OBJECT; i++){
 
  state_result_worm_ventral[i] = (struct state_vector **) malloc(sizeof(struct state_vector*) * SIZE_OBJECT);  // FIX 2
  for(j = 0; j < 12; j++){
 
        state_result_worm_ventral[i][j] = (struct state_vector *) malloc(sizeof(struct state_vector) * HOW_MANY_FLUIDS);
  }
}

 

state_result_worm_ventral_feather= (struct state_vector ***) malloc(sizeof(struct state_vector**) * SIZE_OBJECT); //FIX 1
for(i = 0; i < SIZE_OBJECT; i++){
 
  state_result_worm_ventral_feather[i] = (struct state_vector **) malloc(sizeof(struct state_vector*) * SIZE_OBJECT);  // FIX 2
  for(j = 0; j < 12; j++){
 
        state_result_worm_ventral_feather[i][j] = (struct state_vector *) malloc(sizeof(struct state_vector) * HOW_MANY_FLUIDS);
  }
}


reall= (struct state_vector ***) malloc(sizeof(struct state_vector**) * SIZE_OBJECT); //FIX 1
for(i = 0; i < SIZE_OBJECT; i++){
 
  reall[i] = (struct state_vector **) malloc(sizeof(struct state_vector*) * SIZE_OBJECT);  // FIX 2
  for(j = 0; j < 12; j++){
 
        reall[i][j] = (struct state_vector *) malloc(sizeof(struct state_vector) * HOW_MANY_FLUIDS);
  }
}
printf("done ! \n");
 
  
 
if(initonce==1)
{
	Vec3 *v ;
 
	int ll;

for(int kk = 0; kk < HOW_MANY_FLUIDS  ; kk++)
	{
	for(ll = 0; ll < SIZE_OBJECT  ; ll+=45       )
	{
		 
		  v= (Vec3 *)(V + 3*(KvvVENTALA[ll]-1));
         	  state_result_worm_ventral[c_m][0][kk].pos_new_x = v->x+kk*10 ;
 
 	          state_result_worm_ventral[c_m][0][kk].pos_new_y = v->y+13 ;
 		  state_result_worm_ventral[c_m][0][kk].pos_new_z = v->z +kk*10;
 
 	  	  state_result_worm_ventral[c_m][0][kk].vel_new_x = 8.04;
 		  state_result_worm_ventral[c_m][0][kk].vel_new_y = 0;
 		  state_result_worm_ventral[c_m][0][kk].vel_new_z =  0 ;
 
c_m++;
	 
}
}
 
 
	initonce = 0;
}

KvvVENTAL2=0;
  loadOBJ__big("tjockkorv3.obj");
  wall_info = (struct state_vector*)malloc(sizeof(struct state_vector*)*KvvVENTAL2*1);

 
 
for(int i=0; i<KvvVENTAL2*1; i++)
	wall_info[i] = (struct state_vector*)malloc(sizeof(struct state_vector)*1);
 
 	Vec3 *v ;  
	int ll;
	for(ll = 0; ll < KvvVENTAL2  ; ll+=10  )
	{
     	
		  v= (Vec3 *)(V2 + 3*(KvvVENTALA2[ll]-1));
         	  wall_info[cm2]->pos_new_x = v->x ;
 			 
 	          wall_info[cm2]->pos_new_y =  v->y ;
 		  wall_info[cm2]->pos_new_z = v->z ;
 
 	  	  wall_info[cm2]->vel_new_x =  0 ;
 		  wall_info[cm2]->vel_new_y = 0;
 		  wall_info[cm2]->vel_new_z =  0 ;
		  wall_info[cm2]->mass = ll;
	cm2++;
 if(ll % 10000==0)
     	 	  printf("%f::%f::%f  %d \n",wall_info[ll]->pos_new_x,wall_info[ll]->pos_new_y,wall_info[ll]->pos_new_z);
	}



for(ll = 0; ll < cm2; ll++ )
	{
 		findnearestpoint(KvvVENTAL2  ,NULL,ll,1,0,0 );
}
	//KvvVENTAL2 = cm2;
 
	glutInit(&argc, argv);
	init_mpgeg();
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	GLint glut_display;

	glutInitWindowSize (1080,1920);
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
	pipe = loadOBJ("18764_Common_N_Atlantic_Starfish_v1.obj");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	dirLight = (Light*)malloc(sizeof(Light));
	dirLight->setPosition_light = setPosition_light;
	dirLight->setAmbient_light = setAmbient_light;
	dirLight->setDifusse_light = setDifusse_light;
	dirLight->setSpecular_light = setSpecular_light;
 
	dirLight->setPosition_light(100,100, 800);
	dirLight->setAmbient_light(0, 0, 0);
	dirLight->setDifusse_light(1, 1, 1);
	dirLight->setSpecular_light(0.9999f, 0.1f, 0.88f);
 
	material =(Material*)malloc(sizeof(Material));
	material->setAmbient_material =setAmbient_material;
	material->setDifusse_material = setDifusse_material;
	material->setSpecular_material =setSpecular_material;
	material->setShininess_material = setShininess_material;
	material->setAmbient_material(0, 0, 1, 1);
	material->setDifusse_material(0.5f, 0.1f, 0.9999f, 1.0f/50.0f);
	material->setSpecular_material(0.01f, 0.01f, 0.01f, 1.0f/50.0f);
	material->setShininess_material(128);
	glLightfv(GL_LIGHT0, GL_POSITION, dirLight->pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT,   dirLight->ambient);
	glLightfv(GL_LIGHT0, GL_SPECULAR,  dirLight->specular);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,   dirLight->difusse);  


	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);  
 
	glutMouseFunc(mouse);
	glutKeyboardFunc(key);
  	
 
	glutMotionFunc(mouseMotion);
 
  	glutDisplayFunc(display);
 	glutReshapeFunc(reshape);

#ifdef USE_GLEW
    {
        GLenum err = glewInit();
        if( GLEW_OK != err ) {
            fprintf(stderr, "Error initializing GLEW: %s\n", glewGetErrorString(err) );
            return;
        }
    }
#endif 
 
glutMainLoop();

}

struct state_vector       worm_ventral(struct state_vector        next_state, int num )
{
	double f0[6];
	double f0_result[6];

	double z[6];
 	double t0[6] = {0,0,0,0,0,0};
 	double tburn = 0.01;
	double tf[6] = {tburn,tburn,tburn,tburn,tburn,tburn};

	z[0] =  next_state.pos_new_x;
	z[1] =  next_state.pos_new_y;
	z[2] = next_state.pos_new_z;
	z[3] =  next_state.vel_new_x;
	z[4] =  next_state.vel_new_y;
	z[5] = next_state.vel_new_z;

	f0[0] = next_state.pos_new_x;
	f0[1] =next_state.pos_new_y;
	f0[2] =next_state.pos_new_z;
	f0[3] =next_state.vel_new_x;
	f0[4] =next_state.vel_new_y;
	f0[5] = next_state.vel_new_z;
 
	rk45(rates_dorsal, t0, f0,f0_result, tf,z,6, 1  );

	next_state.pos_new_x = f0[0];
	next_state.pos_new_y = f0[1];
	next_state.pos_new_z = f0[2];

	next_state.vel_new_x = f0[3];
	next_state.vel_new_y = f0[4];
	next_state.vel_new_z = f0[5];

	return   next_state ;
}
 
void key(unsigned char key, int x, int y)
{
 
   if(key == 'x') cameraEye[0]-= 50;
   if(key == 'X') cameraEye[0]+= 50;
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
void vDrawScene(struct state_vector  *** worm,int j,int k) ;
int cool_once = 1;
void display  (void){
 
if(cool_once == 1)
{
 	unsigned char total_texture[4 * 256 * 256];
  	unsigned char alpha_texture[256 * 256];
  	unsigned char caustic_texture[3 * 256 * 256];
 

 	InitNoise ();
 
  	glClearColor (0, 0, 0,0);
 
  	glEnable (GL_BLEND);
  	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  	glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
  	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

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

cool_once = 0;
}
 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_COLOR_MATERIAL);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
 
    glFrustum(-1.0, 1.0, -1.0, 1.0, 3.0, 25900.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraEye[0]+0, cameraEye[1]+150, cameraEye[2]+930  , cameraLookAt[0], cameraLookAt[1], cameraLookAt[2], cameraUp[0], cameraUp[1], cameraUp[2]);
    glTranslatef(1.0, 0.0, 0.0);
 
 //for(int kk = 0; kk <   HOW_MANY_FLUIDS; kk++)  
 //	  {
 int kk = 0;
 for(int ll = 0; ll <  c_m; ll++)  
		  {

  
 int xx = 0;
  // for(int xx = 0; xx <  TOTALFLAPPERS[kk]; xx++)  
	//	  {
 int INDEX_NR___ =      state_result_worm_ventral_feather[ll][xx][kk].INDEX_NR ;

 taskworm(kk,ll,0);
      calc_fluid_velocity(INDEX_NR___,kk,xx);
 
 

       calc_fluid_acceleration(INDEX_NR___,kk,xx);
       glPushMatrix();
 
	 glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
 	glScalef(30,30,30);
  	glTranslatef(state_result_worm_ventral[INDEX_NR___][xx][kk].pos_new_x-0,   state_result_worm_ventral[INDEX_NR___][xx][kk].pos_new_y+0,state_result_worm_ventral[INDEX_NR___][xx][kk].pos_new_z);
 
        glutSolidSphere(0.15,20,20  );
 
glPopMatrix();


glPushMatrix();
     glColor3f(255,255,255);      
 
 
 glutPostRedisplay();
//}
//}
//}
}
   for(int ll = 0; ll <  cm2; ll+=10  )  
   {
		glPushMatrix();
		glScalef(30,30,30);
  		glTranslatef(wall_info[ll]->pos_new_x, wall_info[ll]->pos_new_y,wall_info[ll]->pos_new_z);


 		glutSolidSphere(0.04,12,12  );
 		glutPostRedisplay();
		glPopMatrix(); 
} 
	display_mpeg();
	glutSwapBuffers();
} 




void rates_dorsal ( double *t, double *f, double result[]   )
{
 
 
if(state_result_worm_ventral[llll][k_flappers][howmanyfluids_g].force_sign == 1)
{
    	result[0] =             state_result_worm_ventral[llll][k_flappers][howmanyfluids_g].vel_new_x/1.0;
    	result[1] =             state_result_worm_ventral[llll][k_flappers][howmanyfluids_g].vel_new_y/1.0;
    	result[2] =             state_result_worm_ventral[llll][k_flappers][howmanyfluids_g].vel_new_z/1.0;
  
	result[3] = (F_total[0][llll][howmanyfluids_g][k_flappers]+acc2_fluid[0][llll][howmanyfluids_g][k_flappers])/(1.0   );
	result[4] = (F_total[1][llll][howmanyfluids_g][k_flappers] +acc2_fluid[1][llll][howmanyfluids_g][k_flappers]    )/(1.0   )-9.8;
	result[5] = (F_total[2][llll][howmanyfluids_g][k_flappers] +acc2_fluid[2][llll][howmanyfluids_g][k_flappers])/(1.0); 

 
}
else if(state_result_worm_ventral[INDEX_NR][k_flappers][howmanyfluids_g].force_sign == -1)
{
    result[0] =             state_result_worm_ventral[INDEX_NR][k_flappers][howmanyfluids_g].vel_new_x/1.0;
    result[1] =             state_result_worm_ventral[INDEX_NR][k_flappers][howmanyfluids_g].vel_new_y/1.0;
    result[2] =             state_result_worm_ventral[INDEX_NR][k_flappers][howmanyfluids_g].vel_new_z/1.0;
  
    result[3] = -(F_total[0][INDEX_NR][howmanyfluids_g][k_flappers]+acc2_fluid[0][INDEX_NR][howmanyfluids_g][k_flappers])/(1.0  );
    result[4] = -(F_total[1][INDEX_NR][howmanyfluids_g][k_flappers]  +acc2_fluid[1][INDEX_NR][howmanyfluids_g][k_flappers] ) /(1.0) -9.8;
    result[5] = -(F_total[2][INDEX_NR][howmanyfluids_g][k_flappers] +acc2_fluid[2][INDEX_NR][howmanyfluids_g][k_flappers])/(1.0  ); //522
 

}
else if(state_result_worm_ventral[INDEX_NRmore][k_flappers][howmanyfluids_g].force_sign == 1)
{
    	result[0] =             state_result_worm_ventral[INDEX_NRmore][k_flappers][howmanyfluids_g].vel_new_x/1.0;
    	result[1] =             state_result_worm_ventral[INDEX_NRmore][k_flappers][howmanyfluids_g].vel_new_y/1.0;
    	result[2] =             state_result_worm_ventral[INDEX_NRmore][k_flappers][howmanyfluids_g].vel_new_z/1.0;
  
	result[3] = (F_total[0][INDEX_NRmore][howmanyfluids_g][k_flappers]+acc2_fluid[0][INDEX_NRmore][howmanyfluids_g][k_flappers])/(1.0   );
	result[4] = (F_total[1][INDEX_NRmore][howmanyfluids_g][k_flappers]  +acc2_fluid[1][INDEX_NRmore][howmanyfluids_g][k_flappers] )/(1.0     )-9.8;
	result[5] = (F_total[2][INDEX_NRmore][howmanyfluids_g][k_flappers] +acc2_fluid[2][INDEX_NRmore][howmanyfluids_g][k_flappers])/(1.0); 

 
}
  acc1 = sqrtf(pow(result[3],2.0) + pow(result[4],2.0)+pow(result[5],2.0));
 
}
 
void normalize(double *v)
{
	double length_Sqrtme =  (v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);


		double complex zz;
	double addme;
	double ii;
 
	if((length_Sqrtme)  < 0)
	  {
 
 	 	   zz = csqrt(length_Sqrtme);
	 	 addme = creal(zz);
	}
	else if ((length_Sqrtme)  > 0)
		addme = sqrtf(length_Sqrtme );

	for(int i = 0; i < 3; i++)
	{
		v[i] = v[i]/addme;
	}
}
 
void cross_product(double *a, double *b,double * result)
{
   result[0] =    a[1]*b[2]-a[2]*b[1];
   result[1] =   (a[2]*b[0] - a[0]*b[2]);
   result[2] =    a[0]*b[1] - a[1]*b[0] ;
//printf("result[0] = %f  result[1] = %f\n", result[0],result[1]);
   normalize(result);
}
 
void calculate_normal(double*a, double* b, double * c,double *result)
{
	double x[] = {b[0]-a[0],b[1]-a[1],b[2]-a[2]};
	double y[] = {c[0]-a[0],c[1]-a[1],c[2]-a[2]};
 
//printf("x[0] = %f :: y[0] = %f\n", x[0],y[0]);
	 cross_product(x,y,result);
}
 
double * dot(double *a, double *b , double *c )
{
	c[0] = a[0]*b[0]+a[1]*b[1]+a[2]*b[2];
  	  
}
int pressure_once = 1;
void calc_fluid_acceleration(int ll, int j,int xxx)
{
	  density = 1000*total_molekules_nearby;
 	  Area_tube = powf(water_radius,2)*M_PI;
	  mass_fluid_element =  density*(M_PI*powf(water_radius,3.0))*4.0/3.0;

	  double gMASS;
        if(pressure_once ==1)
	{
		state_result_worm_ventral[ll][j]->pressure =  ((   9.8+0)*mass_fluid_element/(M_PI*pow(water_radius,2.0)));
		//pressure_once =0;
	}

	double sqrtme = state_result_worm_ventral[ll][j]->pressure/density;

	double complex zz;
	double addme;
	double ii;
	double zzz;
	if((sqrtme)  < 0)
	  {
 	 	 double zzz = csqrt(sqrtme);
	 	 addme = creal(zzz);
	}
	else if ((sqrtme)  > 0)
		addme = sqrtf(sqrtme );

    acc2_fluid[0][ll][j][xxx] = (-1*(state_result_worm_ventral[ll][xxx][j].pressure )/(( state_result_worm_ventral[ll][xxx][j].pos_new_x)))/density + (viscous_constant*Area_tube*addme)/mass_fluid_element;
    acc2_fluid[1][ll][j][xxx] = (-1*(state_result_worm_ventral[ll][xxx][j].pressure )/(( state_result_worm_ventral[ll][xxx][j].pos_new_y)))/density + (viscous_constant*Area_tube*addme)/mass_fluid_element;
    acc2_fluid[2][ll][j][xxx] = (-1*(state_result_worm_ventral[ll][xxx][j].pressure )/(( state_result_worm_ventral[ll][xxx][j].pos_new_z)))/density + (viscous_constant*Area_tube*addme)/mass_fluid_element;
}
int largestFloat(float array[], int length){     // return type is int
    float max = array[0];                        // max is float
    int i;

    for( i=1; i<length; i++){
         if(array[i] > max){
               max = array[i];
         }
    }

    return max;
} 
int alloc_once_2 =1;
double dist__[10000];
void *ptree_____;
 int result________;
void findnearestpoint_3_points (int points , int num,double r_collision, struct state_vector **vvv,int ll, int j )
{
	int i, num_pts ;
 	int result;
	char  *pch;
	struct kdres *presults;
	double pos[3], dist;
	//double *pt;
	double pt[3];
	pt[0] = state_result_worm_ventral[num][0]->pos_new_x;
	pt[1] = state_result_worm_ventral[num][0]->pos_new_y;
	pt[2] = state_result_worm_ventral[num][0]->pos_new_z;

	double radius =r_collision ;
 
	num_pts =points;
 
 	if(alloc_once == 1 || alloc_once_2 == 1)
	{
		ptree_____ = kd_create( 3 );
 
  		for( i=0; i<KvvVENTAL2; i++ ) {   
		       //  printf("target node at (%.3f, %.3f, %.3f) \n", wall_info[i]->pos_new_x, wall_info[i]->pos_new_y,wall_info[i]->pos_new_z);
    			assert( 0 == kd_insert3( ptree_____, wall_info[i]->pos_new_x, wall_info[i]->pos_new_y, wall_info[i]->pos_new_z, NULL ) );

	}
		alloc_once = 0;
alloc_once_2 = 0;
	}
 

 presults = kd_nearest_range( ptree_____, pt, radius );
   		result________ = kd_res_size(presults) ;
result = kd_res_size(presults) ;
  printf("presults = %d \n", result);
double DECREASER = 0.975;
 
 	int vcc =0;
        int vc = 0;
 
 
  while( !kd_res_end( presults ) ) {
   
	 pch = (char*)kd_res_item( presults, pos ); 
    	 dist__[vcc] = sqrt( dist_sq( pt, pos, 3 ) );

 
   		neighbourV_[vcc][0]  = pos[0];
  		neighbourV_[vcc][1]  = pos[1];
   		neighbourV_[vcc][2]  = pos[2];
vcc+=1;
 
 vc++;
   		 kd_res_next( presults );
    	  }
    

   vc = 0;
   kd_res_free( presults );
 
//free(pt);

}

double tdiff[600000][2] ;
double ratiotime[600000][2];

double totaltime[600000];
double disttime[600000];
double time__[600000];
	
int INIT_ALLOCATE[2] = {1,1};


void findnearestpoint_3_points_water_pressure(int points , int num,double r_collision, double **vvv,int ll, int j,int xxx )
{
	int i, num_pts ;
void *ptree_______[600000];
	char  *pch;
	struct kdres *presults;
	
	double pos[3], dist,vel;
	double pt[3] = { state_result_worm_ventral[ll][xxx][j].pos_new_x,state_result_worm_ventral[ll][xxx][j].pos_new_y, state_result_worm_ventral[ll][xxx][j].pos_new_z };
	double velt[3] = { state_result_worm_ventral[ll][xxx][j].vel_new_x,state_result_worm_ventral[ll][xxx][j].vel_new_y, state_result_worm_ventral[ll][xxx][j].vel_new_z };
	double radius = r_collision;
 
	num_pts =SIZE_OBJECT;
 
   if(tdiff[ll][j] > ratiotime[ll][j] || INIT_ALLOCATE[j] == 1    )
{
	tdiff[ll][j]= 0;	
 
	int idx = ll ; 
	for( i=0; i<points; i++ ) {   
 
	int idx = i ; 
	ptree_______[i] = kd_create( 3 );
 
    	assert( 0 == kd_insert3( ptree_______[idx], state_result_worm_ventral[ll][xxx][j].pos_new_x, state_result_worm_ventral[ll][xxx][j].pos_new_y, state_result_worm_ventral[ll][xxx][j].pos_new_z, NULL ) );
 
 
	 }
	INIT_ALLOCATE[j] = 0;
	}
else
{
	tdiff[ll][j]+=0.001;
	return;
}
 
 
	presults = kd_nearest_range( ptree_______[ll], pt, radius );
 
 	// printf( "found %d results:\n", kd_res_size(presults) );

 
 	int INDEX___;
	total_molekules_nearby = kd_res_size(presults);
        int vc = 0;
	TOTALFLAPPERS[j] = kd_res_size(presults);
 
        while( !kd_res_end( presults ) ) {
   
		 pch = (char*)kd_res_item( presults, pos ); 
    		 dist = sqrt( dist_sq( pt, pos, 3 ) );
 
 		reall[ll][vc][j].pos_new_x = pos[0];
 		reall[ll][vc][j].pos_new_y  = pos[1];
 		reall[ll][vc][j].pos_new_z  = pos[2];
                INDEX___ = find_index (c_m,reall,ll,total_molekules_nearby,vc, j);
 
	 	totalneigbours[ll][j] =total_molekules_nearby;
 
 
  		state_result_worm_ventral_feather[ll][vc][j].INDEX_NR = INDEX___;
 	 
		if(INDEX___ == c_m) break; 
		 vel  = sqrtf(powf( state_result_worm_ventral[ll][vc][j].vel_new_x - state_result_worm_ventral[INDEX___][vc][j].vel_new_x ,2.0) +  powf( state_result_worm_ventral[ll][vc][j].vel_new_y - 			 state_result_worm_ventral[INDEX___][vc][j].vel_new_y ,2.0)  + powf( state_result_worm_ventral[ll][vc][j].vel_new_z - state_result_worm_ventral[INDEX___][vc][j].vel_new_z ,2.0  ));
		
 
		 time__[vc] = (r_collision - dist)/vel;
		 totaltime[vc] = 	r_collision/vel;

		 disttime[vc] = 	dist/vel;
 
    		 neighbourV_water[vc][0]  = pos[0];
  		 neighbourV_water[vc][1]  = pos[1];
 		 neighbourV_water[vc][2]  = pos[2];
 
   		 vc++;
 
   		 kd_res_next( presults );
    	  }
     for(int g = 1; g < total_molekules_nearby; ++g)
    {
       	if(time__[0] > time__[g])
           	time__[0] = time__[g];
    }
 
	ratiotime[ll][j] =   time__[0];
 
   	kd_res_free( presults );
 for( i=0; i<points; i++ ) {   
    kd_free( ptree_______[i] );
 }
}
 
 int once_malloc_n = 1;
			double * normal  ;
			double * dotprod_v_n  ;
			double * dotprod_n_n ;
			double * vel  ;
			double * cross  ;
			double *dir1  ;
			double *dir2  ;
 	double *result2 ;
	double *result  ;
			double* pos_new  ;
 

 /*
 :algoritmen:
1. leta upp alla närmaste grannar till alla väggpartiklar
2. loopa igenom alla väggpartiklar och använd grannarna till detta
3. hitta avståndet ifrån fluidpartikeln till en av väggpartiklarna
4. undersök antalet av dess grannar bestäm minsta och största avtånd till dem
5. Bestäm sedan avståndet mellan max och min grannarna, ta den som ger störst avstånd, spara index för bägge grannarna.
6. beräkna normalen mellan fluidenpartikeln och dessa två grannarna
7. beräkna dot produkten mellan fluidenpartikeln hastighet och normalen
8. Ta sen multiplikationen av normalen och resultat vektorn
9. detta resulterar till en "glidningsvektor" för fluiden.

*/
			double *aa  ;
			double *bb ; 
			double *cc ;
int returc=0;
double xxxx;
void calc_fluid_velocity(int ll, int j, int xxx)
{
	int vn = ll;
	double distance_from_origin = sqrtf(powf( state_result_worm_ventral[ll][k_flappers][j].pos_new_x - 0 ,2.0) +  powf( state_result_worm_ventral[ll][k_flappers][j].pos_new_y -0 ,2.0)  + 				powf( state_result_worm_ventral[ll][k_flappers][j].pos_new_z -0 ,2.0  ));
 

 
 for(int n = 0; n < cm2; n++)
	{
		 
 double sqrtthis2 = sqrtf(powf( state_result_worm_ventral[ll][k_flappers][j].pos_new_x - wall_info[n]->pos_new_x ,2.0) +  powf( state_result_worm_ventral[ll][k_flappers][j].pos_new_y - 	wall_info[n]->pos_new_y ,2.0)  + powf( state_result_worm_ventral[ll][k_flappers][j].pos_new_z - wall_info[n ]->pos_new_z ,2.0  ));


if(sqrtthis2 < 2 && sqrtthis2 > 1)
{
	double distance_from_origin_mini = sqrtf(powf( - state_result_worm_ventral[ll][k_flappers][j].pos_new_x    ,2.0) +  powf( state_result_worm_ventral[ll][k_flappers][j].pos_new_y-13.0   ,2.0)  + 				powf( state_result_worm_ventral[ll][k_flappers][j].pos_new_z -0 ,2.0  ));


double circle_distance_to_edge = sqrtf(powf((state_result_worm_ventral[ll][k_flappers][j].pos_new_x+0),2.0)+powf((state_result_worm_ventral[ll][k_flappers][j].pos_new_y-distance_from_origin_mini),2.0)  );
 

		double R = 13.0/1.1653;
		double r = 4.661/2.0;
		double torus_volume_max  = (2*pow(M_PI,2.0)*R*pow(r,2.0));
		double torus_volume_min  = (2*pow(M_PI,2.0)*R*pow(r/15.0,2.0));
 
 
			double u_perpv[3];
			double w_parv[3]; 

 
if( once_malloc_n == 1)
{
			 normal = malloc(100) ;
			 dotprod_v_n= malloc(100) ;
			   dotprod_n_n= malloc(100) ;
			 vel = malloc(100);
			  cross = malloc(100) ;
			  dir1 = malloc(20);
			 dir2 =malloc(20);
 	 result2 = malloc(20);
	 result = malloc(20);
		 pos_new  = malloc(20);
 


			 aa = malloc(100);
			 bb = malloc(100); 
			 cc = malloc(100);

once_malloc_n = 0;
}
 
			aa[0] = state_result_worm_ventral[ll][k_flappers][j].pos_new_x;
			aa[1] = state_result_worm_ventral[ll][k_flappers][j].pos_new_y;
                        aa[2] = state_result_worm_ventral[ll][k_flappers][j].pos_new_z;
 
double dist__[totaln[n]+1];
int first_point =1;
 
 int index_wall[totaln[n]];
 
for (  i = 0; i < totaln[n]+1; ++i)
{
index_wall[i] = neigbours_wall[n][i];
double sqrtthis__;
double pos[3] = {wall_info[index_wall[i]]->pos_new_x,wall_info[index_wall[i]]->pos_new_y,wall_info[index_wall[i]]->pos_new_z};
if(first_point == 1)
{
	  sqrtthis__ = powf( aa[0] - wall_info[n]->pos_new_x ,2.0) +  powf( aa[1]- wall_info[n]->pos_new_y ,2.0)  + powf( aa[2] - wall_info[n]->pos_new_z ,2.0  );
first_point = 0;
}
else
{

  sqrtthis__ = powf( aa[0] - wall_info[index_wall[i]]->pos_new_x ,2.0) +  powf( aa[1]- 	wall_info[index_wall[i]]->pos_new_y ,2.0)  + powf( aa[2] - wall_info[index_wall[i]]->pos_new_z ,2.0  );
}
dist__[i] = sqrtf(sqrtthis__);

neighbourV_[i][0] =pos[0];
neighbourV_[i][1] =pos[1];
neighbourV_[i][2] =pos[2];
 }


	float minimum = dist__[0];
int i;
 
int index2 = 0;
 
	float maximum = dist__[0];
for (  i = 0; i < totaln[n]+1; ++i)
{
  if (dist__[i] > maximum)
  {
    maximum = dist__[i];
   index2 = i;
  }
}
 
double sqrtthis_[totaln[n]];
for (  i = 0; i < totaln[n]+1; ++i)
{

if(index_wall[i] != i)
 sqrtthis_[i] = sqrtf(powf( neighbourV_[index2][0] - neighbourV_[i][0] ,2.0) +  powf(neighbourV_[index2][1] - neighbourV_[i][1] ,2.0)  + powf(neighbourV_[index2][2] -neighbourV_[i][2] ,2.0  ));


}

 float min_dist  = sqrtthis_[0];


int index4 =6;
float max_dist = sqrtthis_[0];
int index3 = 3;
 

 


 
 
 //printf("maximum = %f :: max_dist = %f  index2 = %d :: index3 = %d :: %f :: %f :: %d\n", maximum, max_dist,index2,index3,neighbourV_[index3][0],neighbourV_[index2][0],totaln[n]);
//printf("neigbours_wall[n][0] = %d :: %d \n",neigbours_wall[n][0],neigbours_wall[n][2]);

			bb[0] = wall_info[neigbours_wall[n][index4]]->pos_new_x;
			bb[1] =wall_info[neigbours_wall[n][index4]]->pos_new_y;
                        bb[2] = wall_info[neigbours_wall[n][index4]]->pos_new_z;


			cc[0] = wall_info[neigbours_wall[n][index3]]->pos_new_x;
			cc[1] =wall_info[neigbours_wall[n][index3]]->pos_new_y;
                        cc[2] =wall_info[neigbours_wall[n][index3]]->pos_new_z;
  //printf("index2 = %d :: index3 = %d :: num = %d :: totalt antal grannar =%d :: KvvVENTAL2 =%d\n", neigbours_wall[n][index2], neigbours_wall[n][index3],n,totaln[n],KvvVENTAL2);
 
			/* aa[0] = wall_info[neigbours_wall[n][index2]]->pos_new_x;
			aa[1] =wall_info[neigbours_wall[n][index2]]->pos_new_y;
                        aa[2] = wall_info[neigbours_wall[n][index2]]->pos_new_z;
	 */
 			vel[0] =  state_result_worm_ventral[ll][k_flappers][j].vel_new_x ;
			vel[1] = state_result_worm_ventral[ll][k_flappers][j].vel_new_y;
			vel[2] =state_result_worm_ventral[ll][k_flappers][j].vel_new_z;
 
 //printf("bb[0] = %f  cc[0]= %f aa[0] = %f distance = %f :: number =%d\n", bb[0],cc[0], aa[0], sqrtf(powf(bb[0] - cc[0] ,2.0) +  powf(bb[1] - cc[1] ,2.0)  + powf(bb[2] -cc[2] ,2.0  )),n );
//if(n == 2000) //bb[0] = 12.711427  cc[0]= 9.861178 distance = 3.499536 :: number =2000
//for(;;);
			//v′ = f w − r u
 
 		


 
		double sqrtthis = powf( state_result_worm_ventral[ll][k_flappers][j].pos_new_x - wall_info[neigbours_wall[n][index4]]->pos_new_x ,2.0) +  powf( state_result_worm_ventral[ll][k_flappers][j].pos_new_y - 	wall_info[neigbours_wall[n][index4]]->pos_new_y ,2.0)  + powf( state_result_worm_ventral[ll][k_flappers][j].pos_new_z - wall_info[neigbours_wall[n][index4]]->pos_new_z ,2.0  );
 
		  xxxx=  (sqrtthis ); 
  		complex zzzz;
 

 		 if((sqrtthis)  < 0)
	  	{
 			zzzz = csqrt(sqrtthis);
		  	xxxx = creal(zzzz);
		}
		else if ((sqrtthis)  > 0)	  
			xxxx = sqrtf(sqrtthis );

	calculate_normal(aa ,bb ,cc ,normal);
 
          		dot(vel,normal,result2);
			double result3[3];
			result3[0]  = normal[0]* result2[0];
			result3[1]   = normal[1]* result2[0];
			result3[2]   = normal[2]* result2[0];
 if(fabs(normal[0]) > 0 && fabs(normal[1]) > 0 &&fabs(normal[2]) > 0 && xxxx < 1.2)
{
			state_result_worm_ventral[ll][k_flappers][j].vel_new_x  -=1*result3[0] ;
			state_result_worm_ventral[ll][k_flappers][j].vel_new_y  -=1*result3[1] ;
			state_result_worm_ventral[ll][k_flappers][j].vel_new_z  -=1*result3[2] ;
 printf("xxxx= %f :: posx = %f :: posx_wall =%f :: index2 = %d\n", xxxx,state_result_worm_ventral[ll][k_flappers][j].pos_new_x, wall_info[neigbours_wall[n][index4]]->pos_new_x,index2);	
}
 
  returc++;
  
	 
 
 glutPostRedisplay();
    
 
 
  }
}
 // taskworm(j,ll,xxx);	
}
 
