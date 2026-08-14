#ifndef CACHEMEMORYLIST_H
#define CACHEMEMORYLIST_H
#include "cacheBlock.h"
#define MAXBLOCKS 1024
class cacheMem
{
	public:
		int numberCache = 0;
		int nWay = 0;
		int cacheModIndex = 0;
		cacheBlock *blocks = nullptr;
		cacheMem() : numberCache(0), nWay(0), cacheModIndex(0), blocks(nullptr) {}
		void alocate_blocks(int index_size);
		cacheMem(int n) : numberCache(0), nWay(n), cacheModIndex(0), blocks(nullptr) {}
		bool validate(uint32_t, int);
		bool checkValidation(uint32_t, int);
		bool invalidate(uint32_t, int);
		bool readSetState(uint32_t, int);
		void writeSetState(uint32_t, int);

		virtual ~cacheMem();
	protected:
	private:
};

#endif // CACHEMEMORYLIST_H
