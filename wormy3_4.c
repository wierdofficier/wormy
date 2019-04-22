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
#define NUM_THREADS 5
int KvvVENTALA2[400000 ];
int pipe = 0;
#include <stdbool.h>
extern int KvvVENTAL2;
  struct state_vector  ** wall_info ; 
double  neighbourV_[10000*2][10000] ;
 double acc2_fluid[3][4000 ][4000] ;
double water_radius =0.002;
int alloc_once = 1;
double density =   1000.0;
double viscous_constant = 1.6735;
float acc1;
float acc2;
int INDEX_NRmore;
#define SIZE_OBJECT 3840 //126976
#define HOW_MANY_FLUIDS 3
#define HOWMANY HOW_MANY_FLUIDS
float springlength =  0.004/4.0;
#define AIR_FRICTION 0.9959999
#define FIRST 1
#define WORK 0
float fovy = 45.0;
float dNear = 100;
float dFar = 2000;
double F_total[3][SIZE_OBJECT*104][HOW_MANY_FLUIDS] ;
int totalneigbours[SIZE_OBJECT*104][HOW_MANY_FLUIDS] ;
float     vertexpoint_g_ventral[SIZE_OBJECT][3];
struct state_vector * springVector;
struct state_vector ** state_result_worm_ventral_feather;
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
int findnearestpoint(int points,struct state_vector *** worm , int num,int howmany,int kk);
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
int howmanyfluids_g;
int acc1once= 1;
void taskworm(int howmanyfluids )
{
howmanyfluids_g =howmanyfluids;
int k;
for(k = 0; k <= totalneigbours[llll][0]; k++)
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
	// printf("1totalneigbours[llll]  %d :: number %d :: index_____ = %d\n",totalneigbours[llll],llll,INDEX_NR);
	INDEX_NR =  (state_result_worm_ventral_feather[llll][k].INDEX_NR);
	
if(  INDEX_NR > 0 && INDEX_NR < 1000000  )
{
	//if(state_result_worm_ventral[INDEX_NR][howmanyfluids]->force_sign != -1 && state_result_worm_ventral[INDEX_NR][howmanyfluids]->force_sign != 1 && INDEX_NR >0)
	//{
		 if(feather_once == FIRST && INDEX_NR > 0)
		{	

						 if(fabs(F_total[0][INDEX_NR][howmanyfluids]) > 21 )
	 				 	  F_total[0][INDEX_NR][howmanyfluids] =0;
 
		 		 if(fabs(F_total[1][INDEX_NR][howmanyfluids])  > 21)
						F_total[1][INDEX_NR][howmanyfluids] =0;

			 	 if(fabs(F_total[2][INDEX_NR][howmanyfluids])  > 21)
						F_total[2][INDEX_NR][howmanyfluids] =0;
  //printf("2totalneigbours[llll]  %d :: number %d :: index_____ = %d\n",totalneigbours[llll],llll,INDEX_NR);
		springVector->pos_new_x = state_result_worm_ventral[INDEX_NR][howmanyfluids]->pos_new_x - state_result_worm_ventral[llll][howmanyfluids]->pos_new_x;
		springVector->pos_new_y = state_result_worm_ventral[INDEX_NR][howmanyfluids]->pos_new_y - state_result_worm_ventral[llll][howmanyfluids]->pos_new_y;
		springVector->pos_new_z = state_result_worm_ventral[INDEX_NR][howmanyfluids]->pos_new_z - state_result_worm_ventral[llll][howmanyfluids]->pos_new_z;
 

		float r = length(state_result_worm_ventral[INDEX_NR][howmanyfluids],state_result_worm_ventral[llll][howmanyfluids]);

		if ( r != 0  &&   r < 50  )
		{			
			F_total[0][INDEX_NR][howmanyfluids] +=( springVector->pos_new_x  /  r ) * ( r - springlength ) * ( -springConstant );
			F_total[1][INDEX_NR][howmanyfluids] += ( springVector->pos_new_y /  r ) * ( r - springlength ) * ( -springConstant );
			F_total[2][INDEX_NR][howmanyfluids] +=( springVector->pos_new_z  /  r ) * ( r - springlength ) * ( -springConstant );

			F_total[0][llll][howmanyfluids] +=( springVector->pos_new_x  / r ) * ( r - springlength ) * ( -springConstant );
			F_total[1][llll][howmanyfluids] += ( springVector->pos_new_y  / r ) * ( r - springlength) * ( -springConstant );
			F_total[2][llll][howmanyfluids] +=( springVector->pos_new_z  / r ) * ( r - springlength ) * ( -springConstant );

F_total[0][INDEX_NR][howmanyfluids] +=-(  state_result_worm_ventral[INDEX_NR][howmanyfluids]->vel_new_x - state_result_worm_ventral[llll][howmanyfluids]->vel_new_x ) * frictionConstant;
 		F_total[1][INDEX_NR][howmanyfluids] += -( state_result_worm_ventral[INDEX_NR][howmanyfluids]->vel_new_y- state_result_worm_ventral[llll][howmanyfluids]->vel_new_y ) * frictionConstant;
 		F_total[2][INDEX_NR][howmanyfluids] += -(  state_result_worm_ventral[INDEX_NR][howmanyfluids]->vel_new_z - state_result_worm_ventral[llll ][howmanyfluids]->vel_new_z ) * frictionConstant;


		F_total[0][llll][howmanyfluids] +=-(  state_result_worm_ventral[INDEX_NR][howmanyfluids]->vel_new_x - state_result_worm_ventral[llll][howmanyfluids]->vel_new_x ) * frictionConstant;
 		F_total[1][llll][howmanyfluids] += -( state_result_worm_ventral[INDEX_NR][howmanyfluids]->vel_new_y- state_result_worm_ventral[llll][howmanyfluids]->vel_new_y ) * frictionConstant;
 		F_total[2][llll][howmanyfluids] += -(  state_result_worm_ventral[INDEX_NR][howmanyfluids]->vel_new_z - state_result_worm_ventral[llll ][howmanyfluids]->vel_new_z ) * frictionConstant;
		state_result_worm_ventral[llll][howmanyfluids]->vel_new_x = state_result_worm_ventral[llll][howmanyfluids]->vel_new_x*0.995 ;
		
		state_result_worm_ventral[llll][howmanyfluids]->vel_new_y = state_result_worm_ventral[llll][howmanyfluids]->vel_new_y*0.995 ;

		state_result_worm_ventral[llll][howmanyfluids]->vel_new_z = state_result_worm_ventral[llll][howmanyfluids]->vel_new_z*0.995 ;

		state_result_worm_ventral[INDEX_NR][howmanyfluids]->vel_new_x = state_result_worm_ventral[INDEX_NR][howmanyfluids]->vel_new_x*0.995 ;
		
		state_result_worm_ventral[INDEX_NR][howmanyfluids]->vel_new_y = state_result_worm_ventral[INDEX_NR][howmanyfluids]->vel_new_y*0.995 ;
		state_result_worm_ventral[INDEX_NR][howmanyfluids]->vel_new_z = state_result_worm_ventral[INDEX_NR][howmanyfluids]->vel_new_z*0.995 ;
		}

		

		state_result_worm_ventral[INDEX_NR][howmanyfluids]->force_sign =  -1;
		state_result_worm_ventral[INDEX_NR][howmanyfluids]    =  worm_ventral(  state_result_worm_ventral[INDEX_NR][howmanyfluids] ,1) ;

		state_result_worm_ventral[llll][howmanyfluids]->force_sign =   1; ; 
		state_result_worm_ventral[llll][howmanyfluids]    =  worm_ventral(  state_result_worm_ventral[llll][howmanyfluids] ,1);
		


 

	for(int d = 0; d <= totalneigbours[llll][0]; d++)
	{	 
		
		if(d == k  )
		  continue;

 
   // state_result_worm_ventral_feather[llll][d].INDEX_NR = find_index_____(SIZE_OBJECT,state_result_worm_ventral_feather,llll,d);
INDEX_NRmore = state_result_worm_ventral_feather[llll][d].INDEX_NR;
 

		 

 

				 if(fabs(F_total[0][INDEX_NRmore][howmanyfluids]) > 21 )
	 				  F_total[0][INDEX_NRmore][howmanyfluids] =0;
 
		 		 if(fabs(F_total[1][INDEX_NRmore][howmanyfluids])  > 21)
					  F_total[1][INDEX_NRmore][howmanyfluids] =0;

			 	 if(fabs(F_total[2][INDEX_NRmore][howmanyfluids])  > 21)
					  F_total[2][INDEX_NRmore][howmanyfluids] =0;
			

 				if(fabs(F_total[0][INDEX_NR][howmanyfluids]) > 21 )
	 				  F_total[0][INDEX_NR][howmanyfluids] =0;
 
		 		 if(fabs(F_total[1][INDEX_NR][howmanyfluids])  > 21)
					  F_total[1][INDEX_NR][howmanyfluids] =0;

			 	 if(fabs(F_total[2][INDEX_NR][howmanyfluids])  > 21)
					  F_total[2][INDEX_NR][howmanyfluids] =0;
			
		springVector->pos_new_x = state_result_worm_ventral[INDEX_NR][howmanyfluids]->pos_new_x - state_result_worm_ventral[INDEX_NRmore][howmanyfluids]->pos_new_x;
		springVector->pos_new_y = state_result_worm_ventral[INDEX_NR][howmanyfluids]->pos_new_y - state_result_worm_ventral[INDEX_NRmore][howmanyfluids]->pos_new_y;
		springVector->pos_new_z = state_result_worm_ventral[INDEX_NR][howmanyfluids]->pos_new_z - state_result_worm_ventral[INDEX_NRmore][howmanyfluids]->pos_new_z;
 			

		float r = length(state_result_worm_ventral[INDEX_NR][howmanyfluids],state_result_worm_ventral[INDEX_NRmore][howmanyfluids]);

		if ( r != 0  &&   r < 50    )
		{	
	//	 printf("1totalneigbours[llll]  %d :: number %d :: index_____ = %d\n",totalneigbours[llll],llll,INDEX_NRmore);	
			F_total[0][INDEX_NR][howmanyfluids] +=( springVector->pos_new_x  /  r ) * ( r - springlength ) * ( -springConstant );
			F_total[1][INDEX_NR][howmanyfluids] += ( springVector->pos_new_y /  r ) * ( r - springlength ) * ( -springConstant );
			F_total[2][INDEX_NR][howmanyfluids] +=( springVector->pos_new_z  /  r ) * ( r - springlength ) * ( -springConstant );

			F_total[0][INDEX_NRmore][howmanyfluids] +=( springVector->pos_new_x  / r ) * ( r - springlength ) * ( -springConstant );
			F_total[1][INDEX_NRmore][howmanyfluids] += ( springVector->pos_new_y  / r ) * ( r - springlength) * ( -springConstant );
			F_total[2][INDEX_NRmore][howmanyfluids] +=( springVector->pos_new_z  / r ) * ( r - springlength ) * ( -springConstant );

		F_total[0][INDEX_NR][howmanyfluids] +=-(  state_result_worm_ventral[INDEX_NR][howmanyfluids]->vel_new_x - state_result_worm_ventral[INDEX_NRmore][howmanyfluids]->vel_new_x ) * frictionConstant;
 		F_total[1][INDEX_NR][howmanyfluids] += -( state_result_worm_ventral[INDEX_NR][howmanyfluids]->vel_new_y- state_result_worm_ventral[INDEX_NRmore][howmanyfluids]->vel_new_y ) * frictionConstant;
 		F_total[2][INDEX_NR][howmanyfluids] += -(  state_result_worm_ventral[INDEX_NR][howmanyfluids]->vel_new_z - state_result_worm_ventral[INDEX_NRmore ][howmanyfluids]->vel_new_z ) * frictionConstant;


		F_total[0][INDEX_NRmore][howmanyfluids] +=-(  state_result_worm_ventral[INDEX_NR][howmanyfluids]->vel_new_x - state_result_worm_ventral[INDEX_NRmore][howmanyfluids]->vel_new_x ) * frictionConstant;
 		F_total[1][INDEX_NRmore][howmanyfluids] += -( state_result_worm_ventral[INDEX_NR][howmanyfluids]->vel_new_y- state_result_worm_ventral[INDEX_NRmore][howmanyfluids]->vel_new_y ) * frictionConstant;
 		F_total[2][INDEX_NRmore][howmanyfluids] += -(  state_result_worm_ventral[INDEX_NR][howmanyfluids]->vel_new_z - state_result_worm_ventral[INDEX_NRmore ][howmanyfluids]->vel_new_z ) * frictionConstant;

		state_result_worm_ventral[INDEX_NRmore][howmanyfluids]->vel_new_x = state_result_worm_ventral[INDEX_NRmore][howmanyfluids]->vel_new_x*0.995 ;
		
		state_result_worm_ventral[INDEX_NRmore][howmanyfluids]->vel_new_y = state_result_worm_ventral[INDEX_NRmore][howmanyfluids]->vel_new_y*0.995 ;

		state_result_worm_ventral[INDEX_NRmore][howmanyfluids]->vel_new_z = state_result_worm_ventral[INDEX_NRmore][howmanyfluids]->vel_new_z*0.995 ;

		state_result_worm_ventral[INDEX_NR][howmanyfluids]->vel_new_x = state_result_worm_ventral[INDEX_NR][howmanyfluids]->vel_new_x*0.995 ;
		
		state_result_worm_ventral[INDEX_NR][howmanyfluids]->vel_new_y = state_result_worm_ventral[INDEX_NR][howmanyfluids]->vel_new_y*0.995 ;
		state_result_worm_ventral[INDEX_NR][howmanyfluids]->vel_new_z = state_result_worm_ventral[INDEX_NR][howmanyfluids]->vel_new_z*0.995 ;
		}



		state_result_worm_ventral[INDEX_NR][howmanyfluids]->force_sign =  -1;
		state_result_worm_ventral[INDEX_NR][howmanyfluids]    =  worm_ventral(  state_result_worm_ventral[INDEX_NR][howmanyfluids] ,1) ;

		state_result_worm_ventral[INDEX_NRmore][howmanyfluids]->force_sign =   1; ; 
		state_result_worm_ventral[INDEX_NRmore][howmanyfluids]    =  worm_ventral(  state_result_worm_ventral[INDEX_NRmore][howmanyfluids] ,1);

	//	 printf("f[0] = %.10f:%.10f%.10f \n", state_result_worm_ventral[INDEX_NRmore]->pos_new_x,state_result_worm_ventral[INDEX_NRmore]->pos_new_y,state_result_worm_ventral[INDEX_NRmore]->pos_new_z);


	 
	 
 	} 
 	
//}
		 
 }	
//}
		 
 }		
				 if(fabs(F_total[0][llll][howmanyfluids]) > 21 )
	 				 F_total[0][llll][howmanyfluids] =0;
 
		 		 if(fabs(F_total[1][llll][howmanyfluids])  > 21)
					 F_total[1][llll][howmanyfluids] =0;

			 	 if(fabs(F_total[2][llll][howmanyfluids])  > 21)
					 F_total[2][llll][howmanyfluids] =0;
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

int find_index_____(int NUM, struct state_vector **real[]  ,int num,int k, int howm )
{
int ll = 0;
 	while(ll < NUM)
	{
		if(state_result_worm_ventral_feather[num][k].pos_new_x ==real[ll][howm]->pos_new_x && state_result_worm_ventral_feather[num][k].pos_new_y ==real[ll][howm]->pos_new_y && 		state_result_worm_ventral_feather[num][k].pos_new_z ==real[ll][howm]->pos_new_z)
		{
			//printf("found index_____ @ %d \n", ll);
			return ll;
		}
	ll++;
	}
}

void *ptree;
  
int ptreeonce = 1;
int findnearestpoint(int points,struct state_vector *** worm , int num,int howmany,int kk) {
int i, num_pts = points;
int INDEX;
char *data, *pch;
struct kdres *presults;
double pos[3], dist;
double pt[3] = { worm[num][kk]->pos_new_x,worm[num][kk]->pos_new_y, worm[num][kk]->pos_new_z };
double radius = 0.0004/1.0;

num_pts =points;

srand( time(0) );

if(ptreeonce < 2  )
{
	ptree = kd_create( 3 );
  	for( i=0; i<points; i++ ) {   
    		assert( 0 == kd_insert3( ptree, worm[i][kk]->pos_new_x, worm[i][kk]->pos_new_y, worm[i][kk]->pos_new_z, NULL ) );
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
	 

  	totalneigbours[num][howmanyfluids_g] = state_result_worm_ventral_feather[num][0].totaln;
	// if(worm[num]->pos_new_x != pos[0])
   		state_result_worm_ventral_feather[num][feather_count].pos_new_x = pos[0];
	// if(worm[num]->pos_new_y != pos[1])
  		state_result_worm_ventral_feather[num][feather_count].pos_new_y = pos[1];
	//  if(worm[num]->pos_new_z != pos[2])
   		state_result_worm_ventral_feather[num][feather_count].pos_new_z = pos[2];
	int k;
 
   
  INDEX = find_index_____(SIZE_OBJECT,state_result_worm_ventral,num,feather_count,kk);
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
 

int first_run = 1;
int initonce =1;
extern int KvvVENTAL ;
extern int KvvDORSAL ;

extern float* V; 
extern float* V2;
int main (int argc, char **argv)
{ 

 int i,j,k;
loadOBJ__("klot2_.obj"); //61856 //61856 //13108_Eastern_Hognose_Snake_v1_L3  61856
 
springVector = malloc(SIZE_OBJECT*1);
state_result_worm_ventral_feather = (struct state_vector*)malloc(sizeof(struct state_vector*)*SIZE_OBJECT*4);
 


state_result_worm_ventral= (struct state_vector ***) malloc(sizeof(struct state_vector**) * SIZE_OBJECT); //FIX 1
for(i = 0; i < SIZE_OBJECT; i++){
  //printf("h\n");
  state_result_worm_ventral[i] = (struct state_vector **) malloc(sizeof(struct state_vector*) * SIZE_OBJECT);  // FIX 2
  for(j = 0; j < SIZE_OBJECT; j++){
       // printf("j\n");
        state_result_worm_ventral[i][j] = (struct state_vector *) malloc(sizeof(struct state_vector) * HOW_MANY_FLUIDS);
  }
}

 
printf("done ! \n");
 
for(int i=0; i<SIZE_OBJECT*1; i++)
	state_result_worm_ventral_feather[i] = (struct state_vector*)malloc(sizeof(struct state_vector)*32);
  
printf("done ! \n");
  
 
if(initonce==1)
{
	Vec3 *v ;
 
	int ll;
for(int kk = 0; kk < HOW_MANY_FLUIDS  ; kk++)
	{
	for(ll = 0; ll < SIZE_OBJECT  ; ll++)
	{
		  v= (Vec3 *)(V + 3*(KvvVENTALA[ll]-1));
         	  state_result_worm_ventral[ll][kk]->pos_new_x = v->x+kk*10 ;
 
 	          state_result_worm_ventral[ll][kk]->pos_new_y = v->y ;
 		  state_result_worm_ventral[ll][kk]->pos_new_z = v->z +kk*10;
 
 	  	  state_result_worm_ventral[ll][kk]->vel_new_x =   1e-6 ;
 		  state_result_worm_ventral[ll][kk]->vel_new_y =  1e-6 ;
 		  state_result_worm_ventral[ll][kk]->vel_new_z =  1e-5 ;
	}
}
 
	for(ll = 0; ll < SIZE_OBJECT     ; ll++)
	{
 		findnearestpoint(SIZE_OBJECT  ,state_result_worm_ventral,ll,1,0 );
	}
 
	initonce = 0;
}

pipe = loadOBJ__big("ico.obj");
 wall_info = (struct state_vector*)malloc(sizeof(struct state_vector*)*KvvVENTAL2*4);

 

for(int i=0; i<KvvVENTAL2*1; i++)
	wall_info[i] = (struct state_vector*)malloc(sizeof(struct state_vector)*32);
 

 
	 
	  
 
	printf("done ! \n");
 
	Vec3 *v ;
 
	int ll;
	for(ll = 0; ll < KvvVENTAL2  ; ll++)
	{
		  v= (Vec3 *)(V2 + 3*(KvvVENTALA2[ll]-1));
         	  wall_info[ll]->pos_new_x = v->x ;
 			 
 	          wall_info[ll]->pos_new_y =  v->y ;
 		  wall_info[ll]->pos_new_z = v->z ;
 
 	  	  wall_info[ll]->vel_new_x =  0 ;
 		  wall_info[ll]->vel_new_y = 0;
 		  wall_info[ll]->vel_new_z =  0 ;
		  wall_info[ll]->mass = ll;
 
     		//  printf("%f::%f::%f  %d \n",wall_info[ll]->pos_new_x,wall_info[ll]->pos_new_y,wall_info[ll]->pos_new_z);
	}
printf("done ! \n");
glutInit(&argc, argv);
init_mpgeg();

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
material->setSpecular_material(0.01f, 0.01f, 0.01f, 1.0f/50.0f);
material->setShininess_material(128);
glLightfv(GL_LIGHT0, GL_POSITION, dirLight->pos);
glLightfv(GL_LIGHT0, GL_AMBIENT,   dirLight->ambient);
glLightfv(GL_LIGHT0, GL_SPECULAR,  dirLight->specular);
glLightfv(GL_LIGHT0, GL_DIFFUSE,   dirLight->difusse);  


glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);  
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
 for(int kk = 0; kk <   HOW_MANY_FLUIDS; kk++)  
		  {
   for(int ll = 0; ll <   SIZE_OBJECT/4 ; ll++)  
		  {

int INDEX_NR___ =  (state_result_worm_ventral_feather[ll][kk].INDEX_NR);
      calc_fluid_acceleration(ll,kk);
INDEX_NR___ = ll;
      calc_fluid_velocity(ll,kk);

taskworm(kk);
glPushMatrix();
glScalef(25,25,25);
  glTranslatef(state_result_worm_ventral[ll][kk]->pos_new_x, state_result_worm_ventral[ll][kk]->pos_new_y,state_result_worm_ventral[ll][kk]->pos_new_z);
 glutSolidSphere(0.1,29,29  );


	glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
glPushMatrix();
     glColor3f(255,255,255);     // Red
glScalef(25,25,25);
  glTranslatef(wall_info[ll]->pos_new_x, wall_info[ll]->pos_new_y,wall_info[ll]->pos_new_z);
 glutSolidSphere(0.003,29,29  );
 glutPostRedisplay();
glPopMatrix();
/*glBegin(GL_TRIANGLES);
     glColor3f(state_result_worm_ventral[ll]->pos_new_x, state_result_worm_ventral[ll]->pos_new_y, state_result_worm_ventral[ll]->pos_new_z);     // Red
      glVertex3f(0, 1.0f/50.0f, 0.0f);
      glColor3f(state_result_worm_ventral[ll]->pos_new_x, state_result_worm_ventral[ll]->pos_new_y,state_result_worm_ventral[ll]->pos_new_z);     // Green
      glVertex3f(-1.0f/50.0f, -1.0f/50.0f, 1.0f/50.0f);
      glColor3f(state_result_worm_ventral[ll]->pos_new_x, state_result_worm_ventral[ll]->pos_new_y, state_result_worm_ventral[ll]->pos_new_z);     // Blue
      glVertex3f(1.0f/50.0f, -1.0f/50.0f, 1.0f/50.0f);
 
      // Right
      glColor3f(state_result_worm_ventral[ll]->pos_new_x, state_result_worm_ventral[ll]->pos_new_y, state_result_worm_ventral[ll]->pos_new_z);     // Red
      glVertex3f(0.0f, 1.0f/50.0f, 0.0f);
      glColor3f(state_result_worm_ventral[ll]->pos_new_x, state_result_worm_ventral[ll]->pos_new_y, state_result_worm_ventral[ll]->pos_new_z);     // Blue
      glVertex3f(1.0f/50.0f, -1.0f/50.0f, 1.0f/50.0f);
      glColor3f(state_result_worm_ventral[ll]->pos_new_x,state_result_worm_ventral[ll]->pos_new_y, state_result_worm_ventral[ll]->pos_new_z);     // Green
      glVertex3f(1.0f/50.0f, -1.0f/50.0f, -1.0f/50.0f);
 
      // Back
      glColor3f(state_result_worm_ventral[ll]->pos_new_x,state_result_worm_ventral[ll]->pos_new_y, state_result_worm_ventral[ll]->pos_new_z);     // Red
      glVertex3f(0.0f, 1.0f/50.0f, 0.0f);
      glColor3f(state_result_worm_ventral[ll]->pos_new_x, state_result_worm_ventral[ll]->pos_new_y, state_result_worm_ventral[ll]->pos_new_z);     // Green
      glVertex3f(1.0f/50.0f, -1.0f/50.0f, -1.0f/50.0f);
      glColor3f(state_result_worm_ventral[ll]->pos_new_x, state_result_worm_ventral[ll]->pos_new_y, state_result_worm_ventral[ll]->pos_new_z);     // Blue
      glVertex3f(-1.0f/50.0f, -1.0f/50.0f, -1.0f/50.0f);
 
      // Left
      glColor3f(state_result_worm_ventral[ll]->pos_new_x,state_result_worm_ventral[ll]->pos_new_y,state_result_worm_ventral[ll]->pos_new_z);       // Red
      glVertex3f( 0.0f, 1.0f/50.0f, 0.0f);
      glColor3f(state_result_worm_ventral[ll]->pos_new_x,state_result_worm_ventral[ll]->pos_new_y,state_result_worm_ventral[ll]->pos_new_z);       // Blue
      glVertex3f(-1.0f/50.0f,-1.0f/50.0f,-1.0f/50.0f);
      glColor3f(state_result_worm_ventral[ll]->pos_new_x,state_result_worm_ventral[ll]->pos_new_y,state_result_worm_ventral[ll]->pos_new_z);       // Green
      glVertex3f(-1.0f/50.0f,-1.0f/50.0f, 1.0f/50.0f);
	glEnd(); */ 	
 	
 
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
 
if(state_result_worm_ventral[llll][howmanyfluids_g]->force_sign == 1)
{
    	result[0] =             f[3]/100.0;
    	result[1] =             f[4]/100.0;
    	result[2] =             f[5]/100.0;
  
	result[3] = (F_total[0][llll][howmanyfluids_g]+acc2_fluid[0][llll][howmanyfluids_g])/(40000.0   );
	result[4] = (F_total[1][llll][howmanyfluids_g] +acc2_fluid[1][llll][howmanyfluids_g]  )/(40000.0   );
	result[5] = (F_total[2][llll][howmanyfluids_g] +acc2_fluid[2][llll][howmanyfluids_g])/(40000.0); 

 
}
else if(state_result_worm_ventral[INDEX_NR][howmanyfluids_g]->force_sign == -1)
{
    result[0] =             f[3]/100.0;
    result[1] =             f[4]/100.0;
    result[2] =             f[5]/100.0;
  
    result[3] = -(F_total[0][INDEX_NR][howmanyfluids_g]+acc2_fluid[0][INDEX_NR][howmanyfluids_g])/(40000.0  );
    result[4] = -(F_total[1][INDEX_NR][howmanyfluids_g]  +acc2_fluid[1][INDEX_NR][howmanyfluids_g]) /(40000.0) ;
    result[5] = -(F_total[2][INDEX_NR][howmanyfluids_g] +acc2_fluid[2][INDEX_NR][howmanyfluids_g])/(40000.0  ); //522
 

}
else if(state_result_worm_ventral[INDEX_NRmore][howmanyfluids_g]->force_sign == 1)
{
    	result[0] =             f[3]/100.0;
    	result[1] =             f[4]/100.0;
    	result[2] =             f[5]/100.0;
  
	result[3] = (F_total[0][INDEX_NRmore][howmanyfluids_g]+acc2_fluid[0][INDEX_NRmore][howmanyfluids_g])/(400.0   );
	result[4] = (F_total[1][INDEX_NRmore][howmanyfluids_g] -9.8 +acc2_fluid[1][INDEX_NRmore][howmanyfluids_g] )/(400.0   );
	result[5] = (F_total[2][INDEX_NRmore][howmanyfluids_g] +acc2_fluid[2][INDEX_NRmore][howmanyfluids_g])/(400.0); 

 
}
  acc1 = sqrtf(pow(result[3],2.0) + pow(result[4],2.0)+pow(result[5],2.0));
//printf("acc1 = %.10f \n", acc1);
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

	double Area_tube = powf(water_radius,2)*M_PI;
	double mass_fluid_element = density*(M_PI*powf(water_radius,3.0))*4.0/3.0;
	double gMASS;
 

	if(pressure_once ==1)
	{
		state_result_worm_ventral[ll][j]->pressure =  ((  -9.8+0)*mass_fluid_element/(M_PI*pow(water_radius,2.0)));
		pressure_once =0;
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

    acc2_fluid[0][ll][j] = (-1*(state_result_worm_ventral[ll][j]->pressure )/(( state_result_worm_ventral[ll][j]->pos_new_x)))/density + (viscous_constant*Area_tube*addme)/mass_fluid_element;
    acc2_fluid[1][ll][j] = (-1*(state_result_worm_ventral[ll][j]->pressure )/(( state_result_worm_ventral[ll][j]->pos_new_y)))/density + (viscous_constant*Area_tube*addme)/mass_fluid_element;
    acc2_fluid[2][ll][j] = (-1*(state_result_worm_ventral[ll][j]->pressure )/(( state_result_worm_ventral[ll][j]->pos_new_z)))/density + (viscous_constant*Area_tube*addme)/mass_fluid_element;


	//printf(" %f %f  %f \n",     acc2_fluid[0][ll][j] ,    acc2_fluid[1][ll][j] ,    acc2_fluid[2][ll][j] );

}
int alloc_once_2 =1;
void findnearestpoint_3_points(int points , int num,double r_collision, struct state_vector **vvv,int ll, int j )
{
	int i, num_pts ;
 
	char  *pch;
	struct kdres *presults;
	double pos[3], dist;
	double pt[3] = { state_result_worm_ventral[ll][j]->pos_new_x,state_result_worm_ventral[ll][j]->pos_new_y, state_result_worm_ventral[ll][j]->pos_new_y };
	double radius = r_collision;
 
	num_pts =points;

 	void *ptree_____;

 	if(alloc_once == 1 || alloc_once_2 == 1)
	{
		ptree_____ = kd_create( 3 );
 
  		for( i=0; i<points; i++ ) {   
		     //   printf("target node at (%.3f, %.3f, %.3f) \n", wall_info[i]->pos_new_x, wall_info[i]->pos_new_y,wall_info[i]->pos_new_z);
    			assert( 0 == kd_insert3( ptree_____, vvv[i]->pos_new_x, vvv[i]->pos_new_y, vvv[i]->pos_new_z, NULL ) );

	}
		alloc_once = 0;
alloc_once_2 = 0;
	}
 
 
	presults = kd_nearest_range( ptree_____, pt, radius );
 
  	/* print out all the points found in results */
	// printf( "found %d results:\n", kd_res_size(presults) );

 
	//state_result_worm_dorsal_feather[num]->totaln = kd_res_size(presults) ;
  //printf( "found %d results:\n", kd_res_size(presults) );

 
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
	 
   		
 		// INDEX = find_index_____(SIZE_OBJECT,worm,num,feather_count);
 		 //near_vector[num][feather_count].INDEX_NR = INDEX;
	 	if(vc > 4)
			break;
		
 vc++;
   		 kd_res_next( presults );
 	 
 
    	  }
    

   vc = 0;
   kd_res_free( presults );
   kd_free( ptree_____ );

}

void calc_fluid_velocity(int ll, int j)
{
int vn = ll;
	double distance_from_origin = sqrtf(powf( state_result_worm_ventral[ll][j]->pos_new_x - 0 ,2.0) +  powf( state_result_worm_ventral[ll][j]->pos_new_y -0 ,2.0)  + 		powf( state_result_worm_ventral[ll][j]->pos_new_z -0 ,2.0  ));
 
//if(( distance_from_origin) > 65)
//{

 

 
		 

		//need 3 points	
 
		double sqrtthis = powf( state_result_worm_ventral[ll][j]->pos_new_x - wall_info[vn]->pos_new_x ,2.0) +  powf( state_result_worm_ventral[ll][j]->pos_new_y - 	wall_info[vn]->pos_new_y ,2.0)  + powf( state_result_worm_ventral[ll][j]->pos_new_z - wall_info[vn]->pos_new_z ,2.0  );
 
		double xxxx=  (sqrtthis ); 
  		complex zzzz;
 

 		 if((sqrtthis)  < 0)
	  	{
 			zzzz = csqrt(sqrtthis);
		  	xxxx = creal(zzzz);
		}
		else if ((sqrtthis)  > 0)	  
			xxxx = sqrtf(sqrtthis ); 


		 
		if( fabs(xxxx) > 0 &&  fabs(xxxx) <1)
		{
 
			double u_perpv[3];
			double w_parv[3]; 
			double * normal = malloc(100) ;
			double * dotprod_v_n= malloc(100) ;
			double * dotprod_n_n= malloc(100) ;
			double * vel = malloc(100);


  
		
			findnearestpoint_3_points(KvvVENTAL2/8   ,   vn, 11, wall_info ,ll,j);
 
			double aa[] = {state_result_worm_ventral[ll][j]->pos_new_x,state_result_worm_ventral[ll][j]->pos_new_y,state_result_worm_ventral[ll][j]->pos_new_z};
			double bb[] = {neighbourV_[1][0],neighbourV_[1][1],neighbourV_[1][2]};
			double cc[] = {neighbourV_[2][0],neighbourV_[2][1],neighbourV_[2][2]};
	 

			calculate_normal(aa ,bb ,cc ,normal);


			vel[0] =  state_result_worm_ventral[ll][j]->vel_new_x ;
			vel[1] = state_result_worm_ventral[ll][j]->vel_new_y;
			vel[2] =state_result_worm_ventral[ll][j]->vel_new_z;

 			dot(   vel ,  normal, dotprod_v_n);
			dot( normal, normal, dotprod_n_n);

			//u_perpv = (v · n / n · n) n
			u_perpv[0] = (dotprod_v_n[0]/dotprod_n_n[0])*normal[0];
			u_perpv[1] = (dotprod_v_n[1]/dotprod_n_n[1])*normal[1];
			u_perpv[2] = (dotprod_v_n[2]/dotprod_n_n[2])*normal[2];


			w_parv[0] = vel[0] - u_perpv[0]  ;
			w_parv[1] = vel[1] - u_perpv[1]  ;
			w_parv[2] = vel[2] - u_perpv[2]  ;
 
			printf("vel[0]   =%.10f \n", vel[0] );
			printf("u_perpv[0]   =%.10f \n", u_perpv[0] );
			printf("w_parv[0]  =%.10f\n", w_parv[0] );
			printf("normal[0]  =%.10f\n", normal[0] );
			printf("neighbourV[0][1]  =%.10f\n", neighbourV_[0][1]); 
 
			//v′ = f w − r u
			double r =0.851;
			double frictionforce = 123.00000001;
			state_result_worm_ventral[ll][j]->vel_new_x = frictionforce*w_parv[0] - r*u_perpv[0];
			state_result_worm_ventral[ll][j]->vel_new_y = frictionforce*w_parv[1] - r*u_perpv[1];
			state_result_worm_ventral[ll][j]->vel_new_z = frictionforce*w_parv[2] - r*u_perpv[2];
		 
 		//	state_result_worm_ventral[ll][j] = mass_motion( mass_motion_state[howmanyfluids_g] ,1,0);
 		 	taskworm(j);
//
 glutPostRedisplay();
			free(dotprod_v_n);
			free(dotprod_n_n);
 			free(normal);
 			free(vel);
			alloc_once = 1;


			return;
		}
	 
}






void calc_fluid_velocity_drops_reflect(int ll, int j)
{

 
 
//if(( distance_from_origin) > 65)
//{
for(int n = 0; n < SIZE_OBJECT; n++)
	{
	for(int k = 0; k < HOW_MANY_FLUIDS; k++)
	{

		if(j == k)
			continue;

		//need 3 points	
 
		double sqrtthis = powf( state_result_worm_ventral[ll][j]->pos_new_x - state_result_worm_ventral[n][k]->pos_new_x ,2.0) +  powf( state_result_worm_ventral[ll][j]->pos_new_y - 	state_result_worm_ventral[n][j]->pos_new_y,2.0)  + powf( state_result_worm_ventral[ll][j]->pos_new_z - state_result_worm_ventral[n][j]->pos_new_z ,2.0  );
 
		double xxxx=  (sqrtthis ); 
  		complex zzzz;
 

 		 if((sqrtthis)  < 0)
	  	{
 			zzzz = csqrt(sqrtthis);
		  	xxxx = creal(zzzz);
		}
		else if ((sqrtthis)  > 0)	  
			xxxx = sqrtf(sqrtthis ); 


		 
		if( fabs(xxxx) > 0 &&  fabs(xxxx) <0.1)
		{
 
			double u_perpv[3];
			double w_parv[3]; 
			double * normal = malloc(100) ;
			double * dotprod_v_n= malloc(100) ;
			double * dotprod_n_n= malloc(100) ;
			double * vel = malloc(100);


  
		
			findnearestpoint_3_points(KvvVENTAL/8    ,   k, 11, state_result_worm_ventral ,ll,j);
 
			double aa[] = {state_result_worm_ventral[ll][j]->pos_new_x,state_result_worm_ventral[ll][j]->pos_new_y,state_result_worm_ventral[ll][j]->pos_new_z};
			double bb[] = {neighbourV_[1][0],neighbourV_[1][1],neighbourV_[1][2]};
			double cc[] = {neighbourV_[2][0],neighbourV_[2][1],neighbourV_[2][2]};
	 

			calculate_normal(aa ,bb ,cc ,normal);


			vel[0] =  state_result_worm_ventral[ll][j]->vel_new_x ;
			vel[1] = state_result_worm_ventral[ll][j]->vel_new_y;
			vel[2] =state_result_worm_ventral[ll][j]->vel_new_z;

 			dot(   vel ,  normal, dotprod_v_n);
			dot( normal, normal, dotprod_n_n);

			//u_perpv = (v · n / n · n) n
			u_perpv[0] = (dotprod_v_n[0]/dotprod_n_n[0])*normal[0];
			u_perpv[1] = (dotprod_v_n[1]/dotprod_n_n[1])*normal[1];
			u_perpv[2] = (dotprod_v_n[2]/dotprod_n_n[2])*normal[2];


			w_parv[0] = vel[0] - u_perpv[0]  ;
			w_parv[1] = vel[1] - u_perpv[1]  ;
			w_parv[2] = vel[2] - u_perpv[2]  ;
 
			printf("vel[0]   =%.10f \n", vel[0] );
			printf("u_perpv[0]   =%.10f \n", u_perpv[0] );
			printf("w_parv[0]  =%.10f\n", w_parv[0] );
			printf("normal[0]  =%.10f\n", normal[0] );
			printf("neighbourV[0][1]  =%.10f\n", neighbourV_[0][1]); 
 
			//v′ = f w − r u
			double r =0.851;
			double frictionforce = 123.00000001;
			state_result_worm_ventral[ll][j]->vel_new_x = frictionforce*w_parv[0] - r*u_perpv[0];
			state_result_worm_ventral[ll][j]->vel_new_y = frictionforce*w_parv[1] - r*u_perpv[1];
			state_result_worm_ventral[ll][j]->vel_new_z = frictionforce*w_parv[2] - r*u_perpv[2];
		 
 		//	state_result_worm_ventral[ll][j] = mass_motion( mass_motion_state[howmanyfluids_g] ,1,0);
 		 	taskworm(j);
//
 glutPostRedisplay();
			free(dotprod_v_n);
			free(dotprod_n_n);
 			free(normal);
 			free(vel);
			alloc_once_2 = 1;


			return;
		}
	}
}
}
