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
float acc2;
int INDEX_NRmore;
extern int KvvVENTAL ;
#define VAR KvvVENTAL
float springlength =  0.004/4.0;
#define AIR_FRICTION 0.995995
#define FIRST 1
#define WORK 0
//float fovy = 45.0;
float dNear = 100;
float dFar = 2000;
struct state_vector       worm_ventral(struct state_vector       next_state, int num );
/*double F_total[3][100000*104] ;
int totalneigbours[100000*104];
float     vertexpoint_g_ventral[100000][3];
extern struct state_vector * springVector;
extern struct state_vector ** state_result_worm_ventral_feather;
extern struct state_vector  ** state_result_worm_ventral ; */


#define HOW_MANY_FLUIDS 1
#define HOWMANY HOW_MANY_FLUIDS


double F_total[3][100000*2][HOW_MANY_FLUIDS][10] ;
int totalneigbours[100000*2][HOW_MANY_FLUIDS] ;
float     vertexpoint_g_ventral[100000][3];
extern struct state_vector * springVector;
extern struct state_vector *** state_result_worm_ventral_feather;
extern struct state_vector *** reall;
extern struct state_vector  *** state_result_worm_ventral ;



extern int KvvVENTALA[100000*8];
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
int findnearestpoint(int points,struct state_vector ** worm , int num,int howmany);
 
 
double length (struct state_vector a,struct state_vector b ) { return sqrtf ( b.pos_new_x*a.pos_new_x + b.pos_new_y*a.pos_new_y + b.pos_new_z*a.pos_new_z); }
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
/*
void mouse(int button, int state, int x, int y){
    if(button == GLUT_LEFT_BUTTON && state==GLUT_DOWN){
        mouseDown = true;
        prevX = x - rotationY;
        prevY = y - rotationX;
    }else{
        mouseDown = false;
    }
}*/

