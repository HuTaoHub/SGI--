#ifndef MY_ITERATOR_H
#define MY_ITERATOR_H

namespace pzj
{

typedef int ptrdiff_t;

struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};


template<class _C, class _Ty, class _D = ptrdiff_t, class _Pointer = _Ty*,
	class _Reference = _Ty&>
struct iterator
{
	typedef _C          iterator_category;
	typedef _Ty         value_type;
	typedef _D          difference_type;
	typedef _Pointer    pointer;
	typedef _Reference  reference;
};

template<class _Iterator>
struct iterator_traits
{
	iterator_traits() {}
	typedef typename _Iterator::iterator_category iterator_category;
	typedef typename _Iterator::value_type        value_type;
	typedef typename _Iterator::difference_type   differce_type;
	typedef typename _Iterator::pointer           pointer;
	typedef typename _Iterator::reference         reference;
};
template<class T>
struct iterator_traits<T*>
{
	iterator_traits() {}
	typedef typename random_access_iterator_tag iterator_category;
	typedef typename T                          value_type;
	typedef typename int                        differce_type;
	typedef typename T *                        pointer;
	typedef typename T&                         reference;
};
template<class T>
struct iterator_traits<const T*>
{
	iterator_traits() {}
	typedef typename random_access_iterator_tag iterator_category;
	typedef typename T                          value_type;
	typedef typename int                        differce_type;
	typedef typename const T*                   pointer;
	typedef typename const T&                   reference;
};
/// SGI
template<class _II>
inline typename iterator_traits<_II>::iterator_category
iterator_category(const _II&)
{
	typedef typename iterator_traits<_II>::iterator_category cate;
	return cate();
}

template<class _II>
inline typename iterator_traits<_II>::value_type *
value_type(const _II&)
{
	return  static_cast<typename iterator_traits<_II>::value_type*>(0);
}

template<class _II>
inline typename iterator_traits<_II>::difference_type*
difference_type(const _II&)
{
	return static_cast<typename iterator_traits<_II>::difference_type*> (0);
}

// 正向迭代器
template<class _Ty, class _D = ptrdiff_t>
struct _Forit :public iterator<forward_iterator_tag, _Ty, _D> {};

// 双向迭代器
template<class _Ty,class _D = ptrdiff_t>
struct _Bidit :public iterator<bidirectional_iterator_tag,_Ty,_D>{};

// 随机迭代器
template<class _Ty,class _D = ptrdiff_t>
struct _Ranit :public iterator< random_access_iterator_tag, _Ty, _D> {};



template<class _II,class _D>
inline void __advance(_II& i, _D n, input_iterator_tag)
{
	while (n--)  ++i;
}

template<class _BI, class _D>
inline void __advance(_BI & i, _D n, bidirectional_iterator_tag)
{
	if (n >= 0)
	{
		while (n--) ++i;
	}
	else
	{
		while (n++) --i;
	}
}

template<class _RAI, class _D>
inline void __advance(_RAI& i, _D n, random_access_iterator_tag)
{
	i += n;
}

template<class _II,class _D>
inline void advance(_II& i, _D n)
{
	//iterator_traits<_II>();
	//typedef typename iterator_traits<_II>::iterator_category cate;
	__advance(i, n, iterator_category(i));
}
template<class _II>
inline typename iterator_traits<_II>::difference_type  
__distance(_II _F, _II _L, input_iterator_tag)
{
	typename iterator_traits<_II>::difference_type n = 0;
	while (_F != _L)
	{
		_F++;
		n++;
	}
	return n;
}
template<class _RAI>
inline typename iterator_traits<_RAI>::difference_type
__distance(_RAI _F, _RAI _L, random_access_iterator_tag)
{
	return _L - _F;
}

template<class _II>
inline typename iterator_traits<_II>::difference_type
distance(_II _F, _II _L)
{
	return __disstance(_F, _L, iterator_category(_F));
}

}

#endif

