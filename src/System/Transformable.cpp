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
/// You should have recieved a copy of GNU GPL with this software      ///
///                                                                    ///
////////////////////////////////////////////////////////////////////////// -->
#include <FRONTIER/System/Transformable.hpp>
#include <FRONTIER/System/Vector3.hpp>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/Matrix.hpp>
#include <FRONTIER/System/Angle.hpp>

namespace fm
{
	////////////////////////////////////////////////////////////
	Transformable::Transformable() : m_scaling(1.f,1.f,1.f),
									 m_axis(0.f,0.f,1.f),
									 m_calculated(true),
									 m_invCalculated(true)
	{
		
	}

	////////////////////////////////////////////////////////////
	Transformable::~Transformable()
	{
		
	}
	
	////////////////////////////////////////////////////////////
	Transformable &Transformable::setPosition(float x,float y,float z)
	{
		m_position = vec3(x,y,z);
		m_calculated = false;
		m_invCalculated = false;
		return *this;
	}
	
	////////////////////////////////////////////////////////////
	Transformable &Transformable::setPosition(const vec2 &pos,float z)
	{
		m_position = vec3(pos.x,pos.y,z);
		m_calculated = false;
		m_invCalculated = false;
		return *this;
	}
	
	////////////////////////////////////////////////////////////
	Transformable &Transformable::setPosition(const vec3 &pos)
	{
		m_position = pos;
		m_calculated = false;
		m_invCalculated = false;
		return *this;
	}
	
	////////////////////////////////////////////////////////////
	Transformable &Transformable::setRotation(const Angle &angle)
	{
		m_angle = angle;
		m_calculated = false;
		m_invCalculated = false;
		return *this;
	}
	
	////////////////////////////////////////////////////////////
	Transformable &Transformable::setRotation(const vec3 &axis,const Angle &angle)
	{
		m_axis = axis.sgn();
		m_angle = angle;
		m_calculated = false;
		m_invCalculated = false;
		return *this;
	}
	
	////////////////////////////////////////////////////////////
	Transformable &Transformable::setScale(float x,float y,float z)
	{
		m_scaling = vec3(x,y,z);
		m_calculated = false;
		m_invCalculated = false;
		return *this;
	}
	
	////////////////////////////////////////////////////////////
	Transformable &Transformable::setScale(const vec2 &scaling,float z)
	{
		m_scaling = vec3(scaling.x,scaling.y,z);
		m_calculated = false;
		m_invCalculated = false;
		return *this;
	}
	
	////////////////////////////////////////////////////////////
	Transformable &Transformable::setScale(const vec3 &scaling)
	{
		m_scaling = scaling;
		m_calculated = false;
		m_invCalculated = false;
		return *this;
	}
	
	////////////////////////////////////////////////////////////
	Transformable &Transformable::setOrigin(float x,float y,float z)
	{
		m_origin = vec3(x,y,z);
		m_calculated = false;
		m_invCalculated = false;
		return *this;
	}
	
	////////////////////////////////////////////////////////////
	Transformable &Transformable::setOrigin(const vec2 &origin,float z)
	{
		m_origin = vec3(origin.x,origin.y,z);
		m_calculated = false;
		m_invCalculated = false;
		return *this;
	}
	
	////////////////////////////////////////////////////////////
	Transformable &Transformable::setOrigin(const vec3 &origin)
	{
		m_origin = origin;
		m_calculated = false;
		m_invCalculated = false;
		return *this;
	}
	
	////////////////////////////////////////////////////////////
	vec3 Transformable::getPosition() const
	{
		return m_position;
	}
	
	////////////////////////////////////////////////////////////
	Angle Transformable::getRotation(vec3 *axis) const
	{
		if (axis)
			(*axis) = m_axis;
		return m_angle;
	}
	
	////////////////////////////////////////////////////////////
	vec3 Transformable::getScale() const
	{
		return m_scaling;
	}
	
	////////////////////////////////////////////////////////////
	vec3 Transformable::getOrigin() const
	{
		return m_origin;
	}
	