void mouseMotion(int x, int y){
    if(mouseDown){
        rotationX = y - prevY;
        rotationY = x - prevX;
        glutPostRedisplay();
    }
}
 
 /*
void reshape(int w, int h){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fovy, (GLdouble)w/(GLdouble)h, dNear, dFar);
    glViewport(0, 0, w, h);
} */
int k_flappers = 0;
int acc1once= 1;
void taskworm( int kkkkkkkk, int xxx )
{
	llll = kkkkkkkk;
	///0_g =0;
	 k_flappers = xxx;
	int k = xxx;

	//findnearestpoint_3_points_water_pressure(c_m      ,   llll, .065 , NULL ,llll,0,xxx);
 
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
 
	INDEX_NR =   state_result_worm_ventral_feather[llll][k][0].INDEX_NR ;
	
if(  INDEX_NR > 0  )
{
		 if(feather_once == FIRST && INDEX_NR > 0)
		{	

				 if(fabs(F_total[0][INDEX_NR][0][k]) > 21 )
	 				 	  F_total[0][INDEX_NR][0][k] =0;
 
		 		 if(fabs(F_total[1][INDEX_NR][0][k])  > 21)
						F_total[1][INDEX_NR][0][k] =0;

			 	 if(fabs(F_total[2][INDEX_NR][0][k])  > 21)
						F_total[2][INDEX_NR][0][k] =0;
 
				springVector->pos_new_x = state_result_worm_ventral[INDEX_NR][k][0].pos_new_x - state_result_worm_ventral[llll][k][0].pos_new_x;
				springVector->pos_new_y = state_result_worm_ventral[INDEX_NR][k][0].pos_new_y - state_result_worm_ventral[llll][k][0].pos_new_y;
				springVector->pos_new_z = state_result_worm_ventral[INDEX_NR][k][0].pos_new_z - state_result_worm_ventral[llll][k][0].pos_new_z;
 

				float r = length(state_result_worm_ventral[INDEX_NR][k][0],state_result_worm_ventral[llll][k][0]);

				if ( r != 0  &&    (r) <50  )
				{			
					F_total[0][INDEX_NR][0][k] +=( springVector->pos_new_x  /  r ) * ( r - springlength ) * ( -springConstant );
					F_total[1][INDEX_NR][0][k] += ( springVector->pos_new_y /  r ) * ( r - springlength ) * ( -springConstant );
					F_total[2][INDEX_NR][0][k] +=( springVector->pos_new_z  /  r ) * ( r - springlength ) * ( -springConstant );

					F_total[0][llll][0][k] +=( springVector->pos_new_x  / r ) * ( r - springlength ) * ( -springConstant );
					F_total[1][llll][0][k] += ( springVector->pos_new_y  / r ) * ( r - springlength) * ( -springConstant );
					F_total[2][llll][0][k] +=( springVector->pos_new_z  / r ) * ( r - springlength ) * ( -springConstant );

					F_total[0][INDEX_NR][0][k] +=-(  state_result_worm_ventral[INDEX_NR][k][0].vel_new_x - state_result_worm_ventral[llll][k][0].vel_new_x ) * frictionConstant;
 					F_total[1][INDEX_NR][0][k] += -( state_result_worm_ventral[INDEX_NR][k][0].vel_new_y- state_result_worm_ventral[llll][k][0].vel_new_y ) * frictionConstant;
 					F_total[2][INDEX_NR][0][k] += -(  state_result_worm_ventral[INDEX_NR][k][0].vel_new_z - state_result_worm_ventral[llll ][k][0].vel_new_z ) * frictionConstant;


					F_total[0][llll][0][k] +=-(  state_result_worm_ventral[INDEX_NR][k][0].vel_new_x - state_result_worm_ventral[llll][k][0].vel_new_x ) * frictionConstant;
 					F_total[1][llll][0][k] += -( state_result_worm_ventral[INDEX_NR][k][0].vel_new_y- state_result_worm_ventral[llll][k][0].vel_new_y ) * frictionConstant;
 					F_total[2][llll][0][k] += -(  state_result_worm_ventral[INDEX_NR][k][0].vel_new_z - state_result_worm_ventral[llll ][k][0].vel_new_z ) * frictionConstant;
	  state_result_worm_ventral[llll][k][0].vel_new_x = state_result_worm_ventral[llll][k][0].vel_new_x*0.995 ;
		
		state_result_worm_ventral[llll][k][0].vel_new_y = state_result_worm_ventral[llll][k][0].vel_new_y*0.995 ;
		state_result_worm_ventral[llll][k][0].vel_new_z = state_result_worm_ventral[llll][k][0].vel_new_z*0.995 ;
		state_result_worm_ventral[INDEX_NR][k][0].vel_new_x = state_result_worm_ventral[INDEX_NR][k][0].vel_new_x*0.995 ;
		
		state_result_worm_ventral[INDEX_NR][k][0].vel_new_y = state_result_worm_ventral[INDEX_NR][k][0].vel_new_y*0.995 ; 
		state_result_worm_ventral[INDEX_NR][k][0].vel_new_z= state_result_worm_ventral[INDEX_NR][k][0].vel_new_z*0.995 ; 
		}

		

		state_result_worm_ventral[INDEX_NR][k][0].force_sign =  -1;
		state_result_worm_ventral[INDEX_NR][k][0]    =  worm_ventral(   state_result_worm_ventral[INDEX_NR][k][0] ,1) ;

		state_result_worm_ventral[llll][k][0].force_sign =   1; ; 
		state_result_worm_ventral[llll][k][0]    =  worm_ventral(   state_result_worm_ventral[llll][k][0] ,1);
 
	for(int d = 0; d <= totalneigbours[llll][0]; d++)
	{	 
		
		if(d == k  )
		  continue;
 
		INDEX_NRmore = state_result_worm_ventral_feather[llll][d][0].INDEX_NR;
  
		if(fabs(F_total[0][INDEX_NRmore][0][k]) > 21 )
	 				  F_total[0][INDEX_NRmore][0][k] =0;
 
		if(fabs(F_total[1][INDEX_NRmore][0][k])  > 21)
					  F_total[1][INDEX_NRmore][0][k] =0;

		if(fabs(F_total[2][INDEX_NRmore][0][k])  > 21)
					  F_total[2][INDEX_NRmore][0][k] =0;
			

 		if(fabs(F_total[0][INDEX_NR][0][k]) > 21 )
	 				  F_total[0][INDEX_NR][0][k] =0;
 
		if(fabs(F_total[1][INDEX_NR][0][k])  > 21)
					  F_total[1][INDEX_NR][0][k] =0;

		if(fabs(F_total[2][INDEX_NR][0][k])  > 21)
					  F_total[2][INDEX_NR][0][k] =0;
			
		springVector->pos_new_x = state_result_worm_ventral[INDEX_NR][k][0].pos_new_x - state_result_worm_ventral[INDEX_NRmore][k][0].pos_new_x;
		springVector->pos_new_y = state_result_worm_ventral[INDEX_NR][k][0].pos_new_y - state_result_worm_ventral[INDEX_NRmore][k][0].pos_new_y;
		springVector->pos_new_z = state_result_worm_ventral[INDEX_NR][k][0].pos_new_z - state_result_worm_ventral[INDEX_NRmore][k][0].pos_new_z;
 			

		float r = length(state_result_worm_ventral[INDEX_NR][k][0],state_result_worm_ventral[INDEX_NRmore][k][0]);

		if ( r != 0  &&   (r) < 50    )
		{	
 
			F_total[0][INDEX_NR][0][k] +=( springVector->pos_new_x  /  r ) * ( r - springlength ) * ( -springConstant );
			F_total[1][INDEX_NR][0][k] += ( springVector->pos_new_y /  r ) * ( r - springlength ) * ( -springConstant );
			F_total[2][INDEX_NR][0][k] +=( springVector->pos_new_z  /  r ) * ( r - springlength ) * ( -springConstant );

			F_total[0][INDEX_NRmore][0][k] +=( springVector->pos_new_x  / r ) * ( r - springlength ) * ( -springConstant );
			F_total[1][INDEX_NRmore][0][k] += ( springVector->pos_new_y  / r ) * ( r - springlength) * ( -springConstant );
			F_total[2][INDEX_NRmore][0][k] +=( springVector->pos_new_z  / r ) * ( r - springlength ) * ( -springConstant );

			F_total[0][INDEX_NR][0][k] +=-(  state_result_worm_ventral[INDEX_NR][k][0].vel_new_x - state_result_worm_ventral[INDEX_NRmore][k][0].vel_new_x ) * frictionConstant;
 			F_total[1][INDEX_NR][0][k] += -( state_result_worm_ventral[INDEX_NR][k][0].vel_new_y- state_result_worm_ventral[INDEX_NRmore][k][0].vel_new_y ) * frictionConstant;
 			F_total[2][INDEX_NR][0][k] += -(  state_result_worm_ventral[INDEX_NR][k][0].vel_new_z - state_result_worm_ventral[INDEX_NRmore ][k][0].vel_new_z ) * frictionConstant;


			F_total[0][INDEX_NRmore][0][k] +=-(  state_result_worm_ventral[INDEX_NR][k][0].vel_new_x - state_result_worm_ventral[INDEX_NRmore][k][0].vel_new_x ) * frictionConstant;
 			F_total[1][INDEX_NRmore][0][k] += -( state_result_worm_ventral[INDEX_NR][k][0].vel_new_y- state_result_worm_ventral[INDEX_NRmore][k][0].vel_new_y ) * frictionConstant;
 			F_total[2][INDEX_NRmore][0][k] += -(  state_result_worm_ventral[INDEX_NR][k][0].vel_new_z - state_result_worm_ventral[INDEX_NRmore ][k][0].vel_new_z ) * frictionConstant;

		  state_result_worm_ventral[INDEX_NRmore][k][0].vel_new_x = state_result_worm_ventral[INDEX_NRmore][k][0].vel_new_x*0.995 ;
		
		state_result_worm_ventral[INDEX_NRmore][k][0].vel_new_y = state_result_worm_ventral[INDEX_NRmore][k][0].vel_new_y*0.995 ;
		state_result_worm_ventral[INDEX_NRmore][k][0].vel_new_z = state_result_worm_ventral[INDEX_NRmore][k][0].vel_new_z*0.995 ;
		state_result_worm_ventral[INDEX_NR][k][0].vel_new_x = state_result_worm_ventral[INDEX_NR][k][0].vel_new_x*0.995 ;
		
		state_result_worm_ventral[INDEX_NR][k][0].vel_new_y = state_result_worm_ventral[INDEX_NR][k][0].vel_new_y*0.995 ;
		state_result_worm_ventral[INDEX_NR][k][0].vel_new_z = state_result_worm_ventral[INDEX_NR][k][0].vel_new_z*0.995 ;  
		}



		state_result_worm_ventral[INDEX_NR][k][0].force_sign =  -1;
		state_result_worm_ventral[INDEX_NR][k][0]    =  worm_ventral(   state_result_worm_ventral[INDEX_NR][k][0] ,1) ;

		state_result_worm_ventral[INDEX_NRmore][k][0].force_sign =   1; ; 
		state_result_worm_ventral[INDEX_NRmore][k][0]    =  worm_ventral(   state_result_worm_ventral[INDEX_NRmore][k][0] ,1);


glPushMatrix();

  
  glTranslatef(state_result_worm_ventral[INDEX_NR][0]->pos_new_x, state_result_worm_ventral[INDEX_NR][0]->pos_new_y,state_result_worm_ventral[INDEX_NR][0]->pos_new_z);
          glutSolidSphere(0.1,20,20  );	
 	
 

 glutPostRedisplay();

 glPopMatrix();
  
 	} 		 
 }	
 
		 
 }		
				 if(fabs(F_total[0][llll][0][k]) > 21 )
	 				 F_total[0][llll][0][k] =0;
 
		 		 if(fabs(F_total[1][llll][0][k])  > 21)
					 F_total[1][llll][0][k] =0;

			 	 if(fabs(F_total[2][llll][0][k])  > 21)
					 F_total[2][llll][0][k] =0;
 
}

