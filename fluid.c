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
#include <stdbool.h>
#include <math.h>
#include <complex.h>
#include <math.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>
#include <jerror.h>
#include "noise.h"
#define SIZE_OBJECT      9920
#define NUMBER_FLUIDS    6

int alloc_once = 1;
double water_radius =0.2;
double density =   1000.0;
double viscous_constant = 1.6735;
int whatfluid = 0;
float elementradius[100000];
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
Light *dirLight;
Material *material;
int *INDEX_WALL;
double acc1;
int pipe;
double **neighbourV ;
double  neighbourV_[10000][10000] ;
int KvvVENTALA[400000 ];
#define FIRST 1
#define WORK 0
float fovy = 45.0;
float dNear = 100;
float dFar = 2000;
int i; 
double *vector31;
double  *vector32;
  struct state_vector  ** near_vector ;
  struct state_vector  ** fluid_info;
  struct state_vector  ** wall_info ; 
  struct state_vector  ** mass_motion_state ;
  struct state_vector  ** state_result_worm_ventral ;
double acc2_fluid[3][10050];
double acceleration_bounce[3][10050];
double density;


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

 
void key(unsigned char key, int x, int y);
 
void display  (void);
struct state_vector   *   mass_motion(struct state_vector     *  next_state, int num , int what);
 
 
double accen[1000];
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
 
static void reverse(void* x,const int n)
{
   int k;
   char* ch = (char*)x;
   for (k=0;k<n/2;k++)
   {
      char tmp = ch[k];
      ch[k] = ch[n-1-k];
      ch[n-1-k] = tmp;
   }
}

 
unsigned int loadTexBMP(char* file)
{
  unsigned int   texture;    /* Texture name */
  FILE*          f;          /* File pointer */
  unsigned short magic;      /* Image magic */
  unsigned int   dx,dy,size; /* Image dimensions */
  unsigned short nbp,bpp;    /* Planes and bits per pixel */
  unsigned char* image;      /* Image data */
  unsigned int   k;          /* Counter */

  /*  Open file */
  f = fopen(file,"rb");
  if (!f) printf("Cannot open file %s\n",file);
  /*  Check image magic */
  if (fread(&magic,2,1,f)!=1) printf("Cannot read magic from %s\n",file);
  if (magic!=0x4D42 && magic!=0x424D) printf("Image magic not BMP in %s\n",file);
  /*  Seek to and read header */
  if (fseek(f,16,SEEK_CUR) || fread(&dx ,4,1,f)!=1 || fread(&dy ,4,1,f)!=1 ||
      fread(&nbp,2,1,f)!=1 || fread(&bpp,2,1,f)!=1 || fread(&k,4,1,f)!=1)
    printf("Cannot read header from %s\n",file);
  /*  Reverse bytes on big endian hardware (detected by backwards magic) */
  if (magic==0x424D) {
    reverse(&dx,4);
    reverse(&dy,4);
    reverse(&nbp,2);
    reverse(&bpp,2);
    reverse(&k,4);
  }
  /*  Check image parameters */
  if (dx<1 || dx>65536) printf("%s image width out of range: %d\n",file,dx);
  if (dy<1 || dy>65536) printf("%s image height out of range: %d\n",file,dy);
  if (nbp!=1)  printf("%s bit planes is not 1: %d\n",file,nbp);
  if (bpp!=24) printf("%s bits per pixel is not 24: %d\n",file,bpp);
  if (k!=0)    printf("%s compressed files not supported\n",file);
#ifndef GL_VERSION_2_0
  /*  OpenGL 2.0 lifts the restriction that texture size must be a power of two */
  for (k=1;k<dx;k*=2);
  if (k!=dx) printf("%s image width not a power of two: %d\n",file,dx);
  for (k=1;k<dy;k*=2);
  if (k!=dy) printf("%s image height not a power of two: %d\n",file,dy);
#endif

  /*  Allocate image memory */
  size = 3*dx*dy;
  image = (unsigned char*) malloc(size);
  if (!image) printf("Cannot allocate %d bytes of memory for image %s\n",size,file);
  /*  Seek to and read image */
  if (fseek(f,20,SEEK_CUR) || fread(image,size,1,f)!=1) 
    printf("Error reading data from image %s\n",file);
  fclose(f);
  /*  Reverse colors (BGR -> RGB) */
  for (k=0;k<size;k+=3) {
    unsigned char temp = image[k];
    image[k]   = image[k+2];
    image[k+2] = temp;
  }

  /*  Sanity check */
  //errCheck("LoadTexBMP");
  /*  Generate 2D texture */
  glGenTextures(1,&texture);
  glBindTexture(GL_TEXTURE_2D,texture);
  /*  Copy image */
  glTexImage2D(GL_TEXTURE_2D,0,3,dx,dy,0,GL_RGB,GL_UNSIGNED_BYTE,image);
  if (glGetError()) printf("Error in glTexImage2D %s %dx%d\n",file,dx,dy);
  /*  Scale linearly when image size doesn't match */
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

  /*  Free image memory */
  free(image);
  /*  Return texture name */
  return texture;
}
 
