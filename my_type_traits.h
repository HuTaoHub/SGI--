#ifndef MY_TYPE_TRAITS_H
#define MY_TYPE_TRAITS_H
namespace pzj
{
	struct __true_type {};		//真
	struct __false_type {};		//假

	//自设计类型：大部分是假的无关紧要的类型（人话：有关紧要）
	template<class type> 
	struct __type_traits
	{
		typedef __true_type  this_dummy_member_must_be_first;
		typedef __false_type  has_trivial_default_constructor;
		typedef __false_type  has_trivial_copy_constructor;
		typedef __false_type  has_trivial_assignment_operator;
		typedef __false_type  has_trivial_destructor;
		typedef __false_type  is_POD_type; // struct int 
	};

	//内置类型：基本上都是真的无关紧要的类型（人话：无关紧要）
	template<> struct __type_traits<char>
	{
		typedef __true_type  has_trivial_default_constructor;
		typedef __true_type  has_trivial_copy_constructor;
		typedef __true_type  has_trivial_assignment_operator;
		typedef __true_type  has_trivial_destructor;
		typedef __true_type  is_POD_type; // struct int 
	};

	template<> struct __type_traits<signed char>
	{
		typedef __true_type  has_trivial_default_constructor;
		typedef __true_type  has_trivial_copy_constructor;
		typedef __true_type  has_trivial_assignment_operator;
		typedef __true_type  has_trivial_destructor;
		typedef __true_type  is_POD_type; // struct int 
	};

	template<> struct __type_traits<unsigned char>
	{
		typedef __true_type  has_trivial_default_constructor;
		typedef __true_type  has_trivial_copy_constructor;
		typedef __true_type  has_trivial_assignment_operator;
		typedef __true_type  has_trivial_destructor;
		typedef __true_type  is_POD_type; // struct int 
	};

	template<> struct __type_traits<short>
	{
		typedef __true_type  has_trivial_default_constructor;
		typedef __true_type  has_trivial_copy_constructor;
		typedef __true_type  has_trivial_assignment_operator;
		typedef __true_type  has_trivial_destructor;
		typedef __true_type  is_POD_type; // struct int 
	};

	template<> struct __type_traits<unsigned short>
	{
		typedef __true_type  has_trivial_default_constructor;
		typedef __true_type  has_trivial_copy_constructor;
		typedef __true_type  has_trivial_assignment_operator;
		typedef __true_type  has_trivial_destructor;
		typedef __true_type  is_POD_type; // struct int 
	};
	template<> struct __type_traits<int>
	{
		typedef __true_type  has_trivial_default_constructor;
		typedef __true_type  has_trivial_copy_constructor;
		typedef __true_type  has_trivial_assignment_operator;
		typedef __true_type  has_trivial_destructor;
		typedef __true_type  is_POD_type; // struct int 
	};
	template<> struct __type_traits<unsigned int>
	{
		typedef __true_type  has_trivial_default_constructor;
		typedef __true_type  has_trivial_copy_constructor;
		typedef __true_type  has_trivial_assignment_operator;
		typedef __true_type  has_trivial_destructor;
		typedef __true_type  is_POD_type; // struct int 
	};
	template<> struct __type_traits<long int>
	{
		typedef __true_type  has_trivial_default_constructor;
		typedef __true_type  has_trivial_copy_constructor;
		typedef __true_type  has_trivial_assignment_operator;
		typedef __true_type  has_trivial_destructor;
		typedef __true_type  is_POD_type; // struct int 
	};
	template<> struct __type_traits<unsigned long int>
	{
		typedef __true_type  has_trivial_default_constructor;
		typedef __true_type  has_trivial_copy_constructor;
		typedef __true_type  has_trivial_assignment_operator;
		typedef __true_type  has_trivial_destructor;
		typedef __true_type  is_POD_type; // struct int 
	};

	template<> struct __type_traits<long long>
	{
		typedef __true_type  has_trivial_default_constructor;
		typedef __true_type  has_trivial_copy_constructor;
		typedef __true_type  has_trivial_assignment_operator;
		typedef __true_type  has_trivial_destructor;
		typedef __true_type  is_POD_type; // struct int 
	};
	template<> struct __type_traits<unsigned long long>
	{
		typedef __true_type  has_trivial_default_constructor;
		typedef __true_type  has_trivial_copy_constructor;
		typedef __true_type  has_trivial_assignment_operator;
		typedef __true_type  has_trivial_destructor;
		typedef __true_type  is_POD_type; // struct int 
	};
	template<> struct __type_traits<float>
	{
		typedef __true_type  has_trivial_default_constructor;
		typedef __true_type  has_trivial_copy_constructor;
		typedef __true_type  has_trivial_assignment_operator;
		typedef __true_type  has_trivial_destructor;
		typedef __true_type  is_POD_type; // struct int 
	};

	template<> struct __type_traits<double>
	{
		typedef __true_type  has_trivial_default_constructor;
		typedef __true_type  has_trivial_copy_constructor;
		typedef __true_type  has_trivial_assignment_operator;
		typedef __true_type  has_trivial_destructor;
		typedef __true_type  is_POD_type; // struct int 
	};

	template<> struct __type_traits<long double>
	{
		typedef __true_type  has_trivial_default_constructor;
		typedef __true_type  has_trivial_copy_constructor;
		typedef __true_type  has_trivial_assignment_operator;
		typedef __true_type  has_trivial_destructor;
		typedef __true_type  is_POD_type; // struct int 
	};
	//所有的指针类型：真的无关紧要
	template<class T>
	struct __type_traits<T*>
	{
		typedef __true_type  has_trivial_default_constructor;
		typedef __true_type  has_trivial_copy_constructor;
		typedef __true_type  has_trivial_assignment_operator;
		typedef __true_type  has_trivial_destructor;
		typedef __true_type  is_POD_type; // struct int 
	};
}
#endif

