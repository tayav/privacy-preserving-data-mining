#include<stdio.h>
#include<stdlib.h>
#include<string.h>

main(int argc, char **argv)
{
 FILE *tp, *fp, *fp2, *fp3, *op, *sp;
 char s[50000], s1[10];
 char t[50000], t1[10];
 int fis[20000][50], tran[90000];
 char strtp[50], strfp[50], strop[50];
 int i=0,j=0,k=0,l=0,h=0; 
 int ii=0;  
 int cnt;
 int kmax[10000], lmax;
 char trtrue=0;
 char str[50000];
 int tottr=0, elmtr=0;
 int a, max=0;
 int totalitem=0;

 if(argc<=1) {
	printf("Usage: matrix dbname \n");
	exit(1);
 }

 sprintf(strtp,"%s.dat",argv[1]);
 sprintf(strfp,"%s.itemsets.sensitive",argv[1]);
 sprintf(strop,"%s.matrix",argv[1]);

 if((tp=fopen(strtp,"r"))==NULL) {printf("Transactions file could not be opened.\n");exit(2);}

 if((fp=fopen(strfp,"r"))==NULL) {printf("Itemsets file could not be opened.\n");exit(2);}

 if((op=fopen(strop,"w"))==NULL) {printf("Output file could not be opened.\n");exit(3);}


 memset(s,(int)NULL,50000); 

 sprintf(str,"%s.reduced",strop);
 if((fp2=fopen(str,"w"))==NULL) {printf("reduced file could not be opened.\n");exit(5);} 
 sprintf(str,"%s.residue",strtp);
 if((fp3=fopen(str,"w"))==NULL) {printf("residue file could not be opened.\n");exit(5);}


 printf("Looking for itemsets in %s ...",argv[2]);
 while(fgets(s, 50000, fp)!=NULL) {

 //puts(s);
 
  //fprintf(fp2,"\n%d ",h+1);  

  for(i=0,j=0,k=0; s[i]!='('; i++) { 
	
    s1[j++]=s[i];
    if(s[i]==32){ 
	a=atoi(s1); j=0; 
 	if(a!=0) { 
		fis[h][k++]=a; 
		//fprintf(fp2,"%d ",fis[h][k-1]);	
	}
	memset(s1,(int)NULL,10); 
    }

    

  }

  kmax[h++]=k;
  
  
 }
//fclose(fp2);
printf(" Done. %d itemsets found.\n",h);
printf("Generating matrix...\n"); fflush(stdout);


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
		  lmax=l;	
		  
		  totalitem+=lmax;		
		  // search for fis[0..kmax] in trans[0..lmax]
		for(ii=0; ii<h; ii++){
		  cnt=0;
		  for(i=0, cnt=0; i<kmax[ii]; i++)	{

			for(j=0; j<lmax; j++) 
				if(fis[ii][i]==tran[j]) cnt++;

			
			if(i!=(cnt-1)) break;


		  }


		  sprintf(str,"%s%d ",str,(cnt==kmax[ii])?1:0); 

		  if(cnt==kmax[ii]) trtrue=1;
	

  		}

		fprintf(op,"%s\n",str); 
		
		if(trtrue) fprintf(fp2,"%s\n",str); else { fprintf(fp3,"%s\n",t); elmtr++;}

          }



  //printf("\n");
  memset(s,(int)NULL,50000); 

 
printf("\n Done.\n");
printf("Total transactions: %d. Eliminated: %d Final:%u\n",tottr,elmtr,tottr-elmtr);
printf("Total item: %u\n",totalitem);



}
