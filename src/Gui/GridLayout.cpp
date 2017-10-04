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
#include <FRONTIER/Gui/GridLayout.hpp>
#include <FRONTIER/System/util/C.hpp>

namespace fgui
{
	/////////////////////////////////////////////////////////////
	GridLayout::CellData::CellData() : alignPoint(.5,.5)
	{
		
	}
	
	/////////////////////////////////////////////////////////////
	void GridLayout::updateCellPositions()
	{
		fm::vec2  maxDist;
		fm::vec2  pixPos;
		fm::vec2s cellc = getCellCount();
		fm::vec2  pos = getPosition();
		
		std::vector<float> realColWs = m_colWidths;
		std::vector<float> realRowHs = m_rowHeights;
		
		Cv(cellc)
		{
			GuiElement *elem = getChildElement2D(p);
			fm::Size index = indexToOffset(p);
			CellData &data = m_cellData[index];
			
			if (elem)
			{
				fm::vec2 s = elem->getSize();
				
				fm::vec2 padding;
				padding.w = std::max(data.padding.w,m_allPadding.w);
				padding.h = std::max(data.padding.h,m_allPadding.h);
				
				realColWs[p.x] = std::max(realColWs[p.x],s.w + padding.w*2);
				realRowHs[p.y] = std::max(realRowHs[p.y],s.h + padding.h*2);
			}
			
			realColWs[p.x] = std::max(realColWs[p.x],std::max(data.minSize.w,m_allMinSize.w));
			realRowHs[p.y] = std::max(realRowHs[p.y],std::max(data.minSize.h,m_allMinSize.h));
		}
		
		Cy(cellc.h)Cx(cellc.w)
		{
			fm::vec2s p(x,y);
			float cellW = realColWs[p.x];
			float cellH = realRowHs[p.y];
			fm::Size index = indexToOffset(p);
			CellData &data = m_cellData[index];
			
			GuiElement *elem = getChildElement2D(p);
			if (elem)
			{
				fm::vec2 cellS(cellW,cellH);
				fm::vec2 elemS = elem->getSize();
				
				elem->setPosition(pixPos + pos + (cellS - elemS)*data.alignPoint);
			}
			
			pixPos.x += cellW;
			maxDist.w = std::max(maxDist.w,pixPos.x);
			
			if (p.x+1 == cellc.w)
			{
				pixPos.x  = 0;
				pixPos.y += cellH;
				maxDist.h = std::max(maxDist.h,pixPos.y);
			}
		}
		
		setSize(maxDist);
	}
	
	/////////////////////////////////////////////////////////////
	fm::Size GridLayout::indexToOffset(fm::vec2s index) const
	{
		return index.y * m_columnCount + index.x;
	}
	
	/////////////////////////////////////////////////////////////
	void GridLayout::setPosition(fm::vec2i pos)
	{
		fm::vec2 oldPos = getPosition();
		
		Cv(getCellCount())
		{
			GuiElement *elem = getChildElement2D(p);
			if (elem)
				elem->setPosition(elem->getPosition() + pos - oldPos);
		}
		
		GuiLayout::setPosition(pos);
	}
	
	/////////////////////////////////////////////////////////////
	GridLayout::GridLayout(GuiContext &owner) : GuiLayout(owner),
												m_columnCount(0)
	{
		
	}
		
	/////////////////////////////////////////////////////////////
	void GridLayout::setCellSize(fm::vec2s index,fm::vec2 size)
	{
		m_cellData[indexToOffset(index)].minSize = size;
		updateCellPositions();
	}
	
	/////////////////////////////////////////////////////////////
	fm::vec2 GridLayout::getCellSize(fm::vec2s index) const
	{
		return m_cellData[indexToOffset(index)].minSize;
	}

	/////////////////////////////////////////////////////////////
	GuiElement *GridLayout::setChildElement2D(fm::vec2s index,GuiElement *element)
	{
		return GuiLayout::setChildElement(indexToOffset(index),element);
	}

	/////////////////////////////////////////////////////////////
	GuiElement *GridLayout::getChildElement2D(fm::vec2s index) const
	{
		return GuiLayout::getChildElement(indexToOffset(index));
	}

	/////////////////////////////////////////////////////////////
	GuiElement *GridLayout::removeChild(fm::vec2s index,bool del)
	{
		return GuiLayout::removeChild(indexToOffset(index),del);
	}

