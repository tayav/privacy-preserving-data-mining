#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

// minimum support in terms of number of transactions
unsigned int MSUP=4950;

float cost(unsigned int a)
{
//  return 1.0/(a-(float)MSUP+0.1)*(float)MSUP;
 return exp(-1.0*((float)a-(float)MSUP)/(float)MSUP*10.0)*10.0;
//return a-MSUP;
}

main(int argc, char **argv)
{
 FILE *tp, *fp, *fp2, *op, *sp, *mp, *ip, *nsp, *opr;
 char s[30000], s1[10];
 char t[30000], t1[10];
 char strtp[50],strmp[50],strop[50],strfp[50],strnsp[50];
 int fis[10000][50], tran[5000];
 float trancost[10000];
 int nsens[80000][20];
 int nsupport[80000];
 int nsenstot=0;
 int nmax[80000];
 unsigned char constraint[25];
 int i=0,j=0,k=0,l=0,h=0; 
 int ai, aj, ak,al,am,an;
 int ii=0;  
 int cnt, cnt2;
 int kmax[10000], lmax;
 char trtrue=0;
 char str[30000];
 char str2[30000];
 int tottr=0, elmtr=0;
 int a;
 int numofsensitemsets=0;
unsigned long int tot=0;
int freq[1500];

 if(argc<=2) {
	printf("Usage: cost dbname msup\n");
	exit(1);
 }

 sprintf(strtp,"%s.dat",argv[1]);
 sprintf(strfp,"%s.itemsets.sensitive",argv[1]);
 sprintf(strmp,"%s.matrix",argv[1]);
 sprintf(strop,"%s.cost.matrix",argv[1]);
 sprintf(strnsp,"%s.itemsets.nonsensitive",argv[1]);

 MSUP=atoi(argv[2]);

 if((tp=fopen(strtp,"r"))==NULL) {printf("Transactions file could not be opened.\n");exit(2);}

 if((fp=fopen(strfp,"r"))==NULL) {printf("Itemsets file could not be opened.\n");exit(2);}

 if((mp=fopen(strmp,"r"))==NULL) {printf("Constraint matrix file could not be opened.\n");exit(2);}

 if((op=fopen(strop,"w"))==NULL) {printf("Output file could not be opened.\n");exit(3);}

 sprintf(str,"%s.reduced",strop);
 if((opr=fopen(str,"w"))==NULL) {printf("Output file could not be opened.\n");exit(2);}


if((nsp=fopen(strnsp,"r"))==NULL) {printf("Non-sensitive itemsets file could not be opened.\n");exit(3);}

// sprintf(str,"%s.reduced",argv[1]);
// ip=fopen(str,"w");

 memset(s,(int)NULL,30000); 

 // frequencies of items in sensitive itemsets
 for(i=0; i<1500; i++) freq[i]=1;
 freq[87]=freq[64]=freq[86]=freq[85]=freq[90]=1;
 freq[1]=freq[3]=2;
 freq[32]=freq[28]=freq[148]=3;
 freq[7]=freq[218]=4;
 freq[27]=5;
 freq[6]=7;
 freq[11]=8;
 /////////////////////////////////////////////

 printf("Looking for itemsets...");
 while(fgets(s, 30000, fp)!=NULL) {
 
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
numofsensitemsets=h;
//////// Read Nonsensitive itemsets and support values ///////
 printf("Reading nonsensitive itemsets and supprt values (MSUP:%u)...",MSUP);

h=0;

 while(fgets(s, 30000, nsp)!=NULL) {

 
  for(i=0,j=0,k=0; s[i]!='('; i++) { 
	
    s1[j++]=s[i];
    if(s[i]==32){ 
	a=atoi(s1); j=0; 
 	if(a!=0) { 
		nsens[h][k++]=a; 
		//fprintf(fp2,"%d ",fis[h][k-1]);	
	}
	memset(s1,(int)NULL,10); 
    }

    

  }

  nmax[h]=k;
  
  for( ;s[i]!='/' && i<4000; i++);
   i++;
  for(j=0; s[i]!=')'; i++) { 
	
    s1[j++]=s[i];   

  }

  nsupport[h]=atoi(s1);//printf("%s\n",s1);	

  memset(s1,(int)NULL,10); 

 h++;
  //printf("nsup %d\n",nsupport[h]);
  
 }
nsenstot=h;
////////////////////////////////////////////////////////////////

printf("Done\n");
printf("Generating matrix...\n"); fflush(stdout);
  // check transactions against the itemset!

	  memset(t,(int)NULL,30000);           

	  while(fgets(t, 30000, tp)!=NULL) {


		trtrue=0;
		tottr++;
		printf("%d\r",tottr);fflush(stdout);

		// Read constraint matrix to know which sens. itemsets are supported!

		if(fgets(s, 30000, mp)==NULL) {perror("error 66"); exit(6);}

		for(i=0,j=0,l=0; s[i]!='\n'; i++) { 

		    if(s[i]=='0') constraint[l++]=0;
		    else if(s[i]=='1')	constraint[l++]=1;

	   	}

		////////////////////////////////////////////////////////////////////////


	  	//puts(t);
		memset(str,(int)NULL,30000);
		memset(str2,(int)NULL,30000);
		memset(trancost,0,10000*sizeof(float));


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

		  // search for fis[0..kmax] in trans[0..lmax]
		for(ii=0; ii<numofsensitemsets; ii++){
		  cnt=0;

		  // if transaction doesn't support sensitive itemset skip this itemset!	
		  if(constraint[ii]==0) continue;
 		  /////////////////////////////////////////////////////////////////////
	
		  for(i=0, cnt=0; i<kmax[ii]; i++)	{

			for(j=0; j<lmax; j++) 
				if(fis[ii][i]==tran[j]) {
					cnt++;
//////////////////////////// item found, calculate its cost now!! //////////////
 trancost[j]=0;
 // find itemset nsens[nsenstot][nmax[]] in transaction tran[lmax]
 
  for(ai=0; ai<nsenstot; ai++){
	
	for(aj=0; aj<nmax[ai]; aj++){

	   if(nsens[ai][aj]==tran[j]){
	
		// check if itemset is in the transaction?
		for(cnt2=0,ak=0; ak<nmax[ai]; ak++){
		 
		 for(am=0; am<lmax; am++){
//			freq[am]=0;

			if(nsens[ai][ak]==tran[am]) cnt2++;

		}

		  if(ak!=(cnt2-1)) break;

		}

		if(cnt2==nmax[ai]) 
		 trancost[j]+=cost(nsupport[ai]);
//trancost[j]+=(float)1.0/(nsupport[ai]-(float)MSUP+0.1)*(float)MSUP/(float)freq[tran[j]];


	   }

	}

  }

			

	//if(trancost[j]>=0.01)printf("Trans:%d Item:%d - Cost:%.0f\n",tottr,fis[ii][i],trancost[j]);

///////////////////////////////////////////////////////////////////////////////////////
				}

			
			//if(i!=(cnt-1)) break;


		  }

	
		  //sprintf(str,"%s%d ",str,(cnt==kmax[ii])?1:0); 

		  if(cnt==kmax[ii]) trtrue=1;
	

  		}

		for(j=0;j<lmax;j++){
		 sprintf(str,"%s%.0f ",str,trancost[j]); 
		 sprintf(str2,"%s%u ",str2,tran[j]); 
		}

		fprintf(op,"%s\n",str);

		if(trtrue){fprintf(opr,"%d %s\n",tottr,str);/* fprintf(ip,"%d %s\n",tottr,str2); */}
		else elmtr++;	

          }



  //printf("\n");
  memset(s,(int)NULL,30000); 

 
printf("\n Done.\n");
printf("Total transactions: %d. Eliminated: %d. Final:%d\n",tottr,elmtr,tottr-elmtr);


}
