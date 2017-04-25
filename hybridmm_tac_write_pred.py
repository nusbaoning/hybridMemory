# -*- coding: utf-8 -*-
import codecs
import time
import sys
from operator import itemgetter
from enum import Enum
from random import randint


HEADLINENUMBER = 11
MINLENGH = 10
READ_FUNCTION_NAME = "generic_file_read_iter"
STAGEMAX = 3

page = {}  
writePeriod = 100000
writeReq = 0
totalWriteReq = 0
sortedList = []
current = 0
maxCount = 0
class PartitionType(Enum):
	time = 0
	nrreq = 1
		


def load_file(filename, parttType):  
    global page, writeReq, current, maxCount, totalWriteReq
    s = time.time()
    fin = codecs.open(filename, 'r', encoding='utf-8', errors='ignore')          
    lines = fin.readlines()
    e = time.time()
    print("load file consumed ", (e-s), "s")
    s = e
    lineNum = len(lines)
    stage = 0
    # sign = False
    i = 0
    for line in lines:
        i=i+1
        # print(i, readreq, writeReq)
#            if line is null
        if len(line)<=MINLENGH:
            continue
#            get head information
        if i==10:
            task_tail_loc = line.find('-')
            print(task_tail_loc)
            cpu_loc = line.find('CPU')
            print(cpu_loc)
            timestamp_loc = cpu_loc + 10
            function_loc = line.find('FUNCTION')
        if i<=HEADLINENUMBER:
            continue           
#           parse the head
        task = line[0:task_tail_loc].strip()
        pid = int(line[task_tail_loc+1:cpu_loc])
        timestamp = float(line[timestamp_loc:function_loc].replace(':',''))
        # print(task, pid, timestamp)
        line = line[function_loc:]
        colon_loc = line.find(':')
        function_name = line[:colon_loc]
        # print(function_name)
        line = line[colon_loc+1:]
        items = line.split(',')
#           for return sentence
        if len(items)==2:
            continue
        j = 0
        inode = int(items[j])
        j+=1
        if function_name.strip() == READ_FUNCTION_NAME:
            continue
        pos = int(items[j])
        j+=1
        count = int(items[j])
        j+=2
        if count > maxCount:
        	maxCount = count
        filename = items[j]            
        current = timestamp
        # if not sign:
        # 	print(task, pid, timestamp, inode, pos, count, filename)
        # 	sign = True
#           finish parse
        
        
        index_0 = pos >> 12
        index_1 = (pos + count - 1) >> 12
        writeReq += index_1 + 1 - index_0
        totalWriteReq += index_1 + 1 - index_0
        for index in range(index_0, index_1 + 1):     
            if not (inode, index) in page.keys():                
                page[(inode, index)] = 1
            else:
                written = page[(inode, index)]
                page[(inode, index)] = written + 1              
        
        if finishPeriod(parttType, timestamp, stage):
        	stage = store_page(stage)
        	
        
        if i%10000==0:
            e = time.time()
            print(i, int(100*i/lineNum), "%", "consumed", (e-s), "s")            
            s = e

    fin.close()


def finishPeriod(parttType, timestamp, stage):
	global writeReq
	periodCoef = [1, 3, 5, 10]
	if parttType == PartitionType.time:
		return False
	# req mode
	writeStandard = writePeriod * periodCoef[stage]
	if writeReq >= writeStandard:
		if stage == 0 or stage == STAGEMAX:
			writeReq = 0		
		return True
	else:
		return False

def get_sorted_list():
	l = []
	for k,v in page.items():
		l.append((k,v)) 	
	l.sort(key=itemgetter(1), reverse=True)
	return l
		

def compare_two_list(newList, d):
	percent = [0.1, 0.2, 0.5]
	r = []
	for item in percent:
		num = int(item*len(sortedList))
		# print("num = ", num)
		_, req = newList[num-1]
		print(req)
		r.append(compare_two_list_core(sortedList[0:num], req, d))
	return r

def compare_two_list_core(l1, req, d):
	# print(l1)
	i = 0
	for item in l1:
		page, _ = item
		if page in d:
			reqPage = d[page]
			if reqPage >= req:
				i += 1
	return round(i/len(l1)*100, 2)


def store_page(stage):
	global page, sortedList
	# stage can be 0,1,2,3
	# 0 is for observation
	# 1,2,3 are for comparing
	
	if stage == 0:
		sortedList = get_sorted_list()
		page = {}
		return stage+1

	newSortedList = get_sorted_list()
	r = compare_two_list(newSortedList, page)
	record_result(stage, r)

	if stage == STAGEMAX:
		page = {}
		return 0
	else:
		return stage+1

def record_result(stage, r):
	print(stage, r)
	print(stage, r, file=f)

f = open("result.log", "a")

# simple example
# page = {(1,1):1, (1,2):2, (1,3):8, (1,4):9,
# (1,5):3, (1,7):2, (1,6):2, (1,8):3, (1,9):20, (1,10):42}
fn = "nd_filebench5"
print(["*"]*20, file=f)
load_file(fn, PartitionType.nrreq)
f.close()
print(totalWriteReq)