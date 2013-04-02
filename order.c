#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX 120000
#define MAXTRANS 40000
#define MSUP 4950
#define ITEMS 218
#define COL 5000

int comp (const void * elem1, const void * elem2) {
    int f = *((int*)elem1);
    int s = *((int*)elem2);
    if (f > s) return  1;
    if (f < s) return -1;
    return 0;
}

main(int argc, char **argv)
{
 FILE *tp, *fp, *fp2, *fp3, *op, *sp, *it;
 char s[50000], s1[10];
 char t[50000], t1[10];
 unsigned int *db;
 int fis[20000][50], tran[90000];
 char str1[50], str2[50], str3[50], str4[50];
 int i=0,j=0,k=0,l=0,h=0; 
 int ii=0;  
 int cnt;
 int kmax[10000], lmax;
 char trtrue=0;
 char str[50000];
 int tottr=0, elmtr=0;
 int a;
 unsigned max=0;
 unsigned reduced_index=1;
 unsigned residue_index=1;

 if(argc<=1) {
	printf("Usage: prep db_name\n");
	exit(1);
 }


 sprintf(str1,"%s.dat",argv[1]);
 if((tp=fopen(str1,"r"))==NULL) {printf("Transactions file could not be opened.\n");exit(2);}

 sprintf(str2,"%s2.dat",argv[1]);

 if((op=fopen(str2,"w"))==NULL) {printf("Output file could not be opened.\n");exit(3);}

 db=(unsigned int *) malloc(MAXTRANS*COL*sizeof(unsigned int));
 if(db==NULL) perror("error221");



  // check transactions against the itemset!

	  memset(t,(int)NULL,50000);           

	  while(fgets(t, 50000, tp)!=NULL) {

	  	//puts(t);
		memset(str,(int)NULL,50000);
		trtrue=0;
		tottr++;
		printf("%d\r",tottr);

		  for(i=0,j=0,l=0; t[i]!='\n'; i++) { 

		    t1[j++]=t[i];
		    if(t[i]==32){ 
			a=atoi(t1); j=0; 
		 	if(a!=0) { 
				tran[l++]=a; 
				//printf("*%d ",tran[l-1]);
 				
			}
			memset(t1,(int)NULL,10); 
		    }

	    
		
	  	  }
		  if(j>0)tran[l++]=atoi(t1);
		  lmax=l;	


			//fprintf(fp2,"%s\n",str); 
	
			fprintf(op,"%s",str); 
			
			qsort (&tran[0], lmax, sizeof(tran[0]), comp);

			for(j=0,k=1; j<lmax; j++){

			  fprintf(op,"%d ",tran[j]); 
				
			}
			  fprintf(op,"\n"); 
			reduced_index++;
			//for(j=0; j<lmax; j++) fprintf(fp2,"%d,%d,%d\n",tottr,j+1,tran[j]);
 



          }



  printf("Max=%u\n",max);
  memset(s,(int)NULL,50000); 

 
printf("\n Done.\n");
printf("Total transactions: %d.\n",tottr);


}
