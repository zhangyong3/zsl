#include "mempool.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>

namespace zsl {

inline size_t MemPool::align(size_t size)
{
	return (size & 3) ? ((size|3) + 1) : size;
}

void *SysMemPool::allocate(size_t size)
{
	return ::malloc(size);
}

void *SysMemPool::reallocate(void *ptr, size_t size, size_t)
{
	return ::realloc(ptr, size);
}

void SysMemPool::free(void *ptr, size_t size)
{
	return ::free(ptr);
}


//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//class NonFreeMemPool
#define MIN_BLOCK_SIZE (4*1024)
NonFreeMemPool::NonFreeMemPool(size_t block_size)
	: block_size_(block_size), head_(NULL)
{
	if (block_size_ < MIN_BLOCK_SIZE) {
		block_size_ = MIN_BLOCK_SIZE;
	}

	head_ = newblock(block_size_);
}

NonFreeMemPool::~NonFreeMemPool()
{
	struct memnode *p = head_;
	while (p) {
		struct memnode *q = p->next;
		::free(p);
		p = q;
	}

}

void *NonFreeMemPool::allocate(size_t size)
{
	if (size > head_->size) {
		struct memnode *p = newblock(size+sizeof(struct memnode));
		p->next = head_->next;
		head_->next = p;

		return (char*)(p+1);
	}

	if (head_->cur + size > head_->size) {
		struct memnode *p = newblock(block_size_);
		p->next = head_;
		head_ = p;
	}

	int cur = head_->cur;
	char *data = (char *)(head_+1);
	head_->cur+= size;
	return data+cur;
}

void *NonFreeMemPool::reallocate(void *ptr, size_t size, size_t old_size)
{
	if (size <= old_size)
		return ptr;

	if (ptr != NULL &&  ((char*)ptr)+old_size == ((char*)(head_+1))+head_->cur) {
		size_t delta = size - old_size;
		if (head_->cur + delta <= head_->size) {
			head_->cur += delta;
			return ptr;
		}
	}

	void *p = allocate(size);
	if (ptr != NULL)  {
		memcpy(p, ptr, old_size);
	}
	return p;
}

void NonFreeMemPool::free(void *ptr, size_t size)
{
}

struct NonFreeMemPool::memnode *NonFreeMemPool::newblock(size_t size)
{
	struct memnode *node;
	if (size < MIN_BLOCK_SIZE) {
		size = MIN_BLOCK_SIZE;
	}

	node = (struct memnode*)::malloc(size);
	node->size = size - sizeof(struct memnode);
	node->cur = 0;
	node->next = NULL;
	return node;
}


SlabedMemPool::SlabedMemPool(size_t min, size_t max, float factor)
	: clzs_(NULL), size_(0)
{
	size_t cur = min;
	size_t prev = 0;
	while (true) {
		cur = align(cur);
		if (cur != prev) {
			prev = cur;
			clzs_ = (struct classz *)pool_.reallocate(clzs_, sizeof(struct classz)*(size_+1), sizeof(struct classz)*size_);
			clzs_[size_].size = cur;
			clzs_[size_].slabs = NULL;
			size_ += 1;
		}

		if (cur > max)
			break;

		cur = ceil(cur*factor);
	}
}

SlabedMemPool::~SlabedMemPool()
{
}

void *SlabedMemPool::allocate(size_t size)
{
	int clzid = classid(size);
	if (clzid < 0) {
		return ::malloc(align(size));
	}

	if (clzs_[clzid].slabs != NULL) {
		struct slab *head = clzs_[clzid].slabs;
		clzs_[clzid].slabs = head->next;

		return head+1;
	} else {
        size_t new_size = sizeof(struct slab)+align(size);
		struct slab * blk = (struct slab *)pool_.allocate(new_size);
		return blk+1;
	}
}

void *SlabedMemPool::reallocate(void *ptr, size_t size, size_t old_size)
{
	void *p = allocate(size);
    if (ptr != NULL && old_size > 0) {
        if (p != NULL) {
	        memcpy(p, ptr, old_size);
        }
	    free(ptr, old_size);
    }

    return p;
}

void SlabedMemPool::free(void *ptr, size_t size)
{
	int clzid = classid(size);
	if (clzid < 0) {
		return ::free(ptr);
	}
	struct slab * blk = ((struct slab *)ptr)-1;
	blk->next = clzs_[clzid].slabs;
	clzs_[clzid].slabs = blk;
}

int SlabedMemPool::classid(size_t size)
{
	size = align(size);
	int min = 0, max = (int)size_-1;
	while (min <= max) {
		int mid = (min+max)/2;
		if (size > clzs_[mid].size) {
			min = mid+1;
		} else if (size < clzs_[mid].size) {
			max = mid-1;
		} else {
			return mid;
		}
	}
	return -1;
}


} //end of namespace
