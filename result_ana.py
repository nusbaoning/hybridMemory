# -*- coding: utf-8 -*-
# author Bao Ning
''' 
input : .req file
function : block is identified by original block id + update times
output : a ranklist of traces, the request lists in time windows of each blocks
# 
'''

import sys



def loadfile(filename):
    fin = open(filename, 'r', encoding='utf-8', errors='ignore')
    equal = 0
    total = 0
    half = 0
    limitLarge = 0
    for line in fin.readlines():
    	index = line.find("thresh = ") 
    	if index < 0:
    		continue
    	# print(index)    	
    	lines = line[index:].split(',')
    	# print(lines)
    	thresh = int(lines[0][9:])
    	bg_thresh = int(lines[1][12:])
    	limit = int(lines[2][8:])
    	wb_thresh = int(lines[3][12:])
    	# print(thresh)
    	# print(wb_thresh)
    	total += 1
    	if thresh == wb_thresh:
    		equal += 1
    	if bg_thresh == int(thresh/2):
    		half += 1
    	if limit > thresh:
    		limitLarge += 1

    fin.close()
    print(equal/total, equal, total)
    print(half/total)
    print(limitLarge/total)


loadfile("thresh.log")
   