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
//     cout<<"The mytime for storing 1,000,000  is "<<duration<<endl;

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

page_descriptor * __find_get_page(doublePageHashMap map, unsigned long inode, unsigned long index) {
	if (map.find(inode) == map.end())
	{
		return NULL;
	}
	pageHashMap pm = map[inode];
	if (pm.find(index) == pm.end())
	{
		return NULL;
	}
	return pm[index];
}

page_descriptor * MemoryDevice::find_get_page(unsigned long inode, unsigned long index) {
	page_descriptor * result = __find_get_page(pageCacheDirty, inode, index);
	if (result != NULL)
	{
		return result;
	}
	result = __find_get_page(pageCacheClean, inode, index);
	return result;
}

void __add_to_page_cache(doublePageHashMap * map, 
	page_descriptor * p, unsigned long inode, unsigned long index) {
	if (map->find(inode) == map->end())
	{
		pageHashMap *map[inode];
	}
	(*map)[inode][index] = p;
}

page_descriptor * MemoryDevice::add_to_page_cache(unsigned long inode, unsigned long index, 
	bool dirty, unsigned long mytime) {
	// cout << "test\t" << inode << "\t" << index << "\t" << dirty << "\t" << mytime << endl;
	if (allocatedPageNumber < size)
	{
		allocatedPageNumber++;

		page_descriptor * p = new page_descriptor();
		p->dirty = dirty;
		p->mytime = mytime;
		p->inode = inode;
		p->index = index;
		if (dirty)
		{
			__add_to_page_cache(&pageCacheDirty, p, inode, index);
		}
		else
			__add_to_page_cache(&pageCacheClean, p, inode, index);
	}
	else {
		shrink_cache();
		add_to_page_cache(inode, index, dirty, mytime);
	}
}


void MemoryDevice::remove_from_page_cache(page_descriptor * p) {
	if (p == NULL)
	{
		cout << "p == null" << endl;
		return;
	}
	// cout << p->inode << "\t" << p->index << "\t" << p->dirty << endl;
	doublePageHashMap * map;
	if (p->dirty)
	{
		map = &pageCacheDirty;
		// pageCacheDirty[p->inode].erase(p->index);
	}
	else {
		map = &pageCacheClean;
		// pageCacheClean[p->inode].erase(p->index);
	}

	(*map)[p->inode].erase(p->index);
	if ((*map)[p->inode].size() == 0)
	{
		map->erase(p->inode);
	}

	allocatedPageNumber--;
	cout << "erase finished" << endl;
	delete p;
}

void MemoryDevice::set_page_dirty(page_descriptor * page, bool dirty, unsigned long mytime) {
	unsigned long inode = page->inode;
	unsigned long index = page->index;
	remove_from_page_cache(page);	
	add_to_page_cache(inode, index, dirty, mytime); 
}

int main() {
	MemoryDevice m(500);
	m.show_page_cache_statics();
	m.add_to_page_cache(0, 0, true, 0);
	m.add_to_page_cache(0, 1, true, 0);
	m.add_to_page_cache(2, 0, true, 0);
	m.add_to_page_cache(2, 1, false, 0);
	m.show_page_cache_statics();
	m.show_page_cache_pages();
	cout << "remove 1" << endl;
	m.remove_from_page_cache(m.find_get_page(0,0));
	cout << "remove 2" << endl;
	m.remove_from_page_cache(m.find_get_page(0,1));
	cout << "remove 3" << endl;
	m.remove_from_page_cache(m.find_get_page(2,0));
	cout << "remove 4" << endl;
	m.remove_from_page_cache(m.find_get_page(2,1));
	m.show_page_cache_statics();
	m.show_page_cache_pages();
}


