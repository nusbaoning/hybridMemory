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
	hash_map<unsigned long, hash_map<unsigned long, page_descriptor> > pageCache;
	unsigned int size;

public:
	MemoryDevice(unsigned int s) {
		size = s;
	}

	void show_page_cache_size() {
		cout << "The size of page cache is\t" << size << endl;
	}
	page_descriptor * find_get_page(unsigned long inode, unsigned long index);
	page_descriptor * add_to_page_cache(unsigned long inode, unsigned long index, 
	bool dirty, unsigned long time);
	void remove_from_page_cache(page_descriptor * p); 
	void set_page_dirty(page_descriptor * page, bool dirty, unsigned long time);
};


