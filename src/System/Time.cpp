#include <FRONTIER/System/Time.hpp>
namespace fm
{
	/// Constructors //////////////////////////////////////////////////////////
	Time::Time() : m_amount(0)
	{
		
	}

	/////////////////////////////////////////////////////////////
	Time::Time(const long double &value,MeasureType type) : m_amount(value*(long double)type)
	{
		
	}

	/////////////////////////////////////////////////////////////
	Time::Time(const Time &copy) : m_amount(copy.m_amount)
	{
		
	}

	/// Functions //////////////////////////////////////////////////////////
	long Time::asMicroseconds() const
	{
		return m_amount;
	}

	/////////////////////////////////////////////////////////////
	int Time::asMilliseconds() const
	{
		return m_amount/1000.0;
	}

	/////////////////////////////////////////////////////////////
	float Time::asSeconds() const
	{
		return m_amount/1000.0/1000.0;
	}
	

	/////////////////////////////////////////////////////////////
	long Time::asMicsecs() const
	{
		return m_amount;
	}

	/////////////////////////////////////////////////////////////
	int Time::asMilsecs() const
	{
		return m_amount/1000.0;
	}

	/////////////////////////////////////////////////////////////
	float Time::asSecs() const
	{
		return m_amount/1000.0/1000.0;
	}

	/////////////////////////////////////////////////////////////
	const Time Time::Zero = Time(0,Microseconds);

	/////////////////////////////////////////////////////////////
	Time operator+(const Time &left,const Time &right)
	{
		return microseconds(left.asMicroseconds()+right.asMicroseconds());
	}

	/////////////////////////////////////////////////////////////
	Time operator-(const Time &left,const Time &right)
	{
		return microseconds(left.asMicroseconds()-right.asMicroseconds());
	}
	
	/////////////////////////////////////////////////////////////
	Time operator-(const Time &time)
	{
		return microseconds(time.asMicroseconds()*-1.0);
	}
	
	/////////////////////////////////////////////////////////////
	Time operator*(const Time &left,const float &right)
	{
		return microseconds(left.asMicroseconds()*right);
	}
	
	/////////////////////////////////////////////////////////////
	Time operator*(const float &left,const Time &right)
	{
		return microseconds(left*right.asMicroseconds());
	}
	
	/////////////////////////////////////////////////////////////
	Time operator/(const Time &left,const float &right)
	{
		return microseconds(left.asMicroseconds()/right);
	}
	
	/////////////////////////////////////////////////////////////
	Time &operator+=(Time &left,const Time &right)
	{
		*((long*)&left) += right.asMicroseconds();
		return left;
	}
	
	/////////////////////////////////////////////////////////////
	Time &operator-=(Time &left,const Time &right)
	{
		*((long*)&left) -= right.asMicroseconds();
		return left;
	}
	
	/////////////////////////////////////////////////////////////
	Time &operator*=(Time &left,const float &right)
	{
		*((long*)&left) *= right;
		return left;
	}
	
	/////////////////////////////////////////////////////////////
	Time &operator/=(Time &left,const float &right)
	{
		*((long*)&left) /= right;
		return left;
	}
	
	/////////////////////////////////////////////////////////////
	bool operator==(const Time &left,const Time &right)
	{
		return (left.asMicroseconds()==right.asMicroseconds());
	}
	
	/////////////////////////////////////////////////////////////
	bool operator!=(const Time &left,const Time &right)
	{
		return (left.asMicroseconds()!=right.asMicroseconds());
	}
	
	/////////////////////////////////////////////////////////////
	bool operator< (const Time &left,const Time &right)
	{
		return (left.asMicroseconds()<right.asMicroseconds());
	}
	
	/////////////////////////////////////////////////////////////
	bool operator<=(const Time &left,const Time &right)
	{
		return (left.asMicroseconds()<=right.asMicroseconds());
	}
	
	/////////////////////////////////////////////////////////////
	bool operator> (const Time &left,const Time &right)
	{
		return (left.asMicroseconds()>right.asMicroseconds());
	}
	
	/////////////////////////////////////////////////////////////
	bool operator>=(const Time &left,const Time &right)
	{
		return (left.asMicroseconds()>=right.asMicroseconds());
	}
	
	/////////////////////////////////////////////////////////////
	Time microseconds(long Msecs)
	{
		return Time(Msecs,Time::Microseconds);
	}
	
	/////////////////////////////////////////////////////////////
	Time milliseconds(int msecs)
	{
		return Time(msecs,Time::Milliseconds);
	}
	
	/////////////////////////////////////////////////////////////
	Time seconds(float secs)
	{
		return Time(secs,Time::Seconds);
	}
}
