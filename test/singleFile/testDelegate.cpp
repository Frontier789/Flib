#include <Frontier.hpp>
#include <iostream>

using namespace std;

int indicator;

void fun_one() {
	indicator = 1;
}

int int_fun() {
	indicator = 2;
	return 42;
}

int par_fun(int a) {
	indicator = a;
	return a;
}

bool success = true;

string check(bool cond) {
	if (cond)
		return "true\n";
	
	success = false;
	return "false\n";
}

struct Functor {
	int val;
	
	Functor() : val(0) {}
	Functor(const Functor &cpy) : val(cpy.val+1) {}
	
	int operator()() {return val++;}
};

struct CnstFunctor {
	int val;
	
	CnstFunctor() : val(0) {}
	CnstFunctor(const CnstFunctor &cpy) : val(cpy.val+1) {}
	
	int operator()() const {return val;}
};

struct Ordinary
{
	int a;
	Ordinary(int a = 2) : a(a) {}
	int get() {return a;}
	int cget() const {return a;}
};

int main()
{
	cout << "Testing some delegate functionalities" << endl;
	
	fm::Delegate<void> d0;
	fm::Delegate<int,int> d1;
	fm::Delegate<int> d2;
	const CnstFunctor ccfunctor;
	CnstFunctor cfunctor;
	Functor functor;
	const Ordinary cordi{7};
	Ordinary ordi{5};
	int iret;
	int five = 5;
	
	cout << "Assigning void()() to Delegate<void> and calling: ";
	d0 = fun_one;
	d0();
	cout << check(indicator == 1);
	indicator = 0;
	
	cout << "Assigning void(*)() to Delegate<void> and calling: ";
	d0 = &fun_one;
	d0();
	cout << check(indicator == 1);
	indicator = 0;
	
	cout << "Assigning int()() to Delegate<void> (discarding return value) and calling: ";
	d0 = &int_fun;
	d0();
	cout << check(indicator == 2);
	indicator = 0;
	
	cout << "Assigning int()(int) to Delegate<int,int> and calling: ";
	d1 = &par_fun;
	iret = d1(3);
	cout << check(iret == 3 && indicator == 3);
	indicator = 0;
	
	cout << "Calling Delegate<int,int> with extra parameters: ";
	iret = d1(3,'a',"kecske",13,0.8);
	cout << check(iret == 3 && indicator == 3);
	indicator = 0;
	
	cout << "Assigning and calling captureless lambda: ";
	d1 = [](int v){return v*2;};
	iret = d1(4);
	cout << check(iret == 8);
	indicator = 0;
	
	cout << "Assigning and calling reference captured lambda: ";
	d1 = [&]{indicator = 4; return 11;};
	iret = d1(400);
	cout << check(iret == 11 && indicator == 4);
	indicator = 0;
	
	cout << "Assigning and calling value captured lambda: ";
	d1 = [&,five](int par){indicator = 4; return five + par*10;};
	iret = d1(13);
	cout << check(iret == 135 && indicator == 4);
	indicator = 0;
	
	cout << "Assigning non-const object (copying) and calling constant functor: ";
	d1 = cfunctor;
	iret = d1(13,"obi");
	cout << check(iret == 1);
	indicator = 0;
	
	cout << "Assigning const object (copying) and calling constant functor: ";
	d1 = ccfunctor;
	iret = d1(13);
	cout << check(iret == 1);
	indicator = 0;
	
	cout << "Assigning (copying) and calling non-constant functor: ";
	d1 = functor;
	iret = d1(13);
	cout << check(iret == 1);
	indicator = 0;
	
	cout << "Assigning non-const object (storing a pointer) and calling constant functor: ";
	d1 = &cfunctor;
	iret = d1(13,"obi");
	cout << check(iret == 0);
	indicator = 0;
	
	cout << "Assigning const object (storing a pointer) and calling constant functor: ";
	d1 = &ccfunctor;
	iret = d1(13);
	cout << check(iret == 0);
	indicator = 0;
	
	cout << "Assigning (storing a pointer) and calling non-constant functor: ";
	d1 = &functor;
	iret = d1(13);
	cout << check(iret == 0);
	indicator = 0;
	
	cout << "Assigning class member function and using default constructor: ";
	d2 = &Ordinary::get;
	iret = d2();
	cout << check(iret == 2);
	indicator = 0;
	
	cout << "Assigning class member function and using explicit constructor: ";
	d2 = fm::Delegate<int>(&Ordinary::get,Ordinary(10));
	iret = d2();
	cout << check(iret == 10);
	indicator = 0;
	
	cout << "Assigning class member function and using copy constructor: ";
	d2 = fm::Delegate<int>(&Ordinary::get,ordi);
	iret = d2();
	cout << check(iret == ordi.a);
	indicator = 0;
	
	cout << "Assigning class member function and using copy constructor (const): ";
	d2 = fm::Delegate<int>(&Ordinary::get,cordi);
	iret = d2();
	cout << check(iret == cordi.a);
	indicator = 0;
	
	cout << "Assigning const class member function and using explicit constructor: ";
	d2 = fm::Delegate<int>(&Ordinary::cget,Ordinary(10));
	iret = d2();
	cout << check(iret == 10);
	indicator = 0;
	
	cout << "Assigning const class member function and using copy constructor: ";
	d2 = fm::Delegate<int>(&Ordinary::cget,ordi);
	iret = d2();
	cout << check(iret == ordi.a);
	indicator = 0;
	
	cout << "Assigning const class member function and using copy constructor (const): ";
	d2 = fm::Delegate<int>(&Ordinary::cget,cordi);
	iret = d2();
	cout << check(iret == cordi.a);
	indicator = 0;
	
	
	cout << "Assigning class member function and using pointer: ";
	d2 = fm::Delegate<int>(&Ordinary::get,&ordi);
	iret = d2();
	cout << check(iret == ordi.a);
	indicator = 0;
	
	cout << "Assigning const class member function and using pointer: ";
	d2 = fm::Delegate<int>(&Ordinary::cget,&ordi);
	iret = d2();
	cout << check(iret == ordi.a);
	indicator = 0;
	
	cout << "Assigning const class member function and using pointer (const): ";
	d2 = fm::Delegate<int>(&Ordinary::cget,&cordi);
	iret = d2();
	cout << check(iret == cordi.a);
	indicator = 0;
	
	if (success) {
		cout << "All tests succeeded" << endl;
	} else {
		cout << "Shit failed" << endl;
		return 1;
	}
}
