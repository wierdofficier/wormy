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
#include <complex.h>
#include "system.h"
#include <jpeglib.h>
#include <jerror.h>
#include "noise.h"
#include "kdtree.h"
int pipe;

#define SIZE_OBJECT      10000
#define NUMBER_FLUIDS    10

 double acc2_fluid[3][10050*2];
double water_radius =0.002;
double density =   1000.0;
double viscous_constant = 1.6735;
 
 
const double PI          = 3.141592653589793;
const double HALF_PI     = 1.570796326794897;
const double DOUBLE_PI   = 6.283185307179586;
const double SIN_CURVE_A = 0.0415896;
const double SIN_CURVE_B = 0.00129810625032;
 
 

double cos1(double x) {
    if (x < 0) {
        int q = -x / DOUBLE_PI;
        q += 1;
        double y = q * DOUBLE_PI;
        x = -(x - y);
    }
    if (x >= DOUBLE_PI) {
        int q = x / DOUBLE_PI;
        double y = q * DOUBLE_PI;
        x = x - y;
    }
    int s = 1;
    if (x >= PI) {
        s = -1;
        x -= PI;
    }
    if (x > HALF_PI) {
        x = PI - x;
        s = -s;
    }
}

double sin1(double x) {
    return cos1(x - HALF_PI);
}


#define NUM_THREADS 5
#include <stdbool.h>
float acc1;
float acc2;
int INDEX_NRmore;
#define SIZE_OBJECT 975 //126976
float springlength =  0.004/4.0;
#define AIR_FRICTION 0.9959999
#define FIRST 1
#define WORK 0
float fovy = 45.0;
float dNear = 100;
float dFar = 2000;
double F_total[3][SIZE_OBJECT*4][100] ;
float   INDEX_pos[SIZE_OBJECT*2][3][25][6] ;
int totalneigbours[SIZE_OBJECT*4][1000];
int INDEX_from_find[SIZE_OBJECT ][100][100];
float     vertexpoint_g_ventral[SIZE_OBJECT][3];
struct state_vector *  springVector;
struct state_vector ** state_result_worm_ventral_feather;
struct state_vector  ** state_result_worm_ventral ;
extern int KvvVENTALA[SIZE_OBJECT*8];
int llll = 0;
int i; 
int feather_once = 1;
int INDEX_NR = 0;
Light *dirLight;
Material *material;
int totalnum = 0;
extern int KvvVENTAL ;
float springConstant =   200.202f;
double  neighbourV_[10000*2][10000] ;
float frictionConstant =    0.2200000f;
  struct state_vector  ** wall_info ; 
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
int findnearestpoint(int points,struct state_vector ** worm , int num,int howmany,int k);
void display  (void);
void   worm_ventral(struct state_vector     **  next_state, int num, int k );
double length (struct state_vector *a,struct state_vector *b, int k ) { return sqrtf ( b[k].pos_new_x*a[k].pos_new_x + b[k].pos_new_y*a[k].pos_new_y + b[k].pos_new_z*a[k].pos_new_z); }
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

