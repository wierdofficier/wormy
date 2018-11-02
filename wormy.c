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
#define SIZE_OBJECT 7936 //126976
float springlength =  0.004;
#define AIR_FRICTION 0.99999
#define FIRST 1
#define WORK 0
float fovy = 45.0;
float dNear = 100;
float dFar = 2000;
double F_total[3][SIZE_OBJECT*104] ;
int totalneigbours[SIZE_OBJECT*104];
float     vertexpoint_g_ventral[SIZE_OBJECT][3];
struct state_vector * springVector;
struct state_vector ** state_result_worm_ventral_feather;
struct state_vector  ** state_result_worm_ventral ;
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
		springConstant =   .001 *1;
//	printf("springConstant = %.10f \n", springConstant);
		 frictionConstant = 0.002;
	}
	}
acc1once = 0;	
	// printf("1totalneigbours[llll]  %d :: number %d :: index_____ = %d\n",totalneigbours[llll],llll,INDEX_NR);
	INDEX_NR =  (state_result_worm_ventral_feather[llll][k].INDEX_NR);
	
if(INDEX_NR != llll && INDEX_NR > 0 && INDEX_NR < 1000000 && INDEX_NR != SIZE_OBJECT)
{
	//if(state_result_worm_ventral[INDEX_NR]->force_sign != -1 && state_result_worm_ventral[INDEX_NR]->force_sign != 1 && INDEX_NR >0)
	//{
		 if(feather_once == FIRST && INDEX_NR > 0)
		{	

						 if(fabs(F_total[0][INDEX_NR]) > 10 )
	 				 	  F_total[0][INDEX_NR] =0;
 
		 		 if(fabs(F_total[1][INDEX_NR])  > 10)
						F_total[1][INDEX_NR] =0;

			 	 if(fabs(F_total[2][INDEX_NR])  > 10)
						F_total[2][INDEX_NR] =0;
  //printf("2totalneigbours[llll]  %d :: number %d :: index_____ = %d\n",totalneigbours[llll],llll,INDEX_NR);
		springVector->pos_new_x = state_result_worm_ventral[INDEX_NR]->pos_new_x - state_result_worm_ventral[llll]->pos_new_x;
		springVector->pos_new_y = state_result_worm_ventral[INDEX_NR]->pos_new_y - state_result_worm_ventral[llll]->pos_new_y;
		springVector->pos_new_z = state_result_worm_ventral[INDEX_NR]->pos_new_z - state_result_worm_ventral[llll]->pos_new_z;
 

		float r = length(state_result_worm_ventral[INDEX_NR],state_result_worm_ventral[llll]);

		if ( r != 0  &&   r < 1  )
		{			
			F_total[0][INDEX_NR] +=( springVector->pos_new_x  /  r ) * ( r - springlength ) * ( -springConstant );
			F_total[1][INDEX_NR] += ( springVector->pos_new_y /  r ) * ( r - springlength ) * ( -springConstant );
			F_total[2][INDEX_NR] +=( springVector->pos_new_z  /  r ) * ( r - springlength ) * ( -springConstant );

			F_total[0][llll] +=( springVector->pos_new_x  / r ) * ( r - springlength ) * ( -springConstant );
			F_total[1][llll] += ( springVector->pos_new_y  / r ) * ( r - springlength) * ( -springConstant );
			F_total[2][llll] +=( springVector->pos_new_z  / r ) * ( r - springlength ) * ( -springConstant );
		}

		F_total[0][INDEX_NR] +=-(  state_result_worm_ventral[INDEX_NR]->vel_new_x - state_result_worm_ventral[llll]->vel_new_x ) * frictionConstant;
 		F_total[1][INDEX_NR] += -( state_result_worm_ventral[INDEX_NR]->vel_new_y- state_result_worm_ventral[llll]->vel_new_y ) * frictionConstant;
 		F_total[2][INDEX_NR] += -(  state_result_worm_ventral[INDEX_NR]->vel_new_z - state_result_worm_ventral[llll ]->vel_new_z ) * frictionConstant;


		F_total[0][llll] +=-(  state_result_worm_ventral[INDEX_NR]->vel_new_x - state_result_worm_ventral[llll]->vel_new_x ) * frictionConstant;
 		F_total[1][llll] += -( state_result_worm_ventral[INDEX_NR]->vel_new_y- state_result_worm_ventral[llll]->vel_new_y ) * frictionConstant;
 		F_total[2][llll] += -(  state_result_worm_ventral[INDEX_NR]->vel_new_z - state_result_worm_ventral[llll ]->vel_new_z ) * frictionConstant;

		state_result_worm_ventral[INDEX_NR]->force_sign =  -1;
		state_result_worm_ventral[INDEX_NR]    =  worm_ventral(  state_result_worm_ventral[INDEX_NR] ,1) ;

		state_result_worm_ventral[llll]->force_sign =   1; ; 
		state_result_worm_ventral[llll]    =  worm_ventral(  state_result_worm_ventral[llll] ,1);
		

 
	for(int d = 1; d <= totalneigbours[llll]; d++)
	{	 
		
		if(d != k  )
		{
		  INDEX_NRmore =  (state_result_worm_ventral_feather[llll][d].INDEX_NR);
		
		if(INDEX_NRmore != llll && INDEX_NRmore != INDEX_NR)
		{

					if(fabs(F_total[0][INDEX_NRmore]) > 10 )
	 				 	  F_total[0][INDEX_NRmore] =0;
 
		 		 if(fabs(F_total[1][INDEX_NRmore])  > 10)
						F_total[1][INDEX_NRmore] =0;

			 	 if(fabs(F_total[2][INDEX_NRmore])  > 10)
						F_total[2][INDEX_NRmore] =0;
			
		springVector->pos_new_x = state_result_worm_ventral[INDEX_NR]->pos_new_x - state_result_worm_ventral[INDEX_NRmore]->pos_new_x;
		springVector->pos_new_y = state_result_worm_ventral[INDEX_NR]->pos_new_y - state_result_worm_ventral[INDEX_NRmore]->pos_new_y;
		springVector->pos_new_z = state_result_worm_ventral[INDEX_NR]->pos_new_z - state_result_worm_ventral[INDEX_NRmore]->pos_new_z;
 			

		float r = length(state_result_worm_ventral[INDEX_NR],state_result_worm_ventral[INDEX_NRmore]);

		if ( r != 0  &&   r < 1  )
		{			
			F_total[0][INDEX_NR] +=( springVector->pos_new_x  /  r ) * ( r - springlength ) * ( -springConstant );
			F_total[1][INDEX_NR] += ( springVector->pos_new_y /  r ) * ( r - springlength ) * ( -springConstant );
			F_total[2][INDEX_NR] +=( springVector->pos_new_z  /  r ) * ( r - springlength ) * ( -springConstant );

			F_total[0][INDEX_NRmore] +=( springVector->pos_new_x  / r ) * ( r - springlength ) * ( -springConstant );
			F_total[1][INDEX_NRmore] += ( springVector->pos_new_y  / r ) * ( r - springlength) * ( -springConstant );
			F_total[2][INDEX_NRmore] +=( springVector->pos_new_z  / r ) * ( r - springlength ) * ( -springConstant );
		}

		F_total[0][INDEX_NR] +=-(  state_result_worm_ventral[INDEX_NR]->vel_new_x - state_result_worm_ventral[INDEX_NRmore]->vel_new_x ) * frictionConstant;
 		F_total[1][INDEX_NR] += -( state_result_worm_ventral[INDEX_NR]->vel_new_y- state_result_worm_ventral[INDEX_NRmore]->vel_new_y ) * frictionConstant;
 		F_total[2][INDEX_NR] += -(  state_result_worm_ventral[INDEX_NR]->vel_new_z - state_result_worm_ventral[INDEX_NRmore ]->vel_new_z ) * frictionConstant;


		F_total[0][INDEX_NRmore] +=-(  state_result_worm_ventral[INDEX_NR]->vel_new_x - state_result_worm_ventral[INDEX_NRmore]->vel_new_x ) * frictionConstant;
 		F_total[1][INDEX_NRmore] += -( state_result_worm_ventral[INDEX_NR]->vel_new_y- state_result_worm_ventral[INDEX_NRmore]->vel_new_y ) * frictionConstant;
 		F_total[2][INDEX_NRmore] += -(  state_result_worm_ventral[INDEX_NR]->vel_new_z - state_result_worm_ventral[INDEX_NRmore ]->vel_new_z ) * frictionConstant;

		state_result_worm_ventral[INDEX_NR]->force_sign =  -1;
		state_result_worm_ventral[INDEX_NR]    =  worm_ventral(  state_result_worm_ventral[INDEX_NR] ,1) ;

		state_result_worm_ventral[INDEX_NRmore]->force_sign =   1; ; 
		state_result_worm_ventral[INDEX_NRmore]    =  worm_ventral(  state_result_worm_ventral[INDEX_NRmore] ,1);
	//	 printf("f[0] = %.10f:%.10f%.10f \n", state_result_worm_ventral[INDEX_NRmore]->pos_new_x,state_result_worm_ventral[INDEX_NRmore]->pos_new_y,state_result_worm_ventral[INDEX_NRmore]->pos_new_z);
	}
	}
 	}
}
	
	//}
	 
		 
 }		



 	


 				if(fabs(F_total[0][llll]) > 10 )
	 				 	  F_total[0][llll] =0;
 
		 		 if(fabs(F_total[1][llll])  > 10)
						F_total[1][llll] =0;

			 	 if(fabs(F_total[2][llll])  > 10)
						F_total[2][llll] =0;
	 printf("FORCE(x,y,z) = %.50f:%.50f%.50f \n", fabs(F_total[0][INDEX_NR]),fabs(F_total[1][llll]),fabs(F_total[2][INDEX_NRmore]));
			
			if(llll >= SIZE_OBJECT -1  )
			{
				//feather_once= WORK;
	   			llll =0;
				int var;
				for(var = 0; var <  3 -1 ; var++)
				{
				//	state_result_worm_ventral[var]->force_sign = 0;
 	
			 	
	
 
				}
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

void *ptree;
  
int ptreeonce = 1;
int findnearestpoint(int points,struct state_vector ** worm , int num,int howmany) {
int i, num_pts = points;
int INDEX;
char *data, *pch;
struct kdres *presults;
double pos[3], dist;
double pt[3] = { worm[num]->pos_new_x,worm[num]->pos_new_y, worm[num]->pos_new_z };
double radius = 0.00004;

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
state_result_worm_ventral_feather = (struct state_vector*)malloc(sizeof(struct state_vector*)*SIZE_OBJECT*4);

state_result_worm_ventral =(struct state_vector*)malloc(sizeof(struct state_vector*)*SIZE_OBJECT*32);


 int i,j,k;

for(int i=0; i<SIZE_OBJECT*1; i++)
	state_result_worm_ventral[i] = (struct state_vector*)malloc(sizeof(struct state_vector)*32);
       
printf("done ! \n");
 
for(int i=0; i<SIZE_OBJECT*1; i++)
	state_result_worm_ventral_feather[i] = (struct state_vector*)malloc(sizeof(struct state_vector)*32);
  
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
 
 	  	  state_result_worm_ventral[ll]->vel_new_x =   1e-4 ;
 		  state_result_worm_ventral[ll]->vel_new_y =  1e-4 ;
 		  state_result_worm_ventral[ll]->vel_new_z =  1e-4  ;
	}

 
	for(ll = 0; ll < SIZE_OBJECT     ; ll++)
	{
 		findnearestpoint(SIZE_OBJECT  ,state_result_worm_ventral,ll,1 );
	}
 
	initonce = 0;
}


