#include <Frontier.hpp>
#include <functional>
#include <iostream>
#include <deque>

using namespace std;

uint64_t fib(int a)
{
	if (a < 2) return a;
	
	return fib(a-1) + fib(a-2);
}

double testFib(int a)
{
	Clock clk;
 	uint64_t val = fib(a);
 	(void)val;
 	return clk.getSeconds();
}

string getWithLeading0(fm::Size a,fm::Size n=3)
{
	string ret = (a ? "" : "0");
	
	while (a) ret = char('0'+a%10) + ret, a/=10;
	while (ret.size() < n) ret = ' '+ret;
	
	return ret;
}

void doSomethingSilly(int n)
{
	int *t = new int[n];
	for (int i=0;i<n;++i)
	{
		t[i] = int(t[i]+i);
	}
	
	delete[] t;
}

void testDelegates(int a,int b)
{
	void (*ptr)(int) = &doSomethingSilly;
	Delegate<void,int> instaDelegate = &doSomethingSilly;
	Delegate<void,int,int> indirectDelegate = &doSomethingSilly;
	Delegate<void,int> lambdaDelegate = [](int n)
	{
		int *t = new int[n];
		for (int i=0;i<n;++i)
		{
			t[i] = int(t[i]+i);
		}
		
		delete[] t;
	};
	function<void(int)> stdFunc = &doSomethingSilly;
	
	Clock clk;
	
	cout << "calling w/ "<<a<<", "<<b<<" times\t";
	
	clk.restart(); for (int i=0;i<b;++i) ptr(a);
	cout << getWithLeading0(clk.getSeconds()*1000,6);
	
	clk.restart(); for (int i=0;i<b;++i) instaDelegate(a);
	cout << getWithLeading0(clk.getSeconds()*1000,6);
	
	clk.restart(); for (int i=0;i<b;++i) indirectDelegate(a,42);
	cout << getWithLeading0(clk.getSeconds()*1000,6);
	
	clk.restart(); for (int i=0;i<b;++i) lambdaDelegate(a);
	cout << getWithLeading0(clk.getSeconds()*1000,6);
	
	clk.restart(); for (int i=0;i<b;++i) stdFunc(a);
	cout << getWithLeading0(clk.getSeconds()*1000,6);
	
	cout << endl;
}

int main()
{
	cout << "times to calculate fibonacci numbers:" << endl;
	
	Clock allClk;
	
	for (int i=20;allClk.getSeconds() < 2;++i)
	{
		double t = testFib(i);
		cout << "fib(" << getWithLeading0(i,2) << ") -> " << getWithLeading0(t*1000) << "ms ";
		cout << string(int(80*t),' ') << "x" << endl;
	}
	cout << endl;
	
	
	cout << "benchmarking delegates" << endl;
	
	testDelegates(10,2000000);
	testDelegates(100,500000);
	testDelegates(1000,50000);
	testDelegates(10000,5000);
	testDelegates(100000,500);
}
