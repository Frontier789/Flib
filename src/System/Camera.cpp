#include <FRONTIER/System/CommonTypes.hpp>
#include <FRONTIER/System/MatrixStack.hpp>
#include <FRONTIER/System/Quaternion.hpp>
#include <FRONTIER/System/Vector3.hpp>
#include <FRONTIER/System/Polar2.hpp>
#include <FRONTIER/System/Camera.hpp>
#include <FRONTIER/System/Matrix.hpp>
#include <FRONTIER/System/Angle.hpp>
#include <FRONTIER/System/Rect.hpp>

namespace fm
{
	/////////////////////////////////////////////////////////////
	void Camera::updateProj()
	{
		m_projStack.clear();
		
		if (m_projection == Camera::Perspective)
		{
			m_projStack.push(MATRIX::perspective(m_fieldOfView,
												 m_canvasSize.w / m_canvasSize.h,
												 m_znear,m_zfar));
		}
		else
		{
			m_projStack.push(fm::MATRIX::ortho(0,
											   m_canvasSize.h,
											   m_canvasSize.w,
											   0,
											   m_znear,m_zfar));
		}
	}
	
	/////////////////////////////////////////////////////////////
	void Camera::updateView()
	{
		m_viewStack.clear();
		
		if (m_projection == Camera::Perspective)
		{
			mat4 t = translation(-m_pos);
			vec3 v = this->v();
			vec3 r = this->r();
			vec3 u = this->u();
			
			m_viewStack.push(matrix<4,4,float>( r.x,  r.y,  r.z,  0,
											    u.x,  u.y,  u.z,  0,
											   -v.x, -v.y, -v.z,  0,
												  0,    0,    0,  1)*t);
		}
		else
		{
			m_viewStack.push(fm::MATRIX::translation(-m_pos));
		}
	}
	
	/////////////////////////////////////////////////////////////
	Camera::Camera() : m_up(vec3(0,1,0)),
					   m_right(vec3(1,0,0)),
					   m_zfar(100),
					   m_znear(.1),
					   m_viewDir(vec3(0,0,-1)),
					   m_limitPitch(true),
					   m_fieldOfView(fm::deg(90)),
					   m_projection(Camera::Perspective)
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	Camera::Camera(vec2 canvasSize,
				   vec3 pos,
				   vec3 target,
				   const Anglef &fov,
				   float znear,
				   float zfar) : m_limitPitch(true)
	{
		set3D(canvasSize,pos,target,fov,znear,zfar);
	}

	/////////////////////////////////////////////////////////////
	mat4 Camera::getVPMat() const
	{
		return getProjMat() * getViewMat();
	}

	/////////////////////////////////////////////////////////////
	mat4 Camera::getProjMat() const
	{
		return m_projStack.top();
	}

	/////////////////////////////////////////////////////////////
	mat4 Camera::getViewMat() const
	{
		return m_viewStack.top();
	}

	/////////////////////////////////////////////////////////////
	Camera::reference Camera::lookAt(vec3 target)
	{
		return setViewDir(target - getPosition());
	}

	/////////////////////////////////////////////////////////////
	Camera::reference Camera::setViewDir(vec3 viewDir)
	{
		m_viewDir = viewDir.sgn();
		
		vec3 forw = f();
		
		if (!forw.LENGTH())
		{
			m_right = vec3(1,0,0);
			m_up    = vec3(0,0,1);
		}
		else
		{
			m_right = forw.cross(vec3(0,1,0));
			m_up    = m_right.cross(m_viewDir);
		}
		
		updateView();
		
		return *this;
	}

	/////////////////////////////////////////////////////////////
	Camera::reference Camera::setPosition(vec3 pos)
	{
		m_pos = pos;
		
		updateView();
		
		return *this;
	}

	/////////////////////////////////////////////////////////////
	Camera::reference Camera::movePosition(vec3 delta)
	{
		return setPosition(getPosition() + delta);
	}

	/////////////////////////////////////////////////////////////
	Camera::reference Camera::setCanvasSize(vec2 canvasSize)
	{
		m_canvasSize = canvasSize;
		
		updateProj();
		
		return *this;
	}

	/////////////////////////////////////////////////////////////
	vec2 Camera::getCanvasSize() const
	{
		return m_canvasSize;
	}

	/////////////////////////////////////////////////////////////
	vec3 Camera::u() const
	{
		return m_up;
	}

	/////////////////////////////////////////////////////////////
	vec3 Camera::r() const
	{
		return m_right;
	}

	/////////////////////////////////////////////////////////////
	vec3 Camera::v() const
	{
		return m_viewDir;
	}

	/////////////////////////////////////////////////////////////
	vec3 Camera::f() const
	{
		vec3 proj = v().projToPlane(vec3(0,1,0));
		
		if (proj.LENGTH())
			proj = proj.sgn();
		
		return proj;
	}

	/////////////////////////////////////////////////////////////
	vec3 Camera::getViewDir() const
	{
		return v();
	}

	/////////////////////////////////////////////////////////////
	vec3 Camera::getPosition() const
	{
		return m_pos;
	}

