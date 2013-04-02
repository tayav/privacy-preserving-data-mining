#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#define MAX 50000
#define MAXTRANS 48000
//#define MSUP 4950
#define COL 5000

unsigned int col[MAXTRANS];
unsigned long int tim1, tim2;
unsigned algo=0,  MSUP=999999;

int max_support(int *sup, int tot)
{
 int i;
 int max=0, index=-1;

 for(i=0;i<tot;i++) if(sup[i]>MSUP-1 && sup[i]>max) { max=sup[i]; index=i; }

 return index;
}
int check_support(int *sup, int tot)
{
 int i;
 int max, index;

 for(i=0;i<tot;i++) if(sup[i]>MSUP-1) return i;

 return -1;
}
void print_matrix(FILE *x, unsigned int *db, int tottr, unsigned *col)
{
 int i, j;
 for(i=1;i<=tottr;i++){

   for(j=1;j<col[i]; j++) fprintf(x, "%d ",*(db+i*COL+j));

   fprintf(x,"\n");
 }
 

}

void remove_item(unsigned int *db, unsigned *col, int rowx, int colx)
{
 int i;

 for(i=colx; i<col[rowx]-1; i++) *(db+rowx*COL+i)=*(db+rowx*COL+i+1);

 //col[rowx]--;

}

