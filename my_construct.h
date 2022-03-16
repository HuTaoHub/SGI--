#ifndef MY_CONSTRUCT_H
#define MY_CONSTRUCT_H
#include"my_iterator.h"
#include"my_type_traits.h"
namespace pzj
{
template<class T1,class T2>
inline void construct(T1* p, const T2& val)
{
	new (p) T1(val);
}

template<class T>
inline void construct(T *p)
{
	new (p) T();
}
template<class T>
inline void destroy(T* p)
{
	p->~T();
}

template<class _FI>
inline void __destroy_aux(_FI _F, _FI _L, pzj::__true_type)
{}

template<class _FI>
inline void __destroy_aux(_FI _F, _FI _L, pzj::__false_type)
{
	for (; _F != _L; ++_F)
	{
		destroy(&*_F);
	}
}
template<class _FI, class T>
inline void __destroy(_FI _F, _FI _L, T*) // value_type;
{
	//cout << typeid(T).name() << endl;
	//pzj::__type_traits<T>();
	typedef typename pzj::__type_traits<T>::has_trivial_destructor dest;
	__destroy_aux(_F, _L, dest());
}
template<class _FI>
inline void destroy(_FI _F, _FI _L)
{
	__destroy(_F, _L, pzj::value_type(_F));
}




}
#endif
