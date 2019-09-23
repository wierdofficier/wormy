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
#define NUMEQ  6
#define NUMVAR 16
#define MAXBUFLEN 44*20000000
#define NUM_THREADS 3
#define MAX_PAIRS   16
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m" 
#define HOW_MANY_CHARGES 126976 //126976
#define HOW_MANY_SPHERES 12
double areatriangle[10000];
 typedef struct {
	float x, y, z;
} Vec3;
struct state_vector  *** state_result_e_motion ;
int findnearestcharge(int points,struct state_vector *** e_charge , int num,int howmany,int kk, int what_index );
extern int KvvVENTALA[HOW_MANY_CHARGES];
extern float* V; 
char search[1000][1000] = {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q"};
double solution666[] ={160.72,79.99,110.35,337.10,13.21,0.44};
double solution2[] = {30.0,10.0,20.0,60.0,1.0,25.0};

double solution3[] = {2, 1  ,  3,1.5,4.2,1.4 };

double solution99[] = {9.8,3.7,8.89,24.79,0.62,8.69};
   
 
double solution[] ={0.5000000000, 0.464746811,0.4218106990,0.3634457155,0.2623948664,0.1378011498 };
 

double a[NUMEQ+1];
#define AMP 10 
int bufcnt = 0;
char BIGBUF[1000][1000][3];
char BIGBUF2[1000][1000][3];
char BIGBUF3[1000][1000][3];
double _count = 0;
int iterator = 0;
int isbreaked = 0;
void task1()
{
	char e[] = "abcdefghijkl"; 
        permute_number(e, 0, NUMVAR-1,NULL);

}

void task2()
{
	char e[] = "+-*/^+-*/^+"; 
        permute_sign(e, 0, 10,NULL);
}


void task3()
{
while(1)
    matching_parenthesis_pairs(0, 0);

}
typedef struct _thread_data_t {
  int tid;
  double stuff;
} thread_data_t;


void *thr_func(void *arg) {
  thread_data_t *data = (thread_data_t *)arg;
 
 if(data->tid == 0)
	task1();
 
  	
}

int multitasks( ) {
  pthread_t thr[NUM_THREADS];
  int i, rc;
  thread_data_t thr_data[NUM_THREADS];
 
  for (i = 0; i < NUM_THREADS; ++i) {
    thr_data[i].tid = i;
    if ((rc = pthread_create(&thr[i], NULL, thr_func, &thr_data[i]))) {
      fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
      return EXIT_FAILURE;
    }
  }
  for (i = 0; i < NUM_THREADS; ++i) {
    pthread_join(thr[i], NULL);
  }
 
  return EXIT_SUCCESS;
}

typedef enum _PARENTHESIS {                   
    NONE,
    LEFT,
    RIGHT
} PARENTHESIS;

int n;
PARENTHESIS stack[MAX_PAIRS * 2 + 1] = {NONE, }; 
int ind;                                         
FILE * fp;
 
int onceopen = 1;
int kk = 0;
char equation_[1000];
char eq_[1000];
 
 void print_stack_(char *a,char *var)
{
	if(onceopen == 1)
	{	     
	   fp = fopen ("data2.txt","w");
	   onceopen =0;
	}
  
        int i;
        for (i = 1; i <= n * 2; i++) {
 
        	if (stack[i] == LEFT) {
             
			 if(i < 33  )
			{
			    //printf("%c(%c",a[kk],var[kk]);
			    sprintf(equation_,"%c(%c",a[kk],var[kk]);
 
			    strcpy(eq_,equation_);
			    if(i == 1)
			       fprintf (fp, "%s", (char*)(eq_+1));
			    else
			       fprintf (fp, "%s", eq_);				 
			}
		        else
			{
			   //printf("(");
			   char C = '(';
			   sprintf(eq_,"%c",C);
			   if(i == 1)
				fprintf (fp, "%s", (char*)(eq_+1));
			   else
			     	fprintf (fp, "%s", eq_);					
			}		 
        }
        	else if (stack[i] == RIGHT) {		 
			//printf(")%c%c",a[kk],var[kk]);
			sprintf(equation_,")%c%c",a[kk],var[kk]);
			fprintf (fp, "%s", equation_);		 	  
       		 }
		else {
		    //printf("ERROR!!!!\n");
		    // exit(-1);
		}
	kk++;
	if(kk >(NUMVAR-1))
	{
		kk  = 0; 
	}
    }
 
    //printf("\n");
    fprintf (fp, "\n");
}

 
void matching_parenthesis_pairs(int left_in_stack, int right_in_stack,char *a,char *var)
{
    if (left_in_stack < right_in_stack) {
        return;
    }
 
    if (ind == n * 2 + 1)
    {
        print_stack_(a,var);
        return;
    }

    if (left_in_stack < n) {
        stack[ind++] = LEFT;
        matching_parenthesis_pairs(left_in_stack + 1, right_in_stack,a,var);
        ind--;
    }
    if (right_in_stack < left_in_stack) {
        stack[ind++]= RIGHT;
        matching_parenthesis_pairs(left_in_stack, right_in_stack + 1,a,var);
        ind--;
    }
}
 
 
void swap_num (char *x, char *y)
{
    char temp;
    temp = *x;
    *x = *y;
    *y = temp;
}

void swap_sign (char *x, char *y)
{
    char temp;
    temp = *x;
    *x = *y;
    *y = temp;
}
void permute_number(char *a, int i, int n,char *var)
{
   int j;
   if (i == n) 
	{
 	 //printf("%c%c%c%c%c%c%c%c%c%c\n", a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9]);
	 char e[] = "+-*/^+-*/^+-"; 
	 permute_sign(e, 0, NUMVAR-1,a);
 
	}
  
   else
   {
        for (j = i; j <= n; j++)
       {
          swap_num((a+i), (a+j));
          permute_number(a, i+1, n,var);
          swap_num((a+i), (a+j)); 
       }
   }
}


void permute_sign(char *a, int i, int n,char *var)
{
   int j;
   if (i == n) 
	{
  
 	  //printf("%c%c%c%c%c%c%c%c%c%c\n", a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9]);
 	  matching_parenthesis_pairs(0, 0,a,var);
	}
  
   else
   {
        for (j = i; j <= n; j++)
       {
          swap_sign((a+i), (a+j));
          permute_sign(a, i+1, n,var);
          swap_sign((a+i), (a+j));  
       }
   }
}
// fork from https://github.com/irl/la-cucina/blob/master/str_replace.c
char* str_replace(char* string, const char* substr, const char* replacement) {
	char* tok = NULL;
	char* newstr = NULL;
	char* oldstr = NULL;
	int   oldstr_len = 0;
	int   substr_len = 0;
	int   replacement_len = 0;

	newstr = strdup(string);
	substr_len = strlen(substr);
	replacement_len = strlen(replacement);

	if (substr == NULL || replacement == NULL) {
		return newstr;
	}

	while ((tok = strstr(newstr, substr))) {
		oldstr = newstr;
		oldstr_len = strlen(oldstr);
		newstr = (char*)malloc(sizeof(char) * (oldstr_len - substr_len + replacement_len + 1));

		if (newstr == NULL) {
			free(oldstr);
			return NULL;
		}

		memcpy(newstr, oldstr, tok - oldstr);
		memcpy(newstr + (tok - oldstr), replacement, replacement_len);
		memcpy(newstr + (tok - oldstr) + replacement_len, tok + substr_len, oldstr_len - substr_len - (tok - oldstr));
		memset(newstr + oldstr_len - substr_len + replacement_len, 0, 1);

		free(oldstr);
	}
	//free(string);
	return newstr;
}
double f = 0;
int c = 0;
char   **new_str;
char   **strnew;
 
int newloop = 0;
 
 
 
 
char ** fill_equation(char * line_buf, double ** data , int lll,char ** str,char **str2 )
{
	strcpy(str[lll], line_buf);
	char buf1[1000];


	for(int ll = 0; ll < 16; ll++)
	{			 			
		char * ptr = strstr(str[lll], search[ll]);
		if (ptr != NULL) /* Substring found */
		{
			//printf("%s contains '%s'\n", new_str, search[ll]);												 
			sprintf(buf1,"%f" ,data[lll][ll]  );
							 
			str2[lll] = str_replace(str[lll],search[ll],buf1);	 
			strcpy(str[lll],str2[lll]); 	
			free(str2[lll]);		 
		}
	} 
return str[lll] ;
}
int randme;
double   result[20][1000];
 int newloopnew = 0;


int main()
{
	int i,j,k;  
	loadOBJ__("sphere.obj");  
 
	state_result_e_motion = (struct state_vector*)malloc(sizeof(struct state_vector*)*HOW_MANY_CHARGES);     	 	 
	state_result_e_motion= (struct state_vector ***) malloc(sizeof(struct state_vector**) * HOW_MANY_CHARGES);  
	for(i = 0; i < HOW_MANY_CHARGES; i++){
	 
	  state_result_e_motion[i] = (struct state_vector **) malloc(sizeof(struct state_vector*) * HOW_MANY_CHARGES);   
	  for(j = 0; j < HOW_MANY_SPHERES; j++){
		state_result_e_motion[i][j] = (struct state_vector *) malloc(sizeof(struct state_vector) * HOW_MANY_SPHERES);
	  }
 }

	Vec3 *v ;
 
	int ll;


	for(ll = 0; ll < HOW_MANY_CHARGES  ; ll++)
	{
		  v= (Vec3 *)(V + 3*(KvvVENTALA[ll]-1));
         	  state_result_e_motion[ll][kk]->pos_new_x = v->x;
 	          state_result_e_motion[ll][kk]->pos_new_y = v->y;
 		  state_result_e_motion[ll][kk]->pos_new_z = v->z;

 	  	 // state_result_e_motion[ll][kk]->vel_new_x =  1e-6 ;
 		 // state_result_e_motion[ll][kk]->vel_new_y =  1e-6 ;
 		 // state_result_e_motion[ll][kk]->vel_new_z =  1e-5 ;
	}


	for(ll = 0; ll < HOW_MANY_CHARGES     ; ll++)
	{
 		findnearestcharge(HOW_MANY_CHARGES  ,state_result_e_motion,ll,0,0,0 );
	}
for(;;);
	ind = 1;
        srand(time(NULL));
	//multitasks();

	  //    for(;;);
	FILE *fp5;	
        char* filename = "datad.txt";
        fp5 = fopen(filename, "r");
	ind = 1;
        if (fp5 == NULL  ){
        	printf("Could not open file %s",filename);
        	return 1;
        }
  
 
	strnew = malloc( 1000);
 
  		for(int iiiii=0; iiiii<NUMEQ+1; iiiii++)
			strnew[iiiii] =  malloc(123);


	new_str = malloc( 1000);
	 
  		for(int iiiii=0; iiiii<NUMEQ+1; iiiii++)
			new_str[iiiii] =  malloc(123);
  
 	char * line_buf = NULL;
 	size_t line_buf_size = 0;
	int line_count = 0;
	ssize_t * line_size;

	line_size = malloc(   MAXBUFLEN );	 
	iterator = 0;
   
 	double **VARA1;
	VARA1 = malloc( 100*32);
	 
  		for(int iiiii=0; iiiii<200*1; iiiii++)
			VARA1[iiiii] =  malloc(1233);

 

	double inc  = 0;
	int counter = 0;
	while(line_size >= 0 && line_count < 600000000)
	{
 
double a1[NUMEQ+1][16] = {{ 343.6185298488 ,  179.9997502023,3.1416045179,88.9743848289, 92.1159893467,343.6185298488  , 179.9997502023, 3.1416045179,88.9743848289, 92.1159893467,343.6185298488  , 179.9997502023, 3.1416045179,88.9743848289, 92.1159893467,343.6185298488   }, {299.7507299596,   162.7581576695,2.8406815710, 74.7155452308,77.5562268018,299.7507299596 ,162.7581576695  ,2.8406815710, 74.7155452308,77.5562268018,299.7507299596 ,162.7581576695  ,2.8406815710, 74.7155452308,77.5562268018,299.7507299596    }, { 256.9214900677,  146.2897760739,2.5532536058,60.8763841478 ,63.4296377536,256.9214900677  ,146.2897760739,2.5532536058,60.8763841478 ,63.4296377536,256.9214900677 ,146.2897760739,2.5532536058,60.8763841478 ,63.4296377536,256.9214900677    },{213.6098700566 ,   130.8114901264 ,2.2831062334,46.8034622640,49.0865684974,213.6098700566 ,130.8114901264 , 2.2831062334,   46.8034622640,49.0865684974,213.6098700566 ,130.8114901264 , 2.2831062334,   46.8034622640,49.0865684974,213.6098700566 },{  164.4020800410,117.9351530039 , 2.0583714238,30.2863057619,32.3446771858,164.4020800410, 117.9351530039,2.0583714238,30.2863057619,32.3446771858,164.4020800410,117.9351530039,2.0583714238,30.2863057619,32.3446771858,164.4020800410    },{ 117.3358800260, 114.3873197512 ,1.9964497681,15.1458921258,17.1423418940,117.3358800260,  114.3873197512,1.9964497681,15.1458921258,17.1423418940,117.3358800260 ,  114.3873197512,1.9964497681,15.1458921258,17.1423418940,117.3358800260    }};
 
 
 		newloopnew = 0;
		line_size[line_count] = getline(  &line_buf  , &line_buf_size, fp5);	

		for(int oooo = 0; oooo < NUMEQ; oooo++)
		{ 
			for(int kkk = 0; kkk < NUMVAR; kkk++)
			{ 
				  VARA1[oooo][kkk] = a1[oooo][kkk];
			 
			}
		 a[oooo] = 0;
		}	 	 
		
	       	for(int llll = 0; llll < 50  ; llll++)
		{ 
	 		// for(int oooo = 0; oooo < NUMEQ; oooo++)
			// { 	
			if(newloopnew == 1)
				break;			 
				for(int kkk = 0; kkk < NUMVAR; kkk++)
				{ 
				for(int oooo = 0; oooo < NUMEQ; oooo++)
				{

				new_str[oooo] = fill_equation(line_buf,VARA1 ,0,new_str,strnew);
	 
				result[0][0] = te_interp(new_str[0], 0);
					
					if(newloop == 0  || isnan(result[0][0]) == 0  &&  (result[0][0] < 1.0 && result[0][0] > -1.0) && result[0][0] > 0.0)
						{
 							new_str[oooo] = fill_equation(line_buf,VARA1 ,oooo,new_str,strnew);
							result[oooo][kkk] = te_interp(new_str[oooo], 0);
 							check_eq(oooo,kkk,new_str);
							
							newloop++;
						}
					else
					 {
					  newloopnew = 1;
					  break;
					}
				}
					 if(newloopnew == 1)
							break;	
				for(int oooo = 0; oooo < NUMEQ; oooo++)
				{ 
					double ratio = result[oooo][kkk]/solution[oooo];
			 
					a[oooo] =  a[oooo] + 0.00000001;		 
				}

				for(int lll = 0; lll < NUMVAR; lll++)
				{ 
						 if(kkk != lll)
						 {				
						
 		 
				double earth_i[100][100];
				for(int aa = 0; aa <= NUMEQ; aa++)
				{
				for(int bb = 0; bb < NUMEQ; bb++)
				{
				 	if(bb != aa)
						earth_i[aa][bb] = solution[aa]/solution[bb];	
				} 
				}
							int cc = 0;
							int xxxc = 0;	
 
							cc = 0;
 
		 					for(int xxxx = 0; xxxx <  NUMEQ; xxxx++)
							{ 

								for(int llllll = 0; llllll <  NUMEQ; llllll++)
								{ 
								if(llllll != xxxx)
								{
									double diff = result[xxxx][kkk]/result[llllll][kkk];
										 
									double ratio = earth_i[xxxx][cc];
										 
									if(diff > (ratio))	
									{
										VARA1[llllll][kkk]  += a[xxxx];	
											
									}
									else
									{
										VARA1[llllll][kkk]  -= a[xxxx];
										 									
									}
								}
										cc++;
								}				 
								 cc = 0;
							} 
 				
						for(int oooo = 0; oooo < NUMEQ; oooo++)
						{
							VARA1[oooo][kkk] += a[oooo];

					 	}
							counter++;
 
							}
								 
						 }
						 
			 		 }
					 if(newloopnew == 1)
							break;	

				}
 

		line_count++;	 
		iterator++;
		isbreaked = 0;	
		randme = iterator;
 										
		}
	 for(int i = 0; i < 25; i++)
	{
		printf("eq[%d] %s = %f\n",i,BIGBUF[i][0],te_interp(BIGBUF[i][0], 0));
		//printf("%s\n", BIGBUF2[i][1]);
		//printf("%s\n", BIGBUF3[i][2]);
	}
  //fclose(fp5);

    return 0;
}

void check_eq(int oooo,int kkk, char **resultstr )
{	

	 if((result[0][kkk]) < solution[0]*1.080000000 && ((result[0][kkk])) > solution[0]*0.9200)
	 if((result[1][kkk]) < solution[1]*1.080000000 && ((result[1][kkk])) > solution[1]*0.9200)
	//if((result[2][kkk]) < solution[2]*1.080000000 && ((result[2][kkk])) > solution[2]*0.9200)
	//if((result[3][kkk]) < solution[3]*1.080000000 && ((result[3][kkk])) > solution[3]*0.9200)
	///if((result[4][kkk]) < solution[4]*1.080000000 && ((result[4][kkk])) > solution[4]*0.9200)
	//if((result[5][kkk]) < solution[5]*1.080000000 && ((result[5][kkk])) > solution[5]*0.9200) 
	{		
			printf("eq: ="CYN" %s"RESET" "WHT" solution: "YEL" %f"RESET" "WHT" EQ linenr: "CYN" %d"RESET" "WHT" "RESET"\n",resultstr[oooo], result[oooo][kkk],randme);
			if(oooo == 5) printf("\n");					 
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
static double dist_sq( double *a1, double *a2, int dims ) {
  double dist_sq = 0, diff;
  while( --dims >= 0 ) {
    diff = (a1[dims] - a2[dims]);
    dist_sq += diff*diff;
  }
  return dist_sq;
}
void *ptree;
int ptreeonce = 1;
//find nearest charge from the charge in question

int findnearestcharge(int points,struct state_vector *** e_charge , int num,int howmany,int kk, int what_index ) {
int i, num_pts = points;
double dist[1000];
double length[3];
int indexbuddyfirst = 0;
int indexbuddysecond = 0;
int INDEX;
char *data, *pch;
struct kdres *presults;
double pos[3];
double pt[3] = { e_charge[num][kk]->pos_new_x,e_charge[num][kk]->pos_new_y, e_charge[num][kk]->pos_new_z };
double radius = 0.08/1.0;

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
	if(e_charge[num][kk]->pos_new_x != pos[0])
	if(e_charge[num][kk]->pos_new_y != pos[1])
	if(e_charge[num][kk]->pos_new_z != pos[2])
{
    	 e_charge[num][kk][feather_count].near_buddy_posx = pos[0];
  	 e_charge[num][kk][feather_count].near_buddy_posy = pos[1];
   	 e_charge[num][kk][feather_count].near_buddy_posz = pos[2];
  	
    	dist[feather_count] = sqrt( dist_sq( pt, pos, 3 ) );
 feather_count++;
}
	int k;

// printf( "node from (%.13f, %.13f, %.13f)   \n", 
 		 //     e_charge[num][kk]->pos_new_x, e_charge[num][kk]->pos_new_y, e_charge[num][kk]->pos_new_z  );

// printf( "node to (%.13f, %.13f, %.13f) is %.13f away   \n", 
 	//	      pos[0], pos[1], pos[2], dist[feather_count]   );
   if(what_index == 1)
{
    INDEX = find_index_for_charge(HOW_MANY_CHARGES,e_charge,num,feather_count,kk);
    e_charge[num][kk][kd_res_size(presults)].INDEX_NR = INDEX;
    // printf("INDEX = %d \n", INDEX);
}
 
    
 if(num % 1000 ==0)
 	printf("num @ %d \n", num);

 
kd_res_next( presults );
}
    /* go to the next entry */


	float minimumfirst = dist[0];
for (  i = 0; i < feather_count; ++i)
{
 
  if (dist[i] < minimumfirst  )
  {
    minimumfirst = dist[i];
    
   indexbuddyfirst = i;
  }
}
 

 //printf("#1 minium buddy distance @ %.20f with index %d \n", minimumfirst,indexbuddyfirst);
 	float minimumsecond = dist[0];
for (  i = 0; i < feather_count; ++i)
{
 	 printf("buddy distance @ %.20f  index = %d\n", dist[i],i);
  if (dist[i] < minimumsecond    )
  {
if (dist[i] >  minimumfirst*1.01 )
{
    minimumsecond = dist[i];
   indexbuddysecond = i;
}
 
  }
}
	 printf("#2 minium buddy distance @ %.20f #1 minium buddy distance @ %.20f \n", minimumsecond,minimumfirst);

	 printf("#2 with index %d #1 with index %d \n", indexbuddysecond,indexbuddyfirst);


 printf( "node from (%.13f, %.13f, %.13f)  \n", 
	e_charge[num][kk]->pos_new_x ,e_charge[num][kk]->pos_new_y,e_charge[num][kk]->pos_new_z );


 printf( "to node (%.13f, %.13f, %.13f)  \n", 
	e_charge[num][kk][indexbuddyfirst].near_buddy_posx ,e_charge[num][kk][indexbuddyfirst].near_buddy_posy,e_charge[num][kk][indexbuddyfirst].near_buddy_posz );

 printf( "and node (%.13f, %.13f, %.13f)  \n", 
	e_charge[num][kk][indexbuddysecond].near_buddy_posx ,e_charge[num][kk][indexbuddysecond].near_buddy_posy,e_charge[num][kk][indexbuddysecond].near_buddy_posz );
//hjältens ekvation:
length[0] = sqrt(pow(e_charge[num][kk]->pos_new_x,2.0) + pow(e_charge[num][kk]->pos_new_y,2.0)+pow(e_charge[num][kk]->pos_new_z,2.0));
length[1] = sqrt(pow(e_charge[num][kk][indexbuddyfirst].near_buddy_posx ,2.0) + pow(e_charge[num][kk][indexbuddyfirst].near_buddy_posy,2.0)+pow(e_charge[num][kk][indexbuddyfirst].near_buddy_posz,2.0));
length[2] =  sqrt(pow(e_charge[num][kk][indexbuddysecond].near_buddy_posx ,2.0) + pow(e_charge[num][kk][indexbuddysecond].near_buddy_posy,2.0)+pow(e_charge[num][kk][indexbuddyfirst].near_buddy_posz,2.0));

double perimeter = (length[0]+length[1]+length[2])/2.0;


areatriangle[num] = sqrt(perimeter*(perimeter-length[0])*(perimeter-length[1])*(perimeter-length[2]));

printf("areatriangle[num] = %.13f\n",areatriangle[num] );
  feather_count = 0;
  /* free our tree, results set, and other allocated memory */
  //free( data );
  kd_res_free( presults );
  //kd_free( ptree );

  return 0;
}
 