	/////////////////////////////////////////////////////////////
	Camera::reference Camera::set3D(vec2 canvasSize,
									vec3 pos,
									vec3 target,
									const Anglef &fov,
									float znear,
									float zfar)
	{
		m_pos   = pos;
		m_zfar  = zfar;
		m_znear = znear;
		m_canvasSize  = canvasSize;
		m_fieldOfView = fov;
		m_projection  = Camera::Perspective;
		
		lookAt(target);
		
		updateProj();
		
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	Camera::reference Camera::set2D(vec2 canvasSize,
									float znear,
									float zfar)
	{
		m_up    = vec3(0,1,0);
		m_pos   = vec3(0,0,0);
		m_right = vec3(1,0,0);
		m_zfar  = zfar;
		m_znear = znear;
		m_canvasSize = canvasSize;
		m_viewDir    = vec3(0,0,-1);
		m_projection = Camera::Orthogonal;
		
		updateProj();
		updateView();
		
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	MatrixStack<4,4,float> &Camera::getProjStack()
	{
		return m_projStack;
	}
	
	/////////////////////////////////////////////////////////////
	MatrixStack<4,4,float> &Camera::getViewStack()
	{
		return m_viewStack;
	}

	/////////////////////////////////////////////////////////////
	Camera::Projection Camera::getProjection() const
	{
		return m_projection;
	}

	/////////////////////////////////////////////////////////////
	Camera::reference Camera::setProjection(Projection proj)
	{
		m_projection = proj;
		
		updateProj();
		
		return *this;
	}
	
	namespace priv
	{
		float signum(float f)
		{
			return f < 0 ? -1 : (f > 0 ? 1 : 0);
		}
	}

	/////////////////////////////////////////////////////////////
	Anglef Camera::getPitch() const
	{
		vec3 realf = f() * priv::signum(u().y);
		
		float dotp = realf.dot(v());
		
		return fm::rad(std::acos(dotp) * priv::signum(v().y));
	}

	/////////////////////////////////////////////////////////////
	Anglef Camera::getYaw() const
	{
		vec3 realf = f() * priv::signum(u().y);
		
		float dotp = realf.dot(vec3(0,0,-1));
		
		return fm::rad(std::acos(dotp) * priv::signum(realf.x));
	}

	/////////////////////////////////////////////////////////////
	Anglef Camera::getRoll() const
	{
		vec3 vj_norm = v().cross(vec3(0,1,0));
		vec3 pn = u().projToPlane(vj_norm).sgn() * priv::signum(r().dot(vj_norm));
		
		float dotp = pn.dot(u());
		
		return fm::rad(std::acos(dotp) * priv::signum(u().dot(vj_norm)));
	}
	
	/////////////////////////////////////////////////////////////
	Camera::reference Camera::setPitch(Anglef pitch)
	{
		return addPitch(pitch.standardize() - getPitch());
	}

	/////////////////////////////////////////////////////////////
	Camera::reference Camera::setYaw(Anglef yaw)
	{
		return addPitch(yaw.standardize() - getYaw());
	}

	/////////////////////////////////////////////////////////////
	Camera::reference Camera::setRoll(Anglef roll)
	{
		return addPitch(roll.standardize() - getRoll());
	}

	/////////////////////////////////////////////////////////////
	Camera::reference Camera::addPitch(Anglef delta)
	{
		Anglef pitch = getPitch();
		if (delta + pitch > fm::deg( 89.99)) delta = fm::deg( 89.99) - pitch;
		if (delta + pitch < fm::deg(-89.99)) delta = fm::deg(-89.99) - pitch;
		
		float cosd = std::cos(delta);
		float sind = std::sin(delta);
		
		vec3 newv = cosd*v() + sind*u();
		vec3 newu = cosd*u() - sind*v();
		
		m_viewDir = newv;
		m_up      = newu;
		
		updateView();
		
		return *this;
	}

	/////////////////////////////////////////////////////////////
	Camera::reference Camera::addYaw(Anglef delta)
	{
		float cosd = std::cos(delta);
		float sind = std::sin(delta);
		
		auto rotateY = [&](vec3 &v) {
			float newx = cosd*v.x - sind*v.z;
			float newy = cosd*v.z + sind*v.x;
			v.x = newx;
			v.z = newy;
		};
		
		rotateY(m_viewDir);
		rotateY(m_right);
		rotateY(m_up);
		
		updateView();
		
		return *this;
	}

	/////////////////////////////////////////////////////////////
	Camera::reference Camera::addRoll(Anglef delta)
	{
		float cosd = std::cos(delta);
		float sind = std::sin(delta);
		
		vec3 newu = cosd*u() + sind*r();
		vec3 newr = cosd*r() - sind*u();
		
		m_up      = newu;
		m_right   = newr;
		
		updateView();
		
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	Camera::reference Camera::setNear(float znear)
	{
		m_znear = znear;
		
		updateProj();
		
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	Camera::reference Camera::setFar(float zfar)
	{
		m_zfar = zfar;
		
		updateProj();
		
		return *this;
	}
	
	/////////////////////////////////////////////////////////////
	float Camera::getNear() const
	{
		return m_znear;
	}
	
	/////////////////////////////////////////////////////////////
	float Camera::getFar() const
	{
		return m_zfar;
	}
	
	/////////////////////////////////////////////////////////////
	bool Camera::getLimitPitch() const
	{
		return m_limitPitch;
	}
	
	/////////////////////////////////////////////////////////////
	Camera::reference Camera::setLimitPitch(bool limit)
	{
		m_limitPitch = limit;
		
		return *this;
	}
}
