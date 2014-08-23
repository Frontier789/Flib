#include <FRONTIER/System/Angle.hpp>
#include <FRONTIER/Config.hpp>
#include <cmath>

namespace fm
{
    Angle::MeasureType Angle::globalMeasureType = Angle::Degrees;
    
    
    ////////////////////////////////////////////////////////////
	Angle::Angle() : m_amount(0)
    {
    	
    }
    
    ////////////////////////////////////////////////////////////
	Angle::Angle(float angle) : m_amount(globalMeasureType ? angle : angle/180.f*fm::PI)
    {
    	
    }
    
    ////////////////////////////////////////////////////////////
	Angle::Angle(const Angle &copy) : m_amount(copy.asRadians())
    {
        
    }
    
    ////////////////////////////////////////////////////////////
	Angle::Angle(float angle,MeasureType type) : m_amount(type ? angle : angle/180.f*fm::PI)
    {
        
    }


    float Angle::asDergrees() const
    {
        return m_amount * 180.f / fm::PI;
    }


    ////////////////////////////////////////////////////////////
    float Angle::asRadians() const
    {
        return m_amount;
    }


    ////////////////////////////////////////////////////////////
    float Angle::asDegs() const
    {
        return asDergrees();
    }


    ////////////////////////////////////////////////////////////
    float Angle::asRads() const
    {
        return asRadians();
    }


    ////////////////////////////////////////////////////////////
    float Angle::asDeg() const
    {
        return asDergrees();
    }


    ////////////////////////////////////////////////////////////
    float Angle::asRad() const
    {
        return asRadians();
    }


	/// operators //////////////////////////////////////////////////////////
	Angle operator+(const Angle &left,const Angle &right)
	{
		return radians(left.asRadians()+right.asRadians());
	}
	
	/////////////////////////////////////////////////////////////
	Angle operator-(const Angle &left,const Angle &right)
	{
		return radians(left.asRadians()-right.asRadians());
	}
	
	/////////////////////////////////////////////////////////////
	Angle operator*(const Angle &left,const float &right)
	{
		return radians(left.asRadians()*right);
	}
	
	/////////////////////////////////////////////////////////////
    Angle operator*(const float &left,const Angle &right)
	{
		return radians(left*right.asRadians());
	}
	
	/////////////////////////////////////////////////////////////
	Angle operator/(const Angle &left,const float &right)
	{
		return radians(left.asRadians()/right);
	}
	
	/////////////////////////////////////////////////////////////
	Angle &operator+=(Angle &left,const Angle &right)
	{
		*((float*)&left) += right.asRadians();
		return left;
	}
	
	/////////////////////////////////////////////////////////////
	Angle &operator-=(Angle &left,const Angle &right)
	{
		*((float*)&left) -= right.asRadians();
		return left;
	}
	
	/////////////////////////////////////////////////////////////
	Angle &operator*=(Angle &left,const float &right)
	{
		*((float*)&left) *= right;
		return left;
	}
	
	/////////////////////////////////////////////////////////////
	Angle &operator/=(Angle &left,const float &right)
	{
		*((float*)&left) /= right;
		return left;
	}
	
	/////////////////////////////////////////////////////////////
	bool operator==(const Angle &left,const Angle &right)
	{
		return left.asRadians() == right.asRadians();
	}
	
	/////////////////////////////////////////////////////////////
	bool operator!=(const Angle &left,const Angle &right)
	{
		return left.asRadians() != right.asRadians();
	}
	
	/////////////////////////////////////////////////////////////
	bool operator< (const Angle &left,const Angle &right)
	{
		return left.asRadians() < right.asRadians();
	}
	
	/////////////////////////////////////////////////////////////
	bool operator<=(const Angle &left,const Angle &right)
	{
		return left.asRadians() <= right.asRadians();
	}
	
	/////////////////////////////////////////////////////////////
	bool operator> (const Angle &left,const Angle &right)
	{
		return left.asRadians() > right.asRadians();
	}
	
	/////////////////////////////////////////////////////////////
	bool operator>=(const Angle &left,const Angle &right)
	{
		return left.asRadians() >= right.asRadians();
	}
    
    Angle degrees(float amount)
    {
    	Angle ret;
    	*((float*)&ret) = amount/180.f*fm::PI;
        return ret;
    }

    ////////////////////////////////////////////////////////////
    Angle radians(float amount)
    {
    	Angle ret;
    	*((float*)&ret) = amount;
        return ret;
    }


    ////////////////////////////////////////////////////////////
    Angle deg(float amount)
    {
        return degrees(amount);
    }


    ////////////////////////////////////////////////////////////
    Angle rad(float amount)
    {
        return radians(amount);
    }
}
namespace std
{
    float sin(const fm::Angle &angle)
    {
        return std::sin(angle.asRad());
    }


    ////////////////////////////////////////////////////////////
    float cos(const fm::Angle &angle)
    {
        return std::cos(angle.asRad());
    }


    ////////////////////////////////////////////////////////////
    float tan(const fm::Angle &angle)
    {
        return std::tan(angle.asRad());
    }
}

