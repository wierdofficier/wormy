/*
Need variables to find equation for why an volume with a specific size, with a specific number of charges, with a total amount of collisions
come to that moment that it became stable for a while
where the charges that build the volume had a special start position and velocity

ofcourse i want to know if is possible to reproduce the same area with

what i know:

force from a charge1 in motion that came in another charge2 with electric and magnetic field with a specific radius gives the acceleration to charge1:

a = (q*E+q*v*B)/m

 (rand() %100)

 mass_motion_state[whatfluid] = mass_motion( mass_motion_state[whatfluid] ,1,0);
*/

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
int what_n = 0;
int what_k = 0;
#define HOW_MANY_CHARGES 3840 //126976
#define HOW_MANY_SPHERES 1
double q = 1.602e-19;
double m = 9.1e-31;
extern int KvvVENTALA[HOW_MANY_CHARGES];
extern float* V; 
int findnearestcharge(int points,struct state_vector *** e_charge , int num,int howmany,int kk, int what_index );
void calc_e_velocity(void);
void display  (void);
void reshape(int w, int h);
 typedef struct {
	float x, y, z;
} Vec3;

float fovy = 45.0;
float dNear = 100;
float dFar = 2000;
float cameraEye[3] = {0.0, 0.0, 1000.0};
 
float cameraLookAt[4] = {0.0, 0.0, 0.0, 1.0};
float cameraUp[4] = {0.0, 1.0, 0.0};
struct state_vector  *** state_result_e_motion ;