	/////////////////////////////////////////////////////////////
	void GridLayout::onChildSize(GuiElement *)
	{
		updateCellPositions();
	}
	
	/////////////////////////////////////////////////////////////
	void GridLayout::setCellCount(fm::vec2s cellCount,bool del)
	{
		std::vector<GuiElement*> elements_cpy(cellCount.area(),nullptr);
		std::vector<CellData> cellData_cpy(cellCount.area());
		
		fm::vec2s cellnum = getCellCount();
		
		Cx(std::max(cellCount.w,cellnum.w))
		{
			Cy(std::max(cellCount.h,cellnum.h))
			{
				bool inOld = x < cellnum.w && y < cellnum.h;
				bool inNew = x < cellCount.w && y < cellCount.h;
				
				fm::Size oldIndex = y * cellnum.w   + x;
				fm::Size newIndex = y * cellCount.w + x;
				
				if (inOld && inNew)
				{
					elements_cpy[newIndex] = m_elements[oldIndex];
					cellData_cpy[newIndex] = m_cellData[oldIndex];
				}
				if (inOld && !inNew)
				{
					if (del)
						delete m_elements[oldIndex];
				}
			}
		}
		
		m_colWidths.resize(cellCount.w,0);
		m_rowHeights.resize(cellCount.h,0);
		
		m_columnCount = cellCount.w;
		
		m_cellData.swap(cellData_cpy);
		m_elements.swap(elements_cpy);
	}
	
	/////////////////////////////////////////////////////////////
	fm::vec2s GridLayout::getCellCount() const
	{
		fm::Size chl = getChildCount();
		return fm::vec2s(m_columnCount,m_columnCount ? (chl + m_columnCount - 1) / m_columnCount : 0);
	}
	
	/////////////////////////////////////////////////////////////
	void GridLayout::setChildCount(fm::Size childCount)
	{
		if (!m_columnCount)
			m_columnCount = 1;
		
		setCellCount(fm::vec2s(m_columnCount,(childCount + m_columnCount - 1) / m_columnCount));
	}
		
	/////////////////////////////////////////////////////////////
	void GridLayout::setCellSize(fm::vec2 size)
	{
		m_allMinSize = size;
		updateCellPositions();
	}
	
	/////////////////////////////////////////////////////////////
	fm::vec2 GridLayout::getCellSize() const
	{
		return m_allMinSize;
	}
	
	/////////////////////////////////////////////////////////////
	void GridLayout::setColumnWidth(fm::Size colIndex,float width)
	{
		m_colWidths[colIndex] = width;
		updateCellPositions();
	}
	
	/////////////////////////////////////////////////////////////
	float GridLayout::getColumnWidth(fm::Size colIndex) const
	{
		return m_colWidths[colIndex];
	}
	
	/////////////////////////////////////////////////////////////
	void GridLayout::setRowHeight(fm::Size rowIndex,float height)
	{
		m_rowHeights[rowIndex] = height;
		updateCellPositions();
	}
	
	/////////////////////////////////////////////////////////////
	float GridLayout::getRowHeight(fm::Size rowIndex) const
	{
		return m_rowHeights[rowIndex];
	}
	
	/////////////////////////////////////////////////////////////
	void GridLayout::setPadding(fm::vec2s index,fm::vec2 padding)
	{
		m_cellData[indexToOffset(index)].padding = padding;
		updateCellPositions();
	}
	
	/////////////////////////////////////////////////////////////
	fm::vec2 GridLayout::getPadding(fm::vec2s index) const
	{
		return m_cellData[indexToOffset(index)].padding;
	}
	
	/////////////////////////////////////////////////////////////
	void GridLayout::setPadding(fm::vec2 padding)
	{
		m_allPadding = padding;
		updateCellPositions();
	}
	
	/////////////////////////////////////////////////////////////
	fm::vec2 GridLayout::getPadding() const
	{
		return m_allPadding;
	}
		
	/////////////////////////////////////////////////////////////
	void GridLayout::setAlignPoint(fm::vec2s index,fm::vec2 alignPoint)
	{
		m_cellData[indexToOffset(index)].alignPoint = alignPoint;
	}
	
	/////////////////////////////////////////////////////////////
	fm::vec2 GridLayout::setAlignPoint(fm::vec2s index) const
	{
		return m_cellData[indexToOffset(index)].alignPoint;
	}
}
