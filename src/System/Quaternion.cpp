////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2018 Frontier (fr0nt13r789@gmail.com)           ///
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
#include <FRONTIER/System/Quaternion.hpp>
#include <FRONTIER/System/Vector3.hpp>
#include <FRONTIER/System/Vector4.hpp>
#include <FRONTIER/System/Matrix.hpp>
#include <FRONTIER/System/Angle.hpp>
#include <cmath>
namespace fm
{

	/// Constructors /////////////////////////////////////////////////////////
	Quat::Quat() : x(0),
				   y(0),
				   z(0),
				   w(1)
	{

	}

	////////////////////////////////////////////////////////////
	Quat::Quat(const Quat &copy) : x(copy.x),
								   y(copy.y),
								   z(copy.z),
								   w(copy.w)
	{

	}

	////////////////////////////////////////////////////////////
	Quat::Quat(const vec3 &axis,const Angle<float> &angle)
	{
		float sinA = std::sin(angle*.5f);
		x = sinA*axis.x;
		y = sinA*axis.y;
		z = sinA*axis.z;
		w = std::cos(angle*.5f);
	}

	////////////////////////////////////////////////////////////
	Quat::Quat(float X,float Y,float Z,float W) : x(X),
												  y(Y),
												  z(Z),
												  w(W)
	{

	}

	////////////////////////////////////////////////////////////
	Quat::Quat(const Angle<float> &around_x,const Angle<float> &around_y,const Angle<float> &around_z)
	{
		double sx = std::sin(around_x*.5);
		double cx = std::cos(around_x*.5);
		double sy = std::sin(around_y*.5);
		double cy = std::cos(around_y*.5);
		double sz = std::sin(around_z*.5);
		double cz = std::cos(around_z*.5);

		float cycz = cy * cz;
		float sycz = sy * cz;
		float cysz = cy * sz;
		float sysz = sy * sz;

		x = sx * cycz - cx * sysz;
		y = cx * sycz + sx * cysz;
		z = cx * cysz - sx * sycz;
		w = cx * cycz + sx * sysz;

		normalize();
	}

	////////////////////////////////////////////////////////////
	float Quat::dot(const Quat &other) const
	{
		return (x * other.x) + (y * other.y) + (z * other.z) + (w * other.w);
	}

	////////////////////////////////////////////////////////////
	Quat &Quat::normalize()
	{
		(*this) = norm();
		return *this;
	}

	////////////////////////////////////////////////////////////
	Quat Quat::norm() const
	{
		float l = LENGTH();
		return (l==1 || l==0) ? *this : (*this)/std::sqrt(l);
	}

	////////////////////////////////////////////////////////////
	Quat Quat::sgn() const
	{
		float l = LENGTH();
		return (l==1 || l==0) ? *this : (*this)/std::sqrt(l);
	}

	////////////////////////////////////////////////////////////
	Quat Quat::inverse() const
	{
		return Quat(-x,-y,-z,w);
	}

	////////////////////////////////////////////////////////////
	float Quat::length() const
	{
		return std::sqrt(x*x+y*y+z*z+w*w);
	}

	////////////////////////////////////////////////////////////
	float Quat::LENGTH() const
	{
		return x*x+y*y+z*z+w*w;
	}

	////////////////////////////////////////////////////////////
	vec3 Quat::getAxis() const
	{
		float sinA = std::sin(fm::rad(std::acos(w))*.5f);

		return vec3(x,y,z)/sinA;
	}

	////////////////////////////////////////////////////////////
	Angle<float> Quat::getAngle() const
	{
		return fm::rad(std::acos(w));
	}

	////////////////////////////////////////////////////////////
	mat4 Quat::getMatrix() const
	{
		float ret[4*4]={1-2*y*y-2*z*z,   2*x*y+2*w*z,   2*x*z-2*w*y, 0,
						  2*x*y-2*w*z, 1-2*x*x-2*z*z,   2*y*z+2*w*x, 0,
						  2*x*z+2*w*y,   2*y*z-2*w*x, 1-2*x*x-2*y*y, 0,
									0,             0,             0, 1};
		return fm::mat4(ret);
	}

	////////////////////////////////////////////////////////////
	Quat Quat::fromTo(const vec3 &from,const vec3 &to)
	{
		if (from == to)
			return identity;

		if (from == -to)
			return Quat(0,0,0,1);

        float szog = std::acos(from.dot(to));
		szog = szog < 0 ? 3.14159265358979 + szog : szog;

		vec3 axis = from.cross(to).sgn();
		float sinA = std::sin(szog*.5f);

		return Quat (sinA*axis.x,
					 sinA*axis.y,
					 sinA*axis.z,
					 std::cos(szog*.5f));
	}