int find_index_____(int NUM, struct state_vector ***real  ,int num,int k )
{
int ll = 0;
 	while(ll < NUM)
	{
		if(state_result_worm_ventral_feather[num][k][0].pos_new_x ==real[ll][0]->pos_new_x && state_result_worm_ventral_feather[num][k][0].pos_new_y ==real[ll][0]->pos_new_y && 		state_result_worm_ventral_feather[num][k][0].pos_new_z ==real[ll][0]->pos_new_z)
		{
		//	 printf("found index_____ @ %d \n", ll);
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
	double pt[3] = { state_result_worm_ventral[num][0][0].pos_new_x,state_result_worm_ventral[num][0][0].pos_new_y, state_result_worm_ventral[num][0][0].pos_new_z };
double radius = 0.0004/1.0;

num_pts =points;

srand( time(0) );

if(ptreeonce < 2  )
{
	ptree = kd_create( 3 );
  	for( i=0; i<points; i++ ) {   
    		assert( 0 == kd_insert3( ptree, state_result_worm_ventral[i][0][0].pos_new_x, state_result_worm_ventral[i][0][0].pos_new_y, state_result_worm_ventral[i][0][0].pos_new_z, NULL ) );
 	 }
}
ptreeonce++;

 
presults = kd_nearest_range( ptree, pt, radius );
 
  /* print out all the points found in results */
  //printf( "found %d results:\n", kd_res_size(presults) );


 state_result_worm_ventral_feather[num][0][0].totaln = kd_res_size(presults) ;

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
	 

  	totalneigbours[num][0] = state_result_worm_ventral_feather[num][0][0].totaln;
	// if(worm[num]->pos_new_x != pos[0])
   		state_result_worm_ventral_feather[num][feather_count][0].pos_new_x = pos[0];
	// if(worm[num]->pos_new_y != pos[1])
  		state_result_worm_ventral_feather[num][feather_count][0].pos_new_y = pos[1];
	//  if(worm[num]->pos_new_z != pos[2])
   		state_result_worm_ventral_feather[num][feather_count][0].pos_new_z = pos[2];
	int k;
 
   
  INDEX = find_index_____(KvvVENTAL,state_result_worm_ventral,num,feather_count);
 // printf("INDEX = %d \n", INDEX);
  state_result_worm_ventral_feather[num][feather_count][0].INDEX_NR = INDEX;
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

extern int KvvDORSAL ;

 
extern float* V2;

#include "catmull/gl.h"
#include "catmull/gl_util.h"
#include "catmull/mathx.h"
#include "catmull/mesh.h"
#include "catmull/meshrend.h"
#include "catmull/editor.h"
float  **V ; 
static struct editor *ed;
//extern vector v[600000];
#if 0
int main__ (int argc, char **argv)
{ 
V =(struct state_vector*)malloc(sizeof(struct state_vector*)*1000*32);


 int i,j,k;

for(int i=0; i<1000*1; i++)
	V[i] = (struct state_vector*)malloc(sizeof(struct state_vector)*32);


 ed = ed_create();
 //loadOBJ__( "sphere.obj" ); //catmull/objs/bigguy.obj
 
 //loadOBJ__("catmull/objs/bigguy.obj"); //61856 //61856 //13108_Eastern_Hognose_Snake_v1_L3  61856
 
springVector = malloc(KvvVENTAL*1);
state_result_worm_ventral_feather = (struct state_vector*)malloc(sizeof(struct state_vector*)*KvvVENTAL*4);

state_result_worm_ventral =(struct state_vector*)malloc(sizeof(struct state_vector*)*KvvVENTAL*32);


 

for(int i=0; i<KvvVENTAL*1; i++)
	state_result_worm_ventral[i] = (struct state_vector*)malloc(sizeof(struct state_vector)*32);
       
printf("done ! \n");
 
for(int i=0; i<KvvVENTAL*1; i++)
	state_result_worm_ventral_feather[i] = (struct state_vector*)malloc(sizeof(struct state_vector)*32);
  
printf("done ! \n");
  
 
if(initonce==1)
{
	 
 Vec3 * v;
	int ll;
	for(ll = 0; ll < KvvVENTAL  ; ll++)
	{

	   
  		  v= (Vec3 *)(V[ll] + 3*(KvvVENTAL-1)); 
	
         	  state_result_worm_ventral[ll]->pos_new_x = v->x ;
 
 	          state_result_worm_ventral[ll]->pos_new_y = v->y ;
 		  state_result_worm_ventral[ll]->pos_new_z = v->z ;

 	  	  state_result_worm_ventral[ll]->vel_new_x =   1e-6 ;
 		  state_result_worm_ventral[ll]->vel_new_y =  1e-6 ;
 		  state_result_worm_ventral[ll]->vel_new_z =  1e-5 ;
 printf("vector   %.10f  %.10f  %.10f  \n", v->x,v->y,v->z);
	}
 
 
	for(ll = 0; ll < KvvVENTAL     ; ll++)
	{
 		findnearestpoint(KvvVENTAL  ,state_result_worm_ventral,ll,1 );
	}
 
	initonce = 0;
}


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
material->setSpecular_material(0.01f, 0.01f, 0.01f, 1.0f/50.0f);
material->setShininess_material(128);
glLightfv(GL_LIGHT0, GL_POSITION, dirLight->pos);
glLightfv(GL_LIGHT0, GL_AMBIENT,   dirLight->ambient);
glLightfv(GL_LIGHT0, GL_SPECULAR,  dirLight->specular);
glLightfv(GL_LIGHT0, GL_DIFFUSE,   dirLight->difusse);  


glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);  
//glutDisplayFunc (display );
 
//glutReshapeFunc(reshape);
 
//glutMouseFunc(mouse);
glutKeyboardFunc(key);
  	
 
//glutMotionFunc(mouseMotion);
  
	
glutMainLoop();

}
#endif
struct state_vector       worm_ventral(struct state_vector        next_state, int num )
{
	double f0[6];
	double f0_result[6];

	double z[6];
 	double t0[6] = {0,0,0,0,0,0};
 	double tburn = 1.0;
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

void display__  (void){
 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_COLOR_MATERIAL);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glMultMatrixf(lightRotation);
 

    glLoadIdentity();
    gluLookAt(cameraEye[0]+0, cameraEye[1]+0, cameraEye[2]-0, cameraLookAt[0], cameraLookAt[1], cameraLookAt[2], cameraUp[0], cameraUp[1], cameraUp[2]);

    glMultMatrixf(viewRotation);

    glRotatef(rotationX,1,0,0);
    glRotatef(rotationY+320,0,1,0);

    glMultMatrixf(lightRotation);
glScalef(120,120,120);
   for(int ll = 0; ll <  KvvVENTAL; ll++)  
		  { 
taskworm(ll,0);
}
 

display_mpeg();

glutSwapBuffers();

}




void rates_dorsal ( double *t, double *f, double result[]   )
{
// printf("f[0] = %.10f:%.10f%.10f \n", f[0],f[1],f[2]);
if(state_result_worm_ventral[llll][0]->force_sign == 1)
{
    	result[0] =             f[3]/100.0;
    	result[1] =             f[4]/100.0;
    	result[2] =             f[5]/100.0;
  
	result[3] = (F_total[0][llll][0][k_flappers])/(40000.0   );
	result[4] = (F_total[1][llll][0][k_flappers]   )/(40000.0   );
	result[5] = (F_total[2][llll][0][k_flappers] )/(40000.0); 

 
}
else if(state_result_worm_ventral[INDEX_NR][0]->force_sign == -1)
{
    result[0] =             f[3]/100.0;
    result[1] =             f[4]/100.0;
    result[2] =             f[5]/100.0;
  
    result[3] = -(F_total[0][INDEX_NR][0][k_flappers])/(40000.0  );
    result[4] = -(F_total[1][INDEX_NR][0][k_flappers]  ) /(40000.0) ;
    result[5] = -(F_total[2][INDEX_NR][0][k_flappers] )/(40000.0  ); //522
 

}
else if(state_result_worm_ventral[INDEX_NRmore][0]->force_sign == 1)
{
    	result[0] =             f[3]/100.0;
    	result[1] =             f[4]/100.0;
    	result[2] =             f[5]/100.0;
  
	result[3] = (F_total[0][INDEX_NRmore][0][k_flappers])/(400.0   );
	result[4] = (F_total[1][INDEX_NRmore][0][k_flappers] -9.8  )/(400.0   );
	result[5] = (F_total[2][INDEX_NRmore][0][k_flappers] )/(400.0); 

 
}
  acc1 = sqrtf(pow(result[3],2.0) + pow(result[4],2.0)+pow(result[5],2.0));
//printf("acc1 = %.10f \n", acc1);
}





#include <stdio.h>
#include <stdlib.h>
#include "catmull/gl.h"
#include "catmull/gl_util.h"
#include "catmull/mathx.h"
#include "catmull/mesh.h"
#include "catmull/meshrend.h"
#include "catmull/editor.h"

static struct editor *ed;

static vector center = { 0.0, 0.0, 0.0 };
static float focal_len = 5.0f;
static float y_rot = 0.0f, x_rot = 0.0f;

static float fovy  = 60.0f;
static float znear = 0.1f, zfar = 1000.0f;
static GLint width = 960, height = 960;

static enum { NONE, ROTATING, PANNING, ZOOMING } cur_op = NONE;
static int last_x, last_y;

static void focus_camera(const struct mesh *mesh)
{
	vector min, max;

	/* FIXME */
	mesh_calc_bounds(mesh, min, max);
	vec_add(center, min, max);
	vec_mul(center, 0.5f, center);
	focal_len = 2.0f * max[1];
}

static void get_camera_frame(vector x, vector y, vector z)
{
	matrix m;

	vec_set(x, 1.0f, 0.0f, 0.0f);
	vec_set(y, 0.0f, 1.0f, 0.0f);
	vec_set(z, 0.0f, 0.0f, 1.0f);

	mat_rotate(m, y, y_rot);
	mat_mul_vector(x, m, x);
	mat_mul_vector(z, m, z);

	mat_rotate(m, x, x_rot);
	mat_mul_vector(y, m, y);
	mat_mul_vector(z, m, z);
}

static void get_camera(vector eye, vector at, vector up)
{
	vector x, y, z;

	get_camera_frame(x, y, z);
	vec_copy(at, center);
	vec_copy(eye, center);
	vec_mad(eye, focal_len, z);
	vec_copy(up, y);
}

  void display(void)
{
	matrix m;
	vector eye, at, up;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Render scene */
	glMatrixMode(GL_PROJECTION);
	mat_persp(m, fovy, (double) width / height, znear, zfar);
	glLoadMatrixf(m);
	glMatrixMode(GL_MODELVIEW);
	get_camera(eye, at, up);
	mat_lookat(m, eye, at, up);
	glLoadMatrixf(m);

	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,
		  (GLfloat[4]) { 1.0f, 1.0f, 1.0f, 1.0f });
	glLightfv(GL_LIGHT0, GL_POSITION,
		  (GLfloat[4]) { eye[0], eye[1], eye[2], 1.0f });
	glEnable(GL_LIGHT0);

	 ed_render(ed);
	gl_draw_xyz();

	/* Render overlays */
	gl_begin_2d();
	gl_draw_fps(0.925f, 0.975f);
	ed_render_overlay(ed);
	gl_end_2d();
  
	   for(int ll = 0; ll <  KvvVENTAL; ll++)  
		  { 
taskworm(ll,0);
}
	 
 
 display_mpeg();

 
	/* Swap buffers */
	glutSwapBuffers();
	glutPostRedisplay();
}

static void reshape(int w, int h)
{
	width = w > 1 ? w : 1;
	height = h > 1 ? h : 1;
	glViewport(0, 0, width, height);
	glClearDepth(1.0);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
}

static void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 27: case 17:	/* Esc or Ctrl-Q */
		exit(0);
		break;
	case 'f': case 'F':
		focus_camera(ed_cur_obj(ed));
		break;
	case 'e': case 'E':
		ed_toggle_editing(ed);
		break;
	}

	if (!ed_is_editing(ed)) {
		switch (key) {
		case ' ':
			ed_next_obj(ed);
			focus_camera(ed_cur_obj(ed));
			break;
		case 8:	case 127:	/* Backspace or Delete */
			ed_prev_obj(ed);
			focus_camera(ed_cur_obj(ed));
			break;
		case 'w': case 'W':
			ed_toggle_wireframe(ed);
			break;
		case '=': case '+':
			ed_next_level(ed);
			break;
		case '-': case '_':
			ed_prev_level(ed);
			break;
		}
	}
}