int acc1once= 1;
int what_k = 0;
void taskworm(int k , int lllll )
{
 
what_k = k;
llll = lllll;
int kk;
for(kk = 0; kk <= totalneigbours[llll][k]; kk++)
{	// m*a = -k*s
 	//10*a= -k * 0.004 solve k
if(acc1once == 0)
	{
	if(fabs(acc1) > 0)
		{		
		springConstant =   275.0/1 *1;
//	printf("springConstant = %.10f \n", springConstant);
		 frictionConstant = 2.2;
	}
	}
 
acc1once = 0;	
	 
	INDEX_NR =  ( INDEX_from_find[llll][kk][k]);
 	// printf("  %d :: number %d :: index_____ = %d   k = %d\n", INDEX_from_find[llll][kk][k],llll,INDEX_NR,k);

if(  INDEX_NR > 0    )
{
	//if(state_result_worm_ventral[INDEX_NR][k].force_sign != -1 && state_result_worm_ventral[INDEX_NR][k].force_sign != 1 && INDEX_NR >0)
	//{

		 if( INDEX_NR > 0)
		{	

						 if(fabs(F_total[0][INDEX_NR][k]) > 21 )
	 				 	  F_total[0][INDEX_NR][k] =0;
 
		 		 if(fabs(F_total[1][INDEX_NR][k])  > 21)
						F_total[1][INDEX_NR][k] =0;

			 	 if(fabs(F_total[2][INDEX_NR][k])  > 21)
						F_total[2][INDEX_NR][k] =0;
//printf("5 %d %d %d\n",INDEX_NR,llll,k);	
 
		springVector->pos_new_x = state_result_worm_ventral[INDEX_NR][k].pos_new_x - state_result_worm_ventral[llll][k].pos_new_x;
		springVector->pos_new_y = state_result_worm_ventral[INDEX_NR][k].pos_new_y - state_result_worm_ventral[llll][k].pos_new_y;
		springVector->pos_new_z = state_result_worm_ventral[INDEX_NR][k].pos_new_z - state_result_worm_ventral[llll][k].pos_new_z;
 

		float r = length(state_result_worm_ventral[INDEX_NR],state_result_worm_ventral[llll],k);
 
		if ( r != 0  &&   r < 50  )
		{			
			F_total[0][INDEX_NR][k] +=( springVector->pos_new_x  /  r ) * ( r - springlength ) * ( -springConstant );
			F_total[1][INDEX_NR][k] += ( springVector->pos_new_y /  r ) * ( r - springlength ) * ( -springConstant );
			F_total[2][INDEX_NR][k] +=( springVector->pos_new_z  /  r ) * ( r - springlength ) * ( -springConstant );

			F_total[0][llll][k] +=( springVector->pos_new_x  / r ) * ( r - springlength ) * ( -springConstant );
			F_total[1][llll][k] += ( springVector->pos_new_y  / r ) * ( r - springlength) * ( -springConstant );
			F_total[2][llll][k] +=( springVector->pos_new_z  / r ) * ( r - springlength ) * ( -springConstant );

		F_total[0][INDEX_NR][k] +=-(  state_result_worm_ventral[INDEX_NR][k].vel_new_x - state_result_worm_ventral[llll][k].vel_new_x ) * frictionConstant;
 		F_total[1][INDEX_NR][k] += -( state_result_worm_ventral[INDEX_NR][k].vel_new_y- state_result_worm_ventral[llll][k].vel_new_y ) * frictionConstant;
 		F_total[2][INDEX_NR][k] += -(  state_result_worm_ventral[INDEX_NR][k].vel_new_z - state_result_worm_ventral[llll ][k].vel_new_z ) * frictionConstant;
 

		F_total[0][llll][k] +=-(  state_result_worm_ventral[INDEX_NR][k].vel_new_x - state_result_worm_ventral[llll][k].vel_new_x ) * frictionConstant;
 		F_total[1][llll][k] += -( state_result_worm_ventral[INDEX_NR][k].vel_new_y- state_result_worm_ventral[llll][k].vel_new_y ) * frictionConstant;
 		F_total[2][llll][k] += -(  state_result_worm_ventral[INDEX_NR][k].vel_new_z - state_result_worm_ventral[llll ][k].vel_new_z ) * frictionConstant;
	/*	state_result_worm_ventral[llll][k].vel_new_x = state_result_worm_ventral[llll][k].vel_new_x*0.995 ;
		
		state_result_worm_ventral[llll][k].vel_new_y = state_result_worm_ventral[llll][k].vel_new_y*0.995 ;

		state_result_worm_ventral[llll][k].vel_new_z = state_result_worm_ventral[llll][k].vel_new_z*0.995 ;

		state_result_worm_ventral[INDEX_NR][k].vel_new_x = state_result_worm_ventral[INDEX_NR][k].vel_new_x*0.995 ;
		
		state_result_worm_ventral[INDEX_NR][k].vel_new_y = state_result_worm_ventral[INDEX_NR][k].vel_new_y*0.995 ;
		state_result_worm_ventral[INDEX_NR][k].vel_new_z = state_result_worm_ventral[INDEX_NR][k].vel_new_z*0.995 ;*/
		}

		

		state_result_worm_ventral[INDEX_NR][k].force_sign =  -1;
		 worm_ventral(   &state_result_worm_ventral[INDEX_NR] ,1,k) ;

		state_result_worm_ventral[llll][k].force_sign =   1; ; 
		  worm_ventral(   &state_result_worm_ventral[llll]  ,1,k);
		


 

	for(int d = 0; d <= totalneigbours[llll][k]; d++)
	{	 
		
		if(d == kk  )
		  continue;

 
   // state_result_worm_ventral_feather[llll][d].INDEX_NR = find_index_____(SIZE_OBJECT,state_result_worm_ventral_feather,llll,d);
INDEX_NRmore =  INDEX_from_find[llll][d][k];
 

		 

 

				 if(fabs(F_total[0][INDEX_NRmore][k]) > 21 )
	 				  F_total[0][INDEX_NRmore][k] =0;
 
		 		 if(fabs(F_total[1][INDEX_NRmore][k])  > 21)
					  F_total[1][INDEX_NRmore][k] =0;

			 	 if(fabs(F_total[2][INDEX_NRmore][k])  > 21)
					  F_total[2][INDEX_NRmore][k] =0;
			

 				if(fabs(F_total[0][INDEX_NR][k]) > 21 )
	 				  F_total[0][INDEX_NR][k] =0;
 
		 		 if(fabs(F_total[1][INDEX_NR][k])  > 21)
					  F_total[1][INDEX_NR][k] =0;

			 	 if(fabs(F_total[2][INDEX_NR][k])  > 21)
					  F_total[2][INDEX_NR][k] =0;
			
		springVector->pos_new_x = state_result_worm_ventral[INDEX_NR][k].pos_new_x - state_result_worm_ventral[INDEX_NRmore][k].pos_new_x;
		springVector->pos_new_y = state_result_worm_ventral[INDEX_NR][k].pos_new_y - state_result_worm_ventral[INDEX_NRmore][k].pos_new_y;
		springVector->pos_new_z = state_result_worm_ventral[INDEX_NR][k].pos_new_z - state_result_worm_ventral[INDEX_NRmore][k].pos_new_z;
 			

		float r = length(state_result_worm_ventral[INDEX_NR],state_result_worm_ventral[INDEX_NRmore],k);

		if ( r != 0  &&   r < 50    )
		{	
 
			F_total[0][INDEX_NR][k] +=( springVector->pos_new_x  /  r ) * ( r - springlength ) * ( -springConstant );
			F_total[1][INDEX_NR][k] += ( springVector->pos_new_y /  r ) * ( r - springlength ) * ( -springConstant );
			F_total[2][INDEX_NR][k] +=( springVector->pos_new_z  /  r ) * ( r - springlength ) * ( -springConstant );

			F_total[0][INDEX_NRmore][k] +=( springVector->pos_new_x  / r ) * ( r - springlength ) * ( -springConstant );
			F_total[1][INDEX_NRmore][k] += ( springVector->pos_new_y  / r ) * ( r - springlength) * ( -springConstant );
			F_total[2][INDEX_NRmore][k] +=( springVector->pos_new_z  / r ) * ( r - springlength ) * ( -springConstant );

		F_total[0][INDEX_NR][k] +=-(  state_result_worm_ventral[INDEX_NR][k].vel_new_x - state_result_worm_ventral[INDEX_NRmore][k].vel_new_x ) * frictionConstant;
 		F_total[1][INDEX_NR][k] += -( state_result_worm_ventral[INDEX_NR][k].vel_new_y- state_result_worm_ventral[INDEX_NRmore][k].vel_new_y ) * frictionConstant;
 		F_total[2][INDEX_NR][k] += -(  state_result_worm_ventral[INDEX_NR][k].vel_new_z - state_result_worm_ventral[INDEX_NRmore ][k].vel_new_z ) * frictionConstant;


		F_total[0][INDEX_NRmore][k] +=-(  state_result_worm_ventral[INDEX_NR][k].vel_new_x - state_result_worm_ventral[INDEX_NRmore][k].vel_new_x ) * frictionConstant;
 		F_total[1][INDEX_NRmore][k] += -( state_result_worm_ventral[INDEX_NR][k].vel_new_y- state_result_worm_ventral[INDEX_NRmore][k].vel_new_y ) * frictionConstant;
 		F_total[2][INDEX_NRmore][k] += -(  state_result_worm_ventral[INDEX_NR][k].vel_new_z - state_result_worm_ventral[INDEX_NRmore ][k].vel_new_z ) * frictionConstant;

		/*state_result_worm_ventral[INDEX_NRmore][k].vel_new_x = state_result_worm_ventral[INDEX_NRmore][k].vel_new_x*0.995 ;
		
		state_result_worm_ventral[INDEX_NRmore][k].vel_new_y = state_result_worm_ventral[INDEX_NRmore][k].vel_new_y*0.995 ;

		state_result_worm_ventral[INDEX_NRmore][k].vel_new_z = state_result_worm_ventral[INDEX_NRmore][k].vel_new_z*0.995 ;

		state_result_worm_ventral[INDEX_NR][k].vel_new_x = state_result_worm_ventral[INDEX_NR][k].vel_new_x*0.995 ;
		
		state_result_worm_ventral[INDEX_NR][k].vel_new_y = state_result_worm_ventral[INDEX_NR][k].vel_new_y*0.995 ;
		state_result_worm_ventral[INDEX_NR][k].vel_new_z = state_result_worm_ventral[INDEX_NR][k].vel_new_z*0.995 ;*/
		}



		state_result_worm_ventral[INDEX_NR][k].force_sign =  -1;
		  worm_ventral(   &state_result_worm_ventral[INDEX_NR]   ,1,k) ;

		state_result_worm_ventral[INDEX_NRmore][k].force_sign =   1; ; 
		  worm_ventral(   &state_result_worm_ventral[INDEX_NRmore]   ,1,k);
	//	 printf("f[0] = %.10f:%.10f%.10f \n", state_result_worm_ventral[INDEX_NRmore][k].pos_new_x,state_result_worm_ventral[INDEX_NRmore][k].pos_new_y,state_result_worm_ventral[INDEX_NRmore][k].pos_new_z);


	 
	 
 	} 
 	
//}
		 
 }	
//}
		 
 }		
				 if(fabs(F_total[0][llll][k]) > 21 )
	 				 F_total[0][llll][k] =0;
 
		 		 if(fabs(F_total[1][llll][k])  > 21)
					 F_total[1][llll][k] =0;

			 	 if(fabs(F_total[2][llll][k])  > 21)
					 F_total[2][llll][k] =0;
			// printf("FORCE(x,y,z) = %.50f:%.50f%.50f \n", fabs(F_total[0][INDEX_NR][k]),fabs(F_total[1][llll][k]),fabs(F_total[2][INDEX_NRmore][k]));
			
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

  
int ptreeonce = 1;
int findnearestpoint(int points,struct state_vector ** worm , int num,int howmany,int k) {
int i, num_pts = points;
int INDEX;
void *ptree;
char *data, *pch;
struct kdres *presults;
double pos[3], dist;
double pt[3] = { worm[num]->pos_new_x,worm[num]->pos_new_y, worm[num]->pos_new_z };
double radius = 0.0004/1.0;

num_pts =points;

srand( time(0) );

 
	ptree = kd_create( 3 );
  	for( i=0; i<points; i++ ) {   
    		assert( 0 == kd_insert3( ptree, worm[i]->pos_new_x, worm[i]->pos_new_y, worm[i]->pos_new_z, NULL ) );
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
	 

  	totalneigbours[num][k]= state_result_worm_ventral_feather[num][0].totaln;
	// if(worm[num]->pos_new_x != pos[0])
   		state_result_worm_ventral_feather[num][feather_count].pos_new_x = pos[0];
	// if(worm[num]->pos_new_y != pos[1])
  		state_result_worm_ventral_feather[num][feather_count].pos_new_y = pos[1];
	//  if(worm[num]->pos_new_z != pos[2])
   		state_result_worm_ventral_feather[num][feather_count].pos_new_z = pos[2];
	 
 
   
  INDEX = find_index_____(SIZE_OBJECT,state_result_worm_ventral,num,feather_count);
 //printf("INDEX = %d \n", INDEX);
  state_result_worm_ventral_feather[num][feather_count].INDEX_NR = INDEX;


INDEX_from_find[num][feather_count][k] =state_result_worm_ventral_feather[num][feather_count].INDEX_NR;


// state_result_worm_ventral_feather[num][feather_count].INDEX_NR = find_index_____(SIZE_OBJECT,state_result_worm_ventral,num,feather_count );
  //printf(" %d  %d \n", INDEX_from_find[num][feather_count][k],state_result_worm_ventral_feather[num][feather_count].INDEX_NR);


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
   kd_free( ptree );

  return 0;
}

int first_run = 1;
int initonce =1;

extern int KvvDORSAL ;
int *INDEX_WALL;
extern float* V; 
double *vector31;
double  *vector32;
extern float* V2;
int main (int argc, char **argv)
{ 
INDEX_WALL = malloc(1000000);
	vector31 = malloc(100);
	vector32  = malloc(100);
loadOBJ__("ico.obj");  
 totalnum =  KvvVENTAL;
KvvVENTAL = 0;
 
	wall_info = (struct state_vector*)malloc(sizeof(struct state_vector*)*SIZE_OBJECT*4);

 

for(int i=0; i<SIZE_OBJECT*1; i++)
	wall_info[i] = (struct state_vector*)malloc(sizeof(struct state_vector)*32);
 int i,j,k;

 
	 
	  pipe = loadOBJ__("klot3.obj");  
	printf("done ! \n");
 
	Vec3 *v ;
 
	int ll;
	for(ll = 0; ll < SIZE_OBJECT  ; ll++)
	{
		  v= (Vec3 *)(V + 3*(KvvVENTALA[ll]-1));
         	  wall_info[ll]->pos_new_x = v->x ;
 			 
 	          wall_info[ll]->pos_new_y =  v->y ;
 		  wall_info[ll]->pos_new_z = v->z ;
 
 	  	  wall_info[ll]->vel_new_x =  0 ;
 		  wall_info[ll]->vel_new_y = 0;
 		  wall_info[ll]->vel_new_z =  0 ;
		  wall_info[ll]->mass = ll;
 
     		//  printf("%f::%f::%f  %d \n",wall_info[ll]->pos_new_x,wall_info[ll]->pos_new_y,wall_info[ll]->pos_new_z);
	}
springVector =(struct state_vector*)malloc(sizeof(struct state_vector*)*SIZE_OBJECT*32);




 
 
 
 


state_result_worm_ventral_feather = (struct state_vector*)malloc(sizeof(struct state_vector*)*SIZE_OBJECT*4);

state_result_worm_ventral =(struct state_vector*)malloc(sizeof(struct state_vector*)*SIZE_OBJECT*32);


 

for(int i=0; i<SIZE_OBJECT*1; i++)
	state_result_worm_ventral[i] = (struct state_vector*)malloc(sizeof(struct state_vector)*32);
       
printf("done ! \n");
 
for(int i=0; i<SIZE_OBJECT*1; i++)
	state_result_worm_ventral_feather[i] = (struct state_vector*)malloc(sizeof(struct state_vector)*32);
  
printf("done ! \n");
  
 
if(initonce==1)
{
	Vec3 *v ;
 for(int j = 0; j <  2  ; j++)  
 {
	int ll;
	for(ll = 0; ll < SIZE_OBJECT  ; ll++)
	{
		  v= (Vec3 *)(V + 3*(KvvVENTALA[ll]-1));
         	  state_result_worm_ventral[ll][j].pos_new_x = v->x  ;
 
 	          state_result_worm_ventral[ll][j].pos_new_y = v->y+j*60 ;
 		  state_result_worm_ventral[ll][j].pos_new_z = v->z  ;
 
 	  	  state_result_worm_ventral[ll][j].vel_new_x =   1e-4 ;
 		  state_result_worm_ventral[ll][j].vel_new_y =  1e-4 ;
 		  state_result_worm_ventral[ll][j].vel_new_z =  1e-4 ;
	}

 
	for(ll = 0; ll < SIZE_OBJECT     ; ll++)
	{
 		findnearestpoint(SIZE_OBJECT  ,state_result_worm_ventral,ll,1,j );
		
	}
 
	initonce = 0;
	 ptreeonce = 1;
	}
}
 
glutInit(&argc, argv);
init_mpgeg();

GLint glut_display;

glutInitWindowSize (1080,1920);
glutCreateWindow ("wormy");
static GLuint	texture;
int mainMenu;
const GLubyte *str;
str = glGetString (GL_EXTENSIONS);



 
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
/*
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
material->setDifusse_material(0.5f, 0.1f, 0.995f, 1.0f/1.0f);
material->setSpecular_material(0.01f, 0.01f, 0.01f, 1.0f/1.0f);
material->setShininess_material(128);
glLightfv(GL_LIGHT0, GL_POSITION, dirLight->pos);
glLightfv(GL_LIGHT0, GL_AMBIENT,   dirLight->ambient);
glLightfv(GL_LIGHT0, GL_SPECULAR,  dirLight->specular);
glLightfv(GL_LIGHT0, GL_DIFFUSE,   dirLight->difusse);   */


//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);  
glutDisplayFunc (display );
 
glutReshapeFunc(reshape);
 
glutMouseFunc(mouse);
glutKeyboardFunc(key);
  	
 
glutMotionFunc(mouseMotion);
  
	
glutMainLoop();

}

void worm_ventral(struct state_vector     **  next_state, int num, int k )
{
	double f0[6];
	double f0_result[6];

	double z[6];
 	double tburn = 0.6;
	double tf[6] = {tburn,tburn,tburn,tburn,tburn,tburn};
tburn = 0.5;
double t0[6] ={tburn,tburn,tburn,tburn,tburn,tburn};

	z[0] =  next_state[num][k].pos_new_x;
	z[1] =  next_state[num][k].pos_new_y;
	z[2] = next_state[num][k].pos_new_z;
	z[3] =  next_state[num][k].vel_new_x;
	z[4] =  next_state[num][k].vel_new_y;
	z[5] = next_state[num][k].vel_new_z;

	f0[0] = next_state[num][k].pos_new_x;
	f0[1] =next_state[num][k].pos_new_y;
	f0[2] =next_state[num][k].pos_new_z;
	f0[3] =next_state[num][k].vel_new_x;
	f0[4] =next_state[num][k].vel_new_y;
	f0[5] = next_state[num][k].vel_new_z;
 
	rk45(rates_dorsal, t0, f0,f0_result, tf,z,6, 1  );

	next_state[num][k].pos_new_x = f0[0];
	next_state[num][k].pos_new_y = f0[1];
	next_state[num][k].pos_new_z = f0[2];

	next_state[num][k].vel_new_x = f0[3];
	next_state[num][k].vel_new_y = f0[4];
	next_state[num][k].vel_new_z = f0[5];

	 
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

void display  (void){
 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_COLOR_MATERIAL);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glMultMatrixf(lightRotation);
 

    glLoadIdentity();
    gluLookAt(cameraEye[0]+0, cameraEye[1]+253, cameraEye[2]-423, cameraLookAt[0], cameraLookAt[1], cameraLookAt[2], cameraUp[0], cameraUp[1], cameraUp[2]);

    glMultMatrixf(viewRotation);

    glRotatef(rotationX,1,0,0);
    glRotatef(rotationY+320,0,1,0);

    glMultMatrixf(lightRotation);
for(int j = 0; j <   2  ; j++)  
 {
   for(int ll = 0; ll <   totalnum  ; ll++)  
		  {
    calc_fluid_velocity(ll,j);
    calc_fluid_acceleration(ll,j);

taskworm(j,ll);
glPushMatrix();
 if( j == 1 && ll == 4)
	printf("pos x %.10f %d %.10f\n", state_result_worm_ventral[ll][j].pos_new_x,what_k,F_total[2][ll][j]);
  glTranslatef(state_result_worm_ventral[ll][j].pos_new_x, state_result_worm_ventral[ll][j].pos_new_y,state_result_worm_ventral[ll][j].pos_new_z);
glBegin(GL_TRIANGLES);
     glColor3f(state_result_worm_ventral[ll][j].pos_new_x, state_result_worm_ventral[ll][j].pos_new_y, state_result_worm_ventral[ll][j].pos_new_z);     // Red
      glVertex3f(0, 1.0f/1.0f, 0.0f);
      glColor3f(state_result_worm_ventral[ll][j].pos_new_x, state_result_worm_ventral[ll][j].pos_new_y,state_result_worm_ventral[ll][j].pos_new_z);     // Green
      glVertex3f(-1.0f/1.0f, -1.0f/1.0f, 1.0f/1.0f);
      glColor3f(state_result_worm_ventral[ll][j].pos_new_x, state_result_worm_ventral[ll][j].pos_new_y, state_result_worm_ventral[ll][j].pos_new_z);     // Blue
      glVertex3f(1.0f/1.0f, -1.0f/1.0f, 1.0f/1.0f);
 
      // Right
      glColor3f(state_result_worm_ventral[ll][j].pos_new_x, state_result_worm_ventral[ll][j].pos_new_y, state_result_worm_ventral[ll][j].pos_new_z);     // Red
      glVertex3f(0.0f, 1.0f/1.0f, 0.0f);
      glColor3f(state_result_worm_ventral[ll][j].pos_new_x, state_result_worm_ventral[ll][j].pos_new_y, state_result_worm_ventral[ll][j].pos_new_z);     // Blue
      glVertex3f(1.0f/1.0f, -1.0f/1.0f, 1.0f/1.0f);
      glColor3f(state_result_worm_ventral[ll][j].pos_new_x,state_result_worm_ventral[ll][j].pos_new_y, state_result_worm_ventral[ll][j].pos_new_z);     // Green
      glVertex3f(1.0f/1.0f, -1.0f/1.0f, -1.0f/1.0f);
 
      // Back
      glColor3f(state_result_worm_ventral[ll][j].pos_new_x,state_result_worm_ventral[ll][j].pos_new_y, state_result_worm_ventral[ll][j].pos_new_z);     // Red
      glVertex3f(0.0f, 1.0f/1.0f, 0.0f);
      glColor3f(state_result_worm_ventral[ll][j].pos_new_x, state_result_worm_ventral[ll][j].pos_new_y, state_result_worm_ventral[ll][j].pos_new_z);     // Green
      glVertex3f(1.0f/1.0f, -1.0f/1.0f, -1.0f/1.0f);
      glColor3f(state_result_worm_ventral[ll][j].pos_new_x, state_result_worm_ventral[ll][j].pos_new_y, state_result_worm_ventral[ll][j].pos_new_z);     // Blue
      glVertex3f(-1.0f/1.0f, -1.0f/1.0f, -1.0f/1.0f);
 
      // Left
      glColor3f(state_result_worm_ventral[ll][j].pos_new_x,state_result_worm_ventral[ll][j].pos_new_y,state_result_worm_ventral[ll][j].pos_new_z);       // Red
      glVertex3f( 0.0f, 1.0f/1.0f, 0.0f);
      glColor3f(state_result_worm_ventral[ll][j].pos_new_x,state_result_worm_ventral[ll][j].pos_new_y,state_result_worm_ventral[ll][j].pos_new_z);       // Blue
      glVertex3f(-1.0f/1.0f,-1.0f/1.0f,-1.0f/1.0f);
      glColor3f(state_result_worm_ventral[ll][j].pos_new_x,state_result_worm_ventral[ll][j].pos_new_y,state_result_worm_ventral[ll][j].pos_new_z);       // Green
      glVertex3f(-1.0f/1.0f,-1.0f/1.0f, 1.0f/1.0f);
	glEnd(); 	
 	
 //printf(" %.10f:%.10f%.10f   %d\n", state_result_worm_ventral[ll][j].pos_new_x,state_result_worm_ventral[ll][j].pos_new_y,state_result_worm_ventral[ll][j].pos_new_z, what_k );
 glPopMatrix();
 glutPostRedisplay();
}


}

display_mpeg();

glutSwapBuffers();

}




void rates_dorsal ( double *t, double *f, double result[]   )
{
// printf("f[0] = %.10f:%.10f%.10f \n", f[0],f[1],f[2]);
if(state_result_worm_ventral[llll][what_k].force_sign == 1)
{
    	result[0] =             f[3]/1.0;
    	result[1] =             f[4]/1.0;
    	result[2] =             f[5]/1.0;
  
	result[3] = (F_total[0][llll][what_k])/(1.0   )+acc2_fluid[0][what_k]/1.0;
	result[4] = (F_total[1][llll][what_k]  )/(1.0   ) -9.8 +acc2_fluid[1][what_k]/1.0 ;
	result[5] = (F_total[2][llll][what_k] )/(1.0)+acc2_fluid[2][what_k]/1.0; 

 
}
else if(state_result_worm_ventral[INDEX_NR][what_k].force_sign == -1)
{
    result[0] =             f[3]/1.0;
    result[1] =             f[4]/1.0;
    result[2] =             f[5]/1.0;
  
    result[3] = -(F_total[0][INDEX_NR][what_k])/(1.0  )+acc2_fluid[0][what_k]/1.0;
    result[4] = -(F_total[1][INDEX_NR][what_k] ) /(1.0) -9.8+acc2_fluid[1][what_k]/1.0 ;
    result[5] = -(F_total[2][INDEX_NR][what_k] )/(1.0  )+acc2_fluid[2][what_k]/1.0; //522
 

}
else if(state_result_worm_ventral[INDEX_NRmore][what_k].force_sign == 1)
{
    	result[0] =             f[3]/1.0;
    	result[1] =             f[4]/1.0;
    	result[2] =             f[5]/1.0;
  
	result[3] = (F_total[0][INDEX_NRmore][what_k])/(1.0   )+acc2_fluid[0][what_k]/1.0;
	result[4] = (F_total[1][INDEX_NRmore][what_k]   )/(1.0   )-9.8 +acc2_fluid[1][what_k]/1.0;
	result[5] = (F_total[2][INDEX_NRmore][what_k] )/(1.0)+acc2_fluid[2][what_k]/1.0; 

 
}
  acc1 = sqrtf(pow(result[3],2.0) + pow(result[4],2.0)+pow(result[5],2.0));


}
 int alloc_once = 1;
void findnearestpoint_3_points(int points , int num,double r_collision, double **vvv,int ll, int j )
{
	int i, num_pts ;
 
	char  *pch;
	struct kdres *presults;
	double pos[3], dist;
	double pt[3] = { state_result_worm_ventral[ll][j].pos_new_x,state_result_worm_ventral[ll][j].pos_new_y, state_result_worm_ventral[ll][j].pos_new_y };
	double radius = 25;
 
	num_pts =SIZE_OBJECT;

 	void *ptree_____;

 	if(alloc_once == 1)
	{
		ptree_____ = kd_create( 3 );
 
  		for( i=0; i<points; i++ ) {   
			//       printf("target node at (%.3f, %.3f, %.3f) \n", wall_info[i]->pos_new_x, wall_info[i]->pos_new_y,wall_info[i]->pos_new_z);
    			assert( 0 == kd_insert3( ptree_____, wall_info[i]->pos_new_x, wall_info[i]->pos_new_y, wall_info[i]->pos_new_z, NULL ) );

	}
		alloc_once = 0;
	}
 
 
	presults = kd_nearest_range( ptree_____, pt, radius );
 
  	/* print out all the points found in results */
	// printf( "found %d results:\n", kd_res_size(presults) );

 
	//state_result_worm_dorsal_feather[num]->totaln = kd_res_size(presults) ;
 	// printf( "found %d results:\n", kd_res_size(presults) );

 
        int vc = 0;
    //   printf("source node at (%.3f, %.3f, %.3f) \n", pt[0], pt[1],pt[2]);
        while( !kd_res_end( presults ) ) {
   
		pch = (char*)kd_res_item( presults, pos ); 
    		//dist = sqrt( dist_sq( pt, pos, 3 ) );

		 // if(worm[num]->pos_new_x != pos[0])
 		 // printf( "node at (%.3f, %.3f, %.3f) is %.3f away   \n", 
 		 //    pos[0], pos[1], pos[2], dist  );
	
 //printf("destination node at (%.3f, %.3f, %.3f) \n", pos[0], pos[1],pos[2]);
  		//totalneigbours[num] = state_result_worm_ventral[num]->totaln;
		// if(worm[num]->pos_new_x != pos[0])
   		neighbourV_[vc][0]  = pos[0];
		// if(worm[num]->pos_new_y != pos[1])

  		neighbourV_[vc][1]  = pos[1];
		//  if(worm[num]->pos_new_z != pos[2])
   		neighbourV_[vc][2]  = pos[2];
	 
   		vc++;
 		// INDEX = find_index_____(SIZE_OBJECT,worm,num,feather_count);
 		 //near_vector[num][feather_count].INDEX_NR = INDEX;
	 	if(vc > 1)
			break;
		
 
   		 kd_res_next( presults );
 	 
 
    	  }
    

   vc = 0;
   kd_res_free( presults );
   kd_free( ptree_____ );

}


void normalize(double *v)
{
	double length = sqrtf(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);

	for(int i = 0; i < 3; i++)
	{
		v[i] = v[i]/length;
	}
}

void cross_product(double *a, double *b,double * result)
{

   result[0] = a[1]*b[2]-a[2]*b[1];
   result[1] = - (a[0]*b[2] - a[2]*b[0]);
   result[2] =  a[0]*b[1] - a[1]*b[0] ;

	normalize(result);
 

}


//a cross b != b cross a
void calculate_normal(double*a, double* b, double * c,double *result)
{

	double x[] = {b[0]-a[0],b[1]-a[1],b[2]-a[2]};
	double y[] = {c[0]-a[0],c[1]-a[1],c[2]-a[2]};

	 cross_product(x,y,result);
 
}
 
double * dot(double *a, double *b , double *c )
{
	c[0] = a[0]*b[0];
	c[1] = a[1]*b[1];
	c[2] = a[2]*b[2];  	  
}
int pressure_once = 1;
void calc_fluid_acceleration(int ll, int j)
{

	double Area_tube = pow(water_radius,2)*M_PI;
	double mass_fluid_element = density*(M_PI*pow(water_radius,3.0))*4.0/3.0;
	double gMASS;
 

	if(pressure_once ==1)
	{
		state_result_worm_ventral[ll][j].pressure =  ((  -9.8+acc1)*mass_fluid_element/(M_PI*pow(water_radius,2.0)));
		pressure_once =0;
	}
	

	double sqrtme = state_result_worm_ventral[ll][j].pressure/density;

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
		addme = sqrt(sqrtme );
	
    acc2_fluid[0][j] = (-1*(state_result_worm_ventral[ll][j].pressure )/(( state_result_worm_ventral[ll][j].pos_new_x)))/density + (viscous_constant*Area_tube*addme)/mass_fluid_element;
    acc2_fluid[1][j] = (-1*(state_result_worm_ventral[ll][j].pressure )/(( state_result_worm_ventral[ll][j].pos_new_y)))/density + (viscous_constant*Area_tube*addme)/mass_fluid_element;
    acc2_fluid[2][j] = (-1*(state_result_worm_ventral[ll][j].pressure )/(( state_result_worm_ventral[ll][j].pos_new_z)))/density + (viscous_constant*Area_tube*addme)/mass_fluid_element;

}
void calc_fluid_velocity(int ll, int j)
{

	double distance_from_origin = sqrtf(powf( state_result_worm_ventral[ll][j].pos_new_x - 0 ,2.0) +  powf( state_result_worm_ventral[ll][j].pos_new_y -0 ,2.0)  + 		powf( state_result_worm_ventral[ll][j].pos_new_z -0 ,2.0  ));
 
//if(( distance_from_origin) > 65)
//{

	for(int vn = 0; vn < SIZE_OBJECT; vn++)
	{

		if(j == vn)
			continue;

		//need 3 points	
 
		double sqrtthis = powf( state_result_worm_ventral[ll][j].pos_new_x - wall_info[vn]->pos_new_x ,2.0) +  powf( state_result_worm_ventral[ll][j].pos_new_y - 	wall_info[vn]->pos_new_y ,2.0)  + powf( state_result_worm_ventral[ll][j].pos_new_z - wall_info[vn]->pos_new_z ,2.0  );
 
		double xxxx=  (sqrtthis ); 
  		complex zzzz;
 

 		 if((sqrtthis)  < 0)
	  	{
 			zzzz = csqrt(sqrtthis);
		  	xxxx = creal(zzzz);
		}
		else if ((sqrtthis)  > 0)	  
			xxxx = sqrtf(sqrtthis ); 


		 
		if( fabs(xxxx) > 0 &&  fabs(xxxx) < 8.1)
		{
 
			double u_perpv[3];
			double w_parv[3]; 
			double * normal = malloc(100) ;
			double * dotprod_v_n= malloc(100) ;
			double * dotprod_n_n= malloc(100) ;
			double * vel = malloc(100);


  
		
			findnearestpoint_3_points(SIZE_OBJECT,   vn, 100, NULL ,ll,j);
 
			double aa[] = {state_result_worm_ventral[ll][j].pos_new_x,state_result_worm_ventral[ll][j].pos_new_y,state_result_worm_ventral[ll][j].pos_new_z};
			double bb[] = {neighbourV_[1][0],neighbourV_[1][1],neighbourV_[1][2]};
			double cc[] = {neighbourV_[2][0],neighbourV_[2][1],neighbourV_[2][2]};
	 

			calculate_normal(aa ,bb ,cc ,normal);


			vel[0] =  state_result_worm_ventral[ll][j].vel_new_x ;
			vel[1] = state_result_worm_ventral[ll][j].vel_new_y;
			vel[2] =state_result_worm_ventral[ll][j].vel_new_z;

 			dot(   vel ,  normal, dotprod_v_n);
			dot( normal, normal, dotprod_n_n);

			//u_perpv = (v · n / n · n) n
			u_perpv[0] = (dotprod_v_n[0]/dotprod_n_n[0])*normal[0];
			u_perpv[1] = (dotprod_v_n[1]/dotprod_n_n[1])*normal[1];
			u_perpv[2] = (dotprod_v_n[2]/dotprod_n_n[2])*normal[2];


			w_parv[0] = vel[0] - u_perpv[0]  ;
			w_parv[1] = vel[1] - u_perpv[1]  ;
			w_parv[2] = vel[2] - u_perpv[2]  ;
		/*	printf("what_k  =%d \n", what_k );
			printf("vel[0]   =%.10f \n", vel[0] );
			printf("u_perpv[0]   =%.10f \n", u_perpv[0] );
			printf("w_parv[0]  =%.10f\n", w_parv[0] );
			printf("normal[0]  =%.10f\n", normal[0] );
			printf("neighbourV[0][1]  =%.10f\n", neighbourV_[0][1]); */

			//v′ = f w − r u
			double r =0.7;
			double frictionforce = 10;
			state_result_worm_ventral[ll][j].vel_new_x = frictionforce*w_parv[0] - r*u_perpv[0];
			state_result_worm_ventral[ll][j].vel_new_y = frictionforce*w_parv[1] - r*u_perpv[1];
			state_result_worm_ventral[ll][j].vel_new_z = frictionforce*w_parv[2] - r*u_perpv[2];
 		//	state_result_worm_ventral[ll][j] = mass_motion( mass_motion_state[what_k] ,1,0);
 			 worm_ventral(   &state_result_worm_ventral[ll]   ,1,j);
//
			free(dotprod_v_n);
			free(dotprod_n_n);
 			free(normal);
 			free(vel);
			alloc_once = 1;


			return;
		}
	}
}
