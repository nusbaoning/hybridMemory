#include <iostream>
#include <ext/hash_map>
using namespace __gnu_cxx;
using namespace std;

typedef struct {
	bool dirty;
	unsigned long time;
	// unsigned long index;
	// unsigned long inode;
}page_descriptor;

class MemoryDevice {
private:
	hash_map<unsigned long, hash_map<unsigned long, page_descriptor> > pageCacheDirty;
	hash_map<unsigned long, hash_map<unsigned long, page_descriptor> > pageCacheClean;
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
		function : search the page in page cache
		return the page address if found, otherwise return NULL
	*/
	page_descriptor * find_get_page(unsigned long inode, unsigned long index);

	/*
		input : the inode and index of the page, the dirty sign and the time of the request
		function : add the page into page cache, change the page number
	*/
	page_descriptor * add_to_page_cache(unsigned long inode, unsigned long index, 
	bool dirty, unsigned long time);

	void remove_from_page_cache(page_descriptor * p); 

	void set_page_dirty(page_descriptor * page, bool dirty, unsigned long time);

};


