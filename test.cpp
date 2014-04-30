#include <stdio.h>
#include "vector.h"
#include "mempool.h"
#include "string.h"
#include <vector>

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

	int a_;
};

int main()
{
	//zsl::NonFreeMemPool mp;
	zsl::SlabedMemPool mp(4, 128, 1.2);
	zsl::String s(mp);

	s = "hel";
	s += "lo world";

	printf("%s\n", s.c_str());
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