double  neighbourV_[HOW_MANY_CHARGES][HOW_MANY_SPHERES] ;
int main (int argc, char **argv)
{ 
	int i,j,k;  
	loadOBJ__("sphere.obj");  
	state_result_e_motion = (struct state_vector*)malloc(sizeof(struct state_vector*)*HOW_MANY_SPHERES);     	 	 
	state_result_e_motion= (struct state_vector ***) malloc(sizeof(struct state_vector**) * HOW_MANY_SPHERES);  
	for(i = 0; i < HOW_MANY_SPHERES; i++){
	 
	  state_result_e_motion[i] = (struct state_vector **) malloc(sizeof(struct state_vector*) * HOW_MANY_SPHERES);   
	  for(j = 0; j < HOW_MANY_SPHERES; j++){
		state_result_e_motion[i][j] = (struct state_vector *) malloc(sizeof(struct state_vector) * HOW_MANY_CHARGES);
	  }
 
	Vec3 *v ;
 
	int ll;
	for(int kk = 0; kk < HOW_MANY_SPHERES; kk++)
	{
	for(ll = 0; ll < HOW_MANY_CHARGES  ; ll++)
	{
		  v= (Vec3 *)(V + 3*(KvvVENTALA[ll]-1));
         	  state_result_e_motion[ll][kk]->pos_new_x = v->x;
 	          state_result_e_motion[ll][kk]->pos_new_y = v->y;
 		  state_result_e_motion[ll][kk]->pos_new_z = v->z;
 
 	  	  state_result_e_motion[ll][kk]->vel_new_x =  1e-6 ;
 		  state_result_e_motion[ll][kk]->vel_new_y =  1e-6 ;
 		  state_result_e_motion[ll][kk]->vel_new_z =  1e-5 ;
	}
 	}
	for(ll = 0; ll < HOW_MANY_CHARGES     ; ll++)
	{
 		findnearestcharge(HOW_MANY_CHARGES  ,state_result_e_motion,ll,1,0,0 );
	}
	glutInit(&argc, argv);
	init_mpgeg();

	GLint glut_display;

	glutInitWindowSize (1080,1920);
	glutCreateWindow ("gchange");

	int mainMenu;
	const GLubyte *str;
	str = glGetString (GL_EXTENSIONS);

	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
   
    	glutDisplayFunc (display );
 
    	glutReshapeFunc(reshape);
 
    	//glutMouseFunc(mouse);
    	//glutKeyboardFunc(key);
 
    	//glutMotionFunc(mouseMotion);
 
	glutMainLoop();
}


void rates_e_motion ( double *t, double *f, double result[]   )
{
    result[0] =          f[3]/1.0;
    result[1] =          f[4]/1.0;
    result[2] =          f[5]/1.0;
  
    result[3] =   	state_result_e_motion[what_n][what_k][0].force_acc[0];
    result[4] =   	state_result_e_motion[what_n][what_k][0].force_acc[1]; 
    result[5] =         state_result_e_motion[what_n][what_k][0].force_acc[2];  
}

struct state_vector    e_motion(struct state_vector     next_state, int num  )
{
	double f0[6];
	double f0_result[6];

	double z[6];
 	
 	double tburn = 1;
	double tf[6] = {tburn,tburn,tburn,tburn,tburn,tburn};
	tburn = 1;
	double t0[6] ={tburn,tburn,tburn,tburn,tburn,tburn};
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
 
	rk45(rates_e_motion, t0, f0,f0_result, tf,z,6,1);
 
	next_state.pos_new_x = f0[0];
	next_state.pos_new_y = f0[1];
	next_state.pos_new_z = f0[2];

	next_state.vel_new_x = f0[3];
	next_state.vel_new_y = f0[4];
	next_state.vel_new_z = f0[5];

	return   next_state ;
}


void calc_e_velocity(void)
{
			for(int k = 0; k < HOW_MANY_SPHERES; k++)
			{
			for(int n = 0; n < HOW_MANY_CHARGES; n++)
			{	
			what_n = n;
			what_k = k;		
			state_result_e_motion[n][k][0]    =  e_motion(   state_result_e_motion[n][k][0] ,1);

 			findnearestcharge(HOW_MANY_CHARGES  ,state_result_e_motion,n,1,0,0 );
			double sqrtthis = pow( state_result_e_motion[n][k]->pos_new_x - state_result_e_motion[n][k][0].near_buddy_posx ,2.0) +  pow( state_result_e_motion[n][k]->pos_new_y -state_result_e_motion[n][k][0].near_buddy_posy,2.0)  + pow( state_result_e_motion[n][k]->pos_new_z - state_result_e_motion[n][k][0].near_buddy_posz,2.0 );
 
			double xxxx=  (sqrtthis ); 
  			complex zzzz;
 			//distance between two charges
 			 if((sqrtthis)  < 0)
	  		{
 				zzzz = csqrt(sqrtthis);
		  		xxxx = creal(zzzz);
			}
			else if ((sqrtthis)  > 0)	  
				xxxx = sqrtf(sqrtthis ); 
			
		if(( fabs(xxxx)) < 0.1 || fabs(xxxx) > (0.05))
 		{
			double E = state_result_e_motion[n][k][0].E;
			double B = state_result_e_motion[n][k][0].B;		

			state_result_e_motion[n][k][0].force_acc[0] = (q*E+q*state_result_e_motion[n][k]->vel_new_x*B)/m;
			state_result_e_motion[n][k][0].force_acc[1] = (q*E+q*state_result_e_motion[n][k]->vel_new_y*B)/m;
			state_result_e_motion[n][k][0].force_acc[2] = (q*E+q*state_result_e_motion[n][k]->vel_new_z*B)/m;			
 		}

     glPushMatrix();
 
     glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
     glTranslatef(state_result_e_motion[n][k]->pos_new_x,  state_result_e_motion[n][k]->pos_new_y   ,state_result_e_motion[n][k]->pos_new_z );
 
     glutSolidSphere(6,15,15  );  
     glPopMatrix();

    glutPostRedisplay();
}
 }
}
//give random charge a index so we know what state vector it has.
int find_index_for_charge(int NUM, struct state_vector **real[]  ,int num,int k, int howm  )
{
int ll = 0;
 	while(ll < NUM)
	{
		if(state_result_e_motion[num][k]->pos_new_x ==real[ll][howm]->pos_new_x && state_result_e_motion[num][k]->pos_new_y ==real[ll][howm]->pos_new_y && 		state_result_e_motion[num][k]->pos_new_z ==real[ll][howm]->pos_new_z)
		{
			//printf("found index_____ @ %d \n", ll);
			return ll;
		}
	ll++;
	}
}

void *ptree;
int ptreeonce = 1;
//find nearest charge from the charge in question

int findnearestcharge(int points,struct state_vector *** e_charge , int num,int howmany,int kk, int what_index ) {
int i, num_pts = points;
int INDEX;
char *data, *pch;
struct kdres *presults;
double pos[3], dist;
double pt[3] = { e_charge[num][kk]->pos_new_x,e_charge[num][kk]->pos_new_y, e_charge[num][kk]->pos_new_z };
double radius = 0.0004/1.0;

num_pts =points;

srand( time(0) );

if(ptreeonce < 2  )
{
	ptree = kd_create( 3 );
  	for( i=0; i<points; i++ ) {   
    		assert( 0 == kd_insert3( ptree, e_charge[i][kk]->pos_new_x, e_charge[i][kk]->pos_new_y, e_charge[i][kk]->pos_new_z, NULL ) );
 	 }
}
ptreeonce++;
 
presults = kd_nearest_range( ptree, pt, radius );
 
e_charge[num][kk][0].totaln = kd_res_size(presults) ;

 
int feather_count = 0;
 
while( !kd_res_end( presults ) ) {
   
	pch = (char*)kd_res_item( presults, pos ); 
    	dist = sqrt( dist_sq( pt, pos, 3 ) );
 
	int k;
    	 e_charge[num][feather_count][0].near_buddy_posx = pos[0];
  	 e_charge[num][feather_count][0].near_buddy_posy = pos[1];
   	 e_charge[num][feather_count][0].near_buddy_posz = pos[2];
   if(what_index == 1)
{
    INDEX = find_index_for_charge(HOW_MANY_CHARGES,e_charge,num,feather_count,kk);
    e_charge[num][feather_count][0].INDEX_NR = INDEX;
    // printf("INDEX = %d \n", INDEX);
}

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
}
void reshape(int w, int h){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fovy, (GLdouble)w/(GLdouble)h, dNear, dFar);
    glViewport(0, 0, w, h);
}  


void display  (void){
 
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   //glClearColor (1, 1, 1, 0);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_COLOR_MATERIAL);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
 
    glLoadIdentity();

    gluLookAt(cameraEye[0]+0, cameraEye[1]+253, cameraEye[2]-423-300 , cameraLookAt[0], cameraLookAt[1], cameraLookAt[2], cameraUp[0], cameraUp[1], cameraUp[2]);

     calc_e_velocity();

glutSwapBuffers();
 display_mpeg();

}