glutInit(&argc, argv);
init_mpgeg();

GLint glut_display;

glutInitWindowSize (1000,1000);
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
dirLight->setSpecular_light(0.7f, 0.1f, 0.88f);
 
material =(Material*)malloc(sizeof(Material));
material->setAmbient_material =setAmbient_material;
material->setDifusse_material = setDifusse_material;
material->setSpecular_material =setSpecular_material;
material->setShininess_material = setShininess_material;
material->setAmbient_material(0, 0, 1, 1);
material->setDifusse_material(0.5f, 0.1f, 0.9f, 1.0f/200.0f);
material->setSpecular_material(0.01f, 0.01f, 0.01f, 1.0f/200.0f);
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

   for(int ll = 0; ll <   SIZE_OBJECT ; ll++)  
		  {
taskworm();
glPushMatrix();
glScalef(150,150,150);
  glTranslatef(state_result_worm_ventral[ll]->pos_new_x, state_result_worm_ventral[ll]->pos_new_y,state_result_worm_ventral[ll]->pos_new_z);
glBegin(GL_TRIANGLES);
     glColor3f(state_result_worm_ventral[ll]->pos_new_x, state_result_worm_ventral[ll]->pos_new_y, state_result_worm_ventral[ll]->pos_new_z);     // Red
      glVertex3f(0, 1.0f/300.0f, 0.0f);
      glColor3f(state_result_worm_ventral[ll]->pos_new_x, state_result_worm_ventral[ll]->pos_new_y,state_result_worm_ventral[ll]->pos_new_z);     // Green
      glVertex3f(-1.0f/300.0f, -1.0f/300.0f, 1.0f/300.0f);
      glColor3f(state_result_worm_ventral[ll]->pos_new_x, state_result_worm_ventral[ll]->pos_new_y, state_result_worm_ventral[ll]->pos_new_z);     // Blue
      glVertex3f(1.0f/300.0f, -1.0f/300.0f, 1.0f/300.0f);
 
      // Right
      glColor3f(state_result_worm_ventral[ll]->pos_new_x, state_result_worm_ventral[ll]->pos_new_y, state_result_worm_ventral[ll]->pos_new_z);     // Red
      glVertex3f(0.0f, 1.0f/300.0f, 0.0f);
      glColor3f(state_result_worm_ventral[ll]->pos_new_x, state_result_worm_ventral[ll]->pos_new_y, state_result_worm_ventral[ll]->pos_new_z);     // Blue
      glVertex3f(1.0f/300.0f, -1.0f/300.0f, 1.0f/300.0f);
      glColor3f(state_result_worm_ventral[ll]->pos_new_x,state_result_worm_ventral[ll]->pos_new_y, state_result_worm_ventral[ll]->pos_new_z);     // Green
      glVertex3f(1.0f/300.0f, -1.0f/300.0f, -1.0f/300.0f);
 
      // Back
      glColor3f(state_result_worm_ventral[ll]->pos_new_x,state_result_worm_ventral[ll]->pos_new_y, state_result_worm_ventral[ll]->pos_new_z);     // Red
      glVertex3f(0.0f, 1.0f/300.0f, 0.0f);
      glColor3f(state_result_worm_ventral[ll]->pos_new_x, state_result_worm_ventral[ll]->pos_new_y, state_result_worm_ventral[ll]->pos_new_z);     // Green
      glVertex3f(1.0f/300.0f, -1.0f/300.0f, -1.0f/300.0f);
      glColor3f(state_result_worm_ventral[ll]->pos_new_x, state_result_worm_ventral[ll]->pos_new_y, state_result_worm_ventral[ll]->pos_new_z);     // Blue
      glVertex3f(-1.0f/300.0f, -1.0f/300.0f, -1.0f/300.0f);
 
      // Left
      glColor3f(state_result_worm_ventral[ll]->pos_new_x,state_result_worm_ventral[ll]->pos_new_y,state_result_worm_ventral[ll]->pos_new_z);       // Red
      glVertex3f( 0.0f, 1.0f/300.0f, 0.0f);
      glColor3f(state_result_worm_ventral[ll]->pos_new_x,state_result_worm_ventral[ll]->pos_new_y,state_result_worm_ventral[ll]->pos_new_z);       // Blue
      glVertex3f(-1.0f/300.0f,-1.0f/300.0f,-1.0f/300.0f);
      glColor3f(state_result_worm_ventral[ll]->pos_new_x,state_result_worm_ventral[ll]->pos_new_y,state_result_worm_ventral[ll]->pos_new_z);       // Green
      glVertex3f(-1.0f/300.0f,-1.0f/300.0f, 1.0f/300.0f);
	glEnd(); 	
 	
 
 glPopMatrix();
 glutPostRedisplay();
}

display_mpeg();

glutSwapBuffers();

}




void rates_dorsal ( double *t, double *f, double result[]   )
{
// printf("f[0] = %.10f:%.10f%.10f \n", f[0],f[1],f[2]);
if(state_result_worm_ventral[llll]->force_sign == 1)
{
    	result[0] =             f[3]/100.0;
    	result[1] =             f[4]/100.0;
    	result[2] =             f[5]/100.0;
  
	result[3] = (F_total[0][llll])/(1.0   );
	result[4] = (F_total[1][llll]   )/(1.0   );
	result[5] = (F_total[2][llll] )/(1.0); 

 
}
else if(state_result_worm_ventral[INDEX_NR]->force_sign == -1)
{
    result[0] =             f[3]/100.0;
    result[1] =             f[4]/100.0;
    result[2] =             f[5]/100.0;
  
    result[3] = -(F_total[0][INDEX_NR])/(1.0  );
    result[4] = -(F_total[1][INDEX_NR]  ) /(1.0) ;
    result[5] = -(F_total[2][INDEX_NR] )/(1.0  ); //522
 

}

  acc1 = sqrtf(pow(result[3],2.0) + pow(result[4],2.0)+pow(result[5],2.0));
//printf("acc1 = %.10f \n", acc1);
}
