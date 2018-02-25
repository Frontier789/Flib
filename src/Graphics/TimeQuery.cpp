#include <FRONTIER/Graphics/TimeQuery.hpp>
#include <FRONTIER/OpenGL.hpp>

namespace fg
{
	/////////////////////////////////////////////////////////////
	TimeQuery::TimeQuery() : m_running(false)
	{

	}

	/////////////////////////////////////////////////////////////
	fm::Result TimeQuery::start()
	{
		if (m_running) return fm::Result();

		if (!getGlId())
		{
			GLuint id;
			fm::Result res = glCheck(glGenQueries(1, &id));

			if (!res) return res;

			getGlId() = id;
		}

		fm::Result res = glCheck(glBeginQuery(GL_TIME_ELAPSED, getGlId()));

		m_running = res;
		m_runTime = fm::Time::Zero;

		return res;
	}
	
	/////////////////////////////////////////////////////////////
	fm::Result TimeQuery::stop()
	{
		if (getGlId() && m_running)
		{
			m_running = false;

			return glCheck(glEndQuery(GL_TIME_ELAPSED));
		}

		return fm::Result();
	}

	/////////////////////////////////////////////////////////////
	bool TimeQuery::finished() const
	{
		if (!getGlId()) return false;

		GLint available = 0;
		glGetQueryObjectiv(getGlId(), GL_QUERY_RESULT_AVAILABLE, &available);
		
		return available == GL_TRUE;
	}

	/////////////////////////////////////////////////////////////
	fm::Time TimeQuery::getTime() const
	{
		if (getGlId() && m_runTime == fm::Time::Zero)
		{
			GLuint64 result;
			glGetQueryObjectui64v(getGlId(), GL_QUERY_RESULT, &result);

			m_runTime = fm::microseconds(result*1.e-3);
		}

		return m_runTime;
	}
}

