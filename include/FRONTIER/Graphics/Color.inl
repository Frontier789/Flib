////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014 Frontier (fr0nt13r789@gmail.com)                ///
///                                                                    ///
/// Flib is licensed under the terms of GNU GPL.                       ///
/// Therefore you may freely use it in your project,                   ///
/// modify it, redistribute it without any warranty on the             ///
/// condition that this disclaimer is not modified/removed.            ///
/// You may not misclaim the origin of this software.                  ///
///                                                                    ///
/// If you use this software in your program/project a                 ///
/// note about it and an email for the author (fr0nt13r789@gmail.com)  ///
/// is not required but highly appreciated.                            ///
///                                                                    ///
/// You should have received a copy of GNU GPL with this software      ///
///                                                                    ///
////////////////////////////////////////////////////////////////////////// -->
#ifndef FRONTIER_COLOR_INL_INCLUDED
#define FRONTIER_COLOR_INL_INCLUDED

namespace std
{
    template<typename,typename>
    class basic_ostream;
    template<typename,typename>
    class basic_istream;
    template<typename>
    class char_traits;
}

template<class T>
inline std::basic_ostream<T, std::char_traits<T> > &operator<<(std::basic_ostream<T, std::char_traits<T> > &out,const fg::Color &col)
{
	return out<<int(col.r)<<' '<<int(col.g)<<' '<<int(col.b)<<' '<<int(col.a);
}

template<class T>
inline std::basic_istream<T, std::char_traits<T> > &operator>>(std::basic_istream<T, std::char_traits<T> > &in,fg::Color &col)
{
	int r,g,b,a;
	in>>r>>g>>b>>a;
	col(r,g,b,a);
	return in;
}

#endif //FRONTIER_COLOR_INL_INCLUDED
