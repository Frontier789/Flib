#include <Frontier.hpp>
#include <iostream>

using namespace std;

void func01()
{
	cout << "void func01() called" << endl;
}

int func02()
{
	cout << "void func02() called" << endl;
	return 42;
}

int func03(int a)
{
	cout << "void func03() called w/ a = " << a << endl;
	return a*42;
}

class Cls
{
public:
	int mem;
	Cls(int mem = 0) : mem(mem) {}
	
	int func04(int a)
	{
		cout << "void Cls::func04() called w/ mem=" << mem << " a = " << a << endl;
		return mem+a;
	}
	
	void func05(int a,int b) const
	{
		cout << "void Cls::func04() const called w/ mem=" << mem << " a = " << a << " b = " << b << endl;
	}
	
	Cls *operator()(int k) const
	{
		cout << "Cls::operator() const called w/ mem=" << mem << " k = " << k << endl;
		return new Cls();
	}
};

int main()
{
	cout << "testing some delegate functionalities" << endl;
	
	Delegate<void> d0 = &func01; d0();
	d0 = &func02; d0();
	
	Delegate<int,int> d1 = &func02; cout << "d1(5) returned " << d1(5) << endl;
	d1 = &func03; cout << "d1(5) returned " << d1(5) << endl;
	d1 = &Cls::func04; cout << "d1(5) returned " << d1(5) << endl;
	
	Cls c1(4);
	d1 = Delegate<int,int>(&Cls::func04,&c1); cout << "d1(5) returned " << d1(5) << endl;
	d1 = Delegate<int,int>(&Cls::func04,Cls(42)); cout << "d1(5) returned " << d1(5) << endl;
	
	Delegate<Cls*,int> d2 = Cls(10);
	
	Cls *ptr = d2(42);
	cout << ptr << " was returned by d2(42)" << endl;
	delete ptr;
	
	d2 = &c1;
	ptr = d2(10);
	cout << ptr << " was returned by d2(10)" << endl;
	delete ptr;
	
	const Cls c2(11);
	
	d2 = &c2;
	ptr = d2(18);
	cout << ptr << " was returned by d2(18)" << endl;
	delete ptr;
	
	
	d2 = [](int a) -> Cls* {
		cout << "[](int a) -> Cls* lambda was called" << endl;
		return new Cls(a*2);
	};
	
	delete d2(10);
	
	
	d2 = [&]() -> Cls* {
		cout << "[&](int a) -> Cls* lambda was called and it knows that c1.mem = " << c1.mem << endl;
		return new Cls(50);
	};
	
	delete d2(10);
}