////////////////////////////////////////////////////////////////////////// <!--
/// Copyright (C) 2014-2015 Frontier (fr0nt13r789@gmail.com)           ///
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
#ifndef FRONTIER_TABLE_HPP_INCLUDED
#define FRONTIER_TABLE_HPP_INCLUDED
#include <FRONTIER/System/macros/SIZE.hpp>
#include <FRONTIER/System/Collector.hpp>
#include <FRONTIER/Gui/Widget.hpp>
#include <FRONTIER/Gui/Div.hpp>
#define FRONTIER_TABLE
#include <deque>

namespace Fgui
{
	/////////////////////////////////////////////////////////////
	/// @brief A class used to organize widgets in a table
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API Table : public Widget
	{
	private:
		static const std::string m_className; ///< The name of the class, equals to "Table"

	protected:
		std::deque<float> m_cellHeights; ///< The height of rows
		std::deque<float> m_cellWidths;  ///< The width of columns

		std::deque<std::deque<Div> > m_cells; ///< All cells in the table
		std::deque<std::deque<fm::vec2s> > m_cellSizes; ///< The fixed sizes of all cells

		fm::vec2s m_cellCount;  ///< The number of cells in the table
		fm::vec2  m_realSize;   ///< The final size of the table in pixels
		fm::vec2  m_borderSize; ///< The border size of he table

		Widget *m_activeWidget; ///< The active widget

	public:
		typedef Table &reference;
		typedef const Table &const_reference;

		/////////////////////////////////////////////////////////////
		/// @brief Default constructor
		///
		/// @param name The identifier of the instance
		/// @param anchor The anchor of the widget
		/// @param size The size of the widget
		/// @param parent The parent of the widget
		/// @param cellCount The number of cells in the table
		/// @param borderSize The size of the border
		/// @param cells The cells in the table
		///
		/////////////////////////////////////////////////////////////
		Table(const std::string &name = "NoName",
			  const Anchor &anchor = Anchor(),
			  const fm::vec2 &size = fm::vec2(),
			  Widget *parent = fm::nullPtr,
			  const fm::vec2s &cellCount = fm::vec2s(),
			  const fm::vec2 &borderSize = fm::vec2(),
			  const fm::Collector<Widget *> &cells = fm::Collector<Widget *>());

		/////////////////////////////////////////////////////////////
		/// @brief Default destructor
		///
		/////////////////////////////////////////////////////////////
		virtual ~Table();

		/////////////////////////////////////////////////////////////
		/// @brief Calculate the sizes and positions of cells in the table
		///
		/////////////////////////////////////////////////////////////
		virtual void recalc();

		/////////////////////////////////////////////////////////////
		/// @brief Set the content of a given cell of the table
		///
		/// @param pos The index of the cell
		/// @param newWidget The new content
		/// @param doReclac If true the cell sizes and positions will be recalculated
		///
		/////////////////////////////////////////////////////////////
		virtual void setCell(const fm::vec2s &pos,Widget *newWidget,bool doReclac = true);

		/////////////////////////////////////////////////////////////
		/// @brief Set the fixed size of a given cell of the table
		///
		/// @param pos The index of the cell
		/// @param size The new size
		/// @param doReclac If true the cell sizes and positions will be recalculated
		///
		/////////////////////////////////////////////////////////////
		virtual void setCellSize(const fm::vec2s &pos,const fm::vec2 &size,bool doReclac = true);

		/////////////////////////////////////////////////////////////
		/// @brief Resize the table
		///
		/// New cells are left empty, out-of-bounds cells' content
		/// will be deleted
		///
		/// @param cellCount The new cell count
		///
		/////////////////////////////////////////////////////////////
		virtual void setCellCount(const fm::vec2s &cellCount);

		/////////////////////////////////////////////////////////////
		/// @brief Get the number of cells
		///
		/// @return The cell count
		///
		/////////////////////////////////////////////////////////////
		virtual fm::vec2s getCellCount();

		/////////////////////////////////////////////////////////////
		/// @brief Retrieve the content of a given cell
		///
		/// @param pos The index of the cell
		///
		/// @return The content with type Widget*
		///
		/////////////////////////////////////////////////////////////
		virtual Widget *getCell(const fm::vec2s &pos);


		/////////////////////////////////////////////////////////////
		/// @brief Retrieve the content of a given cell of a given type
		///
		/// @param pos The index of the cell
		///
		/// @return The content with the given type WidgetDerived*
		///
		/////////////////////////////////////////////////////////////
		template<class WidgetDerived>
		WidgetDerived *getCell(const fm::vec2s &pos);

		/////////////////////////////////////////////////////////////
		/// @brief Set the anchor of the widget
		///
		/// @param anchor The new anchor
		///
		/////////////////////////////////////////////////////////////
		virtual void setAnchor(const Anchor &anchor);

		/////////////////////////////////////////////////////////////
		/// @brief Set the parent of the widget
		///
		/// @param parent The new parent
		///
		/////////////////////////////////////////////////////////////
		virtual void setParent(Widget *parent);

		/////////////////////////////////////////////////////////////
		/// @brief Get the minimal size of the widget in pixels
		///
		/// @return The minimal size
		///
		/////////////////////////////////////////////////////////////
		virtual fm::vec2 getSize() const;

		/////////////////////////////////////////////////////////////
		/// @brief Set the minimal size of the widget in pixels
		///
		/// @param size The new minimal size in pixels
		///
		/////////////////////////////////////////////////////////////
		virtual void setSize(const fm::vec2 &size);

		/////////////////////////////////////////////////////////////
		/// @brief Set the active widget
		///
		/// By default this function forwards to this widget's parent
		///
		/// @param active The new active widget
		///
		/////////////////////////////////////////////////////////////
		virtual void setActive(Widget *active);

		/////////////////////////////////////////////////////////////
		/// @brief Get the active widget
		///
		/// @return active The active widget
		///
		/////////////////////////////////////////////////////////////
		virtual Widget *getActive();

		/////////////////////////////////////////////////////////////
		/// @brief Search the cells for a widget with a given name
		///
		/// @param name The name to search for
		///
		/// @return The widget if found (NULL otherwise)
		///
		/////////////////////////////////////////////////////////////
		virtual Widget *findNamed(const std::string &name);

		/////////////////////////////////////////////////////////////
		/// @brief Draw the table
		///
		/// Forwards the call to each cell
		///
		/////////////////////////////////////////////////////////////
		virtual void onDraw();

		/////////////////////////////////////////////////////////////
		/// @brief Handle the fixed update
		///
		/// Forwards the call to each cell
		///
		/////////////////////////////////////////////////////////////
		virtual void onUpdate();

		/////////////////////////////////////////////////////////////
		/// @brief Handle the change of parent
		///
		/// Forwards the call to each cell
		///
		/////////////////////////////////////////////////////////////
		virtual void onParentChange();

		/////////////////////////////////////////////////////////////
		/// @brief Handle an event
		///
		/// Forwards the call to each cell
		///
		/////////////////////////////////////////////////////////////
		virtual bool handleEvent(const fw::Event &ev);

		/////////////////////////////////////////////////////////////
		/// @brief Get the real type of the class
		///
		/// When creating a new class it may override this
		/// function to provide a unique name as return value
		///
		/////////////////////////////////////////////////////////////
		virtual const std::string &getRealType() const;
	};
}

#endif // FRONTIER_TABLE_HPP_INCLUDED

#ifndef FRONTIER_DONT_INCLUDE_INL
	#include <FRONTIER/Gui/Table.inl>
#endif // FRONTIER_DONT_INCLUDE_INL