typedef struct
{
  char* name;                 /*  Material name */
  float Ka[4],Kd[4],Ks[4],Ns; /*  Colors and shininess */
  float d;                    /*  Transparency */
  int map;                    /*  Texture */
} mtl_t;

/*  Material count and array */
static int Nmtl=0;
static mtl_t* mtl=NULL;

/*
 *  Return true if CR or LF
 */
static int CRLF(char ch)
{
  return ch == '\r' || ch == '\n';
}
 
static int linelen=0;    /*  Length of line */
static char* line=NULL;  /*  Internal storage for line */
  char* readline(FILE* f)
{
  char ch;  /*  Character read */
  int k=0;  /*  Character count */
  while ((ch = fgetc(f)) != EOF) {
    /*  Allocate more memory for long strings */
    if (k>=linelen) {
      linelen += 8192;
      line = realloc(line,linelen);
      if (!line) printf("Out of memory in readline\n");
    }
    /*  End of Line */
    if (CRLF(ch)) {
      /* Eat extra CR or LF characters (if any) */
      while ((ch = fgetc(f)) != EOF)
	if (!CRLF(ch)) break;
      /*  Stick back the overrun */
      if (ch != EOF) ungetc(ch,f);
      /*  Bail */
      break;
    }
    /*  Pad character to line */
    else
      line[k++] = ch;
  }
  /*  Terminate line if anything was read */
  if (k>0) line[k] = 0;
  /*  Return pointer to line or NULL on EOF */
  return k>0 ? line : NULL;
}

/*
 *  Read to next non-whitespace word
 *  Note that this destroys line in the process
 */
static char* getword(char**line)
{
  char* word;
  /*  Skip leading whitespace */
  while (**line && isspace(**line))
    (*line)++;
  if (!**line) return NULL;
  /*  Start of word */
  word = *line;
  /*  Read until next whitespace */
  while (**line && !isspace(**line))
    (*line)++;
  /*  Mark end of word if not NULL */
  if (**line) {
    **line = 0;
    (*line)++;
  }
  return word;
}
 

static void readfloat(char* line,int n,float x[])
{
  int i;
  for (i=0;i<n;i++) {

    char* str = getword(&line);
    if (!str)  { printf("Premature EOL reading %d floats\n" ); } 
    if (sscanf(str,"%f",x+i)!=1) printf("Error reading float %d\n",i);
  }
}
 
 
 
static void readcoord(char* line,int n,float* x[],int* N,int* M)
{
  /*  Allocate memory if necessary */
  if (*N+n > *M) {
    *M += 8192;
    *x = (float*)realloc(*x,(*M)*sizeof(float));
    if (!*x) printf("Cannot allocate memory\n");
  }
  /*  Read n coordinates */
  readfloat(line,n,(*x)+*N);
  (*N)+=n;
}

 
 