main(int argc, char **argv)
{
 FILE *tp, *fp, *cp, *op, *sp;
 char s[MAX], s1[100];
 char t[MAX], t1[100];


 unsigned int i=0,j=0,k=0,l=0,h=0; 
 int cnt;
 char trtrue=0;
 char str[MAX];
 //unsigned int id[MAXTRANS];
 unsigned int item[MAX];
 unsigned int *mtx;
 unsigned int *db;
 int tottr=0, elmtr=0;
 unsigned m,n;
 unsigned int a;
 int sens[40000][20];
 unsigned char constraint[MAXTRANS][25];
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

 if(argc<=3) {
	printf("Usage: remove dbname algo_version msup\n");
	exit(1);
 }

 char strdb[50], strcost[50], strconst[50], strsens[50], strout[50];

 algo=atoi(argv[2]);
 MSUP=atoi(argv[3]);

 sprintf(strdb,"%s.dat.reduced",argv[1]);
 sprintf(strcost,"%s.cost.matrix.reduced",argv[1]);
 sprintf(strconst,"%s.matrix.reduced",argv[1]);
 sprintf(strsens,"%s.itemsets.sensitive",argv[1]);
 sprintf(strout,"%s.dat.final%u",argv[1],algo);


 if((fp=fopen(strdb,"r"))==NULL) {printf("Reduced DB file could not be opened.\n");exit(2);}

 if((tp=fopen(strcost,"r"))==NULL) {printf("DB cost matrix could not be opened.\n");exit(2);}

 if((cp=fopen(strconst,"r"))==NULL) {printf("Constraint matrix could not be opened.\n");exit(2);}

 if((sp=fopen(strsens,"r"))==NULL) {printf("Sensitive itemsets file could not be opened.\n");exit(2);}

 if((op=fopen("res.dat","w"))==NULL) {printf("Output file could not be opened.\n");exit(3);}

 ////////////// Read sensitive itemsets and support values //////////////////////////
 while(fgets(s, 5000, sp)!=NULL) {

 //puts(s);
  
 
  for(i=0,j=0,k=0; s[i]!='('; i++) { 
	
    s1[j++]=s[i];
    if(s[i]==32){ 
	a=atoi(s1); j=0; 
 	if(a!=0) { 
		sens[h][k++]=a; 
		//fprintf(fp2,"%d ",fis[h][k-1]);	
	}
	memset(s1,(int)NULL,10); 
    }

    

  }

  nmax[h]=k;
  
  for( ;s[i]!='/' && i<2000; i++);
   i++;
  for(j=0; s[i]!=')'; i++) { 
	
    s1[j++]=s[i];   

  }

  support[h]=atoi(s1);//printf("%s\n",s1);	

  memset(s1,(int)NULL,10); 

  printf("sup %d\n",support[h]);
  
 h++;
 }
senstot=h;
////////////////////////////////////////////////////////////////

       


	  mtx=(unsigned int *) malloc(MAXTRANS*COL*sizeof(unsigned int));
	  db=(unsigned int *) malloc(MAXTRANS*COL*sizeof(unsigned int));
	  
	if(mtx==NULL) perror("error1");
	if(db==NULL) perror("error2");

	  memset(t,(int)NULL,MAX); memset(s,(int)NULL,MAX); 
	  memset(t1,(int)NULL,10);  memset(s1,(int)NULL,10); 

	for(i=0; i<MAXTRANS; i++) col[i]=0;


	  while(fgets(t, MAX, tp)!=NULL) {

		

	  	//puts(t);
		//memset(str,(int)NULL,MAX);
		trtrue=0;
		tottr++;
		printf("%d\r",tottr);
		//printf("\n");

		for(i=0,j=0,l=0; t[i]!='\n'; i++) { 

		    s1[j++]=t[i];
		    if(t[i]==32){ 
			a=atoi(s1); j=0; 
		 	//if(a!=0) { 
				item[l++]=a; 
				//fprintf(fp2,"%d ",fis[h][k-1]);	
			//}
			memset(s1,(int)NULL,100); 
		    }		   
	   	}	
		//mtx[tottr]=(unsigned int *) malloc((l+2)*sizeof(int));
		//if(mtx[tottr]==NULL) {perror("out of memory"); exit(2);}
		col[tottr]=l;
		for(k=0; k<l; k++) *(mtx+tottr*COL+k)=item[k];	


		memset(s1,(int)NULL,100); 
		if(fgets(s, MAX, fp)==NULL) {perror("error 22"); exit(5);}

		for(i=0,j=0,l=0; s[i]!='\n'; i++) { 

		    s1[j++]=s[i];
		    if(s[i]==32){ 
			a=atoi(s1); j=0; 
		 	//if(a!=0) { 
				item[l++]=a; 
				//fprintf(fp2,"%d ",fis[h][k-1]);	
			//}
			memset(s1,(int)NULL,100); 
		    }		   
	   	}	
		//db[tottr]=(unsigned int *) malloc((l+2)*sizeof(int));
		//if(db[tottr]==NULL) {perror("out of memory"); exit(2);}

		if(l>COL-1) {perror("too many columns"); exit(2);}

		col[tottr]=l;
		for(k=0; k<l; k++) *(db+tottr*COL+k)=item[k];	

		// read constraint matrix
		if(fgets(s, MAX, cp)==NULL) {perror("error 23"); exit(6);}

		for(i=0,j=0,l=0; s[i]!='\n'; i++) { 

		    s1[j++]=s[i];
		    if(s[i]==32){ 
			a=atoi(s1); j=0; 
		 	//if(a!=0) { 
				constraint[tottr][l++]=(unsigned char)a; 
				//fprintf(fp2,"%d ",fis[h][k-1]);	
			//}
			memset(s1,(int)NULL,100); 
		    }		   
	   	}	

		h++;

	
          }


/// sensitive itemsets
/* for(i=0;i<senstot;i++){

   for(j=0;j<nmax[i]; j++) printf("%d ",sens[i][j]);

   printf("\n");
 }*/
///



// matrix loaded.. 
printf("Total transactions: %d \n",tottr);
printf("Running heuristic algorithm #%u..\n",algo);
printf("MSUP in terms of transactions: %u\n",MSUP);
printf("Removing items..\n");

tim1=time(NULL);
//print_matrix(stdout, db, tottr, col);

if(algo==1){
// Algorithm 1 /////////////////////////////////////////////////////////////////////////////// 
//RANDOMLY..

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

//  printf("row=%u(%u)  col=%u  cost=%u item=%u\n",rowx,*(db+rowx*COL+0),colx,*(mtx+rowx*COL+colx),*(db+rowx*COL+colx));			
printf("%u\r",distance);	
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

}
//////////////////////////////////////////////////////////////////////////////////////////////////
else if(algo==2){
// Algorithm 2 /////////////////////////////////////////////////////////////////////////////// 

distance=0;
while((sup=max_support(support,senstot))!=-1){
 
 
 // find the transaction that supports max sensitive itemsets
 //for(i=1,smax=0 ;i<=tottr;i++){
  
 //  for(k=0,j=0;j<senstot;j++) k+=constraint[i][j];
 
 //  if(k>smax) {sindex=i; smax=k;}
   	
 //}

for(sindex=1; sindex<=tottr; sindex++)	
 for(cost=999999, j=1; j<col[sindex]; j++){

	bool1=0;
   //for(k=0, bool1=0; k<senstot; k++) 
	if(constraint[sindex][sup]==1 && support[sup]>MSUP-1){

		for(l=0; l<nmax[sup]; l++) if(sens[sup][l]==*(db+sindex*COL+j)) bool1=1;

   	}

   if(!bool1) continue;

   if(*(mtx+sindex*COL+j)==0){rowx=sindex; colx=j; j=col[sindex]; sindex=tottr+1; break; }
   else 
   if(*(mtx+sindex*COL+j)>=0 && *(mtx+sindex*COL+j)<cost) {cost=*(mtx+sindex*COL+j); rowx=sindex; colx=j;}
	
 }

//  printf("row=%u(%u)  col=%u  cost=%u item=%u d:%u\n",rowx,*(db+rowx*COL+0),colx,*(mtx+rowx*COL+colx),*(db+rowx*COL+colx), distance);			
printf("%u\r",distance);	
				
 	for(k=0; k<senstot; k++){
	  if(constraint[rowx][k]==0) continue;
	  for(l=0; l<nmax[k]; l++){
		if(*(db+rowx*COL+colx)==sens[k][l]) {
			support[k]--; 
			constraint[rowx][k]=0; 
		}
	  }
	}
	remove_item(db,col,rowx,colx);
	remove_item(mtx,col,rowx,colx);col[rowx]--;
	distance++;
 
}
}
//////////////////////////////////////////////////////////////////////////////////////////////////

// Algorithm 3 //////////////////////////////////////////////////////////////////////// 
else if(algo==3){
distance=0;
while((sup=max_support(support,senstot))!=-1){

 for(i=1,smax=0 ;i<=tottr;i++){  
   for(k=0,j=0;j<senstot;j++) if(constraint[i][j]==1 && support[j]>MSUP-1) k+=constraint[i][j]; 
   if(k>smax) {sindex=i; smax=k;}   	
 }

 rowx=sindex; colx=0;

 for(j=1, smax=0; j<col[sindex]; j++){

   freq[j]=0;	

   for(k=0; k<senstot; k++) {
	
	if(constraint[sindex][k]==1 && support[k]>MSUP-1){

		for(l=0; l<nmax[k]; l++) 
		 if(sens[k][l]==*(db+sindex*COL+j)) 
		      {freq[j]++;}

   	}
   }  

 	if(freq[j]>=smax){
	   colx=j;
	   smax=freq[j];	 	
	}
	
 }
 
//  printf("row=%u(%u)  col=%u  cost=%u item=%u d:%u\n",rowx,*(db+rowx*COL+0),colx,*(mtx+rowx*COL+colx),*(db+rowx*COL+colx), distance);				
printf("%u\r",distance);	
	

  	for(k=0; k<senstot; k++){
	  if(constraint[rowx][k]==0) continue;
	  for(l=0; l<nmax[k]; l++){
		if(*(db+rowx*COL+colx)==sens[k][l]) {
			support[k]--; 
			constraint[rowx][k]=0; 
		}
	  }
	}
	remove_item(db,col,rowx,colx);
	remove_item(mtx,col,rowx,colx);col[rowx]--;
	distance++; 
}
}
///////////////////////////////////////////////////////////////////////////////////////////
else if(algo==4){
// Algorithm 4 //////////////////////////////////////////////////////////////////////// 

distance=0;
while((check_support(support,senstot))!=-1){


  for(i=1,smax=0 ;i<=tottr;i++){  
   for(k=0,j=0;j<senstot;j++) if(constraint[i][j]==1 && support[j]>MSUP-1) k+=constraint[i][j]; 
   if(k>smax) {sindex=i; smax=k;}   	
  }


 rowx=sindex; colx=0;
 
 cost=9999999;

 for(j=1, smax=0; j<col[sindex]; j++){

   freq[j]=0;
   bool1=0;	

	for(k=0; k<senstot; k++) {

		if(constraint[sindex][k]==1 && support[k]>MSUP-1){

			for(l=0; l<nmax[k]; l++) 
			 if(sens[k][l]==*(db+sindex*COL+j)) 
			      {freq[j]++; bool1=1; }

		}
	}  

	if(bool1 && (*(mtx+sindex*COL+j)/freq[j])<cost){  cost=*(mtx+sindex*COL+j)/ freq[j]; rowx=sindex; colx=j; }

	
 }





 
//  printf("row=%u(%u)  col=%u  cost=%u item=%u d:%u\n",rowx,*(db+rowx*COL+0),colx,*(mtx+rowx*COL+colx),*(db+rowx*COL+colx), distance);	
printf("%u\r",distance);	

 	for(k=0; k<senstot; k++){
	  if(constraint[rowx][k]==0) continue;
	  for(l=0; l<nmax[k]; l++){
		if(*(db+rowx*COL+colx)==sens[k][l]) {
			support[k]--; 
			constraint[rowx][k]=0; 
		}
	  }
	}
	remove_item(db,col,rowx,colx);
	remove_item(mtx,col,rowx,colx);col[rowx]--;
	distance++;	
 
}
}
///////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////
//print_matrix(stdout, db, tottr, col);
print_matrix(op, db, tottr, col);

tim2=time(NULL);

printf("\ndistance=%u\t time=%u\n",distance,(unsigned)((tim2-tim1)/1.0));

fclose(op);

sprintf(str,"rm %s", strout);
system(str);
sprintf(str,"cat res.dat %s.dat.residue >> %s", argv[1], strout);
system(str);


}
