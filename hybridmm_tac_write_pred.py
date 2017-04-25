# -*- coding: utf-8 -*-
import codecs
import time
import sys
from operator import itemgetter

HEADLINENUMBER = 11
MINLENGH = 10

page = {}  
writePeriod = 100000
writeReq = 0
sortedList = []
class PartitionType(Enum):
	time = 0
	nrreq = 1
		


def load_file(filename, parttType):  
    global page, writeReq
    s = time.time()
    fin = codecs.open(filename, 'r', encoding='utf-8', errors='ignore')          
    lines = fin.readlines()
    e = time.time()
    print("load file consumed ", (e-s), "s")
    s = e
    lineNum = len(lines)
    stage = 0
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
        filename = items[j]            
        current = timestamp

#           finish parse
        
        
        index_0 = pos >> 12
        index_1 = (pos + count - 1) >> 12
        writeReq += index_1 + 1 - index_0

        for index in range(index_0, index_1 + 1):     
            if not (inode, index) in page.keys():                
                page[(inode, index)] = 1
            else:
                written = page[(inode, index)]
                page[(inode, index)] = written + 1              
        
        if finishPeriod(parttType, timestamp):
        	stage = store_page(stage)
        	
        
        if i%10000==0:
            e = time.time()
            print(i, int(100*i/lineNum), "%", "consumed", (e-s), "s")            
            s = e

    fin.close()


def finishPeriod(parttType, timestamp):
	global writeReq

	if parttType == PartitionType.time:
		return False

	if writeReq >= writePeriod:
		return True
	else:
		return False

def get_sorted_list():
	l = (list)page.items()
	l.sort(key=itemgetter(1), reverse=True)
	return l
		

def compare_two_list(newList):
	percent = 


def store_page(stage):
	global page, sortedList
	# stage can be 0,1,2,3
	# 0 is for observation
	# 1,2,3 are for comparing
	stageMax = 3
	if stage == 0:
		sortedList = get_sorted_list()
		page = {}
		return stage+1

	newSortedList = get_sorted_list()
	compare_two_list(newSortedList)

	if stage == stageMax:
		page = {}
		return 0
	else:
		return stage+1
