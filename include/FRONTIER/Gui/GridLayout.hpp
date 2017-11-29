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
#ifndef FRONTIER_GRIDLAYOUT_HPP_INCLUDED
#define FRONTIER_GRIDLAYOUT_HPP_INCLUDED

#include <FRONTIER/Gui/GuiLayout.hpp>
#include <vector>

#define FRONTIER_GRIDLAYOUT

namespace fgui
{
	/////////////////////////////////////////////////////////////
	/// @brief A layout manager capable of handling a table (grid)
	///
	/// @ingroup Gui
	///
	/////////////////////////////////////////////////////////////
	class FRONTIER_API GridLayout : public GuiLayout
	{
		std::vector<float> m_rowHeights; ///< The calculated height for all rows
		std::vector<float> m_colWidths;  ///< The calculated width for all columns
		fm::Size m_columnCount; ///< The number of columns
		
		/////////////////////////////////////////////////////////////
		/// @brief Internal function used to convert index to offset
		///
		/// @param index The 2d index to convert to offset
		///
		/// @return The offset
		///
		/////////////////////////////////////////////////////////////
		fm::Size indexToOffset(fm::vec2s index) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Internal function used update cell positions
		///
		/////////////////////////////////////////////////////////////
		void updateCellPositions();
		
		/////////////////////////////////////////////////////////////
		/// @brief Internal data structure used to keep track of cell properties
		///
		/////////////////////////////////////////////////////////////
		class CellData
		{
		public:
			fm::vec2 alignPoint; ///< The align of the cell content
			fm::vec2 minSize; ///< The required minimal size of the cell
			fm::vec2 padding; ///< The padding of the cell
			
			CellData(); ///< Default constructor
		};
		
		std::vector<CellData> m_cellData; ///< The cell properties
		fm::vec2 m_allMinSize; ///< The global minimal cell size
		fm::vec2 m_allPadding; ///< The global minimal padding
		
	public:
		/////////////////////////////////////////////////////////////
		/// @brief Default constructr
		///
		/// @param owner The owner context
		///
		/////////////////////////////////////////////////////////////
		GridLayout(GuiContext &owner);
		
		/////////////////////////////////////////////////////////////
		/// @brief Default destructor
		///
		/////////////////////////////////////////////////////////////
		virtual ~GridLayout() = default;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the position of the gui element
		/// 
		/// @param pos The position
		/// 
		/////////////////////////////////////////////////////////////
		virtual void setPosition(fm::vec2i pos) override;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the child at given 2d index 
		/// 
		/// @param index The 2d index of the child to set
		/// @param element The new element
		/// 
		/// @return Pointer to the old child at the index
		/// 
		/////////////////////////////////////////////////////////////
		virtual GuiElement *setChildElement2D(fm::vec2s index,GuiElement *element);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the child at given 2d index
		/// 
		/// @param index The 2d index of the child to get
		/// 
		/// @return Pointer to the child
		/// 
		/////////////////////////////////////////////////////////////
		virtual GuiElement *getChildElement2D(fm::vec2s index) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Remove a child from the layout at a 2d index
		/// 
		/// @param index The 2d index of the child
		/// @param del Whether to call delete on the child
		/// 
		/// @return Pointer to the child (or null if deleted or nonexisting child)
		/// 
		/////////////////////////////////////////////////////////////
		GuiElement *removeChild(fm::vec2s index,bool del = false);
		
		/////////////////////////////////////////////////////////////
		/// @brief Handle the change of a child
		/// 
		/// @param child The child that changed
		/// 
		/////////////////////////////////////////////////////////////
		virtual void onChildSize(GuiElement *child) override;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the number of cells in the grid
		/// 
		/// @param cellCount The number of cells
		/// @param del True to delete obsolete cells
		/// 
		/////////////////////////////////////////////////////////////
		virtual void setCellCount(fm::vec2s cellCount,bool del = true);
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the number children
		/// 
		/// @param childCount The number of children
		/// 
		/////////////////////////////////////////////////////////////
		virtual void setChildCount(fm::Size childCount) override;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the minimum size for a cell in pixels
		/// 
		/// @param index The index of the cell
		/// @param size The min size of the cell
		/// 
		/////////////////////////////////////////////////////////////
		virtual void setCellSize(fm::vec2s index,fm::vec2 size);
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the minimum size for all cells, in pixels
		/// 
		/// does not override per-cell settings
		/// 
		/// @param size The min size of all cells
		/// 
		/////////////////////////////////////////////////////////////
		virtual void setCellSize(fm::vec2 size);
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the minimum width of a column
		/// 
		/// @param colIndex The index of the column
		/// @param width The min width of the column
		/// 
		/////////////////////////////////////////////////////////////
		virtual void setColumnWidth(fm::Size colIndex,float width);
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the minimum height of a row
		/// 
		/// @param rowIndex The index of the row
		/// @param height The min height of the row
		/// 
		/////////////////////////////////////////////////////////////
		virtual void setRowHeight(fm::Size rowIndex,float height);
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the padding for a cell
		/// 
		/// @param index The 2d index
		/// @param padding The new padding
		/// 
		/////////////////////////////////////////////////////////////
		virtual void setPadding(fm::vec2s index,fm::vec2 padding);
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the padding for all cell
		/// 
		/// does not override per-cell settings
		/// 
		/// @param padding The new global padding
		/// 
		/////////////////////////////////////////////////////////////
		virtual void setPadding(fm::vec2 padding);
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the align point for a cell
		/// 
		/// @param index The 2d index
		/// @param alignPoint The new align point
		/// 
		/////////////////////////////////////////////////////////////
		virtual void setAlignPoint(fm::vec2s index,fm::vec2 alignPoint);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the number of cells
		/// 
		/// @return The number of cells
		/// 
		/////////////////////////////////////////////////////////////
		fm::vec2s getCellCount() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the minimum size of a cell
		/// 
		/// @param index The index 2d of the cell
		/// 
		/// @return minimal size of the cell
		/// 
		/////////////////////////////////////////////////////////////
		fm::vec2 getCellSize(fm::vec2s index) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the minimum size for all cells, in pixels
		/// 
		/// @return The minimum size for all cells, in pixels
		/// 
		/////////////////////////////////////////////////////////////
		fm::vec2 getCellSize() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the minimal width of a column
		/// 
		/// @param colIndex The index of the column
		/// 
		/// @return The minimal width of the column
		/// 
		/////////////////////////////////////////////////////////////
		float getColumnWidth(fm::Size colIndex) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the minimal height of a row
		/// 
		/// @param rowIndex The index of the row
		/// 
		/// @return The minimal height of the row
		/// 
		/////////////////////////////////////////////////////////////
		float getRowHeight(fm::Size rowIndex) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the padding for a cell
		/// 
		/// @param index The 2d index
		///
		/// @return The padding
		/// 
		/////////////////////////////////////////////////////////////
		fm::vec2 getPadding(fm::vec2s index) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Set the padding for all cell
		/// 
		/// @return The global padding
		/// 
		/////////////////////////////////////////////////////////////
		fm::vec2 getPadding() const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the align point for a cell
		/// 
		/// @param index The 2d index
		/// 
		/// @return The align point
		/// 
		/////////////////////////////////////////////////////////////
		fm::vec2 setAlignPoint(fm::vec2s index) const;
	};
}

#endif // FRONTIER_GRIDLAYOUT_HPP_INCLUDED