	////////////////////////////////////////////////////////////
	Transformable &Transformable::move(float x,float y,float z)
	{
		m_position += vec3(x,y,z);
		m_calculated = false;
		m_invCalculated = false;
		return *this;
	}
	
	////////////////////////////////////////////////////////////
	Transformable &Transformable::move(const vec3 &pos)
	{
		m_position += pos;
		m_calculated = false;
		m_invCalculated = false;
		return *this;
	}

	////////////////////////////////////////////////////////////
	Transformable &Transformable::rotate(const Angle &angle)
	{
		m_angle += angle;
		m_calculated = false;
		m_invCalculated = false;
		return *this;
	}

	////////////////////////////////////////////////////////////
	Transformable &Transformable::scale(float x,float y,float z)
	{
		m_scaling *= vec3(x,y,z);
		m_calculated = false;
		m_invCalculated = false;
		return *this;
	}

	////////////////////////////////////////////////////////////
	Transformable &Transformable::scale(const vec2 &scaling,float z)
	{
		m_scaling *= vec3(scaling.x,scaling.y,z);
		m_calculated = false;
		m_invCalculated = false;
		return *this;
	}

	////////////////////////////////////////////////////////////
	Transformable &Transformable::scale(const vec3 &scaling)
	{
		m_scaling *= scaling;
		m_calculated = false;
		m_invCalculated = false;
		return *this;
	}

	////////////////////////////////////////////////////////////
	const mat4 &Transformable::getTransform() const
	{
		if (!m_calculated)
		{/*
			m_matrix = fm::MATRIX::translation(m_position+m_origin)*
					   fm::Quat(m_axis,m_angle).getMatrix()*
					   fm::MATRIX::scaling(m_scaling)*fm::MATRIX::translation();*/
			const vec3 &s = m_scaling;
			vec3 p = m_position+m_origin;
			
			if (m_angle != fm::rad(0))
			{
				float halfAngle = m_angle.asRad()/2.f;
				float sinA = std::sin(halfAngle);
				float x = sinA*m_axis.x;
				float y = sinA*m_axis.y;
				float z = sinA*m_axis.z;
				float w = std::cos(halfAngle);
				
				float mat[4*4] = {(1-2*y*y-2*z*z)*s.x, (  2*x*y+2*w*z)*s.y, (  2*x*z-2*w*y)*s.z, p.x,
								  (  2*x*y-2*w*z)*s.x, (1-2*x*x-2*z*z)*s.y, (  2*y*z+2*w*x)*s.z, p.y,
								  (  2*x*z+2*w*y)*s.x, (  2*y*z-2*w*x)*s.y, (1-2*x*x-2*y*y)*s.z, p.z,
													0,                   0,                   0,   1};
				
				m_matrix = mat4(mat);
			}
			else
			{
				float mat[4*4] = {s.x,  0,  0, p.x,
				                 	0,s.y,  0, p.y,
				                 	0,  0,s.z, p.z,
				                 	0,  0,  0,   1};
				
				m_matrix = mat4(mat);
			}
			
			if (m_origin.x || m_origin.y || m_origin.z)
			{
				m_matrix[0][3] -= m_matrix[0][0]*m_origin.x+m_matrix[0][1]*m_origin.y+m_matrix[0][2]*m_origin.z;
				m_matrix[1][3] -= m_matrix[1][0]*m_origin.x+m_matrix[1][1]*m_origin.y+m_matrix[1][2]*m_origin.z;
				m_matrix[2][3] -= m_matrix[2][0]*m_origin.x+m_matrix[2][1]*m_origin.y+m_matrix[2][2]*m_origin.z;
			}

			m_calculated = true;
		}
		return m_matrix;
	}

	////////////////////////////////////////////////////////////
	const mat4 &Transformable::getInverseTransform() const
	{
		if (!m_invCalculated)
			m_invMatrix = MATRIX::inverse(getTransform()),
			m_invCalculated = true;
		return m_invMatrix;
	}

	////////////////////////////////////////////////////////////
	Transformable &Transformable::resetTransform()
	{
		(*this) = Transformable();
		return *this;
	}
}