#include <FRONTIER/System/macros/C.hpp>
#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/System/Math.hpp>
#include <FRONTIER/Gui/Table.hpp>

namespace Fgui
{
	/////////////////////////////////////////////////////////////
	const std::string Table::m_className = "Table";

	/////////////////////////////////////////////////////////////
	Table::Table(const std::string &name,
    			 const Anchor &anchor,
    			 const fm::vec2 &size,
    			 Widget *parent,
    			 const fm::vec2s &cellCount,
    			 const fm::vec2 &borderSize,
    			 const fm::Collector<Widget *> &cells) : Widget(name,anchor,size,parent),
														 m_cellCount(fm::vec2s()),
														 m_borderSize(borderSize),
														 m_activeWidget(fm::nullPtr),
														 m_needRecalc(false)
	{
		setCellCount(cellCount);

		C(cells.size())
			setCell(fm::vec2s(i%cellCount.w,i/cellCount.w),cells[i]);

		recalc();
	}

	/////////////////////////////////////////////////////////////
	Table::~Table()
	{

	}

	/////////////////////////////////////////////////////////////
	void Table::setCell(const fm::vec2s &pos,Widget *newWidget)
	{
		setCellCount( fm::vec2s(fm::math::max(m_cellCount.w,pos.x+1),fm::math::max(m_cellCount.h,pos.y+1)) );
		m_cells[pos.x][pos.y].setContent(newWidget);

		m_needRecalc = true;
	}

	/////////////////////////////////////////////////////////////
	void Table::setCellSize(const fm::vec2s &pos,const fm::vec2 &size)
	{
		m_cellSizes[pos.x][pos.y] = size;

		m_needRecalc = true;
	}

	/////////////////////////////////////////////////////////////
	void Table::setCellCount(const fm::vec2s &cellCount)
	{
		if (m_cellCount == cellCount)
			return;

		m_cells.resize(cellCount.w);
		m_cellSizes.resize(cellCount.w);

		C(cellCount.w)
			m_cells[i].resize(cellCount.h),
			m_cellSizes[i].resize(cellCount.h);

		Cxy(cellCount.w,cellCount.h)
			m_cells[x][y].setParent(this);

		m_cellCount = cellCount;
	}

	/////////////////////////////////////////////////////////////
	fm::vec2s Table::getCellCount()
	{
		return m_cellCount;
	}

	/////////////////////////////////////////////////////////////
	Widget *Table::getCell(const fm::vec2s &pos)
	{
		return m_cells[pos.x][pos.y].getContent();
	}

	/////////////////////////////////////////////////////////////
	void Table::setAnchor(const Anchor &anchor)
	{
		Widget::setAnchor(anchor);

		recalc();

		Cxy(m_cellCount.w,m_cellCount.h)
			m_cells[x][y].onParentChange();
	}

	/////////////////////////////////////////////////////////////
	void Table::setParent(Widget *parent)
	{
		Widget::setParent(parent);

		recalc();

		Cxy(m_cellCount.w,m_cellCount.h)
			m_cells[x][y].onParentChange();
	}

	/////////////////////////////////////////////////////////////
	fm::vec2 Table::getSize() const
	{
		return m_realSize;
	}

	/////////////////////////////////////////////////////////////
	void Table::setSize(const fm::vec2 &size)
	{
		m_size = size;

		recalc();

		Cxy(m_cellCount.w,m_cellCount.h)
			m_cells[x][y].onParentChange();
	}

	/////////////////////////////////////////////////////////////
	void Table::setBorderSize(const fm::vec2 &size)
	{
		m_borderSize = size;

		m_needRecalc = true;
	}

	/////////////////////////////////////////////////////////////
	fm::vec2 Table::getBorderSize()
	{
		return m_borderSize;
	}

	/////////////////////////////////////////////////////////////
	void Table::setActive(Widget *active)
	{
		if (m_parent)
			m_parent->setActive(active);
		else
			m_activeWidget = active;
	}

	/////////////////////////////////////////////////////////////
	Widget *Table::getActive()
	{
		if (m_parent)
			return m_parent->getActive();
		else
			return m_activeWidget;
	}

	/////////////////////////////////////////////////////////////
	Widget *Table::findNamed(const std::string &name)
	{
		if (getName() == name)
			return this;
		
		Widget *w = fm::nullPtr;

		Cxy(m_cellCount.w,m_cellCount.h)
		{
			w = m_cells[x][y].findNamed(name);
			if (w)
				return w;
		}

		return w;
	}