static char* readstr(char* line,char* skip)
{
  /*  Check for a match on the skip string */
  while (*skip && *line && *skip==*line) {
    skip++;
    line++;
  }
  /*  Skip must be NULL for a match */
  if (*skip || !isspace(*line)) return NULL;
  /*  Read string */
  return getword(&line);
}

/*
 *  Load materials from file
 */
static void loadMaterial(char* file)
{
  int k=-1;
  char* line;
  char* str;

  /*  Open file or return with warning on error */
  FILE* f = fopen(file,"r");
  if (!f) {
    fprintf(stderr,"Cannot open material file %s\n",file);
    return;
  }

  /*  Read lines */
  while ((line = readline(f))) {
    /*  New material */
    if ((str = readstr(line,"newmtl"))) {
      int l = strlen(str);
      /*  Allocate memory for structure */
      k = Nmtl++;
      mtl = (mtl_t*)realloc(mtl,Nmtl*sizeof(mtl_t));
      /*  Store name */
      mtl[k].name = malloc(l+1);
      if (!mtl[k].name) printf("Cannot allocate %d for name\n",l+1);
      strcpy(mtl[k].name,str);
      /*  Initialize materials */
      mtl[k].Ka[0] = mtl[k].Ka[1] = mtl[k].Ka[2] = 0;   mtl[k].Ka[3] = 1;
      mtl[k].Kd[0] = mtl[k].Kd[1] = mtl[k].Kd[2] = 0;   mtl[k].Kd[3] = 1;
      mtl[k].Ks[0] = mtl[k].Ks[1] = mtl[k].Ks[2] = 0;   mtl[k].Ks[3] = 1;
      mtl[k].Ns  = 0;
      mtl[k].d   = 0;
      mtl[k].map = 0;
    }
    /*  If no material short circuit here */
    else if (k<0)
      {}
    /*  Ambient color */
    else if (line[0]=='K' && line[1]=='a')
      readfloat(line+2,3,mtl[k].Ka);
    /*  Diffuse color */
    else if (line[0]=='K' && line[1] == 'd')
      readfloat(line+2,3,mtl[k].Kd);
    /*  Specular color */
    else if (line[0]=='K' && line[1] == 's')
      readfloat(line+2,3,mtl[k].Ks);
    /*  Material Shininess */
    else if (line[0]=='N' && line[1]=='s')
      readfloat(line+2,1,&mtl[k].Ns);
    /*  Textures (must be BMP - will fail if not) */
    else if ((str = readstr(line,"map_Kd")))
      mtl[k].map = loadTexBMP(str);
    /*  Ignore line if we get here */
  }
  fclose(f);
}

/*
 *  Set material
 */
static void setMaterial(char* name)
{
  int k;
  /*  Search materials for a matching name */
  for (k=0;k<Nmtl;k++)
    if (!strcmp(mtl[k].name,name)) {
      /*  Set material colors */
      glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT  ,mtl[k].Ka);
      glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE  ,mtl[k].Kd);
      glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR ,mtl[k].Ks);
      glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,&mtl[k].Ns);
      /*  Bind texture if specified */
      if (mtl[k].map) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,mtl[k].map);
      }
      else
	glDisable(GL_TEXTURE_2D);
      return;
    }
  /*  No matches */
  fprintf(stderr,"Unknown material %s\n",name);
}
 

 
 

int number = 0;
int number_ventral =0;
int number_dorsal = 0;
int ventral = 1;

 
float* V;   

int KvvVENTAL ;
 
