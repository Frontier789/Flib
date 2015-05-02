#include <FRONTIER/System/Vector2.hpp>
#include <FRONTIER/Gui/Anchor.hpp>
#include <FRONTIER/Gui/Widget.hpp>

namespace Fgui
{
	/////////////////////////////////////////////////////////////
	Anchor::Anchor(const fm::vec2 &pix,const fm::vec2 &par,const fm::vec2 &chl) : pix(pix),
																				  par(par),
																				  chl(chl)
	{

	}

	/////////////////////////////////////////////////////////////
	fm::vec2 Anchor::getPosInParent(const Widget &owner) const
	{
		return pix - owner.getSize()*chl + owner.getParentSize()*par;
	}

	/////////////////////////////////////////////////////////////
	fm::vec2 Anchor::getPosInRoot(const Widget &owner) const
	{
		const Widget *parent = owner.getParent();
		
		return (parent ? parent->getPosInRoot() : fm::vec2()) + getPosInParent(owner);
	}

	/////////////////////////////////////////////////////////////
	const Anchor Anchor::Middle = Anchor(fm::vec2(),fm::vec2(.5,.5),fm::vec2(.5,.5));
}