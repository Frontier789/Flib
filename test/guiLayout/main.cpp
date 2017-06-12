#include <Frontier.hpp>
#include <iostream>

using namespace std;

#include "ColorPicker.hpp"

namespace fgui
{
	enum CellAlign {
		CellTopLeft,
		CellCenter
	};
	
	/////////////////////////////////////////////////////////////
	/// @brief A layout manager capable of handling a table (grid)
	///
	/////////////////////////////////////////////////////////////
	class GridLayout : public GuiLayout
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
			fm::vec2 minSize; ///< The required minimal size of the cell
			fm::vec2 padding; ///< The padding of the cell
			CellAlign align;  ///< The align of the cell content
			
			CellData(); ///< Default constructor
		};
		
		std::vector<CellData> m_cellData;
		fm::vec2 m_allMinSize;
		fm::vec2 m_allPadding;
		
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
		/// @param index The 2d index of the child to get
		/// @param element The new element
		/// 
		/// @return Pointer to the old child at the index
		/// 
		/////////////////////////////////////////////////////////////
		virtual GuiElement *setChildElement(fm::vec2s index,GuiElement *element);
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the child at given 2d index
		/// 
		/// @param index The 2d index of the child to get
		/// 
		/// @return Pointer to the child
		/// 
		/////////////////////////////////////////////////////////////
		virtual GuiElement *getChildElement(fm::vec2s index) const;
		
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
		/// @param colIndex The index of the row
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
		/// @brief Get the number of cells
		/// 
		/// @return The number of cells
		/// 
		/////////////////////////////////////////////////////////////
		fm::vec2s getCellCount(bool del = false) const;
		
		/////////////////////////////////////////////////////////////
		/// @brief Get the minimum size of a cell
		/// 
		/// @param element The new element
		/// 
		/// @return The number of children the layout has
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
		/// @param colIndex The index of the row
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
	};
	
	/////////////////////////////////////////////////////////////
	GridLayout::CellData::CellData() : align(CellCenter)
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
			GuiElement *elem = getChildElement(p);
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
			
			GuiElement *elem = getChildElement(p);
			if (elem)
			{
				if (data.align == CellTopLeft)
					elem->setPosition(pixPos + pos);
				
				if (data.align == CellCenter)
					elem->setPosition(pixPos + pos + (fm::vec2(cellW,cellH) - elem->getSize())/2 );
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
			GuiElement *elem = getChildElement(p);
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
	GuiElement *GridLayout::setChildElement(fm::vec2s index,GuiElement *element)
	{
		return GuiLayout::setChildElement(indexToOffset(index),element);
	}

	/////////////////////////////////////////////////////////////
	GuiElement *GridLayout::getChildElement(fm::vec2s index) const
	{
		return GuiLayout::getChildElement(indexToOffset(index));
	}

	/////////////////////////////////////////////////////////////
	GuiElement *GridLayout::removeChild(fm::vec2s index,bool del)
	{
		return GuiLayout::removeChild(indexToOffset(index),del);
	}

	/////////////////////////////////////////////////////////////
	void GridLayout::onChildSize(GuiElement *child)
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
	fm::vec2s GridLayout::getCellCount(bool del) const
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
}

int main()
{
	GuiWindow win(vec2(640,480),"gui");
	
	int push_count = 0;
	
	GridLayout *l = new GridLayout(win);
	
	l->setCellCount(vec2(2,3));
	
	l->setChildElement(vec2(0,0),new GuiText(win,"R: "));
	l->setChildElement(vec2(0,1),new GuiText(win,"G: "));
	l->setChildElement(vec2(0,2),new GuiText(win,"B: "));
	
	Color colorRD(0,0,0);
	
	auto updateTitle = [&](){
		win.setTitle("gui (" + fm::toString(colorRD.r) + "," 
							 + fm::toString(colorRD.g) + ","
							 + fm::toString(colorRD.b) + ")");
	};
	
	l->setChildElement(vec2(1,0),new ScrollBar(win,vec2(50,15),[&](GuiScrollBar&,float r){colorRD.r = r*255; updateTitle();}));
	l->setChildElement(vec2(1,1),new ScrollBar(win,vec2(50,15),[&](GuiScrollBar&,float g){colorRD.g = g*255; updateTitle();}));
	l->setChildElement(vec2(1,2),new ScrollBar(win,vec2(50,15),[&](GuiScrollBar&,float b){colorRD.b = b*255; updateTitle();}));
	
	l->setPadding(vec2(10,5));
	
	cout << l->getSize() << endl;
	l->setPosition(win.getSize() - l->getSize());
	
	win.getMainLayout().addChildElement(l);
	
	C(10)
	{
		PushButton *pb = new PushButton(win,"push me");	
		pb->setPosition(vec2(0,i * pb->getSize().h + 5*i));
		
		pb->setCallback([&](GuiButton &b) {
			b.setText("yayy pushed " + fm::toString(++push_count) + " times");
			b.setSize(b.getGuiText().getSize() + vec2(16,10));
			win.setClearColor(vec4::White);
		});
		
		win.getMainLayout().addChildElement(pb);
	}
	
	ColorPicker *cp = new ColorPicker(win);
	cp->setPosition(vec2(150,150));
	cp->setCallback([&](ColorPicker &cp,Color c) {
		win.setClearColor(c.rgba()/255.0);
	});
	
	win.getMainLayout().addChildElement(cp);
	
	ScrollBar *ssb = new ScrollBar(win);
	ssb->setSize(vec2(25,300));
	ssb->setPosition(vec2(640 - 20 - ssb->getSize().w,0));
	ssb->setCallback([&](GuiScrollBar &,float s){
		cp->raiseBrightness(s);
	});
	
	ssb->setScrollSize(0.05);
	
	win.getMainLayout().addChildElement(ssb);
	
	bool running = true;
	while (running)
	{
		// win.handlePendingEvents();
		Event ev;
		while (win.popEvent(ev))
		{
			win.handleEvent(ev);
			if (ev.type == Event::Closed) running = false;
		}
		
		win.clear();
		win.drawElements();
		win.swapBuffers();
			
		win.applyFpsLimit();
	}
}




