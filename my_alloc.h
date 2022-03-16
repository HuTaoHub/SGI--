#ifndef MY_ALLOC_H
#define MY_ALLOC_H
#include<iostream>

namespace pzj
{
#if 0
#include<new>
#define __THROW_BAD_ALLOC throw std::bad_alloc;
#elif !defined(__THROW_BAD_ALLOC)
#define __THROW_BAD_ALLOC std::cerr<<"out of memory"<<std::endl; exit(1)
#endif

template<int inst>
class __malloc_alloc_template
{
public:
	using PFUN = void (*)();
private:
	static void* oom_malloc(size_t n)
	{
		void* result = NULL;
		void (*my_malloc_handler) () = nullptr;
		while(1)
		{
			my_malloc_handler = __malloc_alloc_oom_handler;
			if (nullptr == my_malloc_handler)
			{
				__THROW_BAD_ALLOC;
			}
			my_malloc_handler();
			result = malloc(n);
			if (nullptr != result)
			{
				return result;
			}
		}
	}
	static void* oom_realloc(void* p, size_t new_sz)
	{
		void* result = NULL;
		void (*my_malloc_handler) () = nullptr;
		for (;;)
		{
			my_malloc_handler = __malloc_alloc_oom_handler;
			if (nullptr == my_malloc_handler)
			{
				__THROW_BAD_ALLOC;
			}
			my_malloc_handler();
			result = realloc(p, new_sz);
			if (nullptr != result)
			{
				return result;
			}
		}
	}
	static PFUN __malloc_alloc_oom_handler;
public:
	static void* allocate(size_t n)
	{
		void* result = malloc(n);
		if (nullptr == result)
		{
			result = oom_malloc(n);
		}
		return result;
	}
	static void deallocate(void* p, size_t n)
	{
		free(p);
	}
	static void* reallocate(void* p, size_t old_sz, size_t new_sz)
	{
		void* result = realloc(p, new_sz);
		if (nullptr == result)
		{
			result = oom_realloc(p, new_sz);
		}
		return result;
	}

	static  PFUN set_malloc_handler(PFUN p)
	{
		PFUN old = __malloc_alloc_oom_handler;
		__malloc_alloc_oom_handler = p;
		return old;
	}
};
template<int inst>
typename __malloc_alloc_template<inst>::PFUN
__malloc_alloc_template<inst>::__malloc_alloc_oom_handler = nullptr;

//typedef __malloc_alloc_template<0> malloc_alloc;
using malloc_alloc = __malloc_alloc_template<0>;

enum { __ALIGN = 8 };
enum { __MAX_BYTES = 128 };
enum { __NFREELISTS = __MAX_BYTES / __ALIGN }; // 16

template<bool threads,int inst>
class __default_alloc_template
{
private:
	union obj
	{
		union obj* free_list_link; // next;
		//char client_data[1];
	};
private:
	static obj* volatile free_list[__NFREELISTS];
	static char* start_free;
	static char* end_free;
	static size_t heap_size; // total
	static size_t ROUND_UP(size_t bytes)
	{
		return (bytes + __ALIGN - 1) & ~(__ALIGN - 1);
	}
	static size_t FREELIST_INDEX(size_t bytes)
	{
		return (bytes + __ALIGN - 1) / __ALIGN - 1;
	}

