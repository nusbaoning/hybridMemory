#include <iostream>
#include <ext/hash_map>
using namespace __gnu_cxx;
using namespace std;

typedef struct {
	bool dirty;
	unsigned long mytime;
	unsigned long index;
	unsigned long inode;
}page_descriptor;

typedef hash_map<unsigned long, page_descriptor*> pageHashMap;
typedef hash_map<unsigned long, pageHashMap> doublePageHashMap; 

class MemoryDevice {
private:
	doublePageHashMap pageCacheDirty;
	doublePageHashMap pageCacheClean;
	unsigned int size;
	unsigned int allocatedPageNumber;

public:
	MemoryDevice(unsigned int s) {
		size = s;
	}

	void show_page_cache_statics() {
		cout << "The size of page cache is\t" << size << endl;
		cout << "The allocated page number of page cache is\t" << allocatedPageNumber << endl;
	}

	/*
		Note : Only call for small functions
		output: All the pages with their information, each page one line
	*/
	void show_page_cache_pages() {
		cout << "Dirty" << endl;
		for (doublePageHashMap::iterator iter = pageCacheDirty.begin(); 
                                        iter != pageCacheDirty.end(); ++iter)
		{
			cout << "Key" << iter->first << endl;
    		pageHashMap map = iter->second;
    		for (pageHashMap::iterator iter2 = map.begin(); iter2 != map.end(); ++iter2)
    		{
    			cout << iter -> first << "\t" << iter2->first << "\t" << iter2->second->mytime << endl;
    		}
		}

		cout << "Clean" << endl;
		for (doublePageHashMap::iterator iter = pageCacheClean.begin(); 
                                        iter != pageCacheDirty.end(); ++iter)
		{
			cout << "Key" << iter->first << endl;
    		pageHashMap map = iter->second;
    		for (pageHashMap::iterator iter2 = map.begin(); iter2 != map.end(); ++iter2)
    		{
    			cout << iter -> first << "\t" << iter2->first << "\t" << iter2->second->mytime << endl;
    		}
		}
	}

	/*
		function : search the page in page cache
		return the page address if found, otherwise return NULL
	*/
	page_descriptor * find_get_page(unsigned long inode, unsigned long index);

	/*
		input : the inode and index of the page, the dirty sign and the mytime of the request
		function : add the page into page cache, change the page number
		note : only add, no other checks, do not remove the existed record if it is in another map
	*/
	page_descriptor * add_to_page_cache(unsigned long inode, unsigned long index, 
	bool dirty, unsigned long mytime);

	/*
		input : the pointer of the page, basically come from find_get_page
		function : free the space of the page_descriptor and delete the item from hash_map
	*/
	void remove_from_page_cache(page_descriptor * p); 

	/*
		function : set the page as dirty with time
	*/
	void set_page_dirty(page_descriptor * page, bool dirty, unsigned long mytime);

	virtual void shrink_cache();

};

void MemoryDevice::shrink_cache() {
	return;
}

