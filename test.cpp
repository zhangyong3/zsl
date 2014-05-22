#include <stdio.h>
#include "vector.h"
#include "mempool.h"
#include "string.h"
#include "rbtree.h"
#include "set.h"
#include "list.h"
#include "map.h"
#include "stack.h"
#include "variant.h"
#include <vector>
#include <deque>
#include <string>
#include <iostream>
#include <set>
#include <map>

class A
{
public:
	A()
	{
	}

	A(int a):
		a_(a)
	{
		printf("con(%d)\n", a);
	}

	A(const A&a) {
		a_ = a.a_;
		printf("cpcon(%d)\n", a_);
	}

	~A()
	{
		printf("discon(%d)\n", a_);
	}

	bool operator< (const A &a) const
	{
		return a_ < a.a_;
	}
	A& operator= (const A &a)
	{
		printf("=\n");
		return *this;
	}

	int a_;
};

int main()
{
	{
/*
		std::map<int, A&> map;
		A aa;
		map.insert(std::make_pair(100, aa));
*/
		zsl::SlabedMemPool pool(4, 1024);
		zsl::VarFactory factory(pool);
		zsl::DictVariant *dict = factory.createDict();
		zsl::ArrayVariant *array = factory.createArray();
		array->push_back(factory.create(1));
		array->push_back(factory.create(2));
		array->push_back(factory.create(3));

		dict->data()["hello"] = factory.create(100);
		dict->data()["array"] = array;

		zsl::String str = dict->toString();
		std::cout << str.c_str() << std::endl;

/*
		zsl::DictVariant a;
		zsl::IntVariant x(100);

		zsl::ArrayVariant arr;
		zsl::IntVariant arr1(1);
		zsl::IntVariant arr2(20);

		arr.push_back(&arr1);
		arr.push_back(&arr2);

		a["hello"] = &x;
		a["arr"] = &arr;

		zsl::String str = a.toString();
		std::cout << str.c_str() << std::endl;
*/
	}
/*
	{
		zsl::Stack<int, std::deque<int> > stack;
		for (int i = 0; i < 10; ++i) {
			stack.push(i);
		}

		while (!stack.empty()) {
			std::cout << stack.top() << std::endl;
			stack.pop();
		}
	}
*/
/*
	{
		zsl::Map<int, std::string> map;
		map.insert(std::make_pair(100, "sadfasdf"));
		map[1] = "hello";
		map[10] = "world";

		for (zsl::Map<int, std::string>::Iterator it = map.begin(); it != map.end(); ++it) {
			std::cout << it->first <<", " << it->second << std::endl;
		}
	}
*/

/*
	{
		zsl::Vector<int> a;
		for (int i = 0; i < 10; ++i) {
			a.push_back(i);
		}

		a.push_front(-1);

		a.insert(1, 100);


		a.erase(a.begin()+5);


		for (zsl::Vector<int>::Iterator it = a.begin(); it != a.end(); ++it) {
			std::cout << *it << std::endl;
		}
	}
*/
/*
	{
		zsl::Set<int> a;
		for (int i = 0; i < 10; ++i) {
			a.insert(i);
		}

		zsl::Set<int>::Iterator it = a.find(8);
		if (it != a.end() ) {
			std::cout << "found:" << *it << std::endl;
			a.erase(it);
		}

		for (it = a.rbegin(); it != a.rend(); ++it) {
			std::cout << *it << std::endl;
		}
	}
*/

/*
	{
		zsl::Set<A> a;
		//std::set<A> a;
		for (int i = 0; i < 4; ++i) {
			a.insert(A(i));
		}

		auto it =  a.find(1);
		if (it != a.end()) {
			a.erase(it);
		}
	}
*/


/*
	{
		zsl::List<int> l;
		for (int i = 0; i < 10; ++i) {
			l.push_back(i);
		}
		
		l.insert(++l.begin(), 100);

		for (zsl::List<int>::Iterator it = l.rbegin(); it != l.rend(); ++it) {
			std::cout << *it << std::endl;
		}
	}
*/

	//zsl::Set<std::string> b;
	//zsl::Set<A> c;

/*
	//zsl::NonFreeMemPool mp;
	zsl::SlabedMemPool mp(4, 128, 1.2);
	zsl::String s(mp);

	s = "hel";
	s += "lo world";

	printf("%s\n", s.c_str());
*/
/*
	zsl::Vector<A> v(mp);


	v.push_back(A(10));
	v.push_back(A(20));

	zsl::Vector<A> v2;
	v2 = v;
	for (int i = 0; i < v2.size(); ++i) {
		printf("%d\n", v2[i].a_);
	}
*/


/*
	std::vector<A> v;

	v.resize(2);
	v[0] = A(10);
	v[1] = A(20);
*/

}
