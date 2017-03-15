#include "memoryClass.h"
#include <ext/hash_map>
#include "MemoryController.h"
using namespace __gnu_cxx;
using namespace std;

##ifndef DIRTY_EXPIRE_PERIOD
#define DIRTY_EXPIRE_PERIOD value
#endif

class DRAM : public MemoryDevice
{
public:
	MemoryController * mc;
	DRAM(unsigned int s) {
		MemoryDevice(s);
	};
	void write_back_check(unsigned long time) {
		unsigned long olderThanThis = time - secToJiff(DIRTY_EXPIRE_PERIOD);
		for (doublePageHashMap::iterator iter = pageCacheDirty.begin(); 
                                        iter != pageCacheDirty.end(); ++iter)
		{
			pageHashMap map = iter->second;
    		for (pageHashMap::iterator iter2 = map.begin(); iter2 != map.end(); ++iter2)
    		{
    			if(iter2->second->mytime <= olderThanThis)
    				mc->evict(this, iter2->second);
    		}
		}
	}
	
};