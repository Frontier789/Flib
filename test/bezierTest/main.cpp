#include <Frontier.hpp>
#include <FRONTIER/OpenGL.hpp>
#include <type_traits>
#include <iostream>
#include <future>
#include <thread>

using namespace std;

// TODO:
//  consider Mesh -> DrawData optimizations

int main()
{
	Mesh m = std::move(Mesh::getTeapot(7,4));
	
	m.calcNormals();
	m.calcTangents();
	m.clr.resize(m.pts.size());
	
	for (::size_t i=0;i<m.clr.size();++i)
	 	m.clr[i] = m.norms[i].dot(vec3(1,1,1).sgn());
	
	auto pl = fgui::plot(std::move(m));
	
	pl.wait();
}
