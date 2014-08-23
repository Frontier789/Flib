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