static void special(int key, int x, int y)
{
	if (ed_is_editing(ed))
		return;

	switch (key) {
	case GLUT_KEY_RIGHT:
		ed_next_obj(ed);
		focus_camera(ed_cur_obj(ed));
		break;
	case GLUT_KEY_LEFT:
		ed_prev_obj(ed);
		focus_camera(ed_cur_obj(ed));
		break;
	case GLUT_KEY_UP:
		ed_next_level(ed);
		break;
	case GLUT_KEY_DOWN:
		ed_prev_level(ed);
		break;
	}
}

static void mouse(int button, int state, int x, int y)
{
	int mods;
	static const int cursor[] = {
		GLUT_CURSOR_RIGHT_ARROW,		/* NONE */
		GLUT_CURSOR_CYCLE,			/* ROTATING */
		GLUT_CURSOR_CROSSHAIR,			/* PANNING */
		GLUT_CURSOR_UP_DOWN			/* ZOOMING */
	};

	mods = glutGetModifiers();
	if (state == GLUT_DOWN) {
		if (button == GLUT_LEFT_BUTTON)
			cur_op = (mods & GLUT_ACTIVE_CTRL) ? PANNING : ROTATING;
		else if (button == GLUT_RIGHT_BUTTON)
			cur_op = ZOOMING;
		else if (button == GLUT_MIDDLE_BUTTON)
			cur_op = PANNING;
	} else {
		cur_op = NONE;
	}

	glutSetCursor(cursor[cur_op]);

	last_x = x;
	last_y = y;
}