	////////////////////////////////////////////////////////////
	Quat Quat::lerp(const Quat &quatAt0,const Quat &quatAt1,float deltaTime)
	{
		return quatAt0*(1.f-deltaTime)+quatAt1*deltaTime;
	}

	////////////////////////////////////////////////////////////
	Quat Quat::slerp(Quat quatAt0,const Quat &quatAt1,float deltaTime,float useLerpAfter)
	{
		float dot = quatAt0.dot(quatAt1);

		if (dot < 0.0f)
			quatAt0 *= -1.f,
			dot *= -1.f;

		if (dot <= 1.f-useLerpAfter)
		{
			float theta = std::acos(dot);
			float invsintheta = 1.f/std::sin(theta);
			return quatAt0*(std::sin(theta * (1.0f-deltaTime)) * invsintheta) +
				   quatAt1*(std::sin(theta * deltaTime) * invsintheta);
		}

		return lerp(quatAt0,quatAt1,deltaTime);
	}

	////////////////////////////////////////////////////////////
	Quat::operator mat4() const
	{
		return getMatrix();
	}
	
	////////////////////////////////////////////////////////////
	Quat Quat::identity = Quat();

	////////////////////////////////////////////////////////////
	bool operator==(const Quat &left,const Quat &right)
	{
		return left.x==right.x && left.y==right.y && left.z==right.z && left.w==right.w;
	}

	////////////////////////////////////////////////////////////
	bool operator!=(const Quat &left,const Quat &right)
	{
		return left.x!=right.x || left.y!=right.y || left.z!=right.z || left.w!=right.w;
	}

	////////////////////////////////////////////////////////////
	Quat operator- (const Quat &quat)
	{
		return Quat(-quat.x,
					-quat.y,
					-quat.z,
					-quat.w);
	}

	////////////////////////////////////////////////////////////
	Quat operator+ (const Quat &left,const Quat &right)
	{
		return Quat(left.x+right.x,
					left.y+right.y,
					left.z+right.z,
					left.w+right.w);
	}

	////////////////////////////////////////////////////////////
	Quat operator* (const Quat &left,const Quat &right)
	{
		Quat ret;

		ret.x = (right.w * left.x) + (right.x * left.w) + (right.y * left.z) - (right.z * left.y);
		ret.y = (right.w * left.y) + (right.y * left.w) + (right.z * left.x) - (right.x * left.z);
		ret.z = (right.w * left.z) + (right.z * left.w) + (right.x * left.y) - (right.y * left.x);
		ret.w = (right.w * left.w) - (right.x * left.x) - (right.y * left.y) - (right.z * left.z);

		return ret;
	}

	////////////////////////////////////////////////////////////
	mat4 operator* (const Quat &left,const mat4 &right)
	{
		return left.getMatrix() * right;
	}

	////////////////////////////////////////////////////////////
	mat4 operator* (const mat4 &left,const Quat &right)
	{
		return left * right.getMatrix();
	}

	////////////////////////////////////////////////////////////
	vec3 operator* (const Quat &left,const vec3 &right)
	{
		vec3 uv, uuv;
		vec3 axis(left.x, left.y, left.z);
		uv   = axis.cross(right);
		uuv  = axis.cross(uv);
		uv  *= 2.0f * left.w;
		uuv *= 2.0f;

		return right + uv + uuv;
	}

	////////////////////////////////////////////////////////////
	vec4 operator* (const Quat &left,const vec4 &right)
	{
		return left*vec3(right);
	}

	////////////////////////////////////////////////////////////
	Quat operator* (const Quat &left,float right)
	{
		return Quat(left.x*right,
					left.y*right,
					left.z*right,
					left.w*right);
	}

	////////////////////////////////////////////////////////////
	Quat operator/ (const Quat &left,float right)
	{
		return Quat(left.x/right,
					left.y/right,
					left.z/right,
					left.w/right);
	}

	////////////////////////////////////////////////////////////
	Quat &operator*=(Quat &left,const Quat &right)
	{
		left = left*right;
		return left;
	}

	////////////////////////////////////////////////////////////
	Quat &operator*=(Quat &left,float right)
	{
		left.x*=right;
		left.y*=right;
		left.z*=right;
		left.w*=right;
		return left;
	}

	////////////////////////////////////////////////////////////
	Quat &operator/=(Quat &left,float right)
	{
		left.x/=right;
		left.y/=right;
		left.z/=right;
		left.w/=right;
		return left;
	}
}
