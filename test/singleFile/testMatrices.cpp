#include <Frontier.hpp>
#include <iostream>

using namespace std;

int main()
{
	static_assert(sizeof(matrix<1,1,int>) == sizeof(int)*1*1, "matrix<1,1,i> failed size check");
	static_assert(sizeof(matrix<2,1,int>) == sizeof(int)*2*1, "matrix<2,1,i> failed size check");
	static_assert(sizeof(matrix<1,3,int>) == sizeof(int)*1*3, "matrix<1,3,i> failed size check");
	static_assert(sizeof(matrix<2,3,int>) == sizeof(int)*2*3, "matrix<2,3,i> failed size check");
	static_assert(sizeof(matrix<4,3,int>) == sizeof(int)*4*3, "matrix<4,3,i> failed size check");
	static_assert(sizeof(matrix<4,4,int>) == sizeof(int)*4*4, "matrix<4,4,i> failed size check");
	static_assert(sizeof(matrix<1,1,float>) == sizeof(float)*1*1, "matrix<1,1,f> failed size check");
	static_assert(sizeof(matrix<2,1,float>) == sizeof(float)*2*1, "matrix<2,1,f> failed size check");
	static_assert(sizeof(matrix<1,3,float>) == sizeof(float)*1*3, "matrix<1,3,f> failed size check");
	static_assert(sizeof(matrix<2,3,float>) == sizeof(float)*2*3, "matrix<2,3,f> failed size check");
	static_assert(sizeof(matrix<4,3,float>) == sizeof(float)*4*3, "matrix<4,3,f> failed size check");
	static_assert(sizeof(matrix<4,4,float>) == sizeof(float)*4*4, "matrix<4,4,f> failed size check");
	
	cout << "Testing matrix constructors" << endl;
	cout << mat2() << endl << endl;
	cout << mat3(1,2,3,4,5,6,7,8,9) << endl << endl;
	cout << matrix<3,2>(1,2,3,4,5,6) << endl << endl;
	cout << matrix<3,2>(vec3(1,2,3),vec3(4,7,8)) << endl << endl;
	cout << mat4(2,0,0,1,
				 0,5,0,2,
				 0,0,1,8,
				 0,0,0,1) << endl;
	cout << endl;
	
	cout << "here is a 4x2 column matrix" << endl;
	cout << matrix<4,2>({1,2,3,4,5,6,7,8}) << endl;
	cout << endl;
	
	
	cout << "                          (1,2,3)" << endl;
	cout << "The determinant of matrix (4,5,6) = " << mat3(1,2,3,4,5,6,7,8,9).det() << endl;
	cout << "                          (7,8,9)" << endl;
	cout << endl;
	
	
	mat3 a(1,2,3,
		   4,1,6,
		   7,2,9);
	
	mat3 b(1,1,3,
		   0,1,2,
		   7,3,7);
	
	mat3 m = a*b;
	
	cout << "(1,2,3)   (1,1,3)   (" << m[0][0] << "," << m[0][1] << "," << m[0][2] << ")" << endl;
	cout << "(4,1,6) * (0,1,2) = (" << m[1][0] << "," << m[1][1] << "," << m[1][2] << ")" << endl;
	cout << "(7,2,9)   (7,3,7)   (" << m[2][0] << "," << m[2][1] << "," << m[2][2] << ")" << endl;
	cout << endl;
	
	
	cout << "inverting a matrix" << endl;
	
	matrix<4,4,double> k{1,5,9,7, 5,3,6,8, 4,2,6,8, 5,7,4,1};
	
	cout << "k = " << endl << k << endl << endl;
	cout << "k^-1 = " << endl << k.inverse() << endl << endl;
	cout << "(k^-1)*k = " << endl << fixed << k.inverse()*k << endl << endl;
	cout.unsetf(ios_base::fixed);
	
	cout << "what about a matrix of strings?" << endl;
	
	matrix<3,3,string> sm("lol");
	cout << sm << endl << endl;
	cout << sm + matrix<3,3,string>(string("_k")) << endl << endl;
	
	
	cout << "or a matrix of matrices" << endl;
	
	matrix<2,2,mat2> whatthe;
	cout << whatthe << endl;
	
	cout << "what if i multiply two of these?" << endl;
	
	cout << whatthe * whatthe << endl << endl;
	
	
	cout << "some transformations" << endl;
	
	cout << MATRIX::translation(vec3(1,2,3)) << endl << endl;
	cout << MATRIX::scaling(vec3(1,2,3)) << endl << endl;
	cout << MATRIX::ortho(0,0,100,100) << endl << endl;
	
	cout << "multiplication with vectors" << endl; // note that a vecN can act as a column or a row matrix
	cout << mat3(1,2,3,4,5,6,7,8,9) * vec3(1,2,3) << endl;
	cout << vec3(1,2,3) * mat3(1,2,3,4,5,6,7,8,9) << endl;
}
