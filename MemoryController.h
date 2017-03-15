#include "memoryClass.h"

##ifndef DISK_UPDATE
#define DISK_UPDATE 500
#endif

struct MemoryController
{
private:
	MemoryDevice dev1;
	MemoryDevice dev2;

public:
	MemoryController(MemoryDevice d1, MemoryDevice d2) {
		dev1 = d1;
		dev2 = d2;
	}

	unsigned long request_access(unsigned long inode, unsigned long index);

	unsigned long move(MemoryDevice des, MemoryDevice src, page_descriptor * p) {		
		des.add_to_page_cache(p->inode, p->index, p->dirty, p->mytime);
		src.remove_from_page_cache(p);		
		return 0;
	}	

	unsigned long evict(MemoryDevice* src, page_descriptor *p) {
		src->remove_from_page_cache(p);
		return DISK_UPDATE;
	}
};

