#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#define MAX 120000
#define MAXTRANS 48000
#define COL 8000

unsigned int col[MAXTRANS];
unsigned long int tim1, tim2;

void print_matrix(FILE *x, unsigned int db[MAXTRANS][COL], int tottr, unsigned *col)
{
 int i, j;
 for(i=1;i<=tottr;i++){

   for(j=1;j<col[i]; j++) fprintf(x, "%d ",db[i][j]);

   fprintf(x,"\n");
 }
 

}


void remove_item(unsigned int db[MAXTRANS][COL], unsigned *col, int rowx, int colx)
{
 int i;

 for(i=colx; i<col[rowx]-1; i++) db[rowx][i]=db[rowx][i+1];

 col[rowx]--;

}
 unsigned int db[MAXTRANS][COL];
 unsigned char sol[MAXTRANS][COL];
main(int argc, char **argv)
{
 FILE *tp, *fp, *cp, *op, *sp;
 char s[MAX], s1[1000];
 char t[MAX], t1[1000];


 unsigned int i=0,j=0,k=0,l=0,h=0; 
 int cnt;
 char trtrue=0;
 char str[MAX];
 //unsigned int id[MAXTRANS];
 unsigned int item[MAX];
// unsigned int *mtx;
// unsigned int *db;

 int tottr=0, elmtr=0;
 unsigned m,n;
 unsigned int a;
 int sens[40000][20];
 //unsigned char constraint[MAXTRANS][25];
 int support[40000];
 int senstot=0;
 int nmax[40000];
 int smax=0, sindex, mindex;
 unsigned sup;
 unsigned distance=0;
 unsigned group[MAXTRANS];
 unsigned freq[MAX];
 unsigned char bool1=0;

 unsigned int cost=99999;
 unsigned rowx, colx;

 if(argc<=1) {
	printf("Usage: remove dbname\n");
	exit(1);
 }

 char strdb[50], strsol[50], strout[50];
 sprintf(strdb,"%s.dat.reduced",argv[1]);
 sprintf(strsol,"%s.final.sol",argv[1]);
 sprintf(strout,"%s.dat.final",argv[1]);


 if((tp=fopen(strdb,"r"))==NULL) {printf("Reduced DB file could not be opened.\n");exit(2);}

 if((fp=fopen(strsol,"r"))==NULL) {printf("GLPK solution file could not be opened.\n");exit(2);}

// if((cp=fopen(strout,"r"))==NULL) {printf("Output file could not be opened.\n");exit(2);}

 if((op=fopen("res.dat","w"))==NULL) {printf("Output file could not be opened.\n");exit(3);}


	/*mtx=(unsigned int *) malloc(MAXTRANS*COL*sizeof(unsigned int));
	db=(unsigned int *) malloc(MAXTRANS*COL*sizeof(unsigned int));
	  
	if(mtx==NULL) perror("error1");
	if(db==NULL) perror("error2");*/



	for(i=0; i<MAXTRANS; i++) col[i]=0;

	k=0;

	fgets(t, MAX, fp);
	while(!feof(fp)) {
          
	fscanf(fp,"%u,%u,%u\n",&i,&j,&a);
	  if(a==0 || a==1) sol[i][j]=a;	
	  if(a==1) k++;
	  //printf("%u,%u,%u %u\n",i,j,a,sol[i][j]);
		//if(k==5) break;
	}
	
	printf("Perturbations: %u\n",k);

	while(fgets(t, MAX, tp)!=NULL) {

		tottr++;
		printf("%d\r",tottr);
		//printf("\n");

		for(i=0,j=0,l=0; t[i]!='\n'; i++) { 

		    s1[j++]=t[i];
		    if(t[i]==32){ 
			a=atoi(s1); j=0; 

				item[l++]=a; 
	

			memset(s1,(int)NULL,100); 
		    }		   
	   	}	
		//a=atoi(s1);
		//item[l++]=a; 

		col[tottr]=l; 
		for(k=1; k<l; k++) {db[tottr][k]=item[k]; /*printf("%u ",sol[tottr][k]);*/	}
//printf("\n");
//if(h==5) break;
		h++;

	
          }

//exit(1);

for(k=1; k<col[1]; k++) { printf("%u ",db[1][k]);	}

// matrix loaded.. 
printf("Total transactions: %d \n",tottr);
printf("Removing items..\n");

tim1=time(NULL);

	distance=0;
	for(i=1; i<=tottr; i++)
		for(k=1; k<COL; k++) {

		   if(sol[i][k]==0) continue;	
		   for(j=1; j<col[tottr]; j++) {


			if(sol[i][k]==1 && k==db[i][j]){

				remove_item(db,col,i,j);
				sol[i][k]=0;
				//remove_sol(sol,col,i,k);
				distance++;

			}


		   }
		if(sol[i][k]==1) printf("Missed: %u %u\n",i,k);

		}





//print_matrix(stdout, db, tottr, col);

// Algorithm 1 /////////////////////////////////////////////////////////////////////////////// 
//RANDOMLY..
/*
distance=0;
while((sup=check_support(support,senstot))!=-1){

 
 // find a transaction supporting a sensitive itemsets
 for(i=1;i<=tottr;i++){
  
   for(k=0,j=0;j<senstot;j++) 
	if(constraint[i][j] && support[j]>MSUP-1){sindex=i; m=j; i=tottr+1; j=senstot; break;}
   	
 } // m=the id of SFIS

 for(j=1; j<col[sindex]; j++){

	for(l=0; l<nmax[m]; l++) 
		if(sens[m][l]==*(db+sindex*COL+j)) {

			rowx=sindex; colx=j; j=col[sindex]; l=nmax[m];
			break;
		}
	//   if(*(mtx+sindex*COL+j)>=0 && *(mtx+sindex*COL+j)<cost) {cost=*(mtx+sindex*COL+j); rowx=sindex; colx=j;}
 }

  printf("row=%u(%u)  col=%u  cost=%u item=%u\n",rowx,*(db+rowx*COL+0),colx,*(mtx+rowx*COL+colx),*(db+rowx*COL+colx));			
	
 	for(k=0; k<senstot; k++){
	  
	  if(constraint[rowx][k]==0) continue;

	  for(l=0; l<nmax[k]; l++){

		if(*(db+rowx*COL+colx)==sens[k][l]) {
			support[k]--; 
			constraint[rowx][k]=0; 

			//break;
		}
	  }

	  
	}

			remove_item(db,col,rowx,colx);
			remove_item(mtx,col,rowx,colx);col[rowx]--;
			distance++;


	
 
}
*/
///////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////////////
//print_matrix(stdout, db, tottr, col);
print_matrix(op, db, tottr, col);

tim2=time(NULL);

printf("\ndistance=%u\t time=%u\n",distance,(unsigned)((tim2-tim1)/1.0));

fclose(op);

sprintf(str,"rm %s", strout);
system(str);
sprintf(str,"cat %s.dat.residue res.dat >> %s", argv[1], strout);
system(str);


}
