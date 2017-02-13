#include <iostream>
#include <string>
// #include <hash_map>
#include <ext/hash_map>
#include <ctime>
#include "memoryClass.h"
using namespace __gnu_cxx;
using namespace std;





// void test_hash_map(){
// 	unsigned int sum = 0;
//     hash_map<unsigned int, unsigned int> mymap;
//     clock_t start;
//     double duration;

//     start = clock();

       

//     for (int i = 0; i < 1000000; ++i)
//     {
//     	mymap[i] = i;
//     }
    
//     duration = (clock() - start) / (double) CLOCKS_PER_SEC;
//     cout<<"The time for storing 1,000,000  is "<<duration<<endl;

//     cout << "The size of the hash map is " << mymap.size() << endl;

//     mymap.erase(0);

// 	cout << "The size of the hash map is " << mymap.size() << endl;

// 	if (mymap.find(0) != mymap.end())
// 	{
// 		cout << mymap[0] << endl;
// 	}
// 	else {
// 		cout << "erase is successful" << endl;
// 	}


// }

page_descriptor * MemoryDevice::find_get_page(unsigned long inode, unsigned long index) {
	hash_map<unsigned long, page_descriptor> inodeMap;
	if (pageCacheDirty.find(inode) != pageCacheDirty.end())
		inodeMap = pageCacheDirty[inode];
	else if(pageCacheClean.find(inode) != pageCacheClean.end())
		inodeMap = pageCacheClean[inode];
	else
		return NULL;
	if (inodeMap.find(index) == inodeMap.end())
	{
		return NULL; 
	}
	return &inodeMap[index];
}


page_descriptor * MemoryDevice::add_to_page_cache(unsigned long inode, unsigned long index, 
	bool dirty, unsigned long time) {
	if (allocatedPageNumber < size)
	{
		allocatedPageNumber++;
	}
	return NULL;
}

int main() {
	MemoryDevice m(500);
	m.show_page_cache_statics();
	m.add_to_page_cache(0, 0, true, 0);
	m.show_page_cache_statics();
}


