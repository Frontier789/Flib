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
#ifndef FRONTIER_TRANSFORMLISTENER_HPP_INCLUDED
#define FRONTIER_TRANSFORMLISTENER_HPP_INCLUDED

#include <FRONTIER/System/CommonTypes.hpp>
#include <FRONTIER/Gui/ScrollListener.hpp>
#include <FRONTIER/Gui/ClickListener.hpp>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/util/API.h>
#include <FRONTIER/System/Matrix.hpp>
#include <FRONTIER/System/Angle.hpp>

#define FRONTIER_TRANSFORMLISTENER

namespace fgui
{
	/////////////////////////////////////////////////////////////
	/// @brief Listener for 2d transformations such as translation, zoom, rotation
	///
	/// @ingroup Gui
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API TransformListener : public virtual ScrollListener, public virtual ClickListener
	{
		fm::Delegate<void, fm::mat4> m_cb; ///< Called when the transformation matrix changes 
		fm::vec2 m_fstRight; ///< The position where the right button was pressed first
		float m_rotMinDist;  ///< Minimal distance the mouse needs to travel before rotation kicks is
		fm::vec2 m_offset;   ///< The transformation offset
		float m_dragSens; ///< Stores the sensitivity of the offset
		float m_zoomSens; ///< Stores the sensitivity of zoom
		fm::Anglef m_rot; ///< The rotation
		float m_zoom;     ///< The zoom
		
		void callCb(); ///< Internal function
		
	public:
		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		/// 
		/////////////////////////////////////////////////////////////
		TransformListener();
		
		/////////////////////////////////////////////////////////////
		/// @brief Called when the element is scrolled
		/// 
		/// @param amount The amount the element is scrolled
		/// 
		/////////////////////////////////////////////////////////////
		virtual void onScroll(float amount) override;
		
		/////////////////////////////////////////////////////////////
		/// @brief Called when the mouse moves inside the gui element
		/// 
		/// @param p The position of the mouse after moving
		/// @param prevP The position of the mouse before moving
		/// 
		/////////////////////////////////////////////////////////////
		virtual void onMouseMove(fm::vec2 p,fm::vec2 prevP) override;
		
		/////////////////////////////////////////////////////////////
		/// @brief Called when the offset of the transformation changes
		/// 
		/// @param offset The new offset
		/// 
		/////////////////////////////////////////////////////////////
		virtual void onOffsetChanged(fm::vec2 offset);
		
		/////////////////////////////////////////////////////////////
		/// @brief Called when the zoom of the transformation changes
		/// 
		/// @param zoom The new zoom
		/// 
		/////////////////////////////////////////////////////////////
		virtual void onZoomChanged(float zoom);
		
		/////////////////////////////////////////////////////////////
		/// @brief Called when the rotation of the transformation changes
		/// 
		/// @param rot The new rotation
		/// 
		/////////////////////////////////////////////////////////////
		virtual void onRotationChanged(fm::Anglef rot);
		
		/////////////////////////////////////////////////////////////
		/// @brief Called when the the transformation changes in any way
		/// 
		/////////////////////////////////////////////////////////////
		virtual void onTransform() {};
		
		/////////////////////////////////////////////////////////////
		/// @brief Called when a mouse button is pressed while inside the gui element
		/// 
		/// @param button The button that was pressed
		/// @param p The position of the mouse
		/// 
		/////////////////////////////////////////////////////////////
		virtual void onPress(fw::Mouse::Button button,fm::vec2 p) override;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the current offset
		/// 
		/// @param offset The current offset
		/// 
		/////////////////////////////////////////////////////////////
		void setOffset(fm::vec2 offset);
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the current zoom
		/// 
		/// @param zoom The current zoom
		/// 
		/////////////////////////////////////////////////////////////
		void setZoom(float zoom);
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the current rotation
		/// 
		/// @param rot The current rotation
		/// 
		/////////////////////////////////////////////////////////////
		void setRotation(fm::Anglef rot);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the current offset
		/// 
		/// @return The current offset
		/// 
		/////////////////////////////////////////////////////////////
		fm::vec2 getOffset() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the current zoom
		/// 
		/// @return The current zoom
		/// 
		/////////////////////////////////////////////////////////////
		float getZoom() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the current rotation
		/// 
		/// @return The current rotation
		/// 
		/////////////////////////////////////////////////////////////
		fm::Anglef getRotation() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the current drag sensitivity
		/// 
		/// When the gui element detects a drag by the mouse
		/// The offset changes by r*sens where r is the cursor
		/// Position's delta
		/// 
		/// @param sens The new drag sensitivity
		/// 
		/////////////////////////////////////////////////////////////
		void setDragSensitivity(float sens);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the current drag sensitivity
		/// 
		/// @return The current drag sensitivity
		/// 
		/////////////////////////////////////////////////////////////
		float getDragSensitivity() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the current zoom sensitivity
		/// 
		/// @param sens The new zoom sensitivity
		/// 
		/////////////////////////////////////////////////////////////
		void setZoomSensitivity(float sens);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the current zoom sensitivity
		/// 
		/// @return The current zoom sensitivity
		/// 
		/////////////////////////////////////////////////////////////
		float getZoomSensitivity() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the minimal distance the mouse needs to travel before rotation starts
		/// 
		/// @param minDist The minimal distance
		/// 
		/////////////////////////////////////////////////////////////
		void setRotationMinDistance(float minDist);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the minimal distance the mouse needs to travel before rotation starts
		/// 
		/// @return The minimal distance
		/// 
		/////////////////////////////////////////////////////////////
		float getRotationMinDistance() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the center of the ongoing rotation
		/// 
		/// @return The center
		/// 
		/////////////////////////////////////////////////////////////
		fm::vec2 getRotationCenter() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Check if currently in offsetting mode
		/// 
		/// @return True iff in offsetting mode
		/// 
		/////////////////////////////////////////////////////////////
		bool inOffsetMode() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Check if currently in rotating mode
		/// 
		/// @return True iff in rotating mode
		/// 
		/////////////////////////////////////////////////////////////
		bool inRotationMode() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Check if currently in zooming mode
		/// 
		/// @return True iff in zooming mode
		/// 
		/////////////////////////////////////////////////////////////
		bool inZoomMode() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the transformation matrix defined by the listener
		/// 
		/// @return The matrix
		/// 
		/////////////////////////////////////////////////////////////
		fm::mat4 getTransformMatrix() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the inverse transformation matrix defined by the listener
		/// 
		/// @return The matrix
		/// 
		/////////////////////////////////////////////////////////////
		fm::mat4 getInvTransformMatrix() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the callback of the listener
		/// 
		/////////////////////////////////////////////////////////////
		void setCallback(fm::Delegate<void,fm::mat4> callback);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the callback of the listener
		/// 
		/////////////////////////////////////////////////////////////
		fm::Delegate<void,fm::mat4> getCallback();
	};
	
}

#endif // FRONTIER_TRANSFORMLISTENER_HPP_INCLUDED