static void motion(int x, int y)
{
	float dx, dy;

	dx = (float) (x - last_x) / width;
	dy = (float) (y - last_y) / height;

	if (cur_op == ROTATING) {
		y_rot -= dx * 360.0f;
		x_rot -= dy * 360.0f;
	} else if (cur_op == PANNING) {
		float lx, ly;
		vector x, y, z;

		ly = 2.0f * focal_len * tanf(radians(fovy / 2.0f));
		lx = ly * width / height;

		get_camera_frame(x, y, z);
		vec_mad(center, -dx * lx, x);
		vec_mad(center,  dy * ly, y);
	} else if (cur_op == ZOOMING) {
		focal_len *= (1.0f - dy) - dx;
	}

	last_x = x;
	last_y = y;
}

static void idle(void)
{
	glutPostRedisplay();
}

int main (int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(width, height);
	glutCreateWindow("catmull-clark");

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutIdleFunc(idle);

	printf("Loading... "); fflush(stdout);
	ed = ed_create();
	//ed_add_obj(ed, "objs/cube.obj", 1);
	//ed_add_obj(ed, "objs/tetra.obj", 1);
	//ed_add_obj(ed, "objs/bigguy.obj", 8);
	ed_add_obj(ed, "sphere2.obj", 1);
	printf("done.\n");
	//loadOBJ__("sphere.obj");
	focus_camera(ed_cur_obj(ed));

	glutMainLoop();

	return 0;
}
