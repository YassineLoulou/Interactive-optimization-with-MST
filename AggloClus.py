# -*- coding: utf-8 -*-
"""
Created on Wed Jul  5 17:25:22 2023

@author: yassi
"""
import numpy as np
import matplotlib . pyplot as plt
from scipy .io import arff
import time
from sklearn import cluster
import ast

print("hello")


datanp=[]
with open("C:/Users/yassi/OneDrive/Bureau/points_input.txt", 'r') as f:
    lines = f.readlines()
    for line in lines:
        numbers=[int(num) for num in line.split()]
        print(numbers)
        datanp.append(numbers)

print(datanp)

f0 = [ x[0] for x in datanp]
f1 =[ x[1] for x in datanp] 

# set distance_threshold (0 ensures we compute the full tree )
tps1 = time . time ()


model = cluster . AgglomerativeClustering (linkage ='single')
model = model .fit( datanp )
tps2 = time . time ()
labels = model . labels_
k = model . n_clusters_
leaves = model . n_leaves_
print(len(labels))
print(labels)
# Affichage clustering
print ("nb clusters =",k,", nb feuilles = ", leaves , " runtime = ", round (( tps2 - tps1 )*1000 ,2),"ms")

# plt. scatter (f0 , f1 , c=labels , s=8)
# plt. title (" Agglomerative clustering")
# plt. show ()
# print ("nb clusters =",k,", nb feuilles = ", leaves , " runtime = ", round (( tps2 - tps1 )*1000 ,2),"ms")




f = open("C:/Users/yassi/OneDrive/Bureau/points_output.txt", "w")
for x in labels:
    f.write(str(x)+'\n')
f.close()














