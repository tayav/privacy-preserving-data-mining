#Usage of this toolset.

# Introduction #

This page explains how the utilities are used.

# Details #

**PREREQUISITES:** Christian Borgelt's "apriori" utility and the linear programming toolkit GLPK must be installed.

Assume you are working on the dataset "mushroom.dat" from FIMI repository. First run apriori utility to find the itemsets:

_apriori mushroom.dat mushroom.itemsets -ts -a -m2 -s30_

Then you can extract some itemsets from "mushroom.itemsets" file as sensitive itemsets and store them in a new file called "mushroom.itemsets.sensitive". If you want to automatically create this sensitive sets file from the itemsets file, you need to run "item" utility. For example following command selects 5 itemsets from mushroom.itemsets, store them in "mushroom.itemsets.sensitive" while storing the remaining itemsets in "mushroom.itemsets.nonsensitive":

_item mushroom 2430 5_

Utility _item_ selects the itemsets using some criteria, which are hardcoded. 2430 is the minimum support value as number of transactions.

Then, run "matrix", "cost" and "prep" utilities to create the necessary matrices:

_matrix mushroom_

_cost mushroom 2430_

_prep mushroom 2430 200_

where 200 is the max. item id in sensitive itemsets. This limits the size of the matrices files and also the size of the problem.

To run the linear programming toolkit, type:

_glpsol -m mushroom.mod_

where "mushroom.mod" is the GLPK model file.
The solution is written into "mushroom.final.sol". This is in CSV format. Finally run the utility "rmvexact" to remove the items from the original database and store sanitized database in a different file.

_rmvexact mushroom_

The sanitized database is now stored in "mushroom.dat.final".