	static char* chunk_alloc(size_t size, int& nobjs)
	{ //                           
		char* result = NULL;
		size_t total_bytes = size * nobjs; 
		size_t bytes_left = end_free - start_free; 
		if (bytes_left >= total_bytes) //
		{
			result = start_free;
			start_free = start_free + total_bytes;
			return result;
		}
		else if (bytes_left >= size) // >= 1 
		{
			nobjs = bytes_left / size; // 
			total_bytes = size * nobjs; //
			result = start_free;
			start_free = start_free + total_bytes;
			return result;
		}
		else
		{
			size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
			if (bytes_left > 0)
			{
				obj* volatile* my_free_list = free_list + FREELIST_INDEX(bytes_left);
				((obj*)start_free)->free_list_link = *my_free_list;
				*my_free_list = (obj*)start_free;
			}
			start_free = (char*)malloc(bytes_to_get);//system heap;
			if (NULL == start_free)
			{
				obj* volatile* my_free_list = NULL;
				obj* p = NULL;
				for (int i = size; i <= __MAX_BYTES; i += __ALIGN)
				{
					my_free_list = free_list + FREELIST_INDEX(i);
					p = *my_free_list;
					if (NULL != p)
					{
						*my_free_list = p->free_list_link;
						start_free = (char*)p;
						end_free = start_free + i;
						return chunk_alloc(size, nobjs);
					}
				}
				start_free = (char*)malloc_alloc::allocate(bytes_to_get);
			}
			end_free = start_free + bytes_to_get;
			heap_size += bytes_to_get;
			return chunk_alloc(size, nobjs);
		}
	}
	static void* refill(size_t size) // 
	{
		int nobjs = 20; 
		char* chunk = chunk_alloc(size, nobjs); // 
		if (1 == nobjs)
		{
			return chunk;
		}
		obj* volatile* my_free_list = NULL;
		obj* result = (obj*)chunk;
		obj* current_obj = NULL ,* next_obj = NULL;
		int i = 0;
		my_free_list = free_list + FREELIST_INDEX(size);
		*my_free_list = next_obj = (obj*)(chunk + size);
		for (i = 1; ; ++i)
		{
			current_obj = next_obj;
			next_obj = (obj*)((char*)next_obj + size);
			if (i == nobjs - 1)
			{
				current_obj->free_list_link = NULL;
				break;
			}
			current_obj->free_list_link = next_obj;
		}
		return result;
	}
public:
	//申请空间
	static void* allocate(size_t size)
	{
		//size > 128 调用一级空间适配器
		if (size > (size_t)__MAX_BYTES)
		{
			return malloc_alloc::allocate(size);
		}
		obj* result = nullptr;
		obj* volatile* my_free_list = nullptr; //
		my_free_list = free_list + FREELIST_INDEX(size);
		result = *my_free_list;
		if (nullptr == result)
		{
			void* r = refill(ROUND_UP(size)); // size = 6; 8 ; size = 10; 16 
			return r;
		}
		*my_free_list = result->free_list_link;
		return result;
	}
	static void deallocate(void* p, size_t n)
	{
		if (n > (size_t)__MAX_BYTES)
		{
			malloc_alloc::deallocate(p, n);
			return;
		}
		obj* q = (obj*)p;
		obj* volatile* my_free_list = free_list +  FREELIST_INDEX(n);
		q->free_list_link = *my_free_list;
		*my_free_list = q;
		return;
	}
	static void* reallocate(void* p, size_t old_sz, size_t new_sz)
	{
		if (old_sz > (size_t)__MAX_BYTES && new_sz > (size_t)__MAX_BYTES)
		{
			return malloc_alloc::reallocate(p, old_sz, new_sz);
		}
		if (ROUND_UP(old_sz) == ROUND_UP(new_sz))
		{
			return p;
		}
		size_t sz = old_sz < new_sz ? old_sz : new_sz;
		void* s = allocate(new_sz);
		memmove(s, p, sz);
		deallocate(p, old_sz);
		return s;
	}
};


template<bool threads, int inst>
typename __default_alloc_template<threads, inst>::obj* volatile
__default_alloc_template<threads, inst>::free_list[__NFREELISTS] = {};

template<bool threads, int inst>
char* __default_alloc_template<threads, inst>::start_free = nullptr;

template<bool threads, int inst>
char* __default_alloc_template<threads, inst>::end_free = nullptr;

template<bool threads, int inst>
size_t __default_alloc_template<threads, inst>::heap_size = 0;

///////////////////////////////////////////
// SGI STL
#ifdef __USE_MALLOC
typedef __malloc_alloc_template<0> malloc_alloc;
typedef malloc_alloc alloc;
#else
typedef __default_alloc_template<0, 0> alloc;
#endif

template<class T,class Alloc>
class simple_alloc
{
public:
	static T* allocate(size_t n) // n  T 类型 operator new []
	{
		return (T*)Alloc::allocate(sizeof(T) * n); // 
	}
	static T* allocate() //   1 T
	{
		return (T*)Alloc::allocate(sizeof(T));    // operator new 
	}
	static void deallocate(T* p, size_t n) // n T 
	{
		if (NULL == p) return;
		Alloc::deallocate(p, sizeof(T) * n);
	}
	static void deallocate(T* p)
	{
		if (NULL == p) return;
		Alloc::deallocate(p, sizeof(T));
	}
};

}
#endif

