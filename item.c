#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

// minimum support in terms of number of transactions
unsigned int MSUP=4950;
unsigned int NUM=10;
unsigned th;
float ratio=1.1; //mean=ratio *sigma;
float ratio2=0.1; // stddev=ratio2*sigma;
unsigned int STDDEV;
unsigned char skip=0;
unsigned maxid=10000;

 int nsens[60000][20];
 int nsupport[60000];
 int nsenstot=0;
 int nmax[60000];

unsigned average(unsigned n[], unsigned t[])
{
 int i, a=0;
 for(i=0; i<NUM; i++) a+=n[t[i]];

 return a/NUM;  

}

unsigned stddev(unsigned n[], unsigned t[], int avr)
{
 unsigned i;
 int a=0;

 for(i=0; i<NUM; i++) {a+=(((int)n[t[i]]-avr)*((int)n[t[i]]-avr));

 }

 return sqrt(a/NUM);

}

unsigned check(unsigned i)
{
 int k;
 for(k=0; k<nmax[i]; k++)  if(nsens[i][k]>maxid) return 0;

 return 1;
}

main(int argc, char **argv)
{
 FILE *tp, *fp, *fp2, *op, *sp, *mp, *ip, *nsp, *opr;
 char s[10000], s1[10];
 char t[10000], t1[10];
 char strtp[50],strmp[50],strop[50],strfp[50],strnsp[50];
 int fis[10000][50];
 float var=0;
 float ort=0;
 unsigned int item[25];
 int i=0,j=0,k=0,l=0,h=0; 
 int ai, aj, ak,al,am,an;
 int ii=0;  
 int cnt, cnt2;
 int kmax[60000], lmax;
 char trtrue=0;
 char str[10000];
 char str2[10000];
 int tottr=0, elmtr=0;
 int a, b, a2, b2;
 int numofsensitemsets=0;
unsigned long int tot=0;
int freq[1500];

 if(argc<=4) {
	printf("Usage: item dbname msup #sets maxitemid\n");
	exit(1);
 }

 sprintf(strfp,"%s.itemsets.sensitive",argv[1]);
 sprintf(strop,"%s.itemsets.nonsensitive",argv[1]);
 sprintf(strnsp,"%s.itemsets",argv[1]);

 MSUP=atoi(argv[2]);
 NUM=atoi(argv[3]);
 maxid=atoi(argv[4]);

 if((fp=fopen(strfp,"w"))==NULL) {printf("Itemsets file could not be opened.\n");exit(2);}

 if((op=fopen(strop,"w"))==NULL) {printf("Output file could not be opened.\n");exit(3);}

 if((nsp=fopen(strnsp,"r"))==NULL) {printf("Non-sensitive itemsets file could not be opened.\n");exit(3);}

 memset(s,(int)NULL,10000); 



//////// Read Nonsensitive itemsets and support values ///////
 printf("Reading nonsensitive itemsets and supprt values (MSUP:%u)(N:%u)...",MSUP,NUM);

h=0;

 while(fgets(s, 10000, nsp)!=NULL) {

 
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

printf("\n%u itemsets read\n",nsenstot);
srand(time(NULL));

for(i=0; i<NUM; i++){

  while(nmax[(a=(float)random()/(float)RAND_MAX*(float)nsenstot)]>(i/2+2));
  
  item[i]=a;
  printf("random: %u (%u)\n", item[i], nmax[item[i]]);

}


th=MSUP*ratio;
//th=MSUP+200;
STDDEV=MSUP*ratio2;

do{
 a=average(nsupport,item);
 a2=stddev(nsupport,item,a);
 j=(float)random()/(float)RAND_MAX*(float)NUM;
 i=(float)random()/(float)RAND_MAX*(float)nsenstot;

 k=item[j];
 item[j]=i;
 b=average(nsupport,item);
 b2=stddev(nsupport,item,b);


 if((((b-a)>0 && a<th*0.95) || ((a-b)>0 && a>th*1.05)) && nmax[k]==nmax[i] && check(i)==1) continue;
 else if((((b2-a2)>0 && a2<STDDEV*0.95) || ((a2-b2)>0 && a2>STDDEV*1.05)) && nmax[k]==nmax[i] && check(i)==1) continue;
 else item[j]=k;

}while(!(a>th*0.95 && a<th*1.05 && a2>STDDEV*0.95 && a2<STDDEV*1.05) );

for(i=0; i<NUM; i++){
 for(j=0; j<nmax[item[i]]; j++){ 
  
  printf("%u ", nsens[item[i]][j]);
 
 }
 printf("(%u)\n",nsupport[item[i]]);
 a+=nsupport[item[i]];
}

b=average(nsupport,item);
printf("Sigma=%u Average sup=%u stddev=%u\n",th,b,stddev(nsupport,item,b));


// write into files !!

a=0;
for(i=0; i<NUM; i++){
 for(j=0; j<nmax[item[i]]; j++){ 
  
  fprintf(fp,"%u ", nsens[item[i]][j]);
 
 }
 fprintf(fp,"(99/%u)\n",nsupport[item[i]]);
}


for(i=0; i<nsenstot; i++){


 for(k=0, skip=0; k<NUM; k++) if(item[k]==i) skip=1;

 if(skip) continue;

 for(j=0; j<nmax[i]; j++){ 
  
  fprintf(op,"%u ", nsens[i][j]);
 
 }
 fprintf(op,"(99/%u)\n",nsupport[i]);
}



}
