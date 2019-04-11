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
#include <FRONTIER/Gui/TransformListener.hpp>
#include <FRONTIER/System/Polar2.hpp>
#include <FRONTIER/Window/Event.hpp>

namespace fgui
{
	/////////////////////////////////////////////////////////////
	TransformListener::TransformListener() : m_rotMinDist(5),
											 m_dragSens(1),
											 m_zoomSens(2),
											 m_zoom(1)
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	void TransformListener::onScroll(float amount,bool horizontal)
	{
		if (horizontal) return;
		
		float zoomMul = std::pow(m_zoomSens,amount);
		m_offset = m_offset * zoomMul + getLastMousePos() * (1 - zoomMul);
		
		applyZoom(m_zoom * zoomMul,true);
	}
	
	/////////////////////////////////////////////////////////////
	void TransformListener::onMouseMove(fm::vec2 p,fm::vec2 prevP)
	{
		if (isPressed(fw::Mouse::Left))
			applyOffset(m_offset + (p-prevP) * m_dragSens,true);
		
		auto rot = [](fm::vec2 v,fm::Anglef a) -> fm::vec2 {
			fm::pol2 p = v;
			p.angle += a;
			return p;
		};
		
		if (isPressed(fw::Mouse::Right))
		{
			fm::vec2 vCur  = p     - m_fstRight;
			fm::vec2 vPrev = prevP - m_fstRight;
			
			if (vCur.length() > 10)
			{
				fm::Anglef dAngle = fm::pol2(vCur).angle - fm::pol2(vPrev).angle;
				
				applyRotation(m_rot + dAngle,true);
				applyOffset(m_fstRight + rot(m_offset - m_fstRight, dAngle),true);
			}
		}
	}
	
	/////////////////////////////////////////////////////////////
	void TransformListener::onOffsetChanged(fm::vec2 offset)
	{
		(void)offset;
	}
	
	/////////////////////////////////////////////////////////////
	void TransformListener::onZoomChanged(float zoom)
	{
		(void)zoom;
	}
	 
	/////////////////////////////////////////////////////////////
	void TransformListener::onRotationChanged(fm::Anglef rot)
	{
		(void)rot;
	}

	/////////////////////////////////////////////////////////////
	void TransformListener::onPress(fw::Mouse::Button button,fm::vec2 p)
	{
		if (button == fw::Mouse::Right)
			m_fstRight = p;
	}
	
	/////////////////////////////////////////////////////////////
	void TransformListener::setOffset(fm::vec2 offset)
	{
		applyOffset(offset,false);
	}
	
	/////////////////////////////////////////////////////////////
	void TransformListener::setZoom(float zoom)
	{
		applyZoom(zoom,false);
	}
	
	/////////////////////////////////////////////////////////////
	void TransformListener::setRotation(fm::Anglef rot)
	{
		applyRotation(rot,false);
	}
	
	/////////////////////////////////////////////////////////////
	void TransformListener::applyOffset(fm::vec2 offset,bool userAction)
	{
		m_offset = offset;
		onTransform(userAction);
		onOffsetChanged(m_offset);
		callCb();
	}
	
	/////////////////////////////////////////////////////////////
	void TransformListener::applyZoom(float zoom,bool userAction)
	{
		m_zoom = zoom;
		onTransform(userAction);
		onZoomChanged(m_zoom);
		callCb();
	}
	
	/////////////////////////////////////////////////////////////
	void TransformListener::applyRotation(fm::Anglef rot,bool userAction)
	{
		m_rot = rot;
		onTransform(userAction);
		onRotationChanged(m_rot);
		callCb();
	}
	
	/////////////////////////////////////////////////////////////
	fm::vec2 TransformListener::getOffset() const
	{
		return m_offset;
	}
	
	/////////////////////////////////////////////////////////////
	float TransformListener::getZoom() const
	{
		return m_zoom;
	}
	
	/////////////////////////////////////////////////////////////
	fm::Anglef TransformListener::getRotation() const
	{
		return m_rot;
	}
	
	/////////////////////////////////////////////////////////////
	void TransformListener::setDragSensitivity(float sens)
	{
		m_dragSens = sens;
	}
	
	/////////////////////////////////////////////////////////////
	float TransformListener::getDragSensitivity() const
	{
		return m_dragSens;
	}
	
	/////////////////////////////////////////////////////////////
	void TransformListener::setZoomSensitivity(float sens)
	{
		m_zoomSens = sens;
		
		if (m_zoomSens < 0)
			m_zoomSens *= -1;
	}
	
	/////////////////////////////////////////////////////////////
	float TransformListener::getZoomSensitivity() const
	{
		return m_zoomSens;
	}
		
	/////////////////////////////////////////////////////////////
	void TransformListener::setRotationMinDistance(float minDist)
	{
		m_rotMinDist = minDist;
	}
	
	/////////////////////////////////////////////////////////////
	float TransformListener::getRotationMinDistance() const
	{
		return m_rotMinDist;
	}
	
	/////////////////////////////////////////////////////////////
	fm::vec2 TransformListener::getRotationCenter() const
	{
		return m_fstRight;
	}
	
	/////////////////////////////////////////////////////////////
	bool TransformListener::inOffsetMode() const
	{
		return isPressed(fw::Mouse::Left);
	}
	
	/////////////////////////////////////////////////////////////
	bool TransformListener::inRotationMode() const
	{
		return isPressed(fw::Mouse::Right);
	}
	
	/////////////////////////////////////////////////////////////
	bool TransformListener::inZoomMode() const
	{
		return false;
	}
	
	/////////////////////////////////////////////////////////////
	fm::mat4 TransformListener::getTransformMatrix() const
	{
		return fm::MATRIX::translation(getOffset()) * 
			   fm::MATRIX::scaling(fm::vec2(getZoom())) *
			   fm::MATRIX::rotation(getRotation());
	}
	
	/////////////////////////////////////////////////////////////
	fm::mat4 TransformListener::getInvTransformMatrix() const
	{
		return fm::MATRIX::rotation(-getRotation()) * 
			   fm::MATRIX::scaling(fm::vec2(1.f/getZoom())) *
			   fm::MATRIX::translation(-getOffset());
	}
	
	/////////////////////////////////////////////////////////////
	void TransformListener::setCallback(fm::Delegate<void,fm::mat4> callback)
	{
		m_cb = callback;
	}
	
	/////////////////////////////////////////////////////////////
	fm::Delegate<void,fm::mat4> TransformListener::getCallback()
	{
		return m_cb;
	}
	
	/////////////////////////////////////////////////////////////
	void TransformListener::callCb()
	{
		if (m_cb) m_cb(getTransformMatrix());
	}
}