	/////////////////////////////////////////////////////////////
	void Table::onDraw()
	{
		Cxy(m_cellCount.w,m_cellCount.h)
			m_cells[x][y].onDraw();
	}

	/////////////////////////////////////////////////////////////
	void Table::onUpdate()
	{
		if (m_needRecalc)
			m_needRecalc = false,
			recalc();

		Cxy(m_cellCount.w,m_cellCount.h)
			m_cells[x][y].onUpdate();
	}

	/////////////////////////////////////////////////////////////
	void Table::onParentChange()
	{
		Cxy(m_cellCount.w,m_cellCount.h)
			m_cells[x][y].onParentChange();
	}

	/////////////////////////////////////////////////////////////
	bool Table::handleEvent(const fw::Event &ev)
	{
		if (!getEnabled())
			return false;

		if (m_activeWidget)
			if (m_activeWidget->handleEvent(ev))
				return true;

		Cxy(m_cellCount.w,m_cellCount.h)
			if (m_cells[x][y].getContent() != m_activeWidget && m_cells[x][y].handleEvent(ev))
				return true;

		return false;
	}

	/////////////////////////////////////////////////////////////
	void Table::recalc()
	{
		std::deque<bool> fixedWidths;
		std::deque<bool> fixedHeights;

		fixedWidths.resize(m_cellCount.w,false);
		fixedHeights.resize(m_cellCount.h,false);

		C(m_cellCount.w)
			m_cellWidths[i] = 0;

		C(m_cellCount.h)
			m_cellHeights[i] = 0;

		Cxy(m_cellCount.w,m_cellCount.h)
		{
			if (m_cellSizes[x][y].x)
				fixedWidths[x]  = true;

			if (m_cellSizes[x][y].y)
				fixedHeights[y] = true;
		}

		Cxy(m_cellCount.w,m_cellCount.h)
		{
			Widget *cell = m_cells[x][y].getContent();
			fm::vec2 size = (cell ? cell->getSize() : fm::vec2());
			fm::vec2 fixSize = m_cellSizes[x][y];

			if (fixedWidths[x])
				size.w = fixSize.w;

			if (fixedHeights[y])
				size.h = fixSize.h;

			m_cellWidths[x]  = fm::math::max(m_cellWidths[x], size.w);
			m_cellHeights[y] = fm::math::max(m_cellHeights[y],size.h);
		}

		fm::vec2 tmpSize = fm::vec2();
		fm::vec2 nonFixedSize = fm::vec2();

		C(m_cellCount.w)
		{
			tmpSize.w += m_cellWidths[i];
			if (!fixedWidths[i])
				nonFixedSize.w += m_cellWidths[i];
		}

		C(m_cellCount.h)
		{
			tmpSize.h += m_cellHeights[i];
			if (!fixedHeights[i])
				nonFixedSize.h += m_cellHeights[i];
		}

		tmpSize += m_borderSize*(m_cellCount+fm::vec2(1,1));
		m_realSize = fm::vec2(fm::math::max(tmpSize.w,m_size.w),fm::math::max(tmpSize.h,m_size.h));

		if (nonFixedSize.w)
		{
			C(m_cellCount.w)
				if (!fixedWidths[i])
					m_cellWidths[i] *= (m_realSize.w-tmpSize.w+nonFixedSize.w)/nonFixedSize.w;
		}
		else
			m_realSize.w = tmpSize.w;

		if (nonFixedSize.h)
		{
			C(m_cellCount.h)
				if (!fixedHeights[i])
					m_cellHeights[i] *= (m_realSize.h-tmpSize.h+nonFixedSize.h)/nonFixedSize.h;
		}
		else
			m_realSize.h = tmpSize.h;

		float cw=0;
		float ch=0;
		Cx(m_cellCount.w)
		{
			Cy(m_cellCount.h)
			{
				m_cells[x][y].setAnchor(fm::vec2(cw,ch)+m_borderSize*fm::vec2(x+1,y+1));
				m_cells[x][y].setMinSize(fm::vec2(m_cellWidths[x],m_cellHeights[y]));
				ch += m_cellHeights[y];
			}
			ch  = 0;
			cw += m_cellWidths[x];
		}

		/*
		if (m_parent)
			m_parent->onChildChange(this);
			*/
	}
	/////////////////////////////////////////////////////////////
	const std::string &Table::getRealType() const
	{
		return m_className;
	}
}
