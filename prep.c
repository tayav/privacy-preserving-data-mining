#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX 120000
#define MAXTRANS 40000
#define COL 5000

int MSUP =  99999;
int ITEMS =0;

int comp (const void * elem1, const void * elem2) {
    int f = *((int*)elem1);
    int s = *((int*)elem2);
    if (f > s) return  1;
    if (f < s) return -1;
    return 0;
}

main(int argc, char **argv)
{
 FILE *tp, *fp, *fp2, *fp3, *op, *cp, *sp, *it, *op2;
 char s[50000], s1[10];
 char t[50000], t1[10];
 unsigned int *db;
 int fis[20000][50], tran[90000], cost[90000];
 char str1[50], str2[50], str3[50], str4[50];
 int i=0,j=0,k=0,l=0,h=0; 
 int ii=0;  
 int cnt;
 int kmax[10000], lmax;
 char trtrue=0;
 char str[50000];
 char strx[50000];
 int tottr=0, elmtr=0;
 int a;
 unsigned max=0;
 unsigned reduced_index=1;
 unsigned residue_index=1;

 if(argc<=3) {
	printf("Usage: prep db_name MSUP max_item_number\n");
	exit(1);
 }

 MSUP=atoi(argv[2]);
 ITEMS=atoi(argv[3]);

 sprintf(str1,"%s.dat",argv[1]);
 if((tp=fopen(str1,"r"))==NULL) {printf("Transactions file could not be opened.\n");exit(2);}

 sprintf(str1,"%s.dat.reduced",argv[1]);
 if((op2=fopen(str1,"w"))==NULL) {printf("Reduced transactions file could not be opened.\n");exit(2);}

 sprintf(str1,"%s.itemsets.sensitive",argv[1]);
 if((fp=fopen(str1,"r"))==NULL) {printf("Itemsets file could not be opened.\n");exit(2);}

 sprintf(str1,"%s.itemsets.sensitive.csv",argv[1]);
 if((it=fopen(str1,"w"))==NULL) {printf("Itemsets.csv file could not be opened.\n");exit(2);}

 sprintf(str1,"%s.cost.matrix",argv[1]);
 if((cp=fopen(str1,"r"))==NULL) {printf("Cost matrix file could not be opened.\n");exit(2);}

 sprintf(str1,"%s.cost.matrix.csv",argv[1]);
 if((sp=fopen(str1,"w"))==NULL) {printf("Cost matrix.csv file could not be opened.\n");exit(2);}

 sprintf(str2,"%s.matrix.reduced.csv",argv[1]);
 sprintf(str3,"%s.dat.reduced.csv",argv[1]);
 sprintf(str4,"%s.dat.residue",argv[1]);

 if((op=fopen(str2,"w"))==NULL) {printf("Output file could not be opened.\n");exit(3);}


 memset(s,(int)NULL,50000); 

 if((fp2=fopen(str3,"w"))==NULL) {printf("reduced file could not be opened.\n");exit(5);} 
 sprintf(str,"%s.residue",argv[1]);
 if((fp3=fopen(str4,"w"))==NULL) {printf("residue file could not be opened.\n");exit(5);}


 db=(unsigned int *) malloc(MAXTRANS*COL*sizeof(unsigned int));
 if(db==NULL) perror("error221");

 fprintf(op,"ROWX,COLX,VALX\n"); 
 fprintf(fp2,"ROWX,COLX,VALX\n"); 
 fprintf(it,"ROWX,COLX,VALX\n"); 
 fprintf(sp,"ROWX,COLX,VALX\n"); 


 printf("Looking for itemsets in %s ...",argv[2]);
 while(fgets(s, 50000, fp)!=NULL) {

 //puts(s);
 
  //fprintf(fp2,"\n%d ",h+1);  

  for(i=0,j=0,k=0; s[i]!='('; i++) { 
	
    s1[j++]=s[i];
    if(s[i]==32){ 
	a=atoi(s1); j=0; 
 	if(a!=0) { 
		fis[h][k]=a; 
		tran[k]=a;
		k++;
		//fprintf(fp2,"%d ",fis[h][k-1]);	
	}
	memset(s1,(int)NULL,10); 
    }

    

  }

  kmax[h++]=k;
  


			qsort (&tran[0], k, sizeof(tran[0]), comp);

			for(j=1,k=1; j<=ITEMS; j++){

				if(j==tran[k-1]) {fprintf(it,"%d,%d,1\n",h,j); k++;}
				else fprintf(it,"%d,%d,0\n",h,j);
			}


  
 }



fclose(it);
printf(" Done. %d itemsets found.\n",h);
printf("Generating matrix...\n"); fflush(stdout);


  // check transactions against the itemset!

	  memset(t,(int)NULL,50000);           

	  while(fgets(t, 50000, tp)!=NULL) {

		memset(str,(int)NULL,50000);
		memset(strx,(int)NULL,50000);
		trtrue=0;
		tottr++;
//		printf("%d\r",tottr);

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
		  lmax=l;	

		 if(fgets(s,50000,cp)==NULL) perror("error34");
		  for(i=0,j=0,l=0; s[i]!='\n'; i++) { 

		    s1[j++]=s[i];
		    if(s[i]==32){ 
			a=atoi(s1); j=0; 
		 	//if(a!=0) { 
				cost[l++]=a; 
				//printf("*%d ",tran[l-1]);
 				
			//}
			memset(s1,(int)NULL,10); 
		    }
		
	  	  }
		  //lmax=l;	


		  // search for fis[0..kmax] in trans[0..lmax]
		for(ii=0; ii<h; ii++){
		  cnt=0;
		  for(i=0, cnt=0; i<kmax[ii]; i++)	{

			for(j=0; j<lmax; j++) 
				if(fis[ii][i]==tran[j]) {cnt++; }

			
			if(i!=(cnt-1)) break;


		  }

		  sprintf(str,"%s%d,%d,%d\n",str,reduced_index,ii+1,(cnt==kmax[ii])?1:0); 
		  

		  if(cnt==kmax[ii]) trtrue=1;
	

  		}


		
		
		if(trtrue) {
			//fprintf(fp2,"%s\n",str);
			for(j=0;j<lmax;j++){			 
			 sprintf(strx,"%s%u ",strx,tran[j]); 
			}
			fprintf(op2,"%d %s\n",tottr,strx);  
	
			fprintf(op,"%s",str); 
			
			qsort (&tran[0], lmax, sizeof(tran[0]), comp);

			for(j=1,k=1; j<=ITEMS; j++){


				if(j==tran[k-1]) {fprintf(fp2,"%d,%d,1\n",reduced_index,j);
				fprintf(sp,"%d,%d,%d\n",reduced_index,j,cost[k-1]); k++;

				}
				else {fprintf(fp2,"%d,%d,0\n",reduced_index,j);
				fprintf(sp,"%d,%d,%d\n",reduced_index,j,0);

				}
			}

			reduced_index++;
			//for(j=0; j<lmax; j++) fprintf(fp2,"%d,%d,%d\n",tottr,j+1,tran[j]);
 

		}
		else { 
			/*qsort (&tran[1], lmax-1, sizeof(tran[0]), comp);

			for(j=1,k=1; j<=ITEMS; j++){

				if(j==tran[k-1]) {fprintf(fp3,"%d,%d,1\n",residue_index,j); k++;}
				else fprintf(fp3,"%d,%d,0\n",residue_index,j);
			}

			residue_index++;
			*/
			for(j=0; j<lmax; j++){ fprintf(fp3,"%d ",tran[j]); }
			fprintf(fp3,"\n");
			 elmtr++;
	
		}


          }



  printf("Max=%u\n",max);
  memset(s,(int)NULL,50000); 

 
printf("\n Done.\n");
printf("Total transactions: %d. Eliminated: %d Final:%d\n",tottr,elmtr,tottr-elmtr);


}
