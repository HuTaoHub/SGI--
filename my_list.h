#include"my_alloc.h"   
#include"my_iterator.h"
#include"my_construct.h" 

#ifndef MY_LIST_H
#define MY_LIST_H

namespace pzj
{
template<class _Ty,class _A = alloc >
class mylist
{

private:
	struct _Node;
	typedef struct _Node *  _Nodeptr;
	struct _Node
	{
		_Nodeptr _Prev,_Next;
		_Ty      _Value;
	};// 

	struct _Acc;
	struct _Acc
	{
		typedef struct _Node *& _Nodepref;
		typedef _Ty &           _Vref;
		static _Vref _Value(_Nodeptr _P)
		{
			return (*_P)._Value;
		}
		static _Nodepref _Prev(_Nodeptr _P)
		{
			return (*_P)._Prev;
		}
		static _Nodepref _Next(_Nodeptr _P)
		{
			return (*_P)._Next;
		}
	};
public:
	typedef _Ty          value_type;
	typedef _Ty &        reference;
	typedef _Ty *        pointer;
	typedef const _Ty &  const_reference;
	typedef const _Ty *  const_pointer;

	typedef _A           allocator_type;

	typedef pzj::simple_alloc<_Node,_A> data_allocate;
public:
	class iterator;
	class const_iterator;
	class const_iterator :  public pzj::_Bidit<_Ty,int>
	{
	public:
		const_iterator(_Nodeptr _P = NULL):_Ptr(_P) {}
		const_reference operator*() const 
		{
			return _Acc::_Value(_Ptr);
		}
		const_pointer operator->() const 
		{
			return (&**this);
		}
		const_iterator operator++()
		{
			_Ptr = _Acc::_Next(_Ptr);
			return *this;
		}
		const_iterator operator++(int)
		{
			const_iterator _Tmp = *this;
			++*this;
			return _Tmp;
		}

		const_iterator operator--()
		{
			_Ptr = _Acc::_Prev(_Ptr);
			return *this;
		}
		const_iterator operator--(int)
		{
			const_iterator _Tmp = *this;
			--*this;
			return _Tmp;
		}

		bool operator==(const const_iterator &_X) const
		{
			return (this->_Ptr == _X._Ptr);
		}
		bool operator!=(const const_iterator &_X) const
		{
			return !(*this == _X);
		}
		_Nodeptr _Mynode() const 
		{
			return _Ptr;
		}
	protected:
		_Nodeptr _Ptr;
	};
	class iterator : public const_iterator
	{
	public:
		iterator(_Nodeptr _P = NULL):const_iterator(_P) {}
		reference operator*() const 
		{
			return _Acc::_Value(this->_Ptr);
		}
		pointer operator->() const 
		{
			return (&**this);
		}
		iterator operator++()
		{
			this->_Ptr = _Acc::_Next(this->_Ptr);
			return *this;
		}
		iterator operator++(int)
		{
			iterator _Tmp = *this;
			++*this;
			return _Tmp;
		}

		iterator operator--()
		{
			this->_Ptr = _Acc::_Prev(this->_Ptr);
			return *this;
		}
		iterator operator--(int)
		{
			iterator _Tmp = *this;
			--*this;
			return _Tmp;
		}

		bool operator==(const iterator &_X) const
		{
			return (this->_Ptr == _X._Ptr);
		}
		bool operator!=(const iterator &_X) const
		{
			return !(*this == _X);
		}
	};
public:
	iterator begin() { return iterator(_Acc::_Next(_Head));}
	iterator end() { return iterator(_Head);}
	const_iterator begin() const { return const_iterator(_Acc::_Next(_Head));}
	const_iterator end() const { return const_iterator(_Head);}


public:
	mylist():_Size(0),_Head(_Buynode()) {}

	~mylist()
	{
		clear();
		_Freenode(_Head);
		_Head = NULL;
	}

	typedef const_iterator _It;

	reference front() { return *begin();}
	const_reference front() const { return *begin();}
	
	reference back() { return *--end();}
	const_reference back() const { return *--end();}

	void push_front(const _Ty _X)
	{
		insert(begin(),_X);
	}
	void push_back(const _Ty &_X)
	{
		insert(end(),_X);
	}
	void insert(iterator _P,size_t _N,const _Ty &_X)
	{
		for(int i = 0;i<_N;++i)
		{
			insert(_P,_X);
		}
	}
	void insert(iterator _P,const _Ty *_F, const _Ty *_L)
	{
		for(; _F != _L; ++_F)
		{
			insert(_P,*_F);
		}
	}
	void insert(iterator _P,_It _F, _It _L)
	{
		for(; _F != _L; ++_F)
		{
			insert(_P,*_F);
		}
	}
	iterator insert(iterator _P,const _Ty &_X)
	{
		_Nodeptr _S = _P._Mynode();
		_Acc::_Prev(_S)  = _Buynode(_Acc::_Prev(_S),_S);
		_S = _Acc::_Prev(_S);
		_Acc::_Next(_Acc::_Prev(_S)) = _S;
		//_Acc::_Value(_S) = _X;
		construct(&_Acc::_Value(_S),_X);
		_Size+=1;
		return iterator(_S);
	}
	void clear()
	{
		erase(begin(),end());
	}
	void pop_back()
	{
		erase(--end());
	}
	void pop_front()
	{
		erase(begin());
	}
	void erase(iterator _F, iterator _L)
	{
		for(; _F != _L; )
		{
			erase(_F++);
		}
	}
	void remove(const _Ty &_X)
	{
		iterator _F = begin(), _L = end();
		for(; _F != _L; )
		{
			if(*_F == _X)
			{
				erase(_F++);
			}
			else
			{
				++_F;
			}
		}
	}
	iterator erase(iterator _P)
	{
		_Nodeptr _S = _P++._Mynode();// 
		_Acc::_Next(_Acc::_Prev(_S)) = _Acc::_Next(_S);
		_Acc::_Prev(_Acc::_Next(_S)) = _Acc::_Prev(_S);
		destroy(&_Acc::_Value(_S));
		_Freenode(_S);
		_Size-=1;
		return _P;// 
	}

private:
	_Nodeptr _Buynode(_Nodeptr _Parg = NULL, _Nodeptr _Narg = NULL)
	{
		//_Nodeptr _S = (_Nodeptr)malloc(sizeof(_Node));
		_Nodeptr _S = data_allocate::allocate(1);// malloc 
		_Acc::_Prev(_S) = _Parg == NULL ? _S:_Parg;
		_Acc::_Next(_S) = _Narg == NULL ? _S:_Narg;
		return _S;
	}
	void _Freenode(_Nodeptr _P)
	{
		data_allocate::deallocate(_P,1);
	}

private:
	_Nodeptr _Head;
	size_t   _Size;
	_A       allocator;
};

}

#endif