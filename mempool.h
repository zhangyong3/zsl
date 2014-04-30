#ifndef MEM_POOL_H
#define MEM_POOL_H

#include <stdio.h>

namespace zsl {
	class MemPool
	{
	public:
		virtual void *allocate(size_t size) = 0;
		virtual void *reallocate(void *ptr, size_t size, size_t old_size) = 0;
		virtual void free(void *ptr, size_t size) = 0;

	protected:
		inline size_t align(size_t size);
	};


	class SysMemPool : public MemPool
	{
	public:
		void *allocate(size_t size);
		void *reallocate(void *ptr, size_t size, size_t old_size);
		void free(void *ptr, size_t size);
	};


	class NonFreeMemPool : public MemPool
	{
	public:
		NonFreeMemPool(size_t block_size = 4*1024);
		~NonFreeMemPool();

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


	class SlabedMemPool : public MemPool
	{
	public:
		SlabedMemPool(size_t min, size_t max, float factor=1.2);
		~SlabedMemPool();

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
		NonFreeMemPool pool_;
	};

} //end namespace

#endif