int counting=0;
 
 
int loadOBJ__(char* file)
{
  int k;
  int  Nv,Nn,Nt;  /*  Number of vertex, normal and textures */
  int  Mv,Mn,Mt;  /*  Maximum vertex, normal and textures */
        /*  Array of vertexes */
  float* N;       /*  Array of normals */
  float* T;       /*  Array if textures coordinates */
  char*  line;    /*  Line pointer */
  char*  str;     /*  String pointer */

  /*  Open file */
  FILE* f = fopen(file,"r");
  if (!f) printf("Cannot open file %s\n",file);

  /* Reset materials */
  mtl = NULL;
  Nmtl = 0;

  /*  Start new displaylist */
  int list = glGenLists(1);
  glNewList(list,GL_COMPILE);
  /*  Push attributes for textures */
//  glPushAttrib(GL_TEXTURE_BIT);

  /*  Read vertexes and facets */
  V  = N  = T  = NULL;
  Nv = Nn = Nt = 0;
  Mv = Mn = Mt = 0;
  while ((line = readline(f))) {
	//printf("%s\n", line);
    /*  Vertex coordinates (always 3) */
    if (line[0]=='v' && line[1]==' ')
{
      readcoord(line+2,3,&V,&Nv,&Mv);
     // printf("%f::%f::%f :: ventral____ = %d\n", V[0],V[1],V[2],ventral____);
}
    /*  Normal coordinates (always 3) */
    else if (line[0]=='v' && line[1] == 'n')
      readcoord(line+2,3,&N,&Nn,&Mn);
    /*  Texture coordinates (always 2) */
    else if (line[0]=='v' && line[1] == 't')
      readcoord(line+2,2,&T,&Nt,&Mt);
    /*  Read and draw facets */
    else if (line[0]=='f') {
       line++;
      /*  Read Vertex/Texture/Normal triplets */
      glBegin(GL_POLYGON);
      while ((str = getword(&line))) {
	int Kv,Kt,Kn;
	/*  Try Vertex/Texture/Normal triplet */
	if (sscanf(str,"%d/%d/%d",&Kv,&Kt,&Kn)==3) {
	  if (Kv<0 || Kv>Nv/3) printf("Vertex %d out of range 1-%d\n",Kv,Nv/3);
	  if (Kn<0 || Kn>Nn/3) printf("Normal %d out of range 1-%d\n",Kn,Nn/3);
	  if (Kt<0 || Kt>Nt/2) printf("Texture %d out of range 1-%d\n",Kt,Nt/2);
	}
	/*  Try Vertex//Normal pairs */
	else if (sscanf(str,"%d//%d",&Kv,&Kn)==2) {
	  if (Kv<0 || Kv>Nv/3) continue;
	  if (Kn<0 || Kn>Nn/3) printf("Normal %d out of range 1-%d\n",Kn,Nn/3);
	  Kt = 0;
	}
	/*  Try Vertex index */
	else if (sscanf(str,"%d",&Kv)==1) {
	  if (Kv<0 || Kv>Nv/3) printf("Vertex %d out of range 1-%d\n",Kv,Nv/3);
	  Kn = 0;
	  Kt = 0;
	}
	/*  This is an error */
	else
	  printf("Invalid facet %s\n",str);
	/*  Draw vectors */
	if (Kt) glTexCoord2fv(T+2*(Kt-1));
	if (Kn) glNormal3fv(N+3*(Kn-1));
	if (Kv)
	{
		   glVertex3fv(V+3*(Kv-1));
		   KvvVENTALA[KvvVENTAL] = Kv;
		   KvvVENTAL++;
		   printf("KvvVENTAL = %d \n", KvvVENTAL);
		 
	 
		}
      }
      glEnd();
    }
    /*  Use material */
  // else if ((str = readstr(line,"usemtl")))
  //   setMaterial(str);
  /*  Load materials */
  //  else if ((str = readstr(line,"mtllib")))
  //   loadMaterial(str);
  /*  Skip this line */
  }
  fclose(f);
  /*  Pop attributes (textures) */
  glPopAttrib();
  glEndList();
  
  /*  Free materials */
  for (k=0;k<Nmtl;k++)
    free(mtl[k].name);
  free(mtl);
  
  /*  Free arrays */
 // KvvVENTAL = 0;
  free(T);
  free(N);
 
 
  return list;
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

 


int argc_;
char **argv_;
int initonce = 1;

int main (int argc, char **argv)
{ 
	INDEX_WALL = malloc(1000000);
	vector31 = malloc(100);
	vector32  = malloc(100);


neighbourV = (struct state_vector*)malloc(sizeof(struct state_vector*)*SIZE_OBJECT*4);

 

for(int i=0; i<SIZE_OBJECT*1; i++)
	neighbourV[i] = (struct state_vector*)malloc(sizeof(struct state_vector)*32);

 
		//neighbourV_bounce =(struct state_vector*)malloc(sizeof(struct state_vector*)*10);
 

	//for(int i=0; i<23*1; i++)
		//neighbourV_bounce[i] = (struct state_vector*)malloc(sizeof(struct state_vector)*32);


	argc_=argc;
	argv_=argv;

	 
mass_motion_state = (struct state_vector**)malloc(sizeof(struct state_vector**)*SIZE_OBJECT*4);

 

for(int i=0; i<SIZE_OBJECT*1; i++)
	mass_motion_state[i] = (struct state_vector*)malloc(sizeof(struct state_vector)*32);
 
	near_vector =(struct state_vector*)malloc(sizeof(struct state_vector*)*SIZE_OBJECT);
	int i,j,k;

	 
 
	for(int i=0; i<SIZE_OBJECT*1; i++)
		near_vector[i] = (struct state_vector*)malloc(sizeof(struct state_vector)*SIZE_OBJECT);
	srand(time(0));


if(initonce==1)
{
	int ll;
	for(ll = 0; ll < NUMBER_FLUIDS  ; ll++)
	{
		  double angle =  ((float)rand())/ 9600000000*99 * 6.283185307179586476925286766559;
		  double radius = ((float)rand())/  9600000000*99 ;
		  double x =  (double)( (double)(radius)*cos(angle))-0*2;
		  double z =  (double)( (double)(radius)*sin(angle))+0;
 		  double y = ((float)rand())/ 9600000000*99+75 ;

		  elementradius[ll] = radius;

        	  mass_motion_state[ll]->pos_new_x = x;//  ((float)rand())/1200000000*99+1-72*2;  ;//gör random position sen
 
 	          mass_motion_state[ll]->pos_new_y = y;//  ((float)rand())/1200000000*99+1 +25;
 		  mass_motion_state[ll]->pos_new_z = z;//  ((float)rand())/1200000000*99+1 -2 ;
 
 	  	  mass_motion_state[ll]->vel_new_x =  11 ;
 		  mass_motion_state[ll]->vel_new_y =  11;
 		  mass_motion_state[ll]->vel_new_z =  11   ;

		  mass_motion_state[ll]->pressure = 0.1;		
	          mass_motion_state[ll]->height = 1;
		
		  mass_motion_state[whatfluid]->color_x = ((float)rand())/3200000000*99;
		  mass_motion_state[whatfluid]->color_y = ((float)rand())/3200000000*99;
		  mass_motion_state[whatfluid]->color_z =((float)rand())/3200000000*99;
 		  mass_motion_state[ll]->initial_radius = radius;
		// printf("radius = %.10f %.10f %.10f\n",radius,x,y);
	}
	initonce = 0;
}

   
    	for(int i = 1; i < NUMBER_FLUIDS; ++i)
    	{
    		  // Change < to > if you want to find the smallest element
     		  if(elementradius[0] < elementradius[i])
       		    elementradius[0] = elementradius[i];
  	  }
    	printf("Largest element = %.2f", elementradius[0]);
 
	sleep(1);
	glutInit(&argc_, argv_);
	init_mpgeg();
	sleep(1);
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

 
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

    	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    	dirLight = (Light*)malloc(sizeof(Light));
    	dirLight->setPosition_light = setPosition_light;
    	dirLight->setAmbient_light = setAmbient_light;
    	dirLight->setDifusse_light = setDifusse_light;
    	dirLight->setSpecular_light = setSpecular_light;
 
    	dirLight->setPosition_light(0,0, 0);
    	dirLight->setAmbient_light(0, 0, 0);
   	dirLight->setDifusse_light(1, 1, 1);
    	dirLight->setSpecular_light(0.7f, 0.1f, 0.88f);
 
    	material =(Material*)malloc(sizeof(Material));
    	material->setAmbient_material =setAmbient_material;
    	material->setDifusse_material = setDifusse_material;
    	material->setSpecular_material =setSpecular_material;
    	material->setShininess_material = setShininess_material;
    	material->setAmbient_material(0, 0, 1, 1);
    	material->setDifusse_material(0.9f, 0.9f, 0.9f, 2.0f);
    	material->setSpecular_material(10.01f, 10.01f, 10.01f, 4.0f);
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

int pressure_once = 1;
void calc_fluid_acceleration()
{

	double Area_tube = pow(water_radius,2)*M_PI;
	double mass_fluid_element = density*(M_PI*pow(water_radius,3.0))*4.0/3.0;
	double gMASS;
 

	if(pressure_once ==1)
	{
		mass_motion_state[whatfluid]->pressure =  ((  -9.8+acc1)*mass_fluid_element/(M_PI*pow(water_radius,2.0)));
		pressure_once =0;
	}
	

	double sqrtme = mass_motion_state[whatfluid]->pressure/density;

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
	
    acc2_fluid[0][whatfluid] = (-1*(mass_motion_state[whatfluid]->pressure )/(( mass_motion_state[whatfluid]->pos_new_x)))/density + (viscous_constant*Area_tube*addme)/mass_fluid_element;
    acc2_fluid[1][whatfluid] = (-1*(mass_motion_state[whatfluid]->pressure )/(( mass_motion_state[whatfluid]->pos_new_y)))/density + (viscous_constant*Area_tube*addme)/mass_fluid_element;
    acc2_fluid[2][whatfluid] = (-1*(mass_motion_state[whatfluid]->pressure )/(( mass_motion_state[whatfluid]->pos_new_z)))/density + (viscous_constant*Area_tube*addme)/mass_fluid_element;

}

int ckk=0;
void display  (void){
 
    if(ckk == 0)
    {
	 load_pipe();
 
	 ckk = 1; 
    }
int ll;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   //glClearColor (1, 1, 1, 0);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_COLOR_MATERIAL);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glMultMatrixf(lightRotation);
 

    glLoadIdentity();

    gluLookAt(cameraEye[0]+0, cameraEye[1]+253, cameraEye[2]-423-300 , cameraLookAt[0], cameraLookAt[1], cameraLookAt[2], cameraUp[0], cameraUp[1], cameraUp[2]);

    glMultMatrixf(viewRotation);

    glRotatef(rotationX+149.000000,1,0,0);
    glRotatef(rotationY+98.0+320,0,1,0);
 
    glMultMatrixf(lightRotation);
    
    glPushMatrix();
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    glCallList(pipe);
    glPopMatrix();
for(ll = 0; ll < NUMBER_FLUIDS  ; ll++)
{
whatfluid =ll;
  

	

    glPushMatrix();
 
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
 
    glTranslatef(mass_motion_state[whatfluid]->pos_new_x,  -mass_motion_state[whatfluid]->pos_new_y*1 -0  , mass_motion_state[whatfluid]->pos_new_z );
    //glColor3f(mass_motion_state[whatfluid]->color_x, mass_motion_state[whatfluid]->color_y, mass_motion_state[whatfluid]->color_z);
     glutSolidSphere(6,15,15  );  

   // vDrawScene(mass_motion_state[whatfluid]->pos_new_x,-mass_motion_state[whatfluid]->pos_new_y,mass_motion_state[whatfluid]->pos_new_z);
    glPopMatrix();
    glutPostRedisplay();


    calc_fluid_velocity();
    calc_fluid_acceleration();
    mass_motion_state[whatfluid] = mass_motion( mass_motion_state[whatfluid] ,1,0);
		// printf(" %.5f  %.5f %.5f\n", wall_info[vn]->pos_new_x,wall_info[vn]->pos_new_y,wall_info[vn]->pos_new_z);
}

 glutSwapBuffers();
 display_mpeg();
}


void rates_fluid_motion ( double *t, double *f, double result[]   )
{
    result[0] =          f[3]/1.0;
    result[1] =          f[4]/1.0;
    result[2] =          f[5]/1.0;
  
    result[3] =   	 acc2_fluid[0][whatfluid]/1.0;
    result[4] =   	 -9.8 +acc2_fluid[1][whatfluid]/1.0 ; 
    result[5] =          acc2_fluid[2][whatfluid]/1.0 ;  
}

struct state_vector   *   mass_motion(struct state_vector     *  next_state, int num, int what )
{
	double f0[6];
	double f0_result[6];

	double z[6];
 	
 	double tburn = 0.6;
	double tf[6] = {tburn,tburn,tburn,tburn,tburn,tburn};
tburn = 0.5;
double t0[6] ={tburn,tburn,tburn,tburn,tburn,tburn};
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
 
	rk45(rates_fluid_motion, t0, f0,f0_result, tf,z,6,1);
 
	next_state->pos_new_x = f0[0];
	next_state->pos_new_y = f0[1];
	next_state->pos_new_z = f0[2];

	next_state->vel_new_x = f0[3];
	next_state->vel_new_y = f0[4];
	next_state->vel_new_z = f0[5];

	return   next_state ;
}


 
void findnearestpoint_3_points(int points , int num,double r_collision, double **vvv )
{
	int i, num_pts ;
 
	char  *pch;
	struct kdres *presults;
	double pos[3], dist;
	double pt[3] = { mass_motion_state[whatfluid]->pos_new_x,mass_motion_state[whatfluid]->pos_new_y, mass_motion_state[whatfluid]->pos_new_z };
	double radius = 25;
 
	num_pts =9920;

 	void *ptree_____;

 	if(alloc_once == 1)
	{
		ptree_____ = kd_create( 3 );
 
  		for( i=0; i<points; i++ ) {   
			       printf("target node at (%.3f, %.3f, %.3f) \n", wall_info[i]->pos_new_x, wall_info[i]->pos_new_y,wall_info[i]->pos_new_z);
    			assert( 0 == kd_insert3( ptree_____, wall_info[i]->pos_new_x, wall_info[i]->pos_new_y, wall_info[i]->pos_new_z, NULL ) );

	}
		alloc_once = 0;
	}
 
 
	presults = kd_nearest_range( ptree_____, pt, radius );
 
  	/* print out all the points found in results */
	// printf( "found %d results:\n", kd_res_size(presults) );

 
	//state_result_worm_dorsal_feather[num]->totaln = kd_res_size(presults) ;
 	 printf( "found %d results:\n", kd_res_size(presults) );

 
        int vc = 0;
       printf("source node at (%.3f, %.3f, %.3f) \n", pt[0], pt[1],pt[2]);
        while( !kd_res_end( presults ) ) {
   
		pch = (char*)kd_res_item( presults, pos ); 
    		//dist = sqrt( dist_sq( pt, pos, 3 ) );

		 // if(worm[num]->pos_new_x != pos[0])
 		 // printf( "node at (%.3f, %.3f, %.3f) is %.3f away   \n", 
 		 //    pos[0], pos[1], pos[2], dist  );
	
 printf("destination node at (%.3f, %.3f, %.3f) \n", pos[0], pos[1],pos[2]);
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

static GLuint	texture;
int load_pipe (void)
{ 
	int i,j,k;
	pipe = loadOBJ__("klot3.obj");  

 
	wall_info = (struct state_vector*)malloc(sizeof(struct state_vector*)*SIZE_OBJECT*4);

 

for(int i=0; i<SIZE_OBJECT*1; i++)
	wall_info[i] = (struct state_vector*)malloc(sizeof(struct state_vector)*32);
       
printf("done ! \n");
 
       
	printf("done ! \n");
 
	//for(int i=0; i<SIZE_OBJECT*2; i++)
	//	state_result_worm_ventral_feather[i] = (struct state_vector*)malloc(sizeof(struct state_vector)*2);


	//for(int i=0; i<SIZE_OBJECTs*1; i++)
	//	state_result_worm_ventral_feathers[i] = (struct state_vector*)malloc(sizeof(struct state_vector)*32); 
	printf("done ! \n");
  
 
	//for(int i=0; i<SIZE_OBJECTs*1; i++)
	//	state_result_worm_ventral_small[i] = (struct state_vector*)malloc(sizeof(struct state_vector)*32);

  
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
 
	// for(ll = 0; ll < SIZE_OBJECT      ; ll++)
	//{
 		// findnearestpoint(SIZE_OBJECT  ,state_result_worm_ventral,ll,1 );
	///}
 
 
 

	printf("done ! \n");
 
 
 
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


}


void calc_fluid_velocity(void)
{

	double distance_from_origin = sqrtf(powf( mass_motion_state[whatfluid]->pos_new_x - 0 ,2.0) +  powf( mass_motion_state[whatfluid]->pos_new_y -0 ,2.0)  + 		powf( mass_motion_state[whatfluid]->pos_new_z -0 ,2.0  ));
 
//if(( distance_from_origin) > 65)
//{

	for(int vn = 0; vn < 9920; vn++)
	{

		if(whatfluid == vn)
			continue;

		//need 3 points	

		double sqrtthis = powf( mass_motion_state[whatfluid]->pos_new_x - wall_info[vn]->pos_new_x ,2.0) +  powf( mass_motion_state[whatfluid]->pos_new_y - 	wall_info[vn]->pos_new_y ,2.0)  + powf( mass_motion_state[whatfluid]->pos_new_z - wall_info[vn]->pos_new_z ,2.0  );
 
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


 
		
			findnearestpoint_3_points(2000,   vn, 100, neighbourV );

			double aa[] = {mass_motion_state[whatfluid]->pos_new_x,mass_motion_state[whatfluid]->pos_new_y,mass_motion_state[whatfluid]->pos_new_z};
			double bb[] = {neighbourV_[1][0],neighbourV_[1][1],neighbourV_[1][2]};
			double cc[] = {neighbourV_[2][0],neighbourV_[2][1],neighbourV_[2][2]};
	 

			calculate_normal(aa ,bb ,cc ,normal);


			vel[0] =  mass_motion_state[whatfluid]->vel_new_x ;
			vel[1] = mass_motion_state[whatfluid]->vel_new_y;
			vel[2] =mass_motion_state[whatfluid]->vel_new_z;

 			dot(   vel ,  normal, dotprod_v_n);
			dot( normal, normal, dotprod_n_n);

			//u_perpv = (v · n / n · n) n
			u_perpv[0] = (dotprod_v_n[0]/dotprod_n_n[0])*normal[0];
			u_perpv[1] = (dotprod_v_n[1]/dotprod_n_n[1])*normal[1];
			u_perpv[2] = (dotprod_v_n[2]/dotprod_n_n[2])*normal[2];


			w_parv[0] = vel[0] - u_perpv[0]  ;
			w_parv[1] = vel[1] - u_perpv[1]  ;
			w_parv[2] = vel[2] - u_perpv[2]  ;
			printf("whatfluid  =%d \n", whatfluid );
			printf("vel[0]   =%.10f \n", vel[0] );
			printf("u_perpv[0]   =%.10f \n", u_perpv[0] );
			printf("w_parv[0]  =%.10f\n", w_parv[0] );
			printf("normal[0]  =%.10f\n", normal[0] );
			printf("neighbourV[0][1]  =%.10f\n", neighbourV_[0][1]);

			//v′ = f w − r u
			double r =0.7;
			double frictionforce = 10;
			mass_motion_state[whatfluid]->vel_new_x = frictionforce*w_parv[0] - r*u_perpv[0];
			mass_motion_state[whatfluid]->vel_new_y = frictionforce*w_parv[1] - r*u_perpv[1];
			mass_motion_state[whatfluid]->vel_new_z = frictionforce*w_parv[2] - r*u_perpv[2];
 			mass_motion_state[whatfluid] = mass_motion( mass_motion_state[whatfluid] ,1,0);

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
