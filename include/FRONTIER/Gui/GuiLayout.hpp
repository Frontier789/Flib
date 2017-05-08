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
#ifndef FRONTIER_GUILAYOUTBASE_HPP_INCLUDED
#define FRONTIER_GUILAYOUTBASE_HPP_INCLUDED

#include <FRONTIER/System/Delegate.hpp>
#include <FRONTIER/Gui/GuiElement.hpp>
#include <vector>

#define FRONTIER_GUILAYOUTBASE

namespace fgui
{
	/////////////////////////////////////////////////////////////
	/// @brief Base class for every Layout class
	///
	/// @ingroup Gui
	///
	/////////////////////////////////////////////////////////////
	class GuiLayout : public GuiElement
	{
		std::vector<GuiElement*> m_elements; ///< The elements of the layout
		GuiElement *m_activeElement; ///< The active element 
		
	public:
		/////////////////////////////////////////////////////////////
		/// @brief Default constructr
		///
		/// @param owner The owner context
		///
		/////////////////////////////////////////////////////////////
		GuiLayout(GuiContext &owner);
		
		/////////////////////////////////////////////////////////////
		/// @brief Default destructor
		///
		/////////////////////////////////////////////////////////////
		virtual ~GuiLayout();
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the child at given index
		/// 
		/// @param index The index of the child to get
		/// 
		/// @return Pointer to the child
		/// 
		/////////////////////////////////////////////////////////////
		virtual GuiElement *getChildElement(fm::Size index) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the child at given index 
		/// 
		/// @param index The index of the child to get
		/// @param element The new element
		/// 
		/// @return Pointer to the old child at the index
		/// 
		/////////////////////////////////////////////////////////////
		virtual GuiElement *setChildElement(fm::Size index,GuiElement *element);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the number children
		/// 
		/// @return The number of children
		/// 
		/////////////////////////////////////////////////////////////
		virtual fm::Size getChildCount() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the number children
		/// 
		/// @param childCount The number of children
		/// 
		/////////////////////////////////////////////////////////////
		virtual void setChildCount(fm::Size childCount);
		
		/////////////////////////////////////////////////////////////
		/// @brief Apply a function to all the chlidren
		/// 
		/// @param func The function
		/// 
		/////////////////////////////////////////////////////////////
		void forAll(fm::Delegate<void,GuiElement*,fm::Size,GuiLayout&> func);
		
		/////////////////////////////////////////////////////////////
		/// @brief Apply a function to all the chlidren
		/// 
		/// @param func The function
		/// 
		/////////////////////////////////////////////////////////////
		void forAll(fm::Delegate<void,GuiElement*,fm::Size,const GuiLayout&> func) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Remove a child from the layout
		/// 
		/// @param index The index of the child
		/// @param del Whether to call delete on the child
		/// 
		/// @return Pointer to the child (or null if deleted or nonexisting child)
		/// 
		/////////////////////////////////////////////////////////////
		GuiElement *removeChild(fm::Size index,bool del = false);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the child at given index 
		/// 
		/// @param index The index of the child to get
		/// 
		/// @return Pointer to the child
		/// 
		/////////////////////////////////////////////////////////////
		GuiElement *operator[](fm::Size index) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Handle the change of a child
		/// 
		/// @param child The child that changed
		/// 
		/////////////////////////////////////////////////////////////
		virtual void onChildSize(GuiElement *child);
		
		/////////////////////////////////////////////////////////////
		/// @brief draw the gui element
		///
		/// @param shader The shader to use
		///
		/////////////////////////////////////////////////////////////
		virtual void onDraw(fg::ShaderManager &shader) override;

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
		/////////////////////////////////////////////////////////////
		virtual bool onEvent(fw::Event &ev) override;
		
		/////////////////////////////////////////////////////////////
		/// @brief Find an element by id recursively
		/// 
		/// @param id The id to search
		/// 
		/// @return The element with given id
		/// 
		/////////////////////////////////////////////////////////////
		virtual GuiElement *findById(const fm::String &id) override;
		
		/////////////////////////////////////////////////////////////
		/// @brief Add a child element to the Layout
		/// 
		/// @param element The new element
		/// 
		/// @return The number of children the layout has
		/// 
		/////////////////////////////////////////////////////////////
		virtual fm::Size addChildElement(GuiElement *element);
		
		/////////////////////////////////////////////////////////////
		/// @brief Set owner gui context
		/// 
		/// @param context The owner context
		/// 
		/////////////////////////////////////////////////////////////
		virtual void setOwnerContext(GuiContext &context) override;
		
		/////////////////////////////////////////////////////////////
		/// @brief Call a function for every gui element in the hierarchy
		/// 
		/// @param func The func to call
		/// 
		/////////////////////////////////////////////////////////////
		virtual void traverseHierarchy(fm::Delegate<void,GuiElement &> func) override;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the active gui element
		/// 
		/// @param element The new active element
		/// 
		/////////////////////////////////////////////////////////////
		virtual void setActiveElement(GuiElement *element);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the active gui element
		/// 
		/// @return The active element
		/// 
		/////////////////////////////////////////////////////////////
		GuiElement *getActiveElement() const;
	};
}

#endif // FRONTIER_GUILAYOUTBASE_HPP_INCLUDED
