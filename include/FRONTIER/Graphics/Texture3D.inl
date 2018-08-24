#ifndef FRONTIER_TEXTURE3D_INL_INCLUDED
#define FRONTIER_TEXTURE3D_INL_INCLUDED

namespace fg
{
	/////////////////////////////////////////////////////////////
	template<fm::Size W,fm::Size H,fm::Size D>
	inline Texture3D::Texture3D(const fg::Color (&color)[W][H][D])
	{
		loadFromMemory(fm::vec3s(W,H,D),&color[0][0][0]);
	}
}

#endif