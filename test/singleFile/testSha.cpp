#include <Frontier.hpp>
#include <iostream>

using namespace std;

int main()
{
	size_t data[] = {100,450,488888888,42};
	
	cout << "Empty test: ";
	cout << (fm::sha256(nullptr,0) == "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855") << endl;
	cout << "String test: ";
	cout << (fm::sha256((const char*)"goat",4) == "5480f08f35968440ebe8135a8bf9e58c8c944bf4e3ba0f45acb141e474bd0c9c") << endl;
	cout << "Set of numbers test: ";
	cout << (fm::sha256(data,sizeof(data)) == "5b0bab38489d7ebafa6cd3179672dc9e6c0a9c46e8b552b4c6e326a0d9f8f614") << endl;
}
