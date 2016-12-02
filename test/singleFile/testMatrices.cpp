#include <Frontier.hpp>
#include <iostream>

using namespace std;

int main()
{
	cout << "                          (1,2,3)" << endl;
	cout << "The determinant of matrix (4,5,6) = " << mat3(1,2,3,4,5,6,7,8,9).det() << endl;
	cout << "                          (7,8,9)" << endl;
}