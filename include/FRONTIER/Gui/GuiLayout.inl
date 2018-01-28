#ifndef FRONTIER_GUILAYOUTBASE_INL_INCLUDED
#define FRONTIER_GUILAYOUTBASE_INL_INCLUDED

namespace fgui
{
	/////////////////////////////////////////////////////////////
	template<class GuiClass>
	GuiClass *GuiLayout::getChildElement(fm::Size index) const
	{
		return (GuiClass*)getChildElement(index);
	}
}

#endif // FRONTIER_GUILAYOUTBASE_INL_INCLUDED
