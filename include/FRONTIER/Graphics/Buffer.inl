#ifndef FRONTIER_BUFFER_INL_INCLUDED
#define FRONTIER_BUFFER_INL_INCLUDED
namespace fg
{
	////////////////////////////////////////////////////////////
	template<class T,std::size_t S>
	Buffer &Buffer::setData(const T (&data)[S])
	{
		return setData(&data[0],sizeof(data));
	}

	////////////////////////////////////////////////////////////
	template<class T,std::size_t S>
	Buffer &Buffer::setData(const T (&data)[S],Usage usage)
	{
		return setData(&data[0],sizeof(data),m_type,usage);
	}

	////////////////////////////////////////////////////////////
	template<class T,std::size_t S>
	Buffer &Buffer::setData(const T (&data)[S],BufferType type)
	{
		return setData(&data[0],sizeof(data),type,m_usage);
	}

	////////////////////////////////////////////////////////////
	template<class T,std::size_t S>
	Buffer &Buffer::setData(const T (&data)[S],BufferType type,Usage usage)
	{
		return setData(&data[0],sizeof(data),type,usage);
	}
}
#endif //FRONTIER_BUFFER_INL_INCLUDED