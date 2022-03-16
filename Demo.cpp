#include"my_list.h"
#include"my_alloc.h"
#include "my_construct.h"
#include <iostream>
using namespace std;

int main()
{
	pzj::mylist<int> ilist;
	ilist.push_back(1);

	cout << *ilist.begin() << endl;
	return 0;
}