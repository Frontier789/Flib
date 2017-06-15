////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2016 Frontier (fr0nt13r789@gmail.com)           ///
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
#ifndef FRONTIER_GUIELEMENT_HPP_INCLUDED
#define FRONTIER_GUIELEMENT_HPP_INCLUDED

#include <FRONTIER/System/CommonTypes.hpp>
#include <FRONTIER/Graphics/Drawable.hpp>
#include <FRONTIER/Gui/GuiCallback.hpp>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/util/API.h>
#include <FRONTIER/System/String.hpp>

#define FRONTIER_GUIELEMENT

namespace fw
{
	class Event;
}

namespace fg
{
	class ShaderManager;
}

namespace fgui
{
	class GuiContext;
	class GuiLayout;
	
	/////////////////////////////////////////////////////////////
	/// @brief Basic gui class 
	///
	/// @ingroup Gui
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API GuiElement : public fg::Drawable, public virtual GuiCallback
	{
		GuiContext *m_context; ///< The owner context of the element
		GuiLayout  *m_layout;  ///< The layout that owns the element
		fm::String m_id; ///< The id of the element
		fm::vec2 m_size; ///< The size of the element
		fm::vec2 m_pos;  ///< The position of the element
	public:
		/////////////////////////////////////////////////////////////
		/// @brief Default constructr
		///
		/// @param owner The owner context
		///
		/////////////////////////////////////////////////////////////
		GuiElement(GuiContext &owner);
		
		/////////////////////////////////////////////////////////////
		/// @brief Default constructr
		/// 
		/// @param owner The owner context
		/// @param size The initial size
		///
		/////////////////////////////////////////////////////////////
		GuiElement(GuiContext &owner,fm::vec2 size);
		
		/////////////////////////////////////////////////////////////
		/// @brief draw the gui element
		///
		/// @param shader The shader to use
		///
		/////////////////////////////////////////////////////////////
		virtual void onDraw(fg::ShaderManager &shader) override;
		
		/////////////////////////////////////////////////////////////
		/// @brief draw the gui element using the context's shader
		///
		/////////////////////////////////////////////////////////////
		void onDraw();

		/////////////////////////////////////////////////////////////
		/// @brief update the gui element
		/// 
		/// @param dt The elapsed time since last update
		/// 
		/////////////////////////////////////////////////////////////
		virtual void onUpdate(const fm::Time &dt) override;

		/////////////////////////////////////////////////////////////
		/// @brief Internally handle an event
		/// 
		/// @param ev The event
		/// 
		/// @return True iff the event got handled
		/// 
		/////////////////////////////////////////////////////////////
		virtual bool onEvent(fw::Event &ev);
		
		/////////////////////////////////////////////////////////////
		/// @brief Handle an event and forward to internal functions
		/// 
		/// @param ev The event
		/// 
		/// @return True iff the event got handled
		/// 
		/////////////////////////////////////////////////////////////
		virtual bool handleEvent(fw::Event &ev) final;
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the size of the gui element
		/// 
		/// @return The size
		/// 
		/////////////////////////////////////////////////////////////
		fm::vec2s getSize() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the size of the gui element
		/// 
		/// @param size The size
		/// 
		/////////////////////////////////////////////////////////////
		virtual void setSize(fm::vec2s size);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the position of the gui element
		/// 
		/// @return The position
		/// 
		/////////////////////////////////////////////////////////////
		fm::vec2i getPosition() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the position of the gui element
		/// 
		/// @param pos The position
		/// 
		/////////////////////////////////////////////////////////////
		virtual void setPosition(fm::vec2i pos);
		
		/////////////////////////////////////////////////////////////
		/// @brief Set owner gui context
		/// 
		/// @param context The owner context
		/// 
		/////////////////////////////////////////////////////////////
		virtual void setOwnerContext(GuiContext &context);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get owner gui context
		/// 
		/// @return The owner context
		/// 
		/////////////////////////////////////////////////////////////
		GuiContext &getOwnerContext();
		
		/////////////////////////////////////////////////////////////
		/// @brief Get owner gui context
		/// 
		/// @return The owner context
		/// 
		/////////////////////////////////////////////////////////////
		const GuiContext &getOwnerContext() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Check if a point falls in the reqion of the gui element
		/// 
		/// @return True iff the point is inside
		/// 
		/////////////////////////////////////////////////////////////
		virtual bool contains(fm::vec2 p) const override;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the layout that owns the element
		/// 
		/// @param layout The layout
		/// 
		/////////////////////////////////////////////////////////////
		virtual void setLayout(GuiLayout *layout);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the layout that owns the element
		/// 
		/// @return The layout
		/// 
		/////////////////////////////////////////////////////////////
		GuiLayout *getLayout() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the id of the element
		/// 
		/// @param id The new id
		/// 
		/////////////////////////////////////////////////////////////
		void setId(const fm::String &id);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the id of the element
		/// 
		/// @return The id
		/// 
		/////////////////////////////////////////////////////////////
		const fm::String &getId() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Find an element by id recursively
		/// 
		/// @param id The id to search
		/// 
		/// @return The element with given id
		/// 
		/////////////////////////////////////////////////////////////
		virtual GuiElement *findById(const fm::String &id);
		
		/////////////////////////////////////////////////////////////
		/// @brief Call a function for every gui element in the hierarchy
		/// 
		/// @param func The func to call
		/// 
		/////////////////////////////////////////////////////////////
		virtual void traverseHierarchy(fm::Delegate<void,GuiElement &> func);
		
		/////////////////////////////////////////////////////////////
		/// @brief Set this element as the active element in the owning layout
		/// 
		/// @param active True to activate false to clear being active
		/// 
		/////////////////////////////////////////////////////////////
		virtual void setActive(bool active = true) override;
		
		/////////////////////////////////////////////////////////////
		/// @brief Check if active in the wning layout
		/// 
		/// @return True iff active
		/// 
		/////////////////////////////////////////////////////////////
		virtual bool isActive() const;
	};
}

#endif // FRONTIER_GUIELEMENT_HPP_INCLUDED
