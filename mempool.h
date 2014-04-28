#ifndef MEM_POOL_H
#define MEM_POOL_H

#include <stdio.h>

namespace zsl {
	class mempool
	{
	public:
		virtual void *allocate(size_t size) = 0;
		virtual void *reallocate(void *ptr, size_t size, size_t old_size) = 0;
		virtual void free(void *ptr, size_t size) = 0;

	protected:
		inline size_t align(size_t size);
	};


	class sysmempool : public mempool
	{
	public:
		void *allocate(size_t size);
		void *reallocate(void *ptr, size_t size, size_t old_size);
		void free(void *ptr, size_t size);
	};


	class nonfreemempool : public mempool
	{
	public:
		nonfreemempool(size_t block_size = 4*1024);
		~nonfreemempool();

		void *allocate(size_t size);
		void *reallocate(void *ptr, size_t size, size_t old_size);
		void free(void *ptr, size_t size);

	protected:
		struct memnode {
			size_t size;
			size_t cur;
			struct memnode *next;
		};

		struct memnode *newblock(size_t size);
		size_t block_size_;
		struct memnode *head_;
	};


	class slabedmempool : public mempool
	{
	public:
		slabedmempool(size_t min, size_t max, float factor=1.2);
		~slabedmempool();

		void *allocate(size_t size);
		void *reallocate(void *ptr, size_t size, size_t old_size);
		void free(void *ptr, size_t size);

	protected:
		int classid(size_t size);

	protected:
		struct slab {
			struct slab *next;
		};

		struct classz {
			size_t size;
			struct slab *slabs;
		};

		struct classz *clzs_;
		size_t size_;
		nonfreemempool pool_;
	};

} //end namespace

#endif