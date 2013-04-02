

param transnum, integer, >0;
param itemsnum, integer, >0;
param sfisnum, integer, >0;
param msup, integer, >0;

set ITEMS := 1..itemsnum;
set TRANS := 1..transnum;
set SFIS  := 1..sfisnum ;

set K dimen 2;
set L dimen 2;
set M dimen 2;
set N dimen 2;
set O dimen 2;

param db {i in TRANS, j in ITEMS}, binary;
param cost {i in TRANS, j in ITEMS}, integer, >=0;
param sens {i in SFIS, j in ITEMS};
param sup {i in TRANS, j in SFIS};


table data IN "CSV" "mushroom.matrix.reduced.csv": M<-[ROWX,COLX],sup~VALX; 
table data IN "CSV" "mushroom.itemsets.sensitive.csv": N<-[ROWX,COLX],sens~VALX; 
table data IN "CSV" "mushroom.dat.reduced.csv": K<-[ROWX,COLX],db~VALX;
table data IN "CSV" "mushroom.cost.matrix.csv": L<-[ROWX,COLX],cost~VALX;  

/*var x {i in TRANS, j in ITEMS} >=0, binary;*/
var x {(i,j) in K} >=0, binary;

/* minimize z: sum {i in TRANS, j in ITEMS} (db[i,j]*x[i,j]); */
 minimize z: sum {i in TRANS, j in ITEMS} (db[i,j]*x[i,j]*cost[i,j]); 

s.t. const1{r in SFIS}: sum{i in TRANS, j in ITEMS : sup[i,r]==1} (sens[r,j]*x[i,j]) >= sum{i in TRANS}(sup[i,r])-msup+1;
s.t. const2{r in SFIS, k in TRANS: sup[k,r]==1 }: sum{j in ITEMS} (sens[r,j]*x[k,j]) <= 1;  

solve;

printf "distance= %d",  sum{i in 1..transnum, j in 1..itemsnum} x[i,j];
table result{(i,j) in K} OUT "CSV" "mushroom.final.sol": i~ROW,j~COLX,x[i,j]~VALX;  

data;

param transnum := 7748;
param itemsnum := 100;
param sfisnum := 10;
param msup := 2600;

end